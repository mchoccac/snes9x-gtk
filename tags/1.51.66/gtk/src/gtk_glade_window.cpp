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
#include "gtk_glade_window.h"

GladeWindow::GladeWindow (const char *buffer, int size, const char *root)
{
    glade = glade_xml_new_from_buffer (buffer, size, root, NULL);

    window = get_widget (root);

    return;
}

GtkWidget *
GladeWindow::get_widget (const char *name)
{
    return glade_xml_get_widget (glade, name);
}

void
GladeWindow::signal_connect (const char *name, GCallback func)
{
    glade_xml_signal_connect_data (glade, name, func, (gpointer) this);

    return;
}

void
GladeWindow::signal_connect (const char *name, GCallback func, gpointer data)
{
    glade_xml_signal_connect_data (glade, name, func, (gpointer) data);

    return;
}

void
GladeWindow::signal_connect (GladeWindowCallbacks *callbacks)
{
    if (!callbacks)
        return;

    for (int i = 0; callbacks[i].signal; i++)
    {
        signal_connect (callbacks[i].signal, callbacks[i].function);
    }

    return;
}

void
GladeWindow::enable_widget (const char *name, unsigned char state)
{
    gtk_widget_set_sensitive (get_widget (name), state);

    return;
}

void
GladeWindow::resize (int width, int height)
{
    gtk_window_resize (GTK_WINDOW (window), width, height);

    return;
}

void
GladeWindow::refresh (const char *widget)
{
    gtk_widget_queue_draw (get_widget (widget));
    gtk_widget_show (get_widget (widget));

    return;
}

int
GladeWindow::get_width (void)
{
    return window->allocation.width;
}

int
GladeWindow::get_height (void)
{
    return window->allocation.height;
}

void
GladeWindow::set_button_label (const char *name, const char *label)
{
    gtk_button_set_label (GTK_BUTTON (get_widget (name)), label);

    return;
}

unsigned char
GladeWindow::get_check (const char *name)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (get_widget (name))))
        return 1;
    return 0;
}

unsigned int
GladeWindow::get_entry_value (const char *name)
{
    return atoi (gtk_entry_get_text (GTK_ENTRY (get_widget (name))));;
}

const char *
GladeWindow::get_entry_text (const char *name)
{
    return gtk_entry_get_text (GTK_ENTRY (get_widget (name)));
}

float
GladeWindow::get_slider (const char *name)
{
    return (float) gtk_range_get_value (GTK_RANGE (get_widget (name)));
}

unsigned char
GladeWindow::get_combo (const char *name)
{
    return gtk_combo_box_get_active (GTK_COMBO_BOX (get_widget (name)));
}

void
GladeWindow::set_slider (const char *name, float value)
{
    gtk_range_set_value (GTK_RANGE (get_widget (name)), (double) value);

    return;
}

void
GladeWindow::set_check (const char *name, unsigned char value)
{
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (get_widget (name)),
                                  value);
    return;
}

void
GladeWindow::set_entry_value (const char *name, unsigned int value)
{
    char text[80];

    snprintf (text, 80, "%u", value);
    gtk_entry_set_text (GTK_ENTRY (get_widget (name)), text);

    return;
}

void
GladeWindow::set_entry_text (const char *name, const char *text)
{
    gtk_entry_set_text (GTK_ENTRY (get_widget (name)), text);

    return;
}

void
GladeWindow::set_combo (const char *name, unsigned char value)
{
    gtk_combo_box_set_active (GTK_COMBO_BOX (get_widget (name)), value);

    return;
}

void
GladeWindow::set_spin (const char *name, unsigned int value)
{
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (get_widget (name)),
                               (double) value);

    return;
}

GtkWindow *
GladeWindow::get_window (void)
{
    return GTK_WINDOW (window);
}

unsigned int
GladeWindow::get_spin (const char *name)
{
    return (unsigned int)
        gtk_spin_button_get_value (GTK_SPIN_BUTTON (get_widget (name)));
}

int
GladeWindow::has_focus (const char *widget)
{
    return gtk_widget_is_focus (get_widget (widget));
}
