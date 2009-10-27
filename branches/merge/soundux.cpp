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


#include "snes9x.h"
#include "apu.h"
#include "soundux.h"

// gaussian table by libopenspc and SNEeSe
static const int32	gauss[512] =
{
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001,
	0x001, 0x001, 0x001, 0x002, 0x002, 0x002, 0x002, 0x002,
	0x002, 0x002, 0x003, 0x003, 0x003, 0x003, 0x003, 0x004,
	0x004, 0x004, 0x004, 0x004, 0x005, 0x005, 0x005, 0x005,
	0x006, 0x006, 0x006, 0x006, 0x007, 0x007, 0x007, 0x008,
	0x008, 0x008, 0x009, 0x009, 0x009, 0x00A, 0x00A, 0x00A,
	0x00B, 0x00B, 0x00B, 0x00C, 0x00C, 0x00D, 0x00D, 0x00E,
	0x00E, 0x00F, 0x00F, 0x00F, 0x010, 0x010, 0x011, 0x011,
	0x012, 0x013, 0x013, 0x014, 0x014, 0x015, 0x015, 0x016,
	0x017, 0x017, 0x018, 0x018, 0x019, 0x01A, 0x01B, 0x01B,
	0x01C, 0x01D, 0x01D, 0x01E, 0x01F, 0x020, 0x020, 0x021,
	0x022, 0x023, 0x024, 0x024, 0x025, 0x026, 0x027, 0x028,
	0x029, 0x02A, 0x02B, 0x02C, 0x02D, 0x02E, 0x02F, 0x030,
	0x031, 0x032, 0x033, 0x034, 0x035, 0x036, 0x037, 0x038,
	0x03A, 0x03B, 0x03C, 0x03D, 0x03E, 0x040, 0x041, 0x042,
	0x043, 0x045, 0x046, 0x047, 0x049, 0x04A, 0x04C, 0x04D,
	0x04E, 0x050, 0x051, 0x053, 0x054, 0x056, 0x057, 0x059,
	0x05A, 0x05C, 0x05E, 0x05F, 0x061, 0x063, 0x064, 0x066,
	0x068, 0x06A, 0x06B, 0x06D, 0x06F, 0x071, 0x073, 0x075,
	0x076, 0x078, 0x07A, 0x07C, 0x07E, 0x080, 0x082, 0x084,
	0x086, 0x089, 0x08B, 0x08D, 0x08F, 0x091, 0x093, 0x096,
	0x098, 0x09A, 0x09C, 0x09F, 0x0A1, 0x0A3, 0x0A6, 0x0A8,
	0x0AB, 0x0AD, 0x0AF, 0x0B2, 0x0B4, 0x0B7, 0x0BA, 0x0BC,
	0x0BF, 0x0C1, 0x0C4, 0x0C7, 0x0C9, 0x0CC, 0x0CF, 0x0D2,
	0x0D4, 0x0D7, 0x0DA, 0x0DD, 0x0E0, 0x0E3, 0x0E6, 0x0E9,
	0x0EC, 0x0EF, 0x0F2, 0x0F5, 0x0F8, 0x0FB, 0x0FE, 0x101,
	0x104, 0x107, 0x10B, 0x10E, 0x111, 0x114, 0x118, 0x11B,
	0x11E, 0x122, 0x125, 0x129, 0x12C, 0x130, 0x133, 0x137,
	0x13A, 0x13E, 0x141, 0x145, 0x148, 0x14C, 0x150, 0x153,
	0x157, 0x15B, 0x15F, 0x162, 0x166, 0x16A, 0x16E, 0x172,
	0x176, 0x17A, 0x17D, 0x181, 0x185, 0x189, 0x18D, 0x191,
	0x195, 0x19A, 0x19E, 0x1A2, 0x1A6, 0x1AA, 0x1AE, 0x1B2,
	0x1B7, 0x1BB, 0x1BF, 0x1C3, 0x1C8, 0x1CC, 0x1D0, 0x1D5,
	0x1D9, 0x1DD, 0x1E2, 0x1E6, 0x1EB, 0x1EF, 0x1F3, 0x1F8,
	0x1FC, 0x201, 0x205, 0x20A, 0x20F, 0x213, 0x218, 0x21C,
	0x221, 0x226, 0x22A, 0x22F, 0x233, 0x238, 0x23D, 0x241,
	0x246, 0x24B, 0x250, 0x254, 0x259, 0x25E, 0x263, 0x267,
	0x26C, 0x271, 0x276, 0x27B, 0x280, 0x284, 0x289, 0x28E,
	0x293, 0x298, 0x29D, 0x2A2, 0x2A6, 0x2AB, 0x2B0, 0x2B5,
	0x2BA, 0x2BF, 0x2C4, 0x2C9, 0x2CE, 0x2D3, 0x2D8, 0x2DC,
	0x2E1, 0x2E6, 0x2EB, 0x2F0, 0x2F5, 0x2FA, 0x2FF, 0x304,
	0x309, 0x30E, 0x313, 0x318, 0x31D, 0x322, 0x326, 0x32B,
	0x330, 0x335, 0x33A, 0x33F, 0x344, 0x349, 0x34E, 0x353,
	0x357, 0x35C, 0x361, 0x366, 0x36B, 0x370, 0x374, 0x379,
	0x37E, 0x383, 0x388, 0x38C, 0x391, 0x396, 0x39B, 0x39F,
	0x3A4, 0x3A9, 0x3AD, 0x3B2, 0x3B7, 0x3BB, 0x3C0, 0x3C5,
	0x3C9, 0x3CE, 0x3D2, 0x3D7, 0x3DC, 0x3E0, 0x3E5, 0x3E9,
	0x3ED, 0x3F2, 0x3F6, 0x3FB, 0x3FF, 0x403, 0x408, 0x40C,
	0x410, 0x415, 0x419, 0x41D, 0x421, 0x425, 0x42A, 0x42E,
	0x432, 0x436, 0x43A, 0x43E, 0x442, 0x446, 0x44A, 0x44E,
	0x452, 0x455, 0x459, 0x45D, 0x461, 0x465, 0x468, 0x46C,
	0x470, 0x473, 0x477, 0x47A, 0x47E, 0x481, 0x485, 0x488,
	0x48C, 0x48F, 0x492, 0x496, 0x499, 0x49C, 0x49F, 0x4A2,
	0x4A6, 0x4A9, 0x4AC, 0x4AF, 0x4B2, 0x4B5, 0x4B7, 0x4BA,
	0x4BD, 0x4C0, 0x4C3, 0x4C5, 0x4C8, 0x4CB, 0x4CD, 0x4D0,
	0x4D2, 0x4D5, 0x4D7, 0x4D9, 0x4DC, 0x4DE, 0x4E0, 0x4E3,
	0x4E5, 0x4E7, 0x4E9, 0x4EB, 0x4ED, 0x4EF, 0x4F1, 0x4F3,
	0x4F5, 0x4F6, 0x4F8, 0x4FA, 0x4FB, 0x4FD, 0x4FF, 0x500,
	0x502, 0x503, 0x504, 0x506, 0x507, 0x508, 0x50A, 0x50B,
	0x50C, 0x50D, 0x50E, 0x50F, 0x510, 0x511, 0x511, 0x512,
	0x513, 0x514, 0x514, 0x515, 0x516, 0x516, 0x517, 0x517,
	0x517, 0x518, 0x518, 0x518, 0x518, 0x518, 0x519, 0x519
};

#define	G1(n)	gauss[256 + (n)]
#define	G2(n)	gauss[512 + (n)]
#define	G3(n)	gauss[255 + (n)]
#define	G4(n)	gauss[ -1 + (n)]

