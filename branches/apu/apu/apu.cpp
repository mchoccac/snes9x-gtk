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

#include <math.h>
#include "snes9x.h"
#include "apu.h"
#include "display.h"
#include "resampler.h"
#include "ring_buffer.h"

#undef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static uint8 APUROM[64] =
{
        0xCD, 0xEF, 0xBD, 0xE8, 0x00, 0xC6, 0x1D, 0xD0,
        0xFC, 0x8F, 0xAA, 0xF4, 0x8F, 0xBB, 0xF5, 0x78,
        0xCC, 0xF4, 0xD0, 0xFB, 0x2F, 0x19, 0xEB, 0xF4,
        0xD0, 0xFC, 0x7E, 0xF4, 0xD0, 0x0B, 0xE4, 0xF5,
        0xCB, 0xF4, 0xD7, 0x00, 0xFC, 0xD0, 0xF3, 0xAB,
        0x01, 0x10, 0xEF, 0x7E, 0xF4, 0x10, 0xEB, 0xBA,
        0xF6, 0xDA, 0x00, 0xBA, 0xF4, 0xC4, 0xF4, 0xDD,
        0x5D, 0xD0, 0xDB, 0x1F, 0x00, 0x00, 0xC0, 0xFF
};

SNES_SPC *spc_core = NULL;

namespace spc
{
    static int playback_rates[8] =
    {
        0, 8000, 11025, 16000, 22050, 32000, 44100, 48000
    };

    static samples_available_callback sa_callback = NULL;
    static void *extra_data = NULL;

    static int buffer_size = 8192;

    static unsigned char *landing_buffer = NULL;
    static unsigned char *shrink_buffer = NULL;

    static ring_buffer *buffer = NULL;

    static Resampler *resampler = NULL;

    static int32 reference_time;
    static uint32 remainder;

    static const int32 timing_hack_numerator   = SNES_SPC::tempo_unit;
    static int32       timing_hack_denominator = SNES_SPC::tempo_unit;

    static int sound_in_sync = 1;
}

SoundStatus so;

static void
EightBitize (uint8 *buffer, int sample_count)
{
    uint8 *buf8 = (uint8 *) buffer;
    int16 *buf16 = (int16 *) buffer;

    for (int i = 0; i < sample_count; i++)
    {
        buf8[i] = (uint8) ((buf16[i] / 256) + 128);
    }

    return;
}

static void
DeStereo (uint8 *buffer, int sample_count)
{
    int16 *buf = (int16 *) buffer;
    int32 s1, s2;

    for (int i = 0; i < sample_count >> 1; i++)
    {
        s1 = (int32) buf[2 * i];
        s2 = (int32) buf[2 * i + 1];
        buf[i] = (int16) ((s1 + s2) >> 1);
    }

    return;
}

static void
ReverseStereo (uint8 *src_buffer, int sample_count)
{
    int16 *buffer = (int16 *) src_buffer;

    for (int i = 0; i < sample_count; i += 2)
    {
        buffer[i + 1] ^= buffer[i];
        buffer[i] ^= buffer[i + 1];
        buffer[i + 1] ^= buffer[i];
    }

    return;
}

bool8
S9xMixSamples (uint8 *buffer, int sample_count)
{
    unsigned char *dest;

    if (!so.sixteen_bit || !so.stereo)
    {
        if ((int) sizeof (spc::shrink_buffer) < (sample_count << (so.sixteen_bit ? 1 : 0)))
        {
            delete[] spc::shrink_buffer;
            spc::shrink_buffer = new unsigned char[sample_count << (so.sixteen_bit ? 1 : 0)];
        }

        dest = spc::shrink_buffer;

        if (!so.stereo)
            sample_count <<= 1;
    }
    else
        dest = buffer;


    if (so.mute_sound)
    {
        memset (dest, 0, sample_count << 1);
    }

    else if (so.playback_rate != so.input_rate)
    {
        int samples_to_write = MIN (spc::buffer->space_filled () >> 1,
                                    spc::resampler->max_write ());
        spc::buffer->pull ((unsigned char *) spc::resampler->buffer (),
                           samples_to_write << 1);
        spc::resampler->write (samples_to_write);

        if (spc::resampler->avail () >= sample_count)
        {
            spc::resampler->read ((short *) dest,
                                  sample_count);
        }
        else
        {
            memset (buffer, 0, sample_count << (so.sixteen_bit ? 1 : 0));
            return FALSE;
        }
    }

    else if (!spc::buffer->pull (dest, sample_count << 1))
    {
        memset (buffer, 0, sample_count << (so.sixteen_bit ? 1 : 0));

        return FALSE;
    }

    if (Settings.ReverseStereo && so.stereo)
    {
        ReverseStereo (dest, sample_count);
    }

    if (!so.stereo || !so.sixteen_bit)
    {
        if (!so.stereo)
        {
            DeStereo (spc::shrink_buffer, sample_count);
            sample_count >>= 1;
        }

        if (!so.sixteen_bit)
            EightBitize (spc::shrink_buffer, sample_count);

        memcpy (buffer, spc::shrink_buffer, (sample_count << (so.sixteen_bit ? 1 : 0)));
    }

    return TRUE;
}

