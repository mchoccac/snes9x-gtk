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

#include "gtk_s9xcore.h"

#include "gtk_s9x.h"
#include "gtk_cheat.h"

enum
{
    COLUMN_ENABLED = 0,
    COLUMN_DESCRIPTION = 1,
    COLUMN_CHEAT = 2,
    NUM_COLS
};

extern SCheatData Cheat;

static void
display_errorbox (const char *error)
{
    GtkWidget *dialog = gtk_message_dialog_new (NULL,
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_ERROR,
                                                GTK_BUTTONS_OK,
                                                error);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

    return;
}

static void
event_add_code (GtkButton *button, gpointer data)
{
    ((Snes9xCheats *) data)->add_code ();
}

static void
event_remove_code (GtkButton *button, gpointer data)
{
    ((Snes9xCheats *) data)->remove_code ();
}

static void
event_code_toggled (GtkCellRendererToggle *cell_renderer,
                    gchar                 *path,
                    gpointer               data)
{
    ((Snes9xCheats *) data)->toggle_code (path);

    return;
}

Snes9xCheats::Snes9xCheats (void)
    : GladeWindow (snes9x_glade, snes9x_glade_size, "cheat_window")
{
    GtkTreeView     *view;
    GtkCellRenderer *renderer;
    GladeWindowCallbacks callbacks[] =
    {
        { "add_code", G_CALLBACK (event_add_code) },
        { "remove_code", G_CALLBACK (event_remove_code) },
        { NULL, NULL}
    };

    view = GTK_TREE_VIEW (get_widget ("cheat_treeview"));


    renderer = gtk_cell_renderer_toggle_new ();
    gtk_tree_view_insert_column_with_attributes (view,
                                                 -1,
                                                 "",
                                                 renderer,
                                                 "active", COLUMN_ENABLED,
                                                 NULL);

    g_signal_connect (renderer,
                      "toggled",
                      G_CALLBACK (event_code_toggled),
                      (gpointer) this);

    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (view,
                                                 -1,
                                                 _("Description"),
                                                 renderer,
                                                 "text", COLUMN_DESCRIPTION,
                                                 NULL);

    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (view,
                                                 -1,
                                                 _("Cheat"),
                                                 renderer,
                                                 "text", COLUMN_CHEAT,
                                                 NULL);

    store = gtk_list_store_new (NUM_COLS,
                                G_TYPE_BOOLEAN,
                                G_TYPE_STRING,
                                G_TYPE_STRING);

    gtk_tree_view_set_model (view, GTK_TREE_MODEL (store));

    gtk_widget_realize (window);

    gtk_combo_box_set_active (GTK_COMBO_BOX (get_widget ("code_type_combo")),
                              0);

    signal_connect (callbacks);

    return;
}

Snes9xCheats::~Snes9xCheats (void)
{
    gtk_widget_destroy (window);

    g_object_unref (store);

    return;
}

void
Snes9xCheats::show (void)
{
    top_level->pause_from_focus_change ();

    gtk_window_set_transient_for (GTK_WINDOW (window),
                                  top_level->get_window ());

    refresh_tree_view ();
    gtk_dialog_run (GTK_DIALOG (window));

    top_level->unpause_from_focus_change ();

    return;
}

int
Snes9xCheats::get_selected_index (void)
{
    GtkTreeSelection *selection;
    GList            *rows;
    gint             *indices;
    int              index;
    GtkTreeModel     *model = GTK_TREE_MODEL (store);

    selection =
        gtk_tree_view_get_selection (
            GTK_TREE_VIEW (get_widget ("cheat_treeview")));

    rows = gtk_tree_selection_get_selected_rows (selection,
                                                 &model);

    if (rows)
    {
        indices = gtk_tree_path_get_indices ((GtkTreePath *) (rows->data));

        index = indices[0];

        for (GList *i = rows; i; i = i->next)
            gtk_tree_path_free ((GtkTreePath *)(i->data));

        g_list_free (rows);
    }
    else
        index = -1;

    return index;
}

int
Snes9xCheats::get_index_from_path (const gchar *path)
{
    GtkTreePath *tree_path = gtk_tree_path_new_from_string (path);
    gint        *indices   = gtk_tree_path_get_indices (tree_path);
    int         index      = indices[0];
    gtk_tree_path_free (tree_path);

    return index;
}

void
Snes9xCheats::refresh_tree_view (void)
{
    GtkTreeIter iter;
    char str [1024];

    gtk_list_store_clear (store);

    for (unsigned int i = 0; i < Cheat.num_cheats; i++)
    {
        snprintf (str,
                  1024,
                  "%06x:%02x/%02x",
                  Cheat.c [i].address,
                  Cheat.c [i].byte,
                  Cheat.c [i].saved_byte);

        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter,
                            COLUMN_DESCRIPTION,
                            !strcmp (Cheat.c [i].name, "") ? _("No description")
                                    : Cheat.c [i].name,
                            COLUMN_CHEAT, str,
                            COLUMN_ENABLED, Cheat.c [i].enabled,
                            -1);
    }


    return;
}

void
Snes9xCheats::add_code (void)
{
    uint32 address;
    uint8 byte;
    uint8 bytes [3];
    bool8 sram;
    uint8 num_bytes;
    const char *error;
    int errorno = 0;

    int type = get_combo ("code_type_combo");

    const gchar *code = get_entry_text ("code_entry");

    switch (type)
    {
        case TYPE_GAME_GENIE:
            if ((error = S9xGameGenieToRaw (code, address, byte)) == NULL)
                S9xAddCheat (TRUE, FALSE, address, byte);
            else
            {
                display_errorbox (error);
                errorno = 1;
            }
            break;

        case TYPE_ACTION_REPLAY:
            if ((error = S9xProActionReplayToRaw (code, address, byte)) == NULL)
                S9xAddCheat (TRUE, FALSE, address, byte);
            else
            {
                display_errorbox (error);
                errorno = 1;
            }

            break;

        case TYPE_GOLDFINGER:

            if ((error = S9xGoldFingerToRaw (code, address, sram,
                                             num_bytes, bytes)) == NULL)
            {
                for (int c = 0; c < num_bytes; c++)
                    S9xAddCheat (TRUE, FALSE, address + c, bytes [c]);
            }
            else
            {
                display_errorbox (error);
                errorno = 1;
            }

            break;
    }

    if (!errorno)
    {
        const char *entry_text = get_entry_text ("description_entry");

        if (strlen (entry_text) <= 0)
            strncpy (Cheat.c[Cheat.num_cheats - 1].name, _("No description"), 22);
        else
            strncpy (Cheat.c[Cheat.num_cheats - 1].name, entry_text, 22);
    }

    gtk_widget_grab_focus (get_widget ("code_entry"));

    refresh_tree_view ();

    return;
}

void
Snes9xCheats::remove_code (void)
{
    int index = get_selected_index ();

    if (index < 0)
        return;

    S9xDeleteCheat (index);

    refresh_tree_view ();

    return;
}

void
Snes9xCheats::toggle_code (const gchar *path)
{
    int index = get_index_from_path (path);

    if (index < 0)
        return;

    if (Cheat.c[index].enabled)
        S9xDisableCheat (index);
    else
        S9xEnableCheat (index);

    refresh_tree_view ();

    return;
}

