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
