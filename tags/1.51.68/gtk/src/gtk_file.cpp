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

#include <sys/stat.h>
#include <gtk/gtk.h>

#include "gtk_s9x.h"

static char buf[256];

const char *
S9xChooseMovieFilename (bool8 read_only)
{
    if (!gui_config->rom_loaded)
        return strdup ("");

    return top_level->open_movie_dialog (read_only);
}

const char *
S9xChooseFilename (bool8 read_only)
{

    return strdup ("");
}

/* _splitpath/_makepath: Modified from unix.cpp. See file for credits. */
#ifndef SLASH_CHAR
#define SLASH_CHAR '/'
#endif

void
_splitpath (const char *path, char *drive, char *dir, char *fname, char *ext)
{
    char *slash = strrchr (path, SLASH_CHAR);
    char *dot   = strrchr (path, '.');

    *drive = '\0';

    if (dot && slash && dot < slash)
    {
        dot = 0;
    }

    if (!slash)
    {
        *dir = '\0';
        strcpy (fname, path);

        if (dot)
        {
            fname[dot - path] = '\0';
            strcpy (ext, dot + 1);
        }
        else
        {
            *ext = '\0';
        }
    }
    else
    {
        strcpy (dir, path);
        dir[slash - path] = '\0';
        strcpy (fname, slash + 1);

        if (dot)
        {
            fname[(dot - slash) - 1] = '\0';
            strcpy (ext, dot + 1);
        }
        else
        {
            *ext = '\0';
        }
    }

    return;
}

void
_makepath (char       *path,
           const char *drive,
           const char *dir,
           const char *fname,
           const char *ext)
{
    if (dir && *dir)
    {
        strcpy (path, dir);
        strcat (path, "/");
    }
    else
        *path = '\0';

    strcat (path, fname);

    if (ext && *ext)
    {
        strcat (path, ".");
        strcat (path, ext);
    }

    return;
}

const char *
S9xGetFilenameInc (const char *e, enum s9x_getdirtype dirtype)
{
    static char  filename[PATH_MAX + 1];
    char         dir[_MAX_DIR + 1];
    char         drive[_MAX_DRIVE + 1];
    char         fname[_MAX_FNAME + 1];
    char         ext[_MAX_EXT + 1];
    unsigned int i = 0;
    struct stat  buf;
    const char   *d;

    _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
    d = S9xGetDirectory (dirtype);

    do
    {
        snprintf (filename, sizeof (filename),
                  "%s" SLASH_STR "%s%03d%s", d, fname, i, e);
        i++;
    }
    while (stat (filename, &buf) == 0 && i != 0); /* Overflow? ...riiight :-) */

    return (filename);
}

const char *
S9xGetDirectory (enum s9x_getdirtype dirtype)
{
    static char path[PATH_MAX + 1];
    char        *config_dir = get_config_dir ();

    switch (dirtype)
    {
        default:
        case DEFAULT_DIR:
            strcpy (path, config_dir);
            break;

        case HOME_DIR:
            strcpy (path, getenv ("HOME"));
            break;

        case ROM_DIR:
            sprintf (path, "%s/roms", config_dir);
            break;

        case SNAPSHOT_DIR:
            sprintf (path, "%s/snapshots", config_dir);
            break;

        case PATCH_DIR:
        case SRAM_DIR:
        case ROMFILENAME_DIR:
            char *loc;

            strcpy (path, Memory.ROMFilename);

            loc = strrchr (path, SLASH_CHAR);

            if (loc == NULL)
            {
                getcwd (path, PATH_MAX + 1);
            }
            else
            {
                path[loc - path] = '\0';
            }

            if (gui_config->data_location == DIR_ROM ||
                dirtype == ROMFILENAME_DIR)
            {
                /* printf ("Got %s\n", path); */

                break;
            }

            if (gui_config->data_location == DIR_CONFIG)
                sprintf (path, "%s/sram", config_dir);
            else if (gui_config->data_location == DIR_CUSTOM)
                sprintf (path, "%s", gui_config->custom_sram_directory);

            break;
    }

    /* Try and mkdir, whether it exists or not */
    mkdir (path, 0777);

    free (config_dir);

    /* printf ("path: %s\n", path); */

    return path;
}

