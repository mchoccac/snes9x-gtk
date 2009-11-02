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


#ifndef _APU_H_
#define _APU_H_

#include "../snes9x.h"
#include "SNES_SPC.h"

typedef struct
{
    bool8   stereo;                                 // stereo or mono

    bool8   sixteen_bit;                    // 16bit or 8bit sample
    bool8   disable_echo;                   // force echo disabled
    uint32  playback_rate;                  // 32000Hz is recommended
    bool8   snes_standard;                  // stereo && 16bit && echo && 32000Hz

    bool8   mute_sound;                             // force mute regardless of FLG
    uint8   sound_switch;                   // channel on/off
    uint16  stereo_switch;                  // stereo channel on/off
    double  pitch_mul;                              // used with Settings.FixFrequency

    int     buffer_size;                    // ** port specific
    int     sound_fd;                               // ** port specific
    bool8   encoded;                                // ** port specific
    int32   samples_mixed_so_far;   // ** port specific
    int32   play_position;                  // ** port specific
    uint32  err_counter;                    // ** port specific
    uint32  err_rate;                               // ** port specific
    uint32  input_rate;
} SoundStatus;

extern SoundStatus so;

typedef void (*samples_available_callback) (void *);

#define APU_DEFAULT_INPUT_RATE    31950
#define APU_MINIMUM_BUFFER_SIZE   8192

#define APU_NUMERATOR_NTSC        5632
#define APU_DENOMINATOR_NTSC      118125
#define APU_NUMERATOR_PAL         102400
#define APU_DENOMINATOR_PAL       2128137

#define SPC_SAVE_STATE_BLOCK_SIZE (SNES_SPC::state_size + 8)

/* Legacy defines */
#define SOUND_BUFFER_SIZE         2048
#define MAX_BUFFER_SIZE           SOUND_BUFFER_SIZE
#define SOUND_BUFFER_SIZE_MASK    (SOUND_BUFFER_SIZE - 1)

bool8 S9xInitAPU (void);
void S9xDeinitAPU (void);
void S9xResetAPU (void);
bool8 S9xInitSound (int, bool8, int);
bool8 S9xMixSamples (uint8 *, int);
void S9xSetSoundControl (uint8 sound_switch);
void S9xResetSound (bool8);
void S9xSetPlaybackRate (uint32);
bool8 S9xSetSoundMute (bool8);
bool8 S9xOpenSoundDevice (int, bool8, int);

void S9xAPUTimingSetSpeedup (int ticks);

void S9xAPUEndScanline (void);
void S9xAPUExecute (void);
void S9xAPUSetReferenceTime (int cpucycles);

int S9xAPUReadPort (int port);
void S9xAPUWritePort (int port, int byte);
void S9xAPULoadState (unsigned char *block);
void S9xAPUSaveState (unsigned char *block);

void S9xFinalizeSamples (void);
void S9xSetSamplesAvailableCallback (samples_available_callback callback, void *data);
int S9xGetSampleCount (void);
int S9xSyncSound (void);

extern SNES_SPC *spc_core;

#endif
