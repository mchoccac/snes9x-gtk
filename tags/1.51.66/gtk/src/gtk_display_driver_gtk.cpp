/**********************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2002  Gary Henderson (gary.henderson@ntlworld.com),
                             Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2004  Matthew Kendora

  (c) Copyright 2002 - 2005  Peter Bortas (peter@bortas.org)

  (c) Copyright 2004 - 2005  Joel Yliluoma (http://iki.fi/bisqwit/)

  (c) Copyright 2001 - 2006  John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2006  funkyass (funkyass@spam.shaw.ca),
                             Kris Bleakley (codeviolation@hotmail.com)

  (c) Copyright 2002 - 2007  Brad Jorsch (anomie@users.sourceforge.net),
                             Nach (n-a-c-h@users.sourceforge.net),
                             zones (kasumitokoduck@yahoo.com)

  (c) Copyright 2006 - 2007  nitsuja


  BS-X C emulator code
  (c) Copyright 2005 - 2006  Dreamer Nom,
                             zones

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003  _Demo_ (_demo_@zsnes.com),
                             Nach,
                             zsKnight (zsknight@zsnes.com)

  C4 C++ code
  (c) Copyright 2003 - 2006  Brad Jorsch,
                             Nach

  DSP-1 emulator code
  (c) Copyright 1998 - 2006  _Demo_,
                             Andreas Naive (andreasnaive@gmail.com)
                             Gary Henderson,
                             Ivar (ivar@snes9x.com),
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora,
                             Nach,
                             neviksti (neviksti@hotmail.com)

  DSP-2 emulator code
  (c) Copyright 2003         John Weidman,
                             Kris Bleakley,
                             Lord Nightmare (lord_nightmare@users.sourceforge.net),
                             Matthew Kendora,
                             neviksti


  DSP-3 emulator code
  (c) Copyright 2003 - 2006  John Weidman,
                             Kris Bleakley,
                             Lancer,
                             z80 gaiden

  DSP-4 emulator code
  (c) Copyright 2004 - 2006  Dreamer Nom,
                             John Weidman,
                             Kris Bleakley,
                             Nach,
                             z80 gaiden

  OBC1 emulator code
  (c) Copyright 2001 - 2004  zsKnight,
                             pagefault (pagefault@zsnes.com),
                             Kris Bleakley,
                             Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002         Matthew Kendora with research by
                             zsKnight,
                             John Weidman,
                             Dark Force

  S-DD1 C emulator code
  (c) Copyright 2003         Brad Jorsch with research by
                             Andreas Naive,
                             John Weidman

  S-RTC C emulator code
  (c) Copyright 2001-2006    byuu,
                             John Weidman

  ST010 C++ emulator code
  (c) Copyright 2003         Feather,
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003  _Demo_,
                             pagefault,
                             zsKnight,

  Super FX C emulator code
  (c) Copyright 1997 - 1999  Ivar,
                             Gary Henderson,
                             John Weidman

  Sound DSP emulator code is derived from SNEeSe and OpenSPC:
  (c) Copyright 1998 - 2003  Brad Martin
  (c) Copyright 1998 - 2006  Charles Bilyue'

  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004  Marcus Comstedt (marcus@mc.pp.se)

  2xSaI filter
  (c) Copyright 1999 - 2001  Derek Liauw Kie Fa

  HQ2x, HQ3x, HQ4x filters
  (c) Copyright 2003         Maxim Stepin (maxim@hiend3d.com)

  Win32 GUI code
  (c) Copyright 2003 - 2006  blip,
                             funkyass,
                             Matthew Kendora,
                             Nach,
                             nitsuja

  Mac OS GUI code
  (c) Copyright 1998 - 2001  John Stiles
  (c) Copyright 2001 - 2007  zones


  Specific ports contains the works of other authors. See headers in
  individual files.


  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and/or distribute Snes9x in both binary
  and source form, for non-commercial purposes, is hereby granted without
  fee, providing that this license information and copyright notice appear
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for Snes9x or software derived from
  Snes9x, including Snes9x or derivatives in commercial game bundles, and/or
  using Snes9x as a promotion for your commercial product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
**********************************************************************************/

/**********************************************************************************
  Snes9x GTK Port

  (c) Copyright 2007 - 2008  Brandon Wright (bearoso@gmail.com)
**********************************************************************************/