// envelope/noise table by libopenspc and SNEeSe
static int32	env_counter_table[32] =
{
	0x0000, 0x000F, 0x0014, 0x0018, 0x001E, 0x0028, 0x0030, 0x003C,
	0x0050, 0x0060, 0x0078, 0x00A0, 0x00C0, 0x00F0, 0x0140, 0x0180,
	0x01E0, 0x0280, 0x0300, 0x03C0, 0x0500, 0x0600, 0x0780, 0x0A00,
	0x0C00, 0x0F00, 0x1400, 0x1800, 0x1E00, 0x2800, 0x3C00, 0x7800
};

static int32		env_counter_max;
static const int32	env_counter_max_master = 0x7800;

static int32		msample[SOUND_BUFFER_SIZE];
static int32		esample[SOUND_BUFFER_SIZE];
static int32		dummy_esample[SOUND_BUFFER_SIZE];
static int32		wave[SOUND_BUFFER_SIZE];
static uint16		noise_cache[0x1000];

#define	FIXED_POINT 		0x10000UL
#define	FIXED_POINT_SHIFT	16

#define ENV_RANGE			0x800
#define ENV_MAX				0x7FF
#define ENV_SHIFT			4

static inline int32 absolute (int32);
static inline int32 sclip15 (int32);
static inline int32 sclamp8 (int32);
static inline int32 sclamp15 (int32);
static inline int32 sclamp16 (int32);
static inline uint8 * sample_address (uint8);
static inline void set_echo_delay_and_ring_buffer (void);
static inline void set_env_rate (Channel *, int32, int32);
static inline void set_env_height (Channel *, int32);
static inline void set_adsr_rate (Channel *, int, int, int);
static inline void set_normalized_pitch (int, uint32);
static inline void brr_decode_block (Channel *);
static inline void mix_stereo (int);
static inline void mix_mono (int);
static inline void mix_nonstandard_samples (uint8 *, int);
static void restore_sound_data (void);
static void convert_sound_data (int);


static inline int32 absolute (int32 x)
{
	return ((x < 0) ? -x : x);
}

static inline int32 sclip15 (int32 x)
{
	return ((x & 16384) ? (x | ~16383) : (x & 16383));
}

static inline int32 sclamp8 (int32 x)
{
	return ((x > 127) ? 127 : (x < -128) ? -128 : x);
}

static inline int32 sclamp15 (int32 x)
{
	return ((x > 16383) ? 16383 : (x < -16384) ? -16384 : x);
}

static inline int32 sclamp16 (int32 x)
{
	return ((x > 32767) ? 32767 : (x < -32768) ? -32768 : x);
}

static inline uint8 * sample_address (uint8 srcn)
{
	return (IAPU.RAM + ((APU.DSP[APU_DIR] << 8) + (srcn << 2)));
}

void S9xSoundSetVolume (int channel, int32 volume_left, int32 volume_right)
{
	Channel	*ch = &SoundData.channels[channel];

	if (so.stereo_switch + 1)
	{
		volume_left  = ((so.stereo_switch & (  1 << channel)) ? volume_left  : 0);
		volume_right = ((so.stereo_switch & (256 << channel)) ? volume_right : 0);
	}

	if (!so.stereo)
		volume_left = (absolute(volume_right) + absolute(volume_left)) >> 1;

	ch->volume_left  = volume_left;
	ch->volume_right = volume_right;
}

void S9xSoundSetMasterVolume (int32 volume_left, int32 volume_right)
{
	if (!so.stereo)
		volume_left = (absolute(volume_right) + absolute(volume_left)) >> 1;

	SoundData.master_volume[Settings.ReverseStereo]     = volume_left;
	SoundData.master_volume[1 ^ Settings.ReverseStereo] = volume_right;
}

void S9xSoundSetEchoVolume (int32 volume_left, int32 volume_right)
{
	if (!so.stereo)
		volume_left = (absolute(volume_right) + absolute(volume_left)) >> 1;

	SoundData.echo_volume[Settings.ReverseStereo]     = volume_left;
	SoundData.echo_volume[1 ^ Settings.ReverseStereo] = volume_right;
}

void S9xSoundSetEchoEnable (uint8 byte)
{
	for (int c = 0; c < 8; c++)
		SoundData.channels[c].esample_buffer = (byte & (1 << c)) ? esample : dummy_esample;
}

void S9xSoundSetEchoFeedback (int32 feedback)
{
	SoundData.echo_feedback = feedback;
}

