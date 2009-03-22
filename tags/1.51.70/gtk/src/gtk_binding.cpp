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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "gtk_s9x.h"
#include "gtk_binding.h"

Binding::Binding (void)
{
    value = 0;

    return;
}

Binding::Binding (GdkEventKey *event)
{
    value = BINDING_KEY | (event->keyval & BINDING_KEY_MASK);

    value |= (BINDING_KEY_MASK & event->keyval);

    if (event->state & GDK_SHIFT_MASK)
        value |= BINDING_SHIFT;

    if (event->state & GDK_CONTROL_MASK)
        value |= BINDING_CTRL;

    if (event->state & GDK_MOD1_MASK)
        value |= BINDING_ALT;

    return;
}

Binding::Binding (unsigned int key, bool ctrl, bool shift, bool alt)
{
    value = BINDING_KEY;

    value |= key & BINDING_KEY_MASK;
    if (ctrl)
        value |= BINDING_CTRL;
    if (shift)
        value |= BINDING_SHIFT;
    if (alt)
        value |= BINDING_ALT;
}

Binding::Binding (unsigned int device,
                  unsigned int button,
                  unsigned int threshold)
{
    value = BINDING_JOY;
    value |= JOY_DEVICE_MASK (device + 1);
    value |= BINDING_KEY_MASK & button;
    if (button >= 512)
        value |= THRESHOLD_MASK (threshold);
}

Binding::Binding (unsigned int val)
{
    value = val;

    return;
}

Binding::Binding (const Binding& binding)
{
    this->value = binding.value;
}

bool
Binding::matches (Binding &binding)
{
    if ((value & ~BINDING_THRESHOLD_MASK) ==
        (binding.value & ~BINDING_THRESHOLD_MASK) &&
        is_joy ())
        return true;

    return false;
}

void
Binding::clear (void)
{
    value = 0;

    return;
}

unsigned int
Binding::hex (void)
{
    return value;
}

unsigned int
Binding::base_hex (void)
{
    return (value & ~BINDING_THRESHOLD_MASK);
}

bool
Binding::is_joy (void)
{
    return (value & BINDING_JOY);
}

bool
Binding::is_key (void)
{
    return (value & BINDING_KEY);
}

unsigned int
Binding::get_key (void)
{
    return (value & BINDING_KEY_MASK);
}

unsigned int
Binding::get_device (void)
{
    return JOY_DEVICE_UNMASK (value);
}

unsigned int
Binding::get_threshold (void)
{
    return THRESHOLD_UNMASK (value);
}

unsigned int
Binding::get_axis (void)
{
    return JOY_AXIS_UNMASK (value);
}

GdkModifierType
Binding::get_gdk_modifiers (void)
{
    return (GdkModifierType) ((BINDING_CTRL  & value ? GDK_CONTROL_MASK : 0) |
                              (BINDING_ALT   & value ? GDK_MOD1_MASK    : 0) |
                              (BINDING_SHIFT & value ? GDK_SHIFT_MASK   : 0));
}

bool
Binding::is_positive (void)
{
    return JOY_DIRECTION_UNMASK (value) == AXIS_POS;
}

bool
Binding::is_negative (void)
{
    return JOY_DIRECTION_UNMASK (value) == AXIS_NEG;
}

void
Binding::to_string (char *str)
{
    char buf[256];
    char *c;

    str[0] = '\0';

    if (is_key ())
    {
        char *keyval_name = NULL;
        unsigned int keyval = gdk_keyval_to_lower (get_key ());
        keyval_name = gdk_keyval_name (keyval);

        if (keyval_name == NULL)
        {
            sprintf (buf, _("Unknown"));
        }

        else
        {
            strncpy (buf,
                     keyval_name,
                     256);
        }

        while ((c = strstr (buf, "_")))
        {
            *c = ' ';
        }

        sprintf (str, _("Keyboard %s%s%s%s"),
                 value & BINDING_SHIFT ? "Shift+" : "",
                 value & BINDING_CTRL  ? "Ctrl+"  : "",
                 value & BINDING_ALT   ? "Alt+"   : "",
                 buf);
    }

    else if (is_joy ())
    {
        if ((get_key ()) >= 512)
            sprintf (buf,
                     _("Axis #%d %s %d%%"),
                     get_axis (),
                     is_positive () ? "+" : "-",
                     get_threshold ());
        else
            sprintf (buf, _("Button %d"), get_key ());

        sprintf (str, _("Joystick %d %s"), get_device (), buf);
    }

    else
    {
        sprintf (str, _("Unset"));
    }

    return;
}
