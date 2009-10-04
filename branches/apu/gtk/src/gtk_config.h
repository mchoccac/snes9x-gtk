#ifndef __GTK_CONFIG_H
#define __GTK_CONFIG_H

#include <libxml/parser.h>
#include <X11/Xlib.h>
#ifdef USE_XRANDR
#include <X11/extensions/Xrandr.h>
#endif

#include "gtk_control.h"

#define DIR_ROM                 0
#define DIR_CONFIG              1
#define DIR_CUSTOM              2

#define HWA_NONE                0
#define HWA_OPENGL              1
#define HWA_XV                  2

#define ESC_TOGGLE_MENUBAR      0
#define ESC_EXIT_FULLSCREEN     1
#define ESC_EXIT_SNES9X         2

class Snes9xConfig
{
    public:
        Snes9xConfig (void);
        int load_config_file (void);
        int save_config_file (void);
        int load_defaults (void);
        void reconfigure (void);
        void rebind_keys (void);

        /* XML parsers */
        int parse_snes9x (xmlNodePtr node);
        int parse_preferences (xmlNodePtr node);
        int parse_option (xmlNodePtr node);
        int parse_joypad (xmlNodePtr node);
        int parse_binding (xmlNodePtr node, int joypad_number);
        int parse_controls (xmlNodePtr node);
        int set_option (const char *name, const char *value);

#ifdef USE_JOYSTICK
        void flush_joysticks (void);
        void set_joystick_mode (int mode);
        void joystick_register_centers (void);
        int parse_calibration (xmlNodePtr node);
        int parse_axis (xmlNodePtr node, int joystick_number);
#endif

        /* Screen options */
        unsigned char full_screen_on_open;
        unsigned char change_display_resolution;
        unsigned char fullscreen;
        unsigned char ui_visible;
        unsigned char statusbar_visible;
        unsigned char default_esc_behavior;
        unsigned char prevent_screensaver;
        unsigned int  xrr_index;
        int           xrr_width;
        int           xrr_height;
        unsigned char scale_to_fit;
        unsigned char maintain_aspect_ratio;
        int           aspect_ratio;
        unsigned int  scale_method;
        unsigned char overscan;
        unsigned char multithreading;
        unsigned char force_hires;
        unsigned char force_inverted_byte_order;
        unsigned char ntsc_format;
        float         ntsc_hue;
        float         ntsc_saturation;
        float         ntsc_contrast;
        float         ntsc_brightness;
        float         ntsc_sharpness;
        float         ntsc_warping;
        unsigned int  ntsc_scanline_intensity;
        unsigned int  scanline_filter_intensity;
        unsigned char hw_accel;
        unsigned char allow_opengl;
        unsigned char allow_xv;
        unsigned char allow_xrandr;

        /* Data options */
        unsigned char data_location;
        unsigned char save_sram_after_secs;
        char          custom_sram_directory [PATH_MAX];
        char          last_directory [PATH_MAX];

        /* Controls */
        JoypadBinding pad[NUM_JOYPADS];
        Binding       shortcut[NUM_EMU_LINKS];

        /* Netplay */
        int           netplay_is_server;
        int           netplay_sync_reset;
        int           netplay_send_rom;
        int           netplay_default_port;
        int           netplay_max_frame_loss;
        char          netplay_last_rom [PATH_MAX];
        char          netplay_last_host [PATH_MAX];
        int           netplay_last_port;
        int           netplay_activated;
        int           netplay_server_up;

        /* Operational */
        int           sound_driver;
        int           mute_sound;
        int           sound_buffer_size;
        int           sound_input_rate;
        int           rom_loaded;
        int           window_width, window_height;
        int           preferences_width, preferences_height;
        int           pause_emulation_on_switch;
        int           num_threads;
        unsigned char screensaver_needs_reset;

#ifdef USE_XRANDR
        XRRScreenConfiguration *xrr_config;
        XRRScreenSize          *xrr_sizes;
        int                    xrr_num_sizes;
        Rotation               xrr_rotation;
        SizeID                 xrr_original_size;
#endif


#ifdef USE_OPENGL
        unsigned char bilinear_filter;
        unsigned char sync_to_vblank;
        unsigned char opengl_activated;
        unsigned char use_pbos;
        int           pbo_format;
        unsigned char npot_textures;
#endif

#ifdef USE_JOYSTICK
        JoyDevice **joystick;
        int       joystick_threshold;
#endif

};

char *get_config_dir (void);
char *get_config_file_name (void);

#endif /* __GTK_CONFIG_H */