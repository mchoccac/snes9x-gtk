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

#include "gtk_netplay_dialog.h"
#include "gtk_s9x.h"
#include "gtk_file.h"

static void 
event_browse_clicked (GtkButton *button, gpointer data)
{
    char                *filename;
    Snes9xNetplayDialog *np_dialog = (Snes9xNetplayDialog *) data;
    
    filename = S9xOpenROMDialog ();
    
    if (filename)
    {
        gtk_entry_set_text (GTK_ENTRY (np_dialog->get_widget ("rom_image")), 
                            filename);
        
        g_free (filename);
    }
    
    return;
}

static void 
event_clear_clicked (GtkButton *button, gpointer data)
{
    Snes9xNetplayDialog *np_dialog = (Snes9xNetplayDialog *) data;
    
    gtk_entry_set_text (GTK_ENTRY (np_dialog->get_widget ("rom_image")), "");
                        
    return;
}

static void 
event_server_toggled (GtkToggleButton *toggle, gpointer data)
{
    Snes9xNetplayDialog *np_dialog = (Snes9xNetplayDialog *) data;
    
    np_dialog->update_state ();    
}

Snes9xNetplayDialog::Snes9xNetplayDialog (Snes9xConfig *config) :
    GladeWindow (snes9x_glade, snes9x_glade_size, "netplay_dialog")
{
    GladeWindowCallbacks callbacks[] =
    {
        { "server_toggled", G_CALLBACK (event_server_toggled) },
        { "browse_clicked", G_CALLBACK (event_browse_clicked) },
        { "clear_clicked", G_CALLBACK (event_clear_clicked) },
        { NULL, NULL }
    };
    
    signal_connect (callbacks);
    
    this->config = config;
    
    return;
}

void
Snes9xNetplayDialog::update_state (void)
{
    if (get_check ("host_radio"))
    {
        enable_widget ("connect_box", FALSE);
        enable_widget ("default_port_box", TRUE);
        enable_widget ("sync_reset", TRUE);
        enable_widget ("send_image", TRUE);        
    }
    
    else
    {
        enable_widget ("connect_box", TRUE);
        enable_widget ("default_port_box", FALSE);
        enable_widget ("sync_reset", FALSE);
        enable_widget ("send_image", FALSE);        
    }
    
    return;
}

void
Snes9xNetplayDialog::settings_to_dialog (void)
{
    set_entry_text ("rom_image", config->netplay_last_rom);
    set_entry_text ("ip_entry", config->netplay_last_host);
    set_check ("sync_reset", config->netplay_sync_reset);
    set_check ("send_image", config->netplay_send_rom);
    set_spin ("port", config->netplay_last_port);
    set_spin ("default_port", config->netplay_default_port);
    set_spin ("frames_behind", config->netplay_max_frame_loss);
    set_check ("connect_radio", !config->netplay_is_server);
    set_check ("host_radio", config->netplay_is_server);
    
    update_state ();
    
    return;
}

void
Snes9xNetplayDialog::settings_from_dialog (void)
{
    strncpy (config->netplay_last_rom, get_entry_text ("rom_image"), PATH_MAX);
    strncpy (config->netplay_last_host, get_entry_text ("ip_entry"), PATH_MAX);
    config->netplay_sync_reset = get_check ("sync_reset");
    config->netplay_send_rom = get_check ("send_image");
    config->netplay_last_port = get_spin ("port");
    config->netplay_default_port = get_spin ("default_port");
    config->netplay_max_frame_loss = get_spin ("frames_behind");
    config->netplay_is_server = get_check ("host_radio");
    
    config->save_config_file ();
    
    return;
}

int
Snes9xNetplayDialog::show (void)
{
    int result;
    
    settings_to_dialog ();
        
    result = gtk_dialog_run (GTK_DIALOG (window));

    gtk_widget_hide (window);
    
    if (result == GTK_RESPONSE_OK)
    {
        settings_from_dialog ();    
        
        return TRUE;
    }
    
    else
    {
        return FALSE;
    }
        
}

Snes9xNetplayDialog::~Snes9xNetplayDialog (void)
{
    gtk_widget_destroy (window);
}
