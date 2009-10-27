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


#ifndef _SOUNDUX_H_
#define _SOUNDUX_H_

enum
{
	SOUND_SAMPLE = 0,
	SOUND_NOISE
};

enum
{
	SOUND_SILENT = 0,
	SOUND_ATTACK,
	SOUND_DECAY,
	SOUND_SUSTAIN,
	SOUND_RELEASE,
	SOUND_DIRECT,
	SOUND_INCREASE_LINEAR,
	SOUND_INCREASE_BENT_LINE,
	SOUND_DECREASE_LINEAR,
	SOUND_DECREASE_EXPONENTIAL,
	SOUND_ADJUST
};

#define SOUND_BUFFER_SIZE		(1024 * 16)
#define SOUND_BUFFER_SIZE_MASK	(SOUND_BUFFER_SIZE-1)

typedef struct
{
	bool8	stereo;					// stereo or mono
	bool8	sixteen_bit;			// 16bit or 8bit sample
	bool8	disable_echo;			// force echo disabled
	uint32	playback_rate;			// 32000Hz is recommended
	bool8	snes_standard;			// stereo && 16bit && echo && 32000Hz

	bool8	mute_sound;				// force mute regardless of FLG
	uint8	sound_switch;			// channel on/off
	uint16	stereo_switch;			// stereo channel on/off
	double	pitch_mul;				// used with Settings.FixFrequency

	int		buffer_size;			// ** port specific
	int		sound_fd;				// ** port specific
	bool8	encoded;				// ** port specific
	int32	samples_mixed_so_far;	// ** port specific
	int32	play_position;			// ** port specific
	uint32	err_counter;			// ** port specific
	uint32	err_rate;				// ** port specific
}	SoundStatus;

extern volatile SoundStatus	so;

typedef struct
{
	int32	state;					// ADSR, GAIN, SILENT
	int32	type;					// sample or noise
	int32	volume_left;			// (int32) VOL(L)
	int32	volume_right;			// (int32) VOL(R)
	uint16	pitch;					// ((PITCHH << 8) | PITCHL) & 0x3FFF
	uint32	frequency;				// normalized pitch
	bool8	needs_decode;			// true at key-on
	bool8	last_block;				// end flag in BRR header
	bool8	loop;					// loop flag in BRR header
	int16	decoded[16];			// decoded 16 samples
	int32	previous[2];			// last two decoded samples
	uint32	block_pointer;			// currect BRR block
	uint32	sample_pointer;			// pointer in a BRR block
	int16	sample;					// signed 16 bit sample in decoded[]
	int16	nb_sample[4];			// cached samples for interpolation
	int32	nb_index;				// index of cached samples
	int32	*esample_buffer;		// echo_buffer[] or dummy_echo_buffer[]
	uint8	sample_number;			// SRCN
	int32	xenvx;					// ENVX << 4
	int32	xenvx_target;			// ENVX target << 4
	int32	xenv_count;				// counter for envelope timing
	int32	xenv_rate;				// envelope timing from env_counter_table
	int32	xsmp_count;				// counter for pitch
	int32	xattack_rate;			// envelope timing from env_counter_table
	int32	xdecay_rate;			// envelope timing from env_counter_table
	int32	xsustain_rate;			// envelope timing from env_counter_table
	int32	xsustain_level;			// (128 / 8 * (SR + 1)) << 4
}	Channel;

typedef struct
{
	Channel	channels[8];
	int32	master_volume[2];		// (int32) MVOL
	int32	echo_volume[2];			// (int32) EVOL
	int32	echo_feedback;			// (int32) EFB
	int32	echo_ring_pointer;		// index of echo ring buffer
	int32	echo_ring_size;			// num of echo samples
	int16	*echo_ring_buffer;		// ESA
	int16	alt_echo_ring[24000];	// Substitute of ESA to support flexible sound settings
	bool8	echo_write_enabled;		// (FLG & 0x20) ? FALSE : TRUE
	uint32	fir_index;				// FIR filter index
	int32	fir_tap[8];				// FTR taps
	int32	fir_buffer[16];			// FIR filter samples
	bool8	no_filter;				// true when no echo
	int32	noise_count;			// counter for noise frequency
	int32	noise_rate;				// noise frequency from env_counter_table
	uint16	noise_seed;				// seed for noise generation
	bool8	mute;					// (FLG & 0x40) ? TRUE : FALSE

	bool8	stereo;					// used to recover from the snapshot
	bool8	sixteen_bit;			// used to recover from the snapshot
	bool8	disable_echo;			// used to recover from the snapshot
	uint32	playback_rate;			// used to recover from the snapshot
}	SSoundData;