int
S9xGetSampleCount (void)
{
    int samples = spc::buffer->space_filled () >> 1;

    samples >>= (so.stereo ? 0 : 1);

    samples *= so.playback_rate;
    samples /= so.input_rate;

    return samples;
}

void
S9xFinalizeSamples (void)
{
    if (!spc::buffer->push (spc::landing_buffer, spc_core->sample_count () << 1))
    {
        spc::sound_in_sync = 0;

        if (Settings.SoundSync && !Settings.TurboMode)
            return;
    }

    spc::sound_in_sync = 1;

    spc_core->set_output ((SNES_SPC::sample_t *) spc::landing_buffer,
                          spc::buffer_size);

    return;
}

void
S9xLandSamples (void)
{
    if (spc::sa_callback != NULL)
        spc::sa_callback (spc::extra_data);
    else
        S9xFinalizeSamples ();

    return;
}

int
S9xSyncSound (void)
{
    if (!Settings.SoundSync || spc::sound_in_sync)
        return TRUE;

    S9xLandSamples ();

    return spc::sound_in_sync;
}

void
S9xSetSamplesAvailableCallback (samples_available_callback callback, void *data)
{
    spc::sa_callback = callback;
    spc::extra_data = data;

    return;
}

bool8
S9xInitSound (int mode, bool8 stereo, int buffer_size)
{
    so.stereo = stereo;

    if (spc::buffer_size != buffer_size)
    {
        if (spc::landing_buffer)
        {
            spc::buffer_size = buffer_size;

            if (spc::buffer_size < 4096)
            {
                spc::buffer_size = 4096;
            }

            delete[] spc::landing_buffer;
            delete[] spc::shrink_buffer;
            delete spc::buffer;

            spc::landing_buffer = new unsigned char[spc::buffer_size * 2];
            spc::shrink_buffer  = new unsigned char[spc::buffer_size * 2];

            spc::buffer = new ring_buffer (spc::buffer_size);

            spc::resampler->buffer_size (spc::buffer_size);

            spc_core->set_output ((SNES_SPC::sample_t *) spc::landing_buffer,
                                  spc::buffer_size);
        }
    }

    S9xSetPlaybackRate (so.playback_rate);

    S9xOpenSoundDevice (mode, so.stereo, spc::buffer_size);

    return 1;
}

void
S9xSetPlaybackRate (uint32 playback_rate)
{
    if (playback_rate > 48000)
        playback_rate = 48000;

    if (playback_rate <= 7)
    {
        so.playback_rate = spc::playback_rates[playback_rate];
    }
    else
    {
        so.playback_rate = playback_rate;
    }

    spc::resampler->time_ratio (((double) so.input_rate) * (spc::timing_hack_numerator / spc::timing_hack_denominator) / ((double) so.playback_rate));

    delete[] spc::shrink_buffer;
    spc::shrink_buffer  = new unsigned char[spc::buffer_size * so.playback_rate / so.input_rate + 16];

    return;
}

void
S9xSetSoundControl (uint8 sound_switch)
{
    so.sound_switch ^= sound_switch;

    spc_core->mute_voices (so.sound_switch);

    return;
}

bool8
S9xSetSoundMute (bool8 mute)
{
    so.mute_sound = mute;

    return FALSE;
}

bool8
S9xInitAPU (void)
{
    spc_core = new SNES_SPC;
    spc_core->init ();
    spc_core->init_rom (APUROM);

    spc::landing_buffer = new unsigned char[spc::buffer_size * 2];
    spc::shrink_buffer = new unsigned char[spc::buffer_size * 2];

    spc::buffer = new ring_buffer (spc::buffer_size);

    spc::resampler = new Resampler (spc::buffer_size);
    spc::resampler->buffer_size (spc::buffer_size);

    so.input_rate = 32000;

    return TRUE;
}

