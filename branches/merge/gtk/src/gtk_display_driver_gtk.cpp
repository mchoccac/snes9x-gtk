#include <gtk/gtk.h>
#include "gtk_display.h"
#include "gtk_display_driver_gtk.h"


S9xGTKDisplayDriver::S9xGTKDisplayDriver (Snes9xWindow *window,
                                          Snes9xConfig *config)
{
    this->window = window;
    this->config = config;
    this->drawing_area = GTK_WIDGET (window->drawing_area);

    return;
}

void
S9xGTKDisplayDriver::update (int width, int height)
{
    int   x, y, w, h;
    int   c_width, c_height, final_pitch;
    uint8 *final_buffer;

    c_width = drawing_area->allocation.width;
    c_height = drawing_area->allocation.height;

    if (width == SIZE_FLAG_DIRTY)
    {
        this->clear ();
        return;
    }

    if (width <= 0)
        return;

    if (config->scale_method > 0)
    {
        uint8 *src_buffer = (uint8 *) padded_buffer[0];
        uint8 *dst_buffer = (uint8 *) padded_buffer[1];
        int   src_pitch = image_width * image_bpp;
        int   dst_pitch = scaled_max_width * image_bpp;

        S9xFilter (src_buffer,
                   src_pitch,
                   dst_buffer,
                   dst_pitch,
                   width,
                   height);

        final_buffer = (uint8 *) padded_buffer[1];
        final_pitch = dst_pitch;
    }
    else
    {
        final_buffer = (uint8 *) padded_buffer[0];
        final_pitch = image_width * image_bpp;
    }

    x = width; y = height; w = c_width; h = c_height;
    S9xApplyAspect (x, y, w, h);
    output (final_buffer, final_pitch, x, y, width, height, w, h);

    return;
}

void
S9xGTKDisplayDriver::output (void *src,
                             int  src_pitch,
                             int  x,
                             int  y,
                             int  width,
                             int  height,
                             int  dst_width,
                             int  dst_height)
{
    GdkGC *gc = drawing_area->style->bg_gc[GTK_WIDGET_STATE (drawing_area)];

    if (width != dst_width || height != dst_height)
    {
        S9xConvertScale (src,
                         padded_buffer[2],
                         src_pitch,
                         gdk_buffer_width * 3,
                         width,
                         height,
                         dst_width, dst_height,
                         24);
    }
    else
    {
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;

        if (width > drawing_area->allocation.width)
            width = dst_width = drawing_area->allocation.width;
        if (height > drawing_area->allocation.height)
            height = dst_height = drawing_area->allocation.height;

        S9xConvert (src,
                    padded_buffer[2],
                    src_pitch,
                    gdk_buffer_width * 3,
                    width,
                    height,
                    24);
    }

    gdk_draw_rgb_image (drawing_area->window,
                        gc,
                        x, y,
                        dst_width, dst_height,
                        GDK_RGB_DITHER_NORMAL,
                        (guchar *) padded_buffer[2],
                        gdk_buffer_width * 3);

    window->set_mouseable_area (x, y, width, height);

    return;
}

int
S9xGTKDisplayDriver::init (void)
{
    int padding;

    buffer[0] = malloc (image_padded_size);
    buffer[1] = malloc (scaled_padded_size);

    padding = (image_padded_size - image_size) / 2;
    padded_buffer[0] = (void *) (((uint8 *) buffer[0]) + padding);

    padding = (scaled_padded_size - scaled_size) / 2;
    padded_buffer[1] = (void *) (((uint8 *) buffer[1]) + padding);

    gdk_buffer_width = drawing_area->allocation.width;
    gdk_buffer_height = drawing_area->allocation.height;

    padded_buffer[2] = malloc (gdk_buffer_width * gdk_buffer_height * 3);
    S9xSetEndianess (ENDIAN_MSB);

    memset (buffer[0], 0, image_padded_size);
    memset (buffer[1], 0, scaled_padded_size);

    GFX.Screen = (uint16 *) padded_buffer[0];
    GFX.Pitch = image_width * image_bpp;

    return 0;
}

void
S9xGTKDisplayDriver::deinit (void)
{
    padded_buffer[0] = NULL;
    padded_buffer[1] = NULL;

    free (buffer[0]);
    free (buffer[1]);
    free (padded_buffer[2]);

    return;
}

void
S9xGTKDisplayDriver::clear (void)
{
    int      x, y, w, h;
    int      width = drawing_area->allocation.width;
    int      height = drawing_area->allocation.height;
    GdkColor black = { 0, 0, 0, 0 };
    GdkGC    *gc = NULL;

    gc = drawing_area->style->fg_gc[GTK_WIDGET_STATE (drawing_area)];
    gdk_gc_set_rgb_fg_color (gc, &black);

    if (window->last_width <= 0 || window->last_height <= 0)
    {
        gdk_draw_rectangle (drawing_area->window,
                            gc,
                            TRUE,
                            0, 0,
                            width, height);
        return;
    }

    /* Get width of modified display */
    x = window->last_width;
    y = window->last_height;
    get_filter_scale (x, y);
    w = width;
    h = height;
    S9xApplyAspect (x, y, w, h);
    
    if (x > 0)
    {
        gdk_draw_rectangle (drawing_area->window, gc, TRUE, 0, y, x, h);
    }
    if (x + w < width)
    {
        gdk_draw_rectangle (drawing_area->window, gc, TRUE, x + w, y, width - (x + w), h);
    }
    if (y > 0)
    {
        gdk_draw_rectangle (drawing_area->window, gc, TRUE, 0, 0, width, y);
    }
    if (y + h < height)
    {
        gdk_draw_rectangle (drawing_area->window, gc, TRUE, 0, y + h, width, height - (y + h));
    }

    return;
}

void
S9xGTKDisplayDriver::refresh (int width, int height)
{
    int c_width, c_height;

    c_width = drawing_area->allocation.width;
    c_height = drawing_area->allocation.height;

    if (c_width != gdk_buffer_width || c_height != gdk_buffer_height)
    {
        free (padded_buffer[2]);

        gdk_buffer_width = c_width;
        gdk_buffer_height = c_height;

        padded_buffer[2] = malloc (gdk_buffer_width * gdk_buffer_height * 3);
    }

    if (!config->rom_loaded)
        return;

    clear ();

    return;
}

uint16 *
S9xGTKDisplayDriver::get_next_buffer (void)
{
    return (uint16 *) padded_buffer[0];
}

uint16 *
S9xGTKDisplayDriver::get_current_buffer (void)
{
    return (uint16 *) padded_buffer[0];
}

void
S9xGTKDisplayDriver::push_buffer (uint16 *src)
{
    memmove (GFX.Screen, src, image_size);
    update (window->last_width, window->last_height);

    return;
}

void
S9xGTKDisplayDriver::clear_buffers (void)
{
    memset (buffer[0], 0, image_padded_size);
    memset (buffer[1], 0, scaled_padded_size);

    return;
}

void
S9xGTKDisplayDriver::reconfigure (int width, int height)
{
    return;
}
