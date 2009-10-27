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

#include "spc700.h"

struct SIAPU
{
	uint8	*PC;
	uint8	*RAM;
	uint8	*DirectPage;
	uint8	_Carry;
	uint8	_Zero;
	uint8	_Overflow;
	uint32	Address;
	uint8	Bit;
	bool8	APUExecuting;
	uint8	*WaitAddress1;
	uint8	*WaitAddress2;
	uint32	WaitCounter;
	int32	OneCycle;
	int32	TwoCycles;
};

#define MAX_OUTX_ENVX_BUFFER	4096

struct SAPU
{
	int32	Cycles;
	bool8	ShowROM;
	uint32	Flags;
	uint8	Internal_KON;
	uint8	OutPorts[4];
	uint8	DSP[0x80];
	uint8	ExtraRAM[64];
	uint16	Timer[3];
	uint16	TimerTarget[3];
	bool8	TimerEnabled[3];
	int32	NextAPUTimerPos;
	int32	APUTimerCounter;
	uint32	OUTX_ENVX_pos;
	uint32	OUTX_ENVX_samples;
	int32	OUTX_ENVX_counter;
	int32	OUTX_ENVX_counter_max;
	uint8	OUTX_buffer[8][MAX_OUTX_ENVX_BUFFER];
	uint8	ENVX_buffer[8][MAX_OUTX_ENVX_BUFFER];
};

extern struct SAPU	APU;
extern struct SIAPU	IAPU;

static inline void S9xAPUUnpackStatus (void)
{
	IAPU._Zero = ((APURegisters.P & Zero) == 0) | (APURegisters.P & Negative);
	IAPU._Carry = (APURegisters.P & Carry);
	IAPU._Overflow = (APURegisters.P & Overflow) >> 6;
}

static inline void S9xAPUPackStatus (void)
{
	APURegisters.P &= ~(Zero | Negative | Carry | Overflow);
	APURegisters.P |= IAPU._Carry | ((IAPU._Zero == 0) << 1) | (IAPU._Zero & 0x80) | (IAPU._Overflow << 6);
}

bool8 S9xInitAPU (void);
void S9xDeinitAPU (void);
void S9xResetAPU (void);
void S9xSetAPUControl (uint8);
void S9xSetAPUTimer (uint16, uint8);
void S9xAPUExecute (void);
void S9xSetAPUDSP (uint8);
uint8 S9xGetAPUDSP (void);
bool8 S9xInitSound (int, bool8, int);

#ifdef DEBUGGER
void S9xTraceAPU (void);
void S9xTraceSoundDSP (uint8, uint8);
void S9xAPUOPrint (char *, uint16);
void S9xPrintSoundDSPState (void);
#endif

extern int32	S9xAPUCycles[256];			// Scaled cycle lengths
extern int32	S9xAPUCycleLengths[256];	// Raw data

extern void (*S9xApuOpcodes[256]) (void);

// FLG bits
#define APU_SOFT_RESET		0x80
#define APU_MUTE			0x40
#define APU_ECHO_DISABLED	0x20

// DSP registers
#define APU_VOLL	0x00
#define APU_VOLR	0x01
#define APU_PITCHL	0x02
#define APU_PITCHH	0x03
#define APU_SRCN	0x04
#define APU_ADSR1	0x05
#define APU_ADSR2	0x06
#define APU_GAIN	0x07
#define APU_ENVX	0x08
#define APU_OUTX	0x09

#define APU_MVOLL	0x0C
#define APU_MVOLR	0x1C
#define APU_EVOLL	0x2C
#define APU_EVOLR	0x3C
#define APU_KON		0x4C
#define APU_KOFF	0x5C
#define APU_FLG		0x6C
#define APU_ENDX	0x7C

#define APU_EFB		0x0D
#define APU_PMON	0x2D
#define APU_NON		0x3D
#define APU_EON		0x4D
#define APU_DIR		0x5D
#define APU_ESA		0x6D
#define APU_EDL		0x7D

#define APU_FIR0	0x0F
#define APU_FIR1	0x1F
#define APU_FIR2	0x2F
#define APU_FIR3	0x3F
#define APU_FIR4	0x4F
#define APU_FIR5	0x5F
#define APU_FIR6	0x6F
#define APU_FIR7	0x7F

#endif