extern SSoundData	SoundData;

// For backward compatibility ------------------------------

typedef struct
{
    int32	state;
    int32	type;
    int16	volume_left;
    int16	volume_right;
    uint32	hertz;
    uint32	frequency;
    uint32	count;
    bool8	loop;
    int32	envx;
    int16	left_vol_level;
    int16	right_vol_level;
    int16	envx_target;
    uint32	env_error;
    uint32	erate;
    int32 	direction;
    uint32	attack_rate;
    uint32	decay_rate;
    uint32	sustain_rate;
    uint32	release_rate;
    uint32	sustain_level;
    int16	sample;
    int16	decoded[16];
    int16	previous16[2];
    int16	*block;
    uint16	sample_number;
    bool8	last_block;
    bool8	needs_decode;
    uint32	block_pointer;
    uint32	sample_pointer;
    int32	*echo_buf_ptr;
    int32	mode;
    int32	envxx;
    int16	next_sample;
    int32	interpolate;
    int32	previous[2];
    uint32	dummy[8];
	int32	nb_index;
	int16	nb_sample[4];
	int16	out_sample;
	int32	xenvx;
	int32	xenvx_target;
	int32	xenv_count;
	int32	xenv_rate;
	int32	xsmp_count;
	int32	xattack_rate;
	int32	xdecay_rate;
	int32	xsustain_rate;
	int32	xsustain_level;
}	OldCh;

typedef struct
{
    int16	master_volume_left;
    int16	master_volume_right;
    int16	echo_volume_left;
    int16	echo_volume_right;
    int32	echo_enable;
    int32	echo_feedback;
    int32	echo_ptr;
    int32	echo_buffer_size;
    int32	echo_write_enabled;
    int32	echo_channel_enable;
    int32	pitch_mod;
    uint32	dummy[3];
    OldCh	channels[8];
    bool8	no_filter;
    int32	master_volume[2];
    int32	echo_volume[2];
    int32	noise_hertz;
	int32	noise_count;
	int32	noise_rate;
}	SOldSoundData;

extern SOldSoundData	OldSoundData;

// ---------------------------------------------------------

void S9xSoundSetVolume (int, int32, int32);
void S9xSoundSetMasterVolume (int32, int32);
void S9xSoundSetEchoVolume (int32, int32);
void S9xSoundSetEchoEnable (uint8);
void S9xSoundSetEchoFeedback (int32);
void S9xSoundSetEchoDelay (uint32);
void S9xSoundSetEchoRingBufferAddress (uint8);
void S9xSoundSetEchoWriteEnable (uint8);
void S9xSoundSetFilterCoefficient (int, int32);
void S9xSoundSetMute (uint8);
void S9xSoundSetSustainLevel(int, int);
void S9xSoundSetNoiseRate (int);
void S9xSoundSetType (uint8);
void S9xSoundSetPitch (int, uint16);
void S9xSoundSetSilence (int);
void S9xSoundSetRelease (int);
void S9xSoundSetAttack (int);
void S9xSoundFinishRapidAttack (int);
void S9xSoundAdjustEnvelope (int, uint8, uint8, uint8);

void S9xMixSamples (uint8 *, int);
void S9xResetSound (bool8);
void S9xSetPlaybackRate (uint32);
void S9xSetSoundControl (uint8);
void S9xSoundPreSaveState (void);
void S9xSoundPostLoadState (int);
bool8 S9xSetSoundMute (bool8);

bool8 S9xOpenSoundDevice (int, bool8, int);
extern void S9xGenerateSound (void);

#endif