#include <gtk/gtk.h>
/*
#include <gdk/gdkx.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
*/
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

    if (!config->scale_to_fit &&
            (width > gdk_buffer_width || height > gdk_buffer_height))
    {
        this->clear ();

        return;
    }

    if (config->scale_to_fit)
    {
        if (config->maintain_aspect_ratio)
        {
            float snes_aspect;
            float screen_aspect;

            snes_aspect = 8.0 / 7.0;
            screen_aspect = (float) c_width / (float) c_height;

            if (screen_aspect > snes_aspect)
            {
                output (final_buffer,
                        final_pitch,
                        (c_width - (int) (c_height * snes_aspect)) / 2,
                        0,
                        width,
                        height,
                        (int) (c_height * snes_aspect),
                        c_height);
            }
            else
            {
                output (final_buffer,
                        final_pitch,
                        0,
                        (c_height - c_width / snes_aspect) / 2,
                        width,
                        height,
                        c_width,
                        (c_width / snes_aspect));
            }
        }
        else
        {
            output (final_buffer,
                    final_pitch,
                    0,
                    0,
                    width,
                    height,
                    c_width,
                    c_height);
        }
    }
    else
    {
        output (final_buffer,
                final_pitch,
                (c_width - width) / 2,
                (c_height - height) / 2,
                width,
                height,
                width,
                height);
    }

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

    /*if (use_shm)
    {
        if (width != dst_width || height != dst_height)
        {
            S9xConvertScale (src,
                             ximage->data,
                             src_pitch,
                             ximage->bytes_per_line,
                             width,
                             height,
                             dst_width, dst_height,
                             ximage->bits_per_pixel);
        }
        else
        {
            S9xConvert (src,
                        ximage->data + ximage->xoffset,
                        src_pitch,
                        ximage->bytes_per_line,
                        width,
                        height,
                        ximage->bits_per_pixel);
        }

        gdk_display_sync (gtk_widget_get_display (drawing_area));

        XShmPutImage (display,
                      GDK_WINDOW_XWINDOW (drawing_area->window),
                      GDK_GC_XGC (gc),
                      ximage,
                      0, 0,
                      x, y,
                      dst_width, dst_height,
                      True);

        gdk_display_sync (gtk_widget_get_display (drawing_area));
    }
    else
    { */
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
/*
    } */

    return;
}

int
S9xGTKDisplayDriver::init (void)
{
    int padding;

    use_shm = FALSE;

    buffer[0] = malloc (image_padded_size);
    buffer[1] = malloc (scaled_padded_size);

    padding = (image_padded_size - image_size) / 2;
    padded_buffer[0] = (void *) (((uint8 *) buffer[0]) + padding);

    padding = (scaled_padded_size - scaled_size) / 2;
    padded_buffer[1] = (void *) (((uint8 *) buffer[1]) + padding);

    gdk_buffer_width = drawing_area->allocation.width;
    gdk_buffer_height = drawing_area->allocation.height;

/*    display = gdk_x11_drawable_get_xdisplay (GDK_DRAWABLE (drawing_area->window));

    if (!XShmQueryExtension (display) || 0)
    { */
        padded_buffer[2] = malloc (gdk_buffer_width * gdk_buffer_height * 3);
        S9xSetEndianess (ENDIAN_MSB);
/*    }
    else
    {
        Visual *visual = gdk_x11_visual_get_xvisual (
                             gdk_screen_get_system_visual (
                                 gdk_screen_get_default ()));
        int depth = GDK_SCREEN_XSCREEN (gdk_screen_get_default ())->root_depth;

        use_shm = TRUE;
        ximage = XShmCreateImage (display,
                                  visual,
                                  depth,
                                  ZPixmap,
                                  NULL,
                                  &shm,
                                  gdk_buffer_width,
                                  gdk_buffer_height);

        shm.shmid = shmget (IPC_PRIVATE,
                            ximage->bytes_per_line * ximage->height,
                            IPC_CREAT | 0777);
        shm.shmaddr = (char *) shmat (shm.shmid, 0, 0);
        shm.readOnly = FALSE;
        ximage->data = shm.shmaddr;

        XShmAttach (display, &shm);

        S9xSetEndianess (ximage->byte_order == LSBFirst ? ENDIAN_LSB : ENDIAN_MSB);
    } */


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
/*
    if (use_shm)
    {
        XShmDetach (display, &shm);
        shmdt (shm.shmaddr);
        shmctl (shm.shmid, IPC_RMID, 0);
        XDestroyImage (ximage);
    }
    else
    { */
        free (padded_buffer[2]);
    /*} */

    return;
}