static inline void set_echo_delay_and_ring_buffer (void)
{
	if (SoundData.echo_ring_size == 0)
		SoundData.echo_ring_size = 2;

	if (so.snes_standard)
	{
		// The echo buffer wraps within 16 bits, if the buffer would go beyond address $FFFF.
		if ((APU.DSP[APU_ESA] << 8) + (SoundData.echo_ring_size << 1) > 0x10000)
			SoundData.echo_ring_size = (0x10000 - (APU.DSP[APU_ESA] << 8)) >> 1;
	}
	else
	if (!so.stereo)
		SoundData.echo_ring_size >>= 1;

	if (SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
		SoundData.echo_ring_pointer = 0;
}

void S9xSoundSetEchoDelay (uint32 delay)
{
	SoundData.echo_ring_buffer = (int16 *) (IAPU.RAM + (APU.DSP[APU_ESA] << 8));
	SoundData.echo_ring_size = (int32) ((delay << 10) * so.playback_rate / 32000);
	set_echo_delay_and_ring_buffer();
}

void S9xSoundSetEchoRingBufferAddress (uint8 byte)
{
	SoundData.echo_ring_buffer = (int16 *) (IAPU.RAM + (byte << 8));
	SoundData.echo_ring_size = (int32) (((APU.DSP[APU_EDL] & 0x0F) << 10) * so.playback_rate / 32000);
	set_echo_delay_and_ring_buffer();
}

void S9xSoundSetEchoWriteEnable (uint8 byte)
{
	SoundData.echo_write_enabled = byte ? FALSE : TRUE;
}

void S9xSoundSetFilterCoefficient (int tap, int32 value)
{
	SoundData.fir_tap[tap & 7] = value;
	SoundData.no_filter =
		SoundData.fir_tap[0] == 127 &&
		SoundData.fir_tap[1] == 0   &&
		SoundData.fir_tap[2] == 0   &&
		SoundData.fir_tap[3] == 0   &&
		SoundData.fir_tap[4] == 0   &&
		SoundData.fir_tap[5] == 0   &&
		SoundData.fir_tap[6] == 0   &&
		SoundData.fir_tap[7] == 0;
}

void S9xSoundSetMute (uint8 byte)
{
	SoundData.mute = byte ? TRUE : FALSE;
}

static inline void set_env_rate (Channel *ch, int32 rate_count, int32 xtarget)
{
	ch->xenvx_target = xtarget;
	ch->xenv_rate = rate_count;
}

static inline void set_env_height (Channel *ch, int32 xlevel)
{
	ch->xenvx = ch->xenvx_target = xlevel;
	ch->xenv_rate = 0;
}

static inline void set_adsr_rate (Channel *ch, int ar, int dr, int sr)
{
	ch->xattack_rate  = env_counter_table[(ar << 1) + 0x01];
	ch->xdecay_rate   = env_counter_table[(dr << 1) + 0x10];
	ch->xsustain_rate = env_counter_table[sr];

	switch (ch->state)
	{
		case SOUND_ATTACK:
			set_env_rate(ch, ch->xattack_rate, ENV_MAX);
			break;

		case SOUND_DECAY:
			set_env_rate(ch, ch->xdecay_rate, ch->xsustain_level);
			break;

		case SOUND_SUSTAIN:
			set_env_rate(ch, ch->xsustain_rate, 0);
			break;
	}
}

void S9xSoundSetSustainLevel (int channel, int n)
{
	SoundData.channels[channel].xsustain_level = (ENV_RANGE >> 3) * (n + 1);
}

void S9xSoundSetNoiseRate (int n)
{
	SoundData.noise_rate = env_counter_table[n];
}

void S9xSoundSetType (uint8 byte)
{
	for (int c = 0; c < 8; c++)
		SoundData.channels[c].type = (byte & (1 << c)) ? SOUND_NOISE : SOUND_SAMPLE;
}

static inline void set_normalized_pitch (int channel, uint32 pitch)
{
	if (so.playback_rate)
		SoundData.channels[channel].frequency = (uint32) ((int64) (pitch << (FIXED_POINT_SHIFT - 12)) * 32000 / so.playback_rate);
	if (Settings.FixFrequency)
		SoundData.channels[channel].frequency = (uint32) (SoundData.channels[channel].frequency * so.pitch_mul);
}

void S9xSoundSetPitch (int channel, uint16 pitch)
{
	SoundData.channels[channel].pitch = pitch;
	set_normalized_pitch(channel, pitch);
}

void S9xSoundSetSilence (int channel)
{
	Channel	*ch = &SoundData.channels[channel];

	if (ch->state != SOUND_SILENT)
	{
		ch->state = SOUND_SILENT;
		ch->xenvx = 0;
	}
}

void S9xSoundSetRelease (int channel)
{
	Channel	*ch = &SoundData.channels[channel];

	if ((ch->state != SOUND_SILENT) && (ch->state != SOUND_RELEASE))
	{
		ch->state = SOUND_RELEASE;
		set_env_rate(ch, env_counter_max, 0);
	}
}

void S9xSoundSetAttack (int channel)
{
	Channel	*ch = &SoundData.channels[channel];
	int		ofs = channel << 4;

	ch->xenvx = 0;
	ch->state = (APU.DSP[APU_ADSR1 + ofs] & 0x80) ? SOUND_ATTACK : SOUND_ADJUST;
	S9xSoundAdjustEnvelope(channel, APU.DSP[APU_GAIN + ofs], APU.DSP[APU_ADSR1 + ofs], APU.DSP[APU_ADSR2 + ofs]);

	ch->sample_number = APU.DSP[APU_SRCN + channel * 0x10];
	ch->block_pointer = READ_WORD(sample_address(ch->sample_number));
	ch->needs_decode = TRUE;
	ch->sample_pointer = 0;
	ch->previous[0] = 0;
	ch->previous[1] = 0;

	ch->xenv_count = env_counter_max;
	ch->xsmp_count = 0;
	ch->nb_sample[0] = 0;
	ch->nb_sample[1] = 0;
	ch->nb_sample[2] = 0;
	ch->nb_sample[3] = 0;
	ch->nb_index = 0;
}

void S9xSoundFinishRapidAttack (int channel)
{
	Channel	*ch = &SoundData.channels[channel];

	// Hack for Final Fantasy 4
	// Due to Snes9x's totally wrong APU-DSP timings,
	// calling S9xSoundAdjustEnvelope() from S9xSetAPUDSP() easily overwrites ch->state
	// before mix_stereo() is called.
	if ((ch->state == SOUND_ATTACK) && (ch->xattack_rate == env_counter_max_master))
	{
		ch->xenvx = ENV_MAX;
		if (ch->xsustain_level == ENV_RANGE)
		{
			ch->state = SOUND_SUSTAIN;
			set_env_rate(ch, ch->xsustain_rate, 0);
		}
		else
		{
			ch->state = SOUND_DECAY;
			set_env_rate(ch, ch->xdecay_rate, ch->xsustain_level);
		}
	}
}

void S9xSoundAdjustEnvelope (int channel, uint8 gain, uint8 adsr1, uint8 adsr2)
{
	Channel	*ch = &SoundData.channels[channel];

	if (ch->state == SOUND_SILENT || ch->state == SOUND_RELEASE)
		return;
	else
	if (adsr1 & 0x80)
		set_adsr_rate(ch, adsr1 & 0x0F, (adsr1 >> 4) & 0x07, adsr2 & 0x1F);
	else
	if (gain & 0x80)
	{
		switch (gain & 0x60)
		{
			case 0x00:
				ch->state = SOUND_DECREASE_LINEAR;
				set_env_rate(ch, env_counter_table[gain & 0x1F], 0);
				break;

			case 0x20:
				ch->state = SOUND_DECREASE_EXPONENTIAL;
				set_env_rate(ch, env_counter_table[gain & 0x1F], 0);
				break;

			case 0x40:
				ch->state = SOUND_INCREASE_LINEAR;
				set_env_rate(ch, env_counter_table[gain & 0x1F], ENV_MAX);
				break;

			case 0x60:
				ch->state = SOUND_INCREASE_BENT_LINE;
				set_env_rate(ch, env_counter_table[gain & 0x1F], ENV_MAX);
				break;
		}
	}
	else
	{
		ch->state = SOUND_DIRECT;
		set_env_height(ch, (gain & 0x7F) << ENV_SHIFT);
	}
}

static inline void brr_decode_block (Channel *ch)
{
	uint8	filter;
	int16	*raw = ch->decoded;
	int8	*compressed = (int8 *) &IAPU.RAM[ch->block_pointer];

	if (ch->block_pointer > 0x10000 - 9)
	{
		for (int i = 0; i < 16; i++)
			*raw++ = 0;
		ch->previous[0] = 0;
		ch->previous[1] = 0;
		ch->last_block = TRUE;
		ch->loop = FALSE;
		return;
	}

	filter = (uint8) *compressed++;
	ch->last_block = filter & 1;
	ch->loop = (filter & 2) != 0;

	if (ch->state == SOUND_SILENT)
	{
		for (int i = 0; i < 16; i++)
			*raw++ = 0;
		ch->previous[0] = 0;
		ch->previous[1] = 0;
	}
	else
	{
		int32	out, S1, S2;
		int8	sample1, sample2;
		uint8	shift;
		bool8	valid_header;

		shift = filter >> 4;
		valid_header = (shift < 0xD);
		filter &= 0x0C;

		S1 = ch->previous[0];
		S2 = ch->previous[1];

		for (int i = 0; i < 8; i++)
		{
			sample1 = *compressed++;
			sample2 = sample1 << 4;
			sample1 >>= 4;
			sample2 >>= 4;

			for (int nybble = 0; nybble < 2; nybble++)
			{
				out = nybble ? (int32) sample2 : (int32) sample1;
				out = valid_header ? ((out << shift) >> 1) : (out & ~0x7FF);

				switch (filter)
				{
					case 0x00: // Direct
						break;

					case 0x04: // 15/16
						out += S1 + ((-S1) >> 4);
						break;

					case 0x08: // 61/32 - 15/16
						out += (S1 << 1) + ((-((S1 << 1) + S1)) >> 5) - S2 + (S2 >> 4);
						break;

					case 0x0C: // 115/64 - 13/16
						out += (S1 << 1) + ((-(S1 + (S1 << 2) + (S1 << 3))) >> 6) - S2 + (((S2 << 1) + S2) >> 4);
						break;
				}

				out = sclip15(sclamp16(out));

				S2 = S1;
				S1 = out;
				*raw++ = (int16) out;
			}
		}

		ch->previous[0] = S1;
		ch->previous[1] = S2;
	}

	ch->block_pointer += 9;
}

static inline void mix_stereo (int sample_count)
{
	APU.OUTX_ENVX_pos = 0;
	APU.OUTX_ENVX_samples = sample_count >> 1;
	if (APU.OUTX_ENVX_samples > MAX_OUTX_ENVX_BUFFER)
		APU.OUTX_ENVX_samples = MAX_OUTX_ENVX_BUFFER;
	else
	if (APU.OUTX_ENVX_samples == 0)
		APU.OUTX_ENVX_samples = 1;

	uint8	pitch_mod = APU.DSP[APU_PMON] & ~APU.DSP[APU_NON] & ~1;

	int32	noise_index = 0;
	int32	noise_count = 0;

	if (APU.DSP[APU_NON])
	{
		noise_cache[0] = SoundData.noise_seed;
		noise_count = SoundData.noise_count;

		for (int I = 0; I < sample_count; I += 2)
		{
			noise_count -= SoundData.noise_rate;
			while (noise_count <= 0)
			{
				SoundData.noise_seed = (SoundData.noise_seed >> 1) | (((SoundData.noise_seed << 14) ^ (SoundData.noise_seed << 13)) & 0x4000);
				noise_index = (noise_index + 1) & 0xFFF;
				noise_cache[noise_index] = SoundData.noise_seed;
				noise_count += env_counter_max;
			}
		}
	}

	for (int J = 0; J < 8; J++)
	{
		Channel	*ch = &SoundData.channels[J];
		uint32	freq = ch->frequency;

		if (ch->type == SOUND_NOISE)
		{
			noise_index = 0;
			noise_count = SoundData.noise_count;
		}

		bool8	mod1 = pitch_mod & (1 << J);
		bool8	mod2 = pitch_mod & (1 << (J + 1));
		bool8	sound_switch = so.sound_switch & (1 << J);

		if (ch->needs_decode)
		{
			brr_decode_block(ch);
			ch->sample_pointer = 0;
			ch->sample = ch->decoded[0];
			ch->needs_decode = FALSE;
		}

		for (int I = 0; I < sample_count; I += 2)
		{
			switch (ch->state)
			{
				case SOUND_ATTACK:
					if (ch->xenv_rate == env_counter_max_master)
						ch->xenvx += (ENV_RANGE >> 1);
					else
					{
						ch->xenv_count -= ch->xenv_rate;
						while (ch->xenv_count <= 0)
						{
							ch->xenvx += (ENV_RANGE >> 6); // 1/64
							ch->xenv_count += env_counter_max;
						}
					}

					if (ch->xenvx > ENV_MAX)
					{
						ch->xenvx = ENV_MAX;

						if (ch->xsustain_level != ENV_RANGE)
						{
							ch->state = SOUND_DECAY;
							set_env_rate(ch, ch->xdecay_rate, ch->xsustain_level);
						}
						else
						{
							ch->state = SOUND_SUSTAIN;
							set_env_rate(ch, ch->xsustain_rate, 0);
						}
					}

					break;

				case SOUND_DECAY:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= ((ch->xenvx - 1) >> 8) + 1; // 1 - 1/256
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= ch->xenvx_target)
					{
						if (ch->xenvx <= 0)
							ch->xenvx = 0;

						ch->state = SOUND_SUSTAIN;
						set_env_rate(ch, ch->xsustain_rate, 0);
					}

					break;

				case SOUND_SUSTAIN:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= ((ch->xenvx - 1) >> 8) + 1;  // 1 - 1/256
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= 0)
						ch->xenvx = 0;

					break;

				case SOUND_RELEASE:
					ch->xenv_count -= env_counter_max;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= (ENV_RANGE >> 8); // 1/256
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= 0)
						S9xSoundSetSilence(J);

					break;

				case SOUND_INCREASE_LINEAR:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx += (ENV_RANGE >> 6); // 1/64
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx > ENV_MAX)
					{
						ch->xenvx = ENV_MAX;
						ch->state = SOUND_DIRECT;
						set_env_rate(ch, 0, 0);
					}

					break;

				case SOUND_INCREASE_BENT_LINE:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						if (ch->xenvx >= ((ENV_RANGE * 3) >> 2)) // 0x600
							ch->xenvx += (ENV_RANGE >> 8); // 1/256
						else
							ch->xenvx += (ENV_RANGE >> 6); // 1/64

						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx > ENV_MAX)
					{
						ch->xenvx = ENV_MAX;
						ch->state = SOUND_DIRECT;
						set_env_rate(ch, 0, 0);
					}

					break;

				case SOUND_DECREASE_LINEAR:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= (ENV_RANGE >> 6); // 1/64
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= 0)
						ch->xenvx = 0;

					break;

				case SOUND_DECREASE_EXPONENTIAL:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= ((ch->xenvx - 1) >> 8) + 1; // 1 - 1/256
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= 0)
						ch->xenvx = 0;

					break;

				case SOUND_DIRECT:
					set_env_rate(ch, 0, 0);

					break;
			}

			ch->xsmp_count += mod1 ? (((int64) freq * (32768 + wave[I >> 1])) >> 15) : freq;

			while (ch->xsmp_count >= 0)
			{
				ch->xsmp_count -= FIXED_POINT;
				ch->nb_sample[ch->nb_index] = ch->sample;
				ch->nb_index = (ch->nb_index + 1) & 3;

				ch->sample_pointer++;
				if (ch->sample_pointer == 16)
				{
					ch->sample_pointer = 0;

					if (ch->last_block)
					{
						APU.DSP[APU_ENDX] |= (1 << J);
						ch->block_pointer = READ_WORD(sample_address(ch->sample_number) + 2);
						if (!ch->loop)
							S9xSoundSetSilence(J);
						else
							ch->sample_number = APU.DSP[APU_SRCN + J * 0x10];
					}

					brr_decode_block(ch);
				}

				ch->sample = ch->decoded[ch->sample_pointer];
			}

			int32	outx, d;

			if (ch->type == SOUND_SAMPLE)
			{
				if (Settings.InterpolatedSound)
				{
					d = ch->xsmp_count >> (FIXED_POINT_SHIFT - 8);
					outx  = ((G4(-d) * ch->nb_sample[ ch->nb_index         ]) >> 11);
					outx += ((G3(-d) * ch->nb_sample[(ch->nb_index + 1) & 3]) >> 11);
					outx += ((G2( d) * ch->nb_sample[(ch->nb_index + 2) & 3]) >> 11);
					outx  = sclip15(outx);
					outx += ((G1( d) * ch->nb_sample[(ch->nb_index + 3) & 3]) >> 11);
					outx  = sclamp15(outx);
				}
				else
					outx = ch->sample;
			}
			else
			{
				noise_count -= SoundData.noise_rate;
				while (noise_count <= 0)
				{
					noise_count += env_counter_max;
					noise_index = (noise_index + 1) & 0xFFF;
				}

 				outx = sclip15(noise_cache[noise_index]);
			}

			outx = (outx * ch->xenvx) >> 11;

			if ((I >> 1) < MAX_OUTX_ENVX_BUFFER)
			{
				APU.OUTX_buffer[J][I >> 1] = outx >> 7;
				APU.ENVX_buffer[J][I >> 1] = (ch->xenvx >> ENV_SHIFT) & 0x7F;
			}

			if (mod2)
				wave[I >> 1] = outx << 1;

			int32	VL, VR;

			if (sound_switch)
			{
				VL = ((outx * ch->volume_left ) >> 7) << 1;
				VR = ((outx * ch->volume_right) >> 7) << 1;
			}
			else
				VL = VR = 0;

			msample[I      ^ Settings.ReverseStereo ] += VL;
			msample[I + (1 ^ Settings.ReverseStereo)] += VR;
			ch->esample_buffer[I      ^ Settings.ReverseStereo ] += VL;
			ch->esample_buffer[I + (1 ^ Settings.ReverseStereo)] += VR;
		}
	}

	if (APU.DSP[APU_NON])
		SoundData.noise_count = noise_count;
}