const char *
S9xGetFilename (const char *ex, enum s9x_getdirtype dirtype)
{
    static char filename[PATH_MAX + 1];
    char        dir[_MAX_DIR + 1];
    char        drive[_MAX_DRIVE + 1];
    char        fname[_MAX_FNAME + 1];
    char        ext[_MAX_EXT + 1];

    _splitpath (Memory.ROMFilename, drive, dir, fname, ext);

    snprintf (filename, sizeof (filename), "%s" SLASH_STR "%s%s",
              S9xGetDirectory (dirtype), fname, ex);

    return (filename);
}

const char *
S9xBasename (const char *f)
{
    const char *p;

    if ((p = strrchr (f, '/')) != NULL || (p = strrchr (f, '\\')) != NULL)
        return (p + 1);

    return f;
}

static int
file_exists (const char *name)
{
    FILE *f = NULL;

    f = fopen (name, "r");

    if (!f)
        return 0;
    else
    {
        fclose (f);
        return 1;
    }
}

bool8
S9xOpenSnapshotFile (const char *fname, bool8 read_only, STREAM *file)
{
    char filename [PATH_MAX + 1];
    char drive [_MAX_DRIVE + 1];
    char dir [_MAX_DIR + 1];
    char ext [_MAX_EXT + 1];

    _splitpath (fname, drive, dir, filename, ext);

    if (*drive || *dir == '/' || (*dir == '.' && (*(dir + 1) == '/')))
    {
        strncpy (filename, fname, PATH_MAX);

        if (!file_exists (filename))
        {
            if (!*ext)
                strcat (filename, ".s9x");
        }
    }
    else
    {
        strcpy (filename, S9xGetDirectory (SNAPSHOT_DIR));
        strcat (filename, SLASH_STR);
        strcat (filename, fname);

        if (!file_exists (filename))
        {
            if (!*ext)
                strcat (filename, ".s9x");
        }
    }

#ifdef ZLIB
    if (read_only)
    {
        if ((*file = OPEN_STREAM (filename, "rb")))
            return (TRUE);
    }
    else
    {
        if ((*file = OPEN_STREAM (filename, "wb")))
        {
            chown (filename, getuid (), getgid ());
            return (TRUE);
        }
    }
#else
    char command [PATH_MAX];

    if (read_only)
    {
        sprintf (command, "gzip -d <\"%s\"", filename);
        if (*file = popen (command, "r"))
            return (TRUE);
    }
    else
    {
        sprintf (command, "gzip --best >\"%s\"", filename);
        if (*file = popen (command, "wb"))
            return (TRUE);
    }
#endif
    return (FALSE);
}

void S9xCloseSnapshotFile (STREAM file)
{
#ifdef ZLIB
    CLOSE_STREAM (file);
#else
    pclose (file);
#endif
}

extern "C"
{
    uint8 snes9x_clear_change_log = 0;
}

extern "C" char *osd_GetPackDir (void)
{
    static char filename[_MAX_PATH];
    memset (filename, 0, _MAX_PATH);

    if (strlen (S9xGetDirectory (PATCH_DIR)) != 0)
        strcpy (filename, S9xGetDirectory (PATCH_DIR));
    else
    {
        char dir [_MAX_DIR + 1];
        char drive [_MAX_DRIVE + 1];
        char name [_MAX_FNAME + 1];
        char ext [_MAX_EXT + 1];
        _splitpath (Memory.ROMFilename, drive, dir, name, ext);
        _makepath (filename, drive, dir, NULL, NULL);
    }

    if (!strncmp ((char*) &Memory.ROM [0xffc0], "SUPER POWER LEAG 4   ", 21))
    {
        if (getenv ("SPL4PACK"))
            return getenv ("SPL4PACK");
        else
            strcat (filename, "/SPL4-SP7");
    }

    else if (!strncmp ((char*) &Memory.ROM [0xffc0], "MOMOTETSU HAPPY      ", 21))
    {
        if (getenv ("MDHPACK"))
            return getenv ("MDHPACK");
        else
            strcat (filename, "/SMHT-SP7");
    }

    else if (!strncmp((char*) &Memory.ROM [0xffc0], "HU TENGAI MAKYO ZERO ", 21))
    {
        if (getenv ("FEOEZPACK"))
            return getenv ("FEOEZPACK");
        else
            strcat (filename, "/FEOEZSP7");
    }

    else if (!strncmp ((char*) &Memory.ROM [0xffc0], "JUMP TENGAIMAKYO ZERO", 21))
    {
        if (getenv ("SJNSPACK"))
            return getenv ("SJNSPACK");
        else
            strcat (filename, "/SJUMPSP7");
    }

    else
        strcat (filename, "/MISC-SP7");

    return filename;
}