static inline int
S9xAPUGetClock (int cpucycles)
{
    if (Settings.PAL)
        return floor ((double) APU_NUMERATOR_PAL   * spc::timing_hack_numerator * (cpucycles - spc::reference_time) + spc::remainder) /
                              (APU_DENOMINATOR_PAL * spc::timing_hack_denominator);
    else
        return (APU_NUMERATOR_NTSC   * spc::timing_hack_numerator * (cpucycles - spc::reference_time) + spc::remainder) /
               (APU_DENOMINATOR_NTSC * spc::timing_hack_denominator);
}

static inline int
S9xAPUGetClockRemainder (int cpucycles)
{
    if (Settings.PAL)
        return fmod ((double) APU_NUMERATOR_PAL   * spc::timing_hack_numerator * (cpucycles - spc::reference_time) + spc::remainder,
                              APU_DENOMINATOR_PAL * spc::timing_hack_denominator);
    else
        return (APU_NUMERATOR_NTSC   * spc::timing_hack_numerator * (cpucycles - spc::reference_time) + spc::remainder) %
               (APU_DENOMINATOR_NTSC * spc::timing_hack_denominator);
}

int
S9xAPUReadPort (int port)
{
    return spc_core->read_port (S9xAPUGetClock (CPU.Cycles), port);
}

void
S9xAPUWritePort (int port, int byte)
{
    spc_core->write_port (S9xAPUGetClock (CPU.Cycles), port, byte);
}

void
S9xAPUSetReferenceTime (int cpucycles)
{
    spc::reference_time = cpucycles;

    return;
}

void
S9xAPUExecute (void)
{
    /* Accumulate partial APU cycles */
    spc_core->end_frame (S9xAPUGetClock (CPU.Cycles));

    spc::remainder = S9xAPUGetClockRemainder (CPU.Cycles);

    S9xAPUSetReferenceTime (CPU.Cycles);
}

void
S9xAPUEndScanline (void)
{
    S9xAPUExecute ();

    S9xLandSamples ();

    return;
}

void
S9xAPUTimingSetSpeedup (int ticks)
{
    if (ticks != 0)
        printf("APU speedup hack: %d\n", ticks);

    spc_core->set_tempo (SNES_SPC::tempo_unit - ticks);

    spc::timing_hack_denominator = SNES_SPC::tempo_unit - ticks;

    S9xSetPlaybackRate (so.playback_rate);

    return;
}

void
S9xDeinitAPU (void)
{
    delete spc_core;
    spc_core = NULL;

    delete spc::resampler;
    spc::resampler = NULL;

    delete spc::buffer;
    spc::buffer = NULL;

    delete[] spc::landing_buffer;
    spc::landing_buffer = NULL;
    delete[] spc::shrink_buffer;
    spc::shrink_buffer = NULL;

    return;
}

void
S9xResetAPU (void)
{
    so.mute_sound = FALSE;
    spc::reference_time = 0;
    spc::remainder = 0;
    so.sound_switch = 0;
    spc_core->reset ();
    spc_core->set_output ((SNES_SPC::sample_t *) spc::landing_buffer,
                          spc::buffer_size);

    spc::buffer->clear ();
    spc::resampler->clear ();
    spc::buffer->cache_silence ();
}

/* State saving functions */
static void
from_apu_to_state (unsigned char **buf, void *var, size_t size)
{
    memcpy (*buf, var, size);

    *buf += size;
}

static void
to_apu_from_state (unsigned char **buf, void *var, size_t size)
{
    memcpy (var, *buf, size);

    *buf += size;
}

void
S9xAPUSaveState (unsigned char *block)
{
    unsigned char *ptr = block;

    spc_core->copy_state (&ptr, from_apu_to_state);

    SET_LE32 (ptr, spc::reference_time);
    ptr += sizeof (int);
    SET_LE32 (ptr, spc::remainder);
}

void
S9xAPULoadState (unsigned char *block)
{
    unsigned char *ptr = block;
    S9xResetAPU ();

    spc_core->copy_state (&ptr, to_apu_from_state);

    spc::reference_time = GET_LE32 (ptr);
    ptr += sizeof (int);
    spc::remainder = GET_LE32 (ptr);
}