static inline void mix_mono (int sample_count)
{
	APU.OUTX_ENVX_pos = 0;
	APU.OUTX_ENVX_samples = sample_count;
	if (APU.OUTX_ENVX_samples > MAX_OUTX_ENVX_BUFFER)
		APU.OUTX_ENVX_samples = MAX_OUTX_ENVX_BUFFER;
	else
	if (APU.OUTX_ENVX_samples == 0)
		APU.OUTX_ENVX_samples = 1;

	uint8	pitch_mod = APU.DSP[APU_PMON] & ~APU.DSP[APU_NON] & ~1;

	int32	noise_index = 0;
	int32	noise_count = 0;

	if (APU.DSP[APU_NON])
	{
		noise_cache[0] = SoundData.noise_seed;
		noise_count = SoundData.noise_count;

		for (int I = 0; I < sample_count; I++)
		{
			noise_count -= SoundData.noise_rate;
			while (noise_count <= 0)
			{
				SoundData.noise_seed = (SoundData.noise_seed >> 1) | (((SoundData.noise_seed << 14) ^ (SoundData.noise_seed << 13)) & 0x4000);
				noise_index = (noise_index + 1) & 0xFFF;
				noise_cache[noise_index] = SoundData.noise_seed;
				noise_count += env_counter_max;
			}
		}
	}

	for (int J = 0; J < 8; J++)
	{
		Channel	*ch = &SoundData.channels[J];
		uint32	freq = ch->frequency;

		if (ch->type == SOUND_NOISE)
		{
			noise_index = 0;
			noise_count = SoundData.noise_count;
		}

		bool8	mod1 = pitch_mod & (1 << J);
		bool8	mod2 = pitch_mod & (1 << (J + 1));
		bool8	sound_switch = so.sound_switch & (1 << J);

		if (ch->needs_decode)
		{
			brr_decode_block(ch);
			ch->sample_pointer = 0;
			ch->sample = ch->decoded[0];
			ch->needs_decode = FALSE;
		}

		for (int I = 0; I < sample_count; I++)
		{
			switch (ch->state)
			{
				case SOUND_ATTACK:
					if (ch->xenv_rate == env_counter_max_master)
						ch->xenvx += (ENV_RANGE >> 1);
					else
					{
						ch->xenv_count -= ch->xenv_rate;
						while (ch->xenv_count <= 0)
						{
							ch->xenvx += (ENV_RANGE >> 6); // 1/64
							ch->xenv_count += env_counter_max;
						}
					}

					if (ch->xenvx > ENV_MAX)
					{
						ch->xenvx = ENV_MAX;

						if (ch->xsustain_level != ENV_RANGE)
						{
							ch->state = SOUND_DECAY;
							set_env_rate(ch, ch->xdecay_rate, ch->xsustain_level);
						}
						else
						{
							ch->state = SOUND_SUSTAIN;
							set_env_rate(ch, ch->xsustain_rate, 0);
						}
					}

					break;

				case SOUND_DECAY:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= ((ch->xenvx - 1) >> 8) + 1; // 1 - 1/256
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= ch->xenvx_target)
					{
						if (ch->xenvx <= 0)
							ch->xenvx = 0;

						ch->state = SOUND_SUSTAIN;
						set_env_rate(ch, ch->xsustain_rate, 0);
					}

					break;

				case SOUND_SUSTAIN:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= ((ch->xenvx - 1) >> 8) + 1;  // 1 - 1/256
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= 0)
						ch->xenvx = 0;

					break;

				case SOUND_RELEASE:
					ch->xenv_count -= env_counter_max;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= (ENV_RANGE >> 8); // 1/256
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= 0)
						S9xSoundSetSilence(J);

					break;

				case SOUND_INCREASE_LINEAR:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx += (ENV_RANGE >> 6); // 1/64
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx > ENV_MAX)
					{
						ch->xenvx = ENV_MAX;
						ch->state = SOUND_DIRECT;
						set_env_rate(ch, 0, 0);
					}

					break;

				case SOUND_INCREASE_BENT_LINE:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						if (ch->xenvx >= ((ENV_RANGE * 3) >> 2)) // 0x600
							ch->xenvx += (ENV_RANGE >> 8); // 1/256
						else
							ch->xenvx += (ENV_RANGE >> 6); // 1/64

						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx > ENV_MAX)
					{
						ch->xenvx = ENV_MAX;
						ch->state = SOUND_DIRECT;
						set_env_rate(ch, 0, 0);
					}

					break;

				case SOUND_DECREASE_LINEAR:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= (ENV_RANGE >> 6); // 1/64
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= 0)
						ch->xenvx = 0;

					break;

				case SOUND_DECREASE_EXPONENTIAL:
					ch->xenv_count -= ch->xenv_rate;
					while (ch->xenv_count <= 0)
					{
						ch->xenvx -= ((ch->xenvx - 1) >> 8) + 1; // 1 - 1/256
						ch->xenv_count += env_counter_max;
					}

					if (ch->xenvx <= 0)
						ch->xenvx = 0;

					break;

				case SOUND_DIRECT:
					set_env_rate(ch, 0, 0);

					break;
			}

			ch->xsmp_count += mod1 ? (((int64) freq * (32768 + wave[I >> 1])) >> 15) : freq;

			while (ch->xsmp_count >= 0)
			{
				ch->xsmp_count -= FIXED_POINT;
				ch->nb_sample[ch->nb_index] = ch->sample;
				ch->nb_index = (ch->nb_index + 1) & 3;

				ch->sample_pointer++;
				if (ch->sample_pointer == 16)
				{
					ch->sample_pointer = 0;

					if (ch->last_block)
					{
						APU.DSP[APU_ENDX] |= (1 << J);
						ch->block_pointer = READ_WORD(sample_address(ch->sample_number) + 2);
						if (!ch->loop)
							S9xSoundSetSilence(J);
						else
							ch->sample_number = APU.DSP[APU_SRCN + J * 0x10];
					}

					brr_decode_block(ch);
				}

				ch->sample = ch->decoded[ch->sample_pointer];
			}

			int32	outx, d;

			if (ch->type == SOUND_SAMPLE)
			{
				if (Settings.InterpolatedSound)
				{
					d = ch->xsmp_count >> (FIXED_POINT_SHIFT - 8);
					outx  = ((G4(-d) * ch->nb_sample[ ch->nb_index         ]) >> 11);
					outx += ((G3(-d) * ch->nb_sample[(ch->nb_index + 1) & 3]) >> 11);
					outx += ((G2( d) * ch->nb_sample[(ch->nb_index + 2) & 3]) >> 11);
					outx  = sclip15(outx);
					outx += ((G1( d) * ch->nb_sample[(ch->nb_index + 3) & 3]) >> 11);
					outx  = sclamp15(outx);
				}
				else
					outx = ch->sample;
			}
			else
			{
				noise_count -= SoundData.noise_rate;
				while (noise_count <= 0)
				{
					noise_count += env_counter_max;
					noise_index = (noise_index + 1) & 0xFFF;
				}

				outx = sclip15(noise_cache[noise_index]);
			}

			outx = (outx * ch->xenvx) >> 11;

			if (I < MAX_OUTX_ENVX_BUFFER)
			{
				APU.OUTX_buffer[J][I] = outx >> 7;
				APU.ENVX_buffer[J][I] = (ch->xenvx >> ENV_SHIFT) & 0x7F;
			}

			if (mod2)
				wave[I] = outx << 1;

			int32	V;

			if (sound_switch)
				V = ((outx * ch->volume_left ) >> 7) << 1;
			else
				V = 0;

			msample[I] += V;
			ch->esample_buffer[I] += V;
		}
	}

	if (APU.DSP[APU_NON])
		SoundData.noise_count = noise_count;
}

