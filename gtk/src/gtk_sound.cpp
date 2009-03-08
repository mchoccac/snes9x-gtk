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

#include <errno.h>

#include "gtk_s9x.h"
#include "gtk_sound.h"
#include "gtk_sound_driver.h"

#ifdef USE_PORTAUDIO
#include "gtk_sound_driver_portaudio.h"
#endif
#ifdef USE_OSS
#include "gtk_sound_driver_oss.h"
#endif

int playback_rates[8] =
{
    0, 8000, 11025, 16000, 22050, 32000, 44100, 48000
};

double d_playback_rates[8] =
{
    0.0, 8000.0, 11025.0, 16000.0, 22050.0, 32000.0, 44100.0, 48000.0
};

S9xSoundDriver *driver;

void
S9xPortSoundInit (void)
{
#ifdef USE_PORTAUDIO
#ifdef USE_OSS
    if (gui_config->sound_driver == 0)
        driver = new S9xPortAudioSoundDriver ();
    else
        driver = new S9xOSSSoundDriver ();
#else
    driver = new S9xPortAudioSoundDriver ();
#endif
#endif
#ifdef USE_OSS
#ifndef USE_PORTAUDIO
    driver = new S9xOSSSoundDriver ();
#endif
#endif

    driver->init ();

    S9xInitSound (Settings.SoundPlaybackRate,
                  Settings.Stereo,
                  Settings.SoundBufferSize);

    S9xSetSoundMute (FALSE);

    return;
}

void
S9xPortSoundDeinit (void)
{
    S9xSoundStop ();

    driver->terminate ();

    delete driver;

    return;
}

void
S9xSoundStart (void)
{
    driver->start ();

    return;
}

void
S9xSoundStop (void)
{
    driver->stop ();

    return;
}

void
S9xGenerateSound (void)
{
    driver->mix ();

    return;
}

bool8
S9xOpenSoundDevice (int mode, bool8 stereo, int buffer_size)
{
    if (Settings.APUEnabled == FALSE)
        return FALSE;

    so.playback_rate = playback_rates[Settings.SoundPlaybackRate];
    S9xSetPlaybackRate (so.playback_rate);
    so.stereo = Settings.Stereo ? 1 : 0;
    so.sixteen_bit = Settings.SixteenBitSound;
    so.encoded = 1;

    so.buffer_size = (gui_config->sound_buffer_size * so.playback_rate) / 1000;

    if (so.stereo)
        so.buffer_size *= 2;
    if (so.sixteen_bit)
        so.buffer_size *= 2;
    if (so.buffer_size >= MAX_BUFFER_SIZE)
        so.buffer_size = MAX_BUFFER_SIZE;

    if (gui_config->mute_sound)
        return FALSE;

    return driver->open_device (mode, stereo, buffer_size);
}

/* This really shouldn't be in the port layer */
void
S9xToggleSoundChannel (int c)
{
    if (c == 8)
        so.sound_switch = 255;
    else
        so.sound_switch ^= 1 << c;

    S9xSetSoundControl (so.sound_switch);

    return;
}