void
S9xGTKDisplayDriver::clear (void)
{
    int      w, h;
    int      c_width = drawing_area->allocation.width;
    int      c_height = drawing_area->allocation.height;
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
                            c_width, c_height);
        return;
    }

    /* Get width of modified display */
    w = window->last_width;
    h = window->last_height;
    get_filter_scale (w, h);

    if (config->scale_to_fit)
    {
        if (config->maintain_aspect_ratio)
        {
            int    bar_size;
            double screen_aspect;
            double snes_aspect = 8.0 / 7.0;

            screen_aspect = (double) c_width / c_height;

            if (screen_aspect > snes_aspect)
            {
                /* Black bars on left and right */
                w = (int) (c_height * snes_aspect);
                bar_size = (c_width - w) / 2;

                gdk_draw_rectangle (drawing_area->window,
                                    gc,
                                    TRUE,
                                    0, 0,
                                    bar_size, c_height);
                gdk_draw_rectangle (drawing_area->window,
                                    gc,
                                    TRUE,
                                    bar_size + w, 0,
                                    c_width - bar_size - w,
                                    c_height);
            }
            else
            {
                /* Black bars on top and bottom */
                h = (int) (c_width / snes_aspect);
                bar_size = (c_height - h) / 2;
                gdk_draw_rectangle (drawing_area->window,
                                    gc,
                                    TRUE,
                                    0, 0,
                                    c_width, bar_size);
                gdk_draw_rectangle (drawing_area->window,
                                    gc,
                                    TRUE,
                                    0, bar_size + h,
                                    c_width,
                                    c_height - bar_size - h);
            }
        }
        else
            return;
    }
    else
    {
        /* Black bars on top, bottom, left, and right :-) */
        int bar_width, bar_height;

        bar_height = (c_height - h) / 2;
        bar_width = (c_width - w) / 2;

        gdk_draw_rectangle (drawing_area->window,
                            gc,
                            TRUE,
                            0, 0,
                            c_width, bar_height);
        gdk_draw_rectangle (drawing_area->window,
                            gc,
                            TRUE,
                            0,
                            bar_height + h,
                            c_width,
                            c_height - (bar_height + h));
        gdk_draw_rectangle (drawing_area->window,
                            gc,
                            TRUE,
                            0, bar_height,
                            bar_width, h);
        gdk_draw_rectangle (drawing_area->window,
                            gc,
                            TRUE,
                            bar_width + w, bar_height,
                            c_width - (bar_width + w),
                            h);
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
/*        if (use_shm)
        {
            XShmDetach (display, &shm);
            shmdt (shm.shmaddr);
            shmctl (shm.shmid, IPC_RMID, 0);
            XDestroyImage (ximage);

            gdk_display_sync (gtk_widget_get_display (drawing_area));

            Visual *visual = gdk_x11_visual_get_xvisual (
                                 gdk_screen_get_system_visual (
                                     gdk_screen_get_default ()));
            int depth = GDK_SCREEN_XSCREEN (gdk_screen_get_default ())->root_depth;

            use_shm = TRUE;
            ximage = XShmCreateImage (display,
                                      visual,
                                      depth,
                                      ZPixmap,
                                      NULL,
                                      &shm,
                                      c_width,
                                      c_height);

            shm.shmid = shmget (IPC_PRIVATE,
                                ximage->bytes_per_line * ximage->height,
                                IPC_CREAT | 0777);
            shm.shmaddr = (char *) shmat (shm.shmid, 0, 0);
            shm.readOnly = FALSE;
            ximage->data = shm.shmaddr;

            XShmAttach (display, &shm);

            S9xSetEndianess (ximage->byte_order == LSBFirst ? ENDIAN_LSB : ENDIAN_MSB);;
        }
        else
        { */
            free (padded_buffer[2]);

            gdk_buffer_width = c_width;
            gdk_buffer_height = c_height;

            padded_buffer[2] = malloc (gdk_buffer_width * gdk_buffer_height * 3);
        /*} */
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
S9xGTKDisplayDriver::reconfigure (void)
{
    return;
}