void S9xMixSamples (uint8 *buffer, int sample_count)
{
	if (!so.snes_standard)
	{
		mix_nonstandard_samples (buffer, sample_count);
		return;
	}

	int32	out, E;

	memset(msample, 0, sample_count * sizeof(msample[0]));
	memset(esample, 0, sample_count * sizeof(esample[0]));

	mix_stereo(sample_count);

	if (SoundData.no_filter)
	{
		// for speed-ups
		for (int J = 0; J < sample_count; J++)
		{
			E = (int16) READ_WORD(&SoundData.echo_ring_buffer[SoundData.echo_ring_pointer]);

			SoundData.fir_buffer[SoundData.fir_index & 15] = E;
			E *= 127;
			SoundData.fir_index++;

			if (SoundData.echo_write_enabled)
			{
				out = esample[J] + ((E * SoundData.echo_feedback) >> 14);
				WRITE_WORD(&SoundData.echo_ring_buffer[SoundData.echo_ring_pointer], sclamp16(out));
			}

			if (++SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
				SoundData.echo_ring_pointer = 0;

			out  = (msample[J] * SoundData.master_volume[J & 1]) >> 7;
			out += ((E * SoundData.echo_volume[J & 1]) >> 14);
			((int16 *) buffer)[J] = sclamp16(out);
		}
	}
	else
	{
		for (int J = 0; J < sample_count; J++)
		{
			E = (int16) READ_WORD(&SoundData.echo_ring_buffer[SoundData.echo_ring_pointer]);

			SoundData.fir_buffer[SoundData.fir_index & 15] = E;
			E *= SoundData.fir_tap[0];
			E += SoundData.fir_buffer[(SoundData.fir_index -  2) & 15] * SoundData.fir_tap[1];
			E += SoundData.fir_buffer[(SoundData.fir_index -  4) & 15] * SoundData.fir_tap[2];
			E += SoundData.fir_buffer[(SoundData.fir_index -  6) & 15] * SoundData.fir_tap[3];
			E += SoundData.fir_buffer[(SoundData.fir_index -  8) & 15] * SoundData.fir_tap[4];
			E += SoundData.fir_buffer[(SoundData.fir_index - 10) & 15] * SoundData.fir_tap[5];
			E += SoundData.fir_buffer[(SoundData.fir_index - 12) & 15] * SoundData.fir_tap[6];
			E += SoundData.fir_buffer[(SoundData.fir_index - 14) & 15] * SoundData.fir_tap[7];
			SoundData.fir_index++;

			if (SoundData.echo_write_enabled)
			{
				out = esample[J] + ((E * SoundData.echo_feedback) >> 14);
				WRITE_WORD(&SoundData.echo_ring_buffer[SoundData.echo_ring_pointer], sclamp16(out));
			}

			if (++SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
				SoundData.echo_ring_pointer = 0;

			out  = (msample[J] * SoundData.master_volume[J & 1]) >> 7;
			out += ((E * SoundData.echo_volume[J & 1]) >> 14);
			((int16 *) buffer)[J] = sclamp16(out);
		}
	}

	if (SoundData.mute || so.mute_sound)
		memset(buffer, 0, sample_count << 1);
}

static inline void mix_nonstandard_samples (uint8 *buffer, int sample_count)
{
	int32	out, E;

	memset(msample, 0, sample_count * sizeof(msample[0]));
	if (!so.disable_echo)
		memset(esample, 0, sample_count * sizeof(esample[0]));

	if (so.stereo)
		mix_stereo(sample_count);
	else
		mix_mono(sample_count);

	if (so.sixteen_bit)
	{
		// 16-bit sound
		if (!so.disable_echo)
		{
			// it should be mono
			{
				// 16-bit mono sound with echo enabled...
				if (SoundData.no_filter)
				{
					// ... no filter defined
					for (int J = 0; J < sample_count; J++)
					{
						E = SoundData.alt_echo_ring[SoundData.echo_ring_pointer];

						SoundData.fir_buffer[SoundData.fir_index & 7] = E;
						E *= 127;
						SoundData.fir_index++;

						if (SoundData.echo_write_enabled)
						{
							out = esample[J] + ((E * SoundData.echo_feedback) >> 14);
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = sclamp16(out);
						}
						else
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = 0;

						if (++SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
							SoundData.echo_ring_pointer = 0;

						out  = (msample[J] * SoundData.master_volume[0]) >> 7;
						out += ((E * SoundData.echo_volume[0]) >> 14);
						((int16 *) buffer)[J] = sclamp16(out);
					}
				}
				else
				{
					// ... with filter defined.
					for (int J = 0; J < sample_count; J++)
					{
						E = SoundData.alt_echo_ring[SoundData.echo_ring_pointer];

						SoundData.fir_buffer[SoundData.fir_index & 7] = E;
						E *= SoundData.fir_tap[0];
						E += SoundData.fir_buffer[(SoundData.fir_index - 1) & 7] * SoundData.fir_tap[1];
						E += SoundData.fir_buffer[(SoundData.fir_index - 2) & 7] * SoundData.fir_tap[2];
						E += SoundData.fir_buffer[(SoundData.fir_index - 3) & 7] * SoundData.fir_tap[3];
						E += SoundData.fir_buffer[(SoundData.fir_index - 4) & 7] * SoundData.fir_tap[4];
						E += SoundData.fir_buffer[(SoundData.fir_index - 5) & 7] * SoundData.fir_tap[5];
						E += SoundData.fir_buffer[(SoundData.fir_index - 6) & 7] * SoundData.fir_tap[6];
						E += SoundData.fir_buffer[(SoundData.fir_index - 7) & 7] * SoundData.fir_tap[7];
						SoundData.fir_index++;

						if (SoundData.echo_write_enabled)
						{
							out = esample[J] + ((E * SoundData.echo_feedback) >> 14);
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = sclamp16(out);
						}
						else
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = 0;

						if (++SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
							SoundData.echo_ring_pointer = 0;

						out  = (msample[J] * SoundData.master_volume[0]) >> 7;
						out += ((E * SoundData.echo_volume[0]) >> 14);
						((int16 *) buffer)[J] = sclamp16(out);
					}
				}
			}
		}
		else
		{
			// 16-bit mono or stereo sound, no echo
			for (int J = 0; J < sample_count; J++)
			{
				out = (msample[J] * SoundData.master_volume[J & 1]) >> 7;
				((int16 *) buffer)[J] = sclamp16(out);
			}
		}

		if (SoundData.mute || so.mute_sound)
			memset(buffer, 0, sample_count << 1);
	}
	else
	{
		// 8-bit sound
		if (!so.disable_echo)
		{
			if (so.stereo)
			{
				// 8-bit stereo sound with echo enabled ...
				if (SoundData.no_filter)
				{
					// ... but no filter defined.
					for (int J = 0; J < sample_count; J++)
					{
						E = SoundData.alt_echo_ring[SoundData.echo_ring_pointer];

						SoundData.fir_buffer[SoundData.fir_index & 15] = E;
						E *= 127;
						SoundData.fir_index++;

						if (SoundData.echo_write_enabled)
						{
							out = esample[J] + ((E * SoundData.echo_feedback) >> 14);
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = sclamp16(out);
						}
						else
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = 0;

						if (++SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
							SoundData.echo_ring_pointer = 0;

						out  = (msample[J] * SoundData.master_volume[J & 1]) >> 15;
						out += ((E * SoundData.echo_volume[J & 1]) >> 22);
						buffer[J] = sclamp8(out) + 128;
					}
				}
				else
				{
					// ... with filter defined.
					for (int J = 0; J < sample_count; J++)
					{
						E = SoundData.alt_echo_ring[SoundData.echo_ring_pointer];

						SoundData.fir_buffer[SoundData.fir_index & 15] = E;
						E *= SoundData.fir_tap[0];
						E += SoundData.fir_buffer[(SoundData.fir_index -  2) & 15] * SoundData.fir_tap[1];
						E += SoundData.fir_buffer[(SoundData.fir_index -  4) & 15] * SoundData.fir_tap[2];
						E += SoundData.fir_buffer[(SoundData.fir_index -  6) & 15] * SoundData.fir_tap[3];
						E += SoundData.fir_buffer[(SoundData.fir_index -  8) & 15] * SoundData.fir_tap[4];
						E += SoundData.fir_buffer[(SoundData.fir_index - 10) & 15] * SoundData.fir_tap[5];
						E += SoundData.fir_buffer[(SoundData.fir_index - 12) & 15] * SoundData.fir_tap[6];
						E += SoundData.fir_buffer[(SoundData.fir_index - 14) & 15] * SoundData.fir_tap[7];
						SoundData.fir_index++;

						if (SoundData.echo_write_enabled)
						{
							out = esample[J] + ((E * SoundData.echo_feedback) >> 14);
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = sclamp16(out);
						}
						else
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = 0;

						if (++SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
							SoundData.echo_ring_pointer = 0;

						out  = (msample[J] * SoundData.master_volume[J & 1]) >> 15;
						out += ((E * SoundData.echo_volume[J & 1]) >> 22);
						buffer[J] = sclamp8(out) + 128;
					}
				}
			}
			else
			{
				// 8-bit mono sound with echo enabled...
				if (SoundData.no_filter)
				{
					// ... no filter defined
					for (int J = 0; J < sample_count; J++)
					{
						E = SoundData.alt_echo_ring[SoundData.echo_ring_pointer];

						SoundData.fir_buffer[SoundData.fir_index & 7] = E;
						E *= 127;
						SoundData.fir_index++;

						if (SoundData.echo_write_enabled)
						{
							out = esample[J] + ((E * SoundData.echo_feedback) >> 14);
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = sclamp16(out);
						}
						else
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = 0;

						if (++SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
							SoundData.echo_ring_pointer = 0;

						out  = (msample[J] * SoundData.master_volume[0]) >> 15;
						out += ((E * SoundData.echo_volume[0]) >> 22);
						buffer[J] = sclamp8(out) + 128;
					}
				}
				else
				{
					// ... with filter defined.
					for (int J = 0; J < sample_count; J++)
					{
						E = SoundData.alt_echo_ring[SoundData.echo_ring_pointer];

						SoundData.fir_buffer[SoundData.fir_index & 7] = E;
						E *= SoundData.fir_tap[0];
						E += SoundData.fir_buffer[(SoundData.fir_index - 1) & 7] * SoundData.fir_tap[1];
						E += SoundData.fir_buffer[(SoundData.fir_index - 2) & 7] * SoundData.fir_tap[2];
						E += SoundData.fir_buffer[(SoundData.fir_index - 3) & 7] * SoundData.fir_tap[3];
						E += SoundData.fir_buffer[(SoundData.fir_index - 4) & 7] * SoundData.fir_tap[4];
						E += SoundData.fir_buffer[(SoundData.fir_index - 5) & 7] * SoundData.fir_tap[5];
						E += SoundData.fir_buffer[(SoundData.fir_index - 6) & 7] * SoundData.fir_tap[6];
						E += SoundData.fir_buffer[(SoundData.fir_index - 7) & 7] * SoundData.fir_tap[7];
						SoundData.fir_index++;

						if (SoundData.echo_write_enabled)
						{
							out = esample[J] + ((E * SoundData.echo_feedback) >> 14);
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = sclamp16(out);
						}
						else
							SoundData.alt_echo_ring[SoundData.echo_ring_pointer] = 0;

						if (++SoundData.echo_ring_pointer >= SoundData.echo_ring_size)
							SoundData.echo_ring_pointer = 0;

						out  = (msample[J] * SoundData.master_volume[0]) >> 15;
						out += ((E * SoundData.echo_volume[0]) >> 22);
						buffer[J] = sclamp8(out) + 128;
					}
				}
			}
		}
		else
		{
			// 8-bit mono or stereo sound, no echo
			for (int J = 0; J < sample_count; J++)
			{
				out = (msample[J] * SoundData.master_volume[J & 1]) >> 15;
				buffer[J] = sclamp8(out) + 128;
			}
		}

		if (SoundData.mute || so.mute_sound)
			memset(buffer, 0, sample_count);
	}
}

void S9xResetSound (bool8 full)
{
	// On power on, most registers are uninitialized.
	// On reset, most registers retain their previous values.
	// FLG is 0xE0 after power on or reset.
	// ENVX and OUTX are 0 after power on or reset.
	// But just clear all settings for now...

	memset(&SoundData, 0, sizeof(SoundData));

	SoundData.echo_ring_size = so.stereo ? 2 : 1;
	SoundData.echo_ring_buffer = (int16 *) IAPU.RAM;
	SoundData.fir_tap[0] = 127;
	SoundData.no_filter = TRUE;
	SoundData.mute = TRUE;
	SoundData.noise_seed = 0x4000;
	for (int c = 0; c < 8; c++)
		SoundData.channels[c].esample_buffer = dummy_esample;

	so.mute_sound = TRUE;
	so.sound_switch = ~0;
	so.stereo_switch = ~0;
	so.samples_mixed_so_far = 0;
	so.play_position = 0;
	so.err_counter = 0;

	if (so.playback_rate)
		so.err_rate = (uint32) (FIXED_POINT * SNES_SCANLINE_TIME * so.playback_rate);
	else
		so.err_rate = 0;
}

bool8 S9xInitSound (int mode, bool8 stereo, int buffer_size)
{
	so.stereo = stereo;
	so.sixteen_bit = Settings.SixteenBitSound;
	so.disable_echo = Settings.DisableSoundEcho;
	so.playback_rate = 0;

	so.pitch_mul = 0.985; // XXX: necessary for most cards in linux...?
	so.sound_fd = -1;
	so.buffer_size = 0;
	so.encoded = FALSE;

	if (!(mode & 7))
		return (1);

	S9xSetSoundMute(TRUE);

	if (!S9xOpenSoundDevice(mode, stereo, buffer_size))
	{
	#ifdef NOSOUND
		S9xMessage(S9X_WARNING, S9X_SOUND_NOT_BUILT, "No sound support compiled in");
	#else
		S9xMessage(S9X_ERROR, S9X_SOUND_DEVICE_OPEN_FAILED, "Sound device open failed");
	#endif
		return (0);
	}

	so.snes_standard = so.stereo && so.sixteen_bit && !so.disable_echo && (so.playback_rate == 32000);

	return (1);
}

void S9xSetPlaybackRate (uint32 playback_rate)
{
	if (playback_rate > 48000)
		playback_rate = 48000;

	so.playback_rate = playback_rate;
	so.snes_standard = so.stereo && so.sixteen_bit && !so.disable_echo && (so.playback_rate == 32000);

	restore_sound_data();
}

void S9xSetSoundControl (uint8 sound_switch)
{
	so.sound_switch = sound_switch;
}

bool8 S9xSetSoundMute (bool8 mute)
{
	bool8	old = so.mute_sound;
	so.mute_sound = mute;
	return (old);
}

static void restore_sound_data (void)
{
	printf("Restoring sound status...\n");

	env_counter_max = env_counter_max_master * so.playback_rate / 32000;
	APU.OUTX_ENVX_counter_max = (64000 << SNES_APU_ACCURACY) / so.playback_rate;

	for (int c = 0; c < 8; c++)
	{
		Channel	*ch = &SoundData.channels[c];

		S9xSoundSetVolume(c, (int8) APU.DSP[APU_VOLL + (c << 4)], (int8) APU.DSP[APU_VOLR + (c << 4)]);
		S9xSoundSetPitch(c, ((APU.DSP[APU_PITCHH + (c << 4)] << 8) | APU.DSP[APU_PITCHL + (c << 4)]) & 0x3FFF);
		S9xSoundSetFilterCoefficient(c, (int8) APU.DSP[APU_FIR0 + (c << 4)]);

		ch->sample = 0;
		ch->nb_sample[0] = 0;
		ch->nb_sample[1] = 0;
		ch->nb_sample[2] = 0;
		ch->nb_sample[3] = 0;
		ch->nb_index = 0;
		ch->xsmp_count = 0;
	}

	S9xSoundSetMasterVolume((int8) APU.DSP[APU_MVOLL], (int8) APU.DSP[APU_MVOLR]);
	S9xSoundSetEchoVolume((int8) APU.DSP[APU_EVOLL], (int8) APU.DSP[APU_EVOLR]);
	S9xSoundSetType(APU.DSP[APU_NON]);
	S9xSoundSetEchoEnable(APU.DSP[APU_EON]);
	S9xSoundSetEchoRingBufferAddress(APU.DSP[APU_ESA]);
	S9xSoundSetEchoFeedback((int8) APU.DSP[APU_EFB]);
	S9xSoundSetEchoDelay(APU.DSP[APU_EDL] & 0x0F);
	S9xSoundSetEchoWriteEnable(APU.DSP[APU_FLG] & APU_ECHO_DISABLED);
	S9xSoundSetNoiseRate(APU.DSP[APU_FLG] & 0x1F);
	S9xSoundSetMute(APU.DSP[APU_FLG] & APU_MUTE);

	memset(SoundData.alt_echo_ring, 0, sizeof(SoundData.alt_echo_ring));
	memset(SoundData.fir_buffer, 0, sizeof(SoundData.fir_buffer));
	SoundData.echo_ring_pointer = 0;
	SoundData.fir_index = 0;
	SoundData.noise_count = env_counter_max;
	SoundData.noise_seed = 0x4000;

	so.err_rate = (uint32) (FIXED_POINT * SNES_SCANLINE_TIME * so.playback_rate);
}

void S9xSoundPreSaveState (void)
{
	SoundData.stereo = so.stereo;
	SoundData.sixteen_bit = so.sixteen_bit;
	SoundData.disable_echo = so.disable_echo;
	SoundData.playback_rate = so.playback_rate;
}

void S9xSoundPostLoadState (int version)
{
	if (version < 5)
		convert_sound_data(version);

	if (version < 5 ||
		so.stereo        != SoundData.stereo       ||
		so.sixteen_bit   != SoundData.sixteen_bit  ||
		so.disable_echo  != SoundData.disable_echo ||
		so.playback_rate != SoundData.playback_rate)
		restore_sound_data();
	else
	{
		S9xSoundSetEchoEnable(APU.DSP[APU_EON]);
		S9xSoundSetEchoRingBufferAddress(APU.DSP[APU_ESA]);
	}
}

// For backward compatibility ------------------------------

static uint32 old_attack_rate[16] =
{
	 4100,  2600,  1500,  1000,   640,   380,   260,   160,
	   96,    64,    40,    24,    16,    10,     6,     1
};

static uint32 old_decay_rate[8] =
{
	 1200,   740,   440,   290,   180,   110,    74,    37
};

static uint32 old_sustain_rate[32] =
{
	   ~0, 38000, 28000, 24000, 19000, 14000, 12000,  9400,
	 7100,  5900,  4700,  3500,  2900,  2400,  1800,  1500,
	 1200,   880,   740,   590,   440,   370,   290,   220,
	  180,   150,   110,    92,    74,    55,    37,    18
};

static void convert_sound_data (int version)
{
	if (version < 2)
	{
		printf("Converting 1.43 sound snapshot to 1.51...\n");

		int32	x = env_counter_max_master * so.playback_rate / 32000;
		int		n;

		for (int c = 0; c < 8; c++)
		{
			OldCh	*och = &OldSoundData.channels[c];

			och->xsustain_level = och->sustain_level << 8;
			och->xenvx          = och->envx << 4;
			och->xenvx_target   = och->envx_target << 4;
			och->xenv_rate      = (int32) ((double) och->erate * x / FIXED_POINT);
			och->xenv_count     = x - (int32) ((double) och->env_error * x / FIXED_POINT);
			och->out_sample     = ((och->sample * och->xenvx) >> 11) & ~1;

			for (n = 0; n < 16; n++)
				if (old_attack_rate[n]  == och->attack_rate)
					break;
			och->xattack_rate  = env_counter_table[((n << 1) + 0x01) & 0x1F];

			for (n = 0; n <  8; n++)
				if (old_decay_rate[n]   == och->decay_rate)
					break;
			och->xdecay_rate   = env_counter_table[((n << 1) + 0x10) & 0x1F];

			for (n = 0; n < 32; n++)
				if (old_sustain_rate[n] == och->sustain_rate)
					break;
			och->xsustain_rate = env_counter_table[n & 0x1F];
		}
	}

	printf("Converting 1.51 sound snapshot to 1.52...\n");

	for (int c = 0; c < 8; c++)
	{
		Channel	*ch  = &SoundData.channels[c];
		OldCh	*och = &OldSoundData.channels[c];

		ch->state          = och->state;
		ch->needs_decode   = och->needs_decode;
		ch->last_block     = och->last_block;
		ch->loop           = och->loop;
		ch->block_pointer  = och->block_pointer;
		ch->sample_pointer = och->sample_pointer;
		ch->sample_number  = och->sample_number;
		ch->xenvx          = och->xenvx;
		ch->xenvx_target   = och->xenvx_target;
		ch->xenv_count     = och->xenv_count;
		ch->xenv_rate      = och->xenv_rate;
		ch->xattack_rate   = och->xattack_rate;
		ch->xdecay_rate    = och->xdecay_rate;
		ch->xsustain_rate  = och->xsustain_rate;
		ch->xsustain_level = och->xsustain_level;
		for (int i = 0; i < 16; i++)
			ch->decoded[i]  = och->decoded[i];
		for (int i = 0; i <  2; i++)
			ch->previous[i] = och->previous16[i];

		APU.OUTX_buffer[c][0] = (och->out_sample >> 8);
		APU.ENVX_buffer[c][0] = (och->xenvx >> ENV_SHIFT) & 0x7F;
	}

	APU.OUTX_ENVX_pos = 0;
	APU.OUTX_ENVX_samples = 1;
	APU.OUTX_ENVX_counter = 0;
}