void
S9xLoadSDD1Data (void)
{
    Settings.SDD1Pack = true;

    return;
}

void
S9xAutoSaveSRAM (void)
{
    Memory.SaveSRAM (S9xGetFilename (".srm", SRAM_DIR));
    S9xSaveCheatFile (S9xGetFilename (".cht", PATCH_DIR));
    return;
}

void
S9xLoadState (const char *filename)
{
    if (S9xLoadSnapshot (filename))
    {
        sprintf (buf, "%s loaded", filename);
        S9xSetInfoString (buf);
    }
    else
    {
        fprintf (stderr, "Failed to load state file %s\n", filename);
    }

    return;
}

void
S9xSaveState (const char *filename)
{
    Snapshot (filename);

    sprintf (buf, "%s saved", filename);
    S9xSetInfoString (buf);

    return;
}

char *
S9xOpenROMDialog (void)
{
    GtkWidget     *dialog;
    GtkFileFilter *filter;
    char          *filename = NULL;
    gint          result;
    const char    *extensions[] =
    {
            "*.smc", "*.SMC", "*.fig", "*.FIG", "*.sfc", "*.SFC",
            "*.jma", "*.JMA", "*.zip", "*.ZIP", "*.gd3", "*.GD3",
            "*.swc", "*.SWC",
            NULL
    };

    top_level->pause_from_focus_change ();

    dialog = gtk_file_chooser_dialog_new ("Open SNES ROM",
                                          top_level->get_window (),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

    filter = gtk_file_filter_new ();
    gtk_file_filter_set_name (filter, "SNES ROMs");
    for (int i = 0; extensions[i]; i++)
    {
        gtk_file_filter_add_pattern (filter, extensions[i]);
    }
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

    filter = gtk_file_filter_new ();
    gtk_file_filter_set_name (filter, "All Files");
    gtk_file_filter_add_pattern (filter, "*.*");
    gtk_file_filter_add_pattern (filter, "*");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

    if (strcmp (gui_config->last_directory, ""))
    {
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),
                                             gui_config->last_directory);
    }

    result = gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_hide_all (dialog);

    if (result == GTK_RESPONSE_ACCEPT)
    {
        char *directory;

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        directory =
            gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (dialog));
        strncpy (gui_config->last_directory, directory, PATH_MAX);
        g_free (directory);
    }

    else
    {
        filename = NULL;
    }

    gtk_widget_destroy (dialog);

    top_level->unpause_from_focus_change ();

    return filename;
}

/* QuickSave/Load from S9x base controls.cpp */
void
S9xQuickSaveSlot (int slot)
{
    char def[PATH_MAX];
    char filename[PATH_MAX];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char ext[_MAX_EXT];

    _splitpath (Memory.ROMFilename, drive, dir, def, ext);

    sprintf (filename, "%s%s%s.%03d",
             S9xGetDirectory (SNAPSHOT_DIR), SLASH_STR, def,
             slot);

    sprintf (buf, "%s.%03d saved", def, slot);

    S9xSetInfoString (buf);

    Snapshot (filename);

    return;
}

void
S9xQuickLoadSlot (int slot)
{
    char def[PATH_MAX];
    char filename[PATH_MAX];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char ext[_MAX_EXT];

    _splitpath (Memory.ROMFilename, drive, dir, def, ext);

    sprintf (filename, "%s%s%s.%03d",
             S9xGetDirectory (SNAPSHOT_DIR), SLASH_STR, def,
             slot);

    if (S9xLoadSnapshot (filename))
    {
        sprintf (buf, "%s.%03d loaded", def, slot);
        S9xSetInfoString (buf);
    }
    else
    {
        static const char *digits = "t123456789";

        _splitpath (Memory.ROMFilename, drive, dir, def, ext);

        sprintf (filename, "%s%s%s.zs%c",
                 S9xGetDirectory (SNAPSHOT_DIR), SLASH_STR,
                 def, digits[slot]);

        if (S9xLoadSnapshot (filename))
        {
            sprintf (buf,
                     "Loaded ZSNES freeze file %s.zs%c",
                     def, digits [slot]);
            S9xSetInfoString (buf);
        }
        else
            S9xMessage (S9X_ERROR,
                        S9X_FREEZE_FILE_NOT_FOUND,
                        "Freeze file not found");
    }

    return;
}

