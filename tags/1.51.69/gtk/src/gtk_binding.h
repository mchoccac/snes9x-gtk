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

#ifndef __GTK_BINDING_H
#define __GTK_BINDING_H

#include <gdk/gdk.h>

#define AXIS_POS                    1
#define AXIS_NEG                    0
#define JOY_AXIS(axis, button)      (512 + ((axis) * 2 + (button)))
#define JOY_AXIS_UNMASK(bin)        ((((bin) & 0x0000ffff) - 512) / 2)
#define JOY_DIRECTION_UNMASK(bin)   ((((bin) & 0x0000ffff) - 512) % 2)
#define BINDING_KEY                 0x10000000
#define BINDING_JOY                 0x20000000
#define BINDING_CTRL                0x01000000
#define BINDING_SHIFT               0x02000000
#define BINDING_ALT                 0x04000000
#define BINDING_JOYDEV_MASK         0x0f000000
#define BINDING_THRESHOLD_MASK      0x00ff0000
#define BINDING_KEY_MASK            0x0000ffff
#define THRESHOLD_MASK(threshold)   ((threshold) << 16)
#define THRESHOLD_UNMASK(bin)       (((bin) & 0x00ff0000) >> 16)
#define JOY_DEVICE_MASK(devnum)     ((devnum) << 24)
#define JOY_DEVICE_UNMASK(bin)      (((bin) & 0x0f000000) >> 24)
#define BINDING_MOUSE_POINTER       0x41000000
#define BINDING_MOUSE_BUTTON0       0x42000000
#define BINDING_MOUSE_BUTTON1       0x42000001
#define BINDING_MOUSE_BUTTON2       0x42000002

class Binding 
{
    public:
        Binding (unsigned int key, bool ctrl, bool shift, bool alt);
        Binding (unsigned int device, unsigned int button, unsigned int threshold);
        Binding (const Binding &binding);
        Binding (GdkEventKey *event);
        Binding (unsigned int);
        Binding (void);
        void to_string (char *str);
        unsigned int hex (void);   
        unsigned int base_hex (void);
        bool matches (Binding &binding);
        void clear (void);
        bool is_joy (void);
        bool is_key (void);
        bool is_mouse (void);
        bool is_positive (void);
        bool is_negative (void);
        unsigned int get_key (void);
        unsigned int get_device (void);
        unsigned int get_threshold (void);
        unsigned int get_axis (void);
        GdkModifierType get_gdk_modifiers (void);
        
    private:
        unsigned int value;
};
    
#endif /* __GTK_BINDING_H_ */
