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
#include "memmap.h"
#include "apu.h"
#include "apumem.h"

static uint8	Work8;
static uint16	Work16;
static uint32	Work32;
static int8		Int8;
static int16	Int16;
static int32	Int32;
static uint8	W1;

#define OP1	(*(IAPU.PC + 1))
#define OP2	(*(IAPU.PC + 2))

#ifdef SPC700_SHUTDOWN

#define APUShutdown() \
if (Settings.Shutdown && (IAPU.PC == IAPU.WaitAddress1 || IAPU.PC == IAPU.WaitAddress2)) \
{ \
	if (IAPU.WaitCounter == 0) \
	{ \
		if (!ICPU.CPUExecuting) \
			S9xAPUExecute(); \
		else \
			IAPU.APUExecuting = FALSE; \
	} \
	else \
	if (IAPU.WaitCounter >= 2) \
		IAPU.WaitCounter = 1; \
	else \
		IAPU.WaitCounter--; \
}

#else

#define APUShutdown()

#endif

#define APUSetZN8(b) \
	IAPU._Zero = (b)

#define APUSetZN16(w) \
	IAPU._Zero = ((w) != 0) | ((w) >> 8)

#define TCALL(n) \
	PushW(IAPU.PC - IAPU.RAM + 1); \
	IAPU.PC = IAPU.RAM + S9xAPUGetByte(0xffc0 + ((15 - n) << 1)) + (S9xAPUGetByte(0xffc1 + ((15 - n) << 1)) << 8)

#define SBC(a, b) \
	Int16 = (short) (a) - (short) (b) + (short) (APUCheckCarry()) - 1; \
	IAPU._Carry = Int16 >= 0; \
	if ((((a) ^ (b)) & 0x80) && (((a) ^ (uint8) Int16) & 0x80)) \
		APUSetOverflow(); \
	else \
		APUClearOverflow(); \
	APUSetHalfCarry(); \
	if (((a) ^ (b) ^ (uint8) Int16) & 0x10) \
		APUClearHalfCarry(); \
	(a) = (uint8) Int16; \
	APUSetZN8((uint8) Int16)

#define ADC(a, b) \
	Work16 = (a) + (b) + APUCheckCarry(); \
	IAPU._Carry = Work16 >= 0x100; \
	if (~((a) ^ (b)) & ((b) ^ (uint8) Work16) & 0x80) \
		APUSetOverflow(); \
	else \
		APUClearOverflow(); \
	APUClearHalfCarry(); \
	if (((a) ^ (b) ^ (uint8) Work16) & 0x10) \
		APUSetHalfCarry(); \
	(a) = (uint8) Work16; \
	APUSetZN8((uint8) Work16)

#define CMP(a, b) \
	Int16 = (short) (a) - (short) (b); \
	IAPU._Carry = Int16 >= 0; \
	APUSetZN8((uint8) Int16)

#define ASL(b) \
	IAPU._Carry = ((b) & 0x80) != 0; \
	(b) <<= 1; \
	APUSetZN8(b)

#define LSR(b) \
	IAPU._Carry = (b) & 1; \
	(b) >>= 1; \
	APUSetZN8(b)

#define ROL(b) \
	Work16 = ((b) << 1) | APUCheckCarry(); \
	IAPU._Carry = Work16 >= 0x100; \
	(b) = (uint8) Work16; \
	APUSetZN8(b)

#define ROR(b) \
	Work16 = (b) | ((uint16) APUCheckCarry() << 8); \
	IAPU._Carry = (uint8) Work16 & 1; \
	Work16 >>= 1; \
	(b) = (uint8) Work16; \
	APUSetZN8(b)

#define Push(b) \
	*(IAPU.RAM + 0x100 + APURegisters.S) = b; \
	APURegisters.S--

#define Pop(b) \
	APURegisters.S++; \
	(b) = *(IAPU.RAM + 0x100 + APURegisters.S)

#ifdef FAST_LSB_WORD_ACCESS

#define PushW(w) \
	if (APURegisters.S == 0) \
	{ \
		*(IAPU.RAM + 0x1ff) = (w); \
		*(IAPU.RAM + 0x100) = ((w) >> 8); \
	} \
	else \
		*(uint16 *) (IAPU.RAM + 0xff + APURegisters.S) = (w); \
	APURegisters.S -= 2

#define PopW(w) \
	APURegisters.S += 2; \
	if (APURegisters.S == 0) \
		(w) = *(IAPU.RAM + 0x1ff) | (*(IAPU.RAM + 0x100) << 8); \
	else \
		(w) = *(uint16 *) (IAPU.RAM + 0xff + APURegisters.S)

#else

#define PushW(w) \
	*(IAPU.RAM + 0x100 + APURegisters.S--) = ((w) >> 8); \
	*(IAPU.RAM + 0x100 + APURegisters.S--) = (w)

#define PopW(w) \
	APURegisters.S += 2; \
	if (APURegisters.S == 0) \
		(w) = *(IAPU.RAM + 0x1ff) | (*(IAPU.RAM + 0x100) << 8); \
	else \
		(w) = *(IAPU.RAM + 0xff + APURegisters.S) + (*(IAPU.RAM + 0x100 + APURegisters.S) << 8)

#endif

#define Relative() \
	Int8 = OP1; \
	Int16 = (int32) (IAPU.PC + 2 - IAPU.RAM) + Int8

#define Relative2() \
	Int8 = OP2; \
	Int16 = (int32) (IAPU.PC + 3 - IAPU.RAM) + Int8

#ifdef FAST_LSB_WORD_ACCESS

#define IndexedXIndirect() \
	IAPU.Address = *(uint16 *) (IAPU.DirectPage + ((OP1 + APURegisters.X) & 0xff))

#define Absolute() \
	IAPU.Address = *(uint16 *) (IAPU.PC + 1)

#define AbsoluteX() \
	IAPU.Address = *(uint16 *) (IAPU.PC + 1) + APURegisters.X

#define AbsoluteY() \
	IAPU.Address = *(uint16 *) (IAPU.PC + 1) + APURegisters.YA.B.Y

#define MemBit() \
	IAPU.Address = *(uint16 *) (IAPU.PC + 1); \
	IAPU.Bit = (uint8) (IAPU.Address >> 13); \
	IAPU.Address &= 0x1fff

#define IndirectIndexedY() \
	IAPU.Address = *(uint16 *) (IAPU.DirectPage + OP1) + APURegisters.YA.B.Y

#else

#define IndexedXIndirect() \
	IAPU.Address = *(IAPU.DirectPage + ((OP1 + APURegisters.X) & 0xff)) + (*(IAPU.DirectPage + ((OP1 + APURegisters.X + 1) & 0xff)) << 8)

#define Absolute() \
	IAPU.Address = OP1 + (OP2 << 8)

#define AbsoluteX() \
	IAPU.Address = OP1 + (OP2 << 8) + APURegisters.X

#define AbsoluteY() \
	IAPU.Address = OP1 + (OP2 << 8) + APURegisters.YA.B.Y

#define MemBit() \
	IAPU.Address = OP1 + (OP2 << 8); \
	IAPU.Bit = (int8) (IAPU.Address >> 13); \
	IAPU.Address &= 0x1fff

#define IndirectIndexedY() \
	IAPU.Address = *(IAPU.DirectPage + OP1) + (*(IAPU.DirectPage + OP1 + 1) << 8) + APURegisters.YA.B.Y

#endif

#define SET(b) \
	S9xAPUSetByteZ((uint8) (S9xAPUGetByteZ(OP1) | (1 << (b))), OP1); \
	IAPU.PC += 2

#define CLR(b) \
	S9xAPUSetByteZ((uint8) (S9xAPUGetByteZ(OP1) & ~(1 << (b))), OP1); \
	IAPU.PC += 2

#define BBS(b) \
	Work8 = OP1; \
	Relative2(); \
	if (S9xAPUGetByteZ(Work8) & (1 << (b))) \
	{ \
		IAPU.PC = IAPU.RAM + (uint16) Int16; \
		APU.Cycles += IAPU.TwoCycles; \
	} \
	else \
		IAPU.PC += 3

#define BBC(b) \
	Work8 = OP1; \
	Relative2(); \
	if (!(S9xAPUGetByteZ(Work8) & (1 << (b)))) \
	{ \
		IAPU.PC = IAPU.RAM + (uint16) Int16; \
		APU.Cycles += IAPU.TwoCycles; \
	} \
	else \
		IAPU.PC += 3


static void WARN (const char *s)
{
	char	buffer[100];

#ifdef DEBUGGER
	S9xAPUOPrint(buffer, IAPU.PC - IAPU.RAM);
#endif
	sprintf(String, "Sound CPU executing %s at %04X\n%s\n", s, (uint32) (IAPU.PC - IAPU.RAM), buffer);
	S9xMessage(S9X_ERROR, S9X_APU_STOPPED, String);
}

/*
static void STOP (const char *s)
{
	WARN(s);
	APU.TimerEnabled[0] = APU.TimerEnabled[1] = APU.TimerEnabled[2] = FALSE;
	IAPU.APUExecuting = FALSE;
	Settings.APUEnabled = FALSE; // re-enabled on next APU reset
#ifdef DEBUGGER
	CPU.Flags |= DEBUG_MODE_FLAG;
#else
	S9xExit();
#endif
}
*/

// NOP
static void Apu00 (void)
{
	IAPU.PC++;
}

// TCALL
static void Apu01 (void)
{
	TCALL(0);
}

static void Apu11 (void)
{
	TCALL(1);
}

static void Apu21 (void)
{
	TCALL(2);
}

static void Apu31 (void)
{
	TCALL(3);
}

static void Apu41 (void)
{
	TCALL(4);
}

static void Apu51 (void)
{
	TCALL(5);
}

static void Apu61 (void)
{
	TCALL(6);
}

static void Apu71 (void)
{
	TCALL(7);
}

static void Apu81 (void)
{
	TCALL(8);
}

static void Apu91 (void)
{
	TCALL(9);
}

static void ApuA1 (void)
{
	TCALL(10);
}

static void ApuB1 (void)
{
	TCALL(11);
}

static void ApuC1 (void)
{
	TCALL(12);
}

static void ApuD1 (void)
{
	TCALL(13);
}

static void ApuE1 (void)
{
	TCALL(14);
}

static void ApuF1 (void)
{
	TCALL(15);
}

// CALL abs
static void Apu3F (void)
{
	Absolute();
	// 0xB6f for Star Fox 2
	PushW(IAPU.PC + 3 - IAPU.RAM);
	IAPU.PC = IAPU.RAM + IAPU.Address;
}

// PCALL $XX
static void Apu4F (void)
{
	Work8 = OP1;
	PushW(IAPU.PC + 2 - IAPU.RAM);
	IAPU.PC = IAPU.RAM + 0xff00 + Work8;
}

// SET
static void Apu02 (void)
{
	SET(0);
}

static void Apu22 (void)
{
	SET(1);
}

static void Apu42 (void)
{
	SET(2);
}

static void Apu62 (void)
{
	SET(3);
}

static void Apu82 (void)
{
	SET(4);
}

static void ApuA2 (void)
{
	SET(5);
}

static void ApuC2 (void)
{
	SET(6);
}

static void ApuE2 (void)
{
	SET(7);
}

// CLR
static void Apu12 (void)
{
	CLR(0);
}

static void Apu32 (void)
{
	CLR(1);
}

static void Apu52 (void)
{
	CLR(2);
}

static void Apu72 (void)
{
	CLR(3);
}

static void Apu92 (void)
{
	CLR(4);
}

static void ApuB2 (void)
{
	CLR(5);
}

static void ApuD2 (void)
{
	CLR(6);
}

static void ApuF2 (void)
{
	CLR(7);
}

// BBS
static void Apu03 (void)
{
	BBS(0);
}

static void Apu23 (void)
{
	BBS(1);
}

static void Apu43 (void)
{
	BBS(2);
}

static void Apu63 (void)
{
	BBS(3);
}

static void Apu83 (void)
{
	BBS(4);
}

static void ApuA3 (void)
{
	BBS(5);
}

static void ApuC3 (void)
{
	BBS(6);
}

static void ApuE3 (void)
{
	BBS(7);
}

// BBC
static void Apu13 (void)
{
	BBC(0);
}

static void Apu33 (void)
{
	BBC(1);
}

static void Apu53 (void)
{
	BBC(2);
}

static void Apu73 (void)
{
	BBC(3);
}

static void Apu93 (void)
{
	BBC(4);
}

static void ApuB3 (void)
{
	BBC(5);
}

static void ApuD3 (void)
{
	BBC(6);
}

static void ApuF3 (void)
{
	BBC(7);
}

// OR A, dp
static void Apu04 (void)
{
	APURegisters.YA.B.A |= S9xAPUGetByteZ(OP1);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// OR A, abs
static void Apu05 (void)
{
	Absolute();
	APURegisters.YA.B.A |= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// OR A, (X)
static void Apu06 (void)
{
	APURegisters.YA.B.A |= S9xAPUGetByteZ(APURegisters.X);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// OR A, (dp + X)
static void Apu07 (void)
{
	IndexedXIndirect();
	APURegisters.YA.B.A |= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// OR A, #00
static void Apu08 (void)
{
	APURegisters.YA.B.A |= OP1;
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// OR dp(dest), dp(src)
static void Apu09 (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	Work8 |= S9xAPUGetByteZ(OP2);
	S9xAPUSetByteZ(Work8, OP2);
	APUSetZN8(Work8);
	IAPU.PC += 3;
}

// OR A, dp + X
static void Apu14 (void)
{
	APURegisters.YA.B.A |= S9xAPUGetByteZ(OP1 + APURegisters.X);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// OR A, abs + X
static void Apu15 (void)
{
	AbsoluteX();
	APURegisters.YA.B.A |= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// OR A, abs + Y
static void Apu16 (void)
{
	AbsoluteY();
	APURegisters.YA.B.A |= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// OR A, (dp) + Y
static void Apu17 (void)
{
	IndirectIndexedY();
	APURegisters.YA.B.A |= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// OR dp, #00
static void Apu18 (void)
{
	Work8 = OP1;
	Work8 |= S9xAPUGetByteZ(OP2);
	S9xAPUSetByteZ(Work8, OP2);
	APUSetZN8(Work8);
	IAPU.PC += 3;
}

// OR (X), (Y)
static void Apu19 (void)
{
	Work8 = S9xAPUGetByteZ(APURegisters.X) | S9xAPUGetByteZ(APURegisters.YA.B.Y);
	APUSetZN8(Work8);
	S9xAPUSetByteZ(Work8, APURegisters.X);
	IAPU.PC++;
}

// OR1 C, membit
static void Apu0A (void)
{
	MemBit();

	if (!APUCheckCarry())
	{
		if (S9xAPUGetByte(IAPU.Address) & (1 << IAPU.Bit))
			APUSetCarry();
	}

	IAPU.PC += 3;
}

// OR1 C, not membit
static void Apu2A (void)
{
	MemBit();

	if (!APUCheckCarry())
	{
		if (!(S9xAPUGetByte(IAPU.Address) & (1 << IAPU.Bit)))
			APUSetCarry();
	}

	IAPU.PC += 3;
}

// AND1 C, membit
static void Apu4A (void)
{
	MemBit();

	if (APUCheckCarry())
	{
		if (!(S9xAPUGetByte(IAPU.Address) & (1 << IAPU.Bit)))
			APUClearCarry();
	}

	IAPU.PC += 3;
}

// AND1 C, not membit
static void Apu6A (void)
{
	MemBit();

	if (APUCheckCarry())
	{
		if ((S9xAPUGetByte(IAPU.Address) & (1 << IAPU.Bit)))
			APUClearCarry();
	}

	IAPU.PC += 3;
}

// EOR1 C, membit
static void Apu8A (void)
{
	MemBit();

	if (APUCheckCarry())
	{
		if (S9xAPUGetByte(IAPU.Address) & (1 << IAPU.Bit))
			APUClearCarry();
	}
	else
	{
		if (S9xAPUGetByte(IAPU.Address) & (1 << IAPU.Bit))
			APUSetCarry();
	}

	IAPU.PC += 3;
}

// MOV1 C, membit
static void ApuAA (void)
{
	MemBit();

	if (S9xAPUGetByte(IAPU.Address) & (1 << IAPU.Bit))
		APUSetCarry();
	else
		APUClearCarry();

	IAPU.PC += 3;
}

// MOV1 membit, C
static void ApuCA (void)
{
	MemBit();

	if (APUCheckCarry())
		S9xAPUSetByte(S9xAPUGetByte(IAPU.Address) | (1 << IAPU.Bit), IAPU.Address);
	else
		S9xAPUSetByte(S9xAPUGetByte(IAPU.Address) & ~(1 << IAPU.Bit), IAPU.Address);

	IAPU.PC += 3;
}

// NOT1 membit
static void ApuEA (void)
{
	MemBit();
	S9xAPUSetByte(S9xAPUGetByte(IAPU.Address) ^ (1 << IAPU.Bit), IAPU.Address);
	IAPU.PC += 3;
}

// ASL dp
static void Apu0B (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	ASL(Work8);
	S9xAPUSetByteZ(Work8, OP1);
	IAPU.PC += 2;
}

// ASL abs
static void Apu0C (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	ASL(Work8);
	S9xAPUSetByte(Work8, IAPU.Address);
	IAPU.PC += 3;
}

// ASL dp + X
static void Apu1B (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X);
	ASL(Work8);
	S9xAPUSetByteZ(Work8, OP1 + APURegisters.X);
	IAPU.PC += 2;
}

// ASL A
static void Apu1C (void)
{
	ASL(APURegisters.YA.B.A);
	IAPU.PC++;
}

// PUSH PSW
static void Apu0D (void)
{
	S9xAPUPackStatus();
	Push(APURegisters.P);
	IAPU.PC++;
}

// PUSH A
static void Apu2D (void)
{
	Push(APURegisters.YA.B.A);
	IAPU.PC++;
}

// PUSH X
static void Apu4D (void)
{
	Push(APURegisters.X);
	IAPU.PC++;
}

// PUSH Y
static void Apu6D (void)
{
	Push(APURegisters.YA.B.Y);
	IAPU.PC++;
}

// POP PSW
static void Apu8E (void)
{
	Pop(APURegisters.P);
	S9xAPUUnpackStatus();
	if (APUCheckDirectPage())
		IAPU.DirectPage = IAPU.RAM + 0x100;
	else
		IAPU.DirectPage = IAPU.RAM;
	IAPU.PC++;
}

// POP A
static void ApuAE (void)
{
	Pop(APURegisters.YA.B.A);
	IAPU.PC++;
}

// POP X
static void ApuCE (void)
{
	Pop(APURegisters.X);
	IAPU.PC++;
}

// POP Y
static void ApuEE (void)
{
	Pop(APURegisters.YA.B.Y);
	IAPU.PC++;
}

// TSET1 abs
static void Apu0E (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	S9xAPUSetByte(Work8 | APURegisters.YA.B.A, IAPU.Address);
	Work8 = APURegisters.YA.B.A - Work8;
	APUSetZN8(Work8);
	IAPU.PC += 3;
}

// TCLR1 abs
static void Apu4E (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	S9xAPUSetByte(Work8 & ~APURegisters.YA.B.A, IAPU.Address);
	Work8 = APURegisters.YA.B.A - Work8;
	APUSetZN8(Work8);
	IAPU.PC += 3;
}

// BRK
static void Apu0F (void)
{
#if 0
	STOP("BRK");
#else
	PushW(IAPU.PC + 1 - IAPU.RAM);
	S9xAPUPackStatus();
	Push(APURegisters.P);
	APUSetBreak();
	APUClearInterrupt();
	IAPU.PC = IAPU.RAM + S9xAPUGetByte(0xffde) + (S9xAPUGetByte(0xffdf) << 8);
#endif
}

// SLEEP
static void ApuEF (void)
{
	if (!(APU.Flags & HALTED_FLAG))
		WARN("SLEEP");
	APU.Flags |= HALTED_FLAG;
	APU.TimerEnabled[0] = APU.TimerEnabled[1] = APU.TimerEnabled[2] = FALSE;
	IAPU.APUExecuting = FALSE;
}

// STOP
static void ApuFF (void)
{
	if (!(APU.Flags & HALTED_FLAG))
		WARN("STOP");
	APU.Flags |= HALTED_FLAG;
	APU.TimerEnabled[0] = APU.TimerEnabled[1] = APU.TimerEnabled[2] = FALSE;
	IAPU.APUExecuting = FALSE;
	Settings.APUEnabled = FALSE; // re-enabled on next APU reset
}

// BPL
static void Apu10 (void)
{
	Relative();
	if (!APUCheckNegative())
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
		APUShutdown();
	}
	else
		IAPU.PC += 2;
}

// BMI
static void Apu30 (void)
{
	Relative();
	if (APUCheckNegative())
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
		APUShutdown();
	}
	else
		IAPU.PC += 2;
}

// BCC
static void Apu90 (void)
{
	Relative();
	if (!APUCheckCarry())
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
		APUShutdown();
	}
	else
		IAPU.PC += 2;
}

// BCS
static void ApuB0 (void)
{
	Relative();
	if (APUCheckCarry())
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
		APUShutdown();
	}
	else
		IAPU.PC += 2;
}

// BNE
static void ApuD0 (void)
{
	Relative();
	if (!APUCheckZero())
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
		APUShutdown();
	}
	else
		IAPU.PC += 2;
}

// BEQ
static void ApuF0 (void)
{
	Relative();
	if (APUCheckZero())
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
		APUShutdown();
	}
	else
		IAPU.PC += 2;
}

// BVC
static void Apu50 (void)
{
	Relative();
	if (!APUCheckOverflow())
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
	}
	else
		IAPU.PC += 2;
}

// BVS
static void Apu70 (void)
{
	Relative();
	if (APUCheckOverflow())
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
	}
	else
		IAPU.PC += 2;
}

// BRA
static void Apu2F (void)
{
	Relative();
	IAPU.PC = IAPU.RAM + (uint16) Int16;
}

// SETC
static void Apu80 (void)
{
	APUSetCarry();
	IAPU.PC++;
}

// NOTC
static void ApuED (void)
{
	IAPU._Carry ^= 1;
	IAPU.PC++;
}

// SETP
static void Apu40 (void)
{
	APUSetDirectPage();
	IAPU.DirectPage = IAPU.RAM + 0x100;
	IAPU.PC++;
}

// DECW dp
static void Apu1A (void)
{
	Work16 = S9xAPUGetByteZ(OP1) + (S9xAPUGetByteZ(OP1 + 1) << 8);
	Work16--;
	S9xAPUSetByteZ((uint8) Work16, OP1);
	S9xAPUSetByteZ(Work16 >> 8, OP1 + 1);
	APUSetZN16(Work16);
	IAPU.PC += 2;
}

// CMPW YA, dp
static void Apu5A (void)
{
	Work16 = S9xAPUGetByteZ(OP1) + (S9xAPUGetByteZ(OP1 + 1) << 8);
	Int32 = (int32) APURegisters.YA.W - (int32) Work16;
	IAPU._Carry = Int32 >= 0;
	APUSetZN16((uint16) Int32);
	IAPU.PC += 2;
}

// INCW dp
static void Apu3A (void)
{
	Work16 = S9xAPUGetByteZ(OP1) + (S9xAPUGetByteZ(OP1 + 1) << 8);
	Work16++;
	S9xAPUSetByteZ((uint8) Work16, OP1);
	S9xAPUSetByteZ(Work16 >> 8, OP1 + 1);
	APUSetZN16(Work16);
	IAPU.PC += 2;
}

// ADDW YA, dp
static void Apu7A (void)
{
	Work16 = S9xAPUGetByteZ(OP1) + (S9xAPUGetByteZ(OP1 + 1) << 8);
	Work32 = (uint32) APURegisters.YA.W + Work16;
	IAPU._Carry = Work32 >= 0x10000;
	if (~(APURegisters.YA.W ^ Work16) & (Work16 ^ (uint16) Work32) & 0x8000)
		APUSetOverflow();
	else
		APUClearOverflow();
	APUClearHalfCarry();
	if ((APURegisters.YA.W ^ Work16 ^ (uint16) Work32) & 0x1000)
		APUSetHalfCarry();
	APURegisters.YA.W = (uint16) Work32;
	APUSetZN16(APURegisters.YA.W);
	IAPU.PC += 2;
}

// SUBW YA, dp
static void Apu9A (void)
{
	Work16 = S9xAPUGetByteZ(OP1) + (S9xAPUGetByteZ(OP1 + 1) << 8);
	Int32 = (int32) APURegisters.YA.W - (int32) Work16;
	APUClearHalfCarry();
	IAPU._Carry = Int32 >= 0;
	if (((APURegisters.YA.W ^ Work16) & 0x8000) && ((APURegisters.YA.W ^ (uint16) Int32) & 0x8000))
		APUSetOverflow();
	else
		APUClearOverflow();
	APUSetHalfCarry();
	if ((APURegisters.YA.W ^ Work16 ^ (uint16) Int32) & 0x1000)
		APUClearHalfCarry();
	APURegisters.YA.W = (uint16) Int32;
	APUSetZN16(APURegisters.YA.W);
	IAPU.PC += 2;
}

// MOVW YA,dp
static void ApuBA (void)
{
	APURegisters.YA.B.A = S9xAPUGetByteZ(OP1);
	APURegisters.YA.B.Y = S9xAPUGetByteZ(OP1 + 1);
	APUSetZN16(APURegisters.YA.W);
	IAPU.PC += 2;
}

// MOVW dp, YA
static void ApuDA (void)
{
	S9xAPUSetByteZ(APURegisters.YA.B.A, OP1);
	S9xAPUSetByteZ(APURegisters.YA.B.Y, OP1 + 1);
	IAPU.PC += 2;
}

// CMP A, dp
static void Apu64 (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// CMP A, abs
static void Apu65 (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// CMP A, (X)
static void Apu66 (void)
{
	Work8 = S9xAPUGetByteZ(APURegisters.X);
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC++;
}

// CMP A, (dp + X)
static void Apu67 (void)
{
	IndexedXIndirect();
	Work8 = S9xAPUGetByte(IAPU.Address);
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// CMP A, #00
static void Apu68 (void)
{
	Work8 = OP1;
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// CMP dp(dest), dp(src)
static void Apu69 (void)
{
	W1 = S9xAPUGetByteZ(OP1);
	Work8 = S9xAPUGetByteZ(OP2);
	CMP(Work8, W1);
	IAPU.PC += 3;
}

// CMP A, dp + X
static void Apu74 (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X);
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// CMP A, abs + X
static void Apu75 (void)
{
	AbsoluteX();
	Work8 = S9xAPUGetByte(IAPU.Address);
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// CMP A, abs + Y
static void Apu76 (void)
{
	AbsoluteY();
	Work8 = S9xAPUGetByte(IAPU.Address);
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// CMP A, (dp) + Y
static void Apu77 (void)
{
	IndirectIndexedY();
	Work8 = S9xAPUGetByte(IAPU.Address);
	CMP(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// CMP dp, #00
static void Apu78 (void)
{
	Work8 = OP1;
	W1 = S9xAPUGetByteZ(OP2);
	CMP(W1, Work8);
	IAPU.PC += 3;
}

// CMP (X), (Y)
static void Apu79 (void)
{
	W1 = S9xAPUGetByteZ(APURegisters.X);
	Work8 = S9xAPUGetByteZ(APURegisters.YA.B.Y);
	CMP(W1, Work8);
	IAPU.PC++;
}

// CMP X, abs
static void Apu1E (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	CMP(APURegisters.X, Work8);
	IAPU.PC += 3;
}

// CMP X, dp
static void Apu3E (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	CMP(APURegisters.X, Work8);
	IAPU.PC += 2;
}

// CMP X, #00
static void ApuC8 (void)
{
	CMP(APURegisters.X, OP1);
	IAPU.PC += 2;
}

// CMP Y, abs
static void Apu5E (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	CMP(APURegisters.YA.B.Y, Work8);
	IAPU.PC += 3;
}

// CMP Y, dp
static void Apu7E (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	CMP(APURegisters.YA.B.Y, Work8);
	IAPU.PC += 2;
}

// CMP Y, #00
static void ApuAD (void)
{
	Work8 = OP1;
	CMP(APURegisters.YA.B.Y, Work8);
	IAPU.PC += 2;
}

// JMP (abs + X)
static void Apu1F (void)
{
	Absolute();
	IAPU.PC = IAPU.RAM + S9xAPUGetByte(IAPU.Address + APURegisters.X) + (S9xAPUGetByte(IAPU.Address + APURegisters.X + 1) << 8);
}

// JMP abs
static void Apu5F (void)
{
	Absolute();
	IAPU.PC = IAPU.RAM + IAPU.Address;
}

// CLRP
static void Apu20 (void)
{
	APUClearDirectPage();
	IAPU.DirectPage = IAPU.RAM;
	IAPU.PC++;
}

// CLRC
static void Apu60 (void)
{
	APUClearCarry();
	IAPU.PC++;
}

// CLRV
static void ApuE0 (void)
{
	APUClearHalfCarry();
	APUClearOverflow();
	IAPU.PC++;
}

// AND A, dp
static void Apu24 (void)
{
	APURegisters.YA.B.A &= S9xAPUGetByteZ(OP1);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// AND A, abs
static void Apu25 (void)
{
	Absolute();
	APURegisters.YA.B.A &= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// AND A, (X)
static void Apu26 (void)
{
	APURegisters.YA.B.A &= S9xAPUGetByteZ(APURegisters.X);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// AND A, (dp + X)
static void Apu27 (void)
{
	IndexedXIndirect();
	APURegisters.YA.B.A &= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// AND A, #00
static void Apu28 (void)
{
	APURegisters.YA.B.A &= OP1;
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// AND dp(dest), dp(src)
static void Apu29 (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	Work8 &= S9xAPUGetByteZ(OP2);
	S9xAPUSetByteZ(Work8, OP2);
	APUSetZN8(Work8);
	IAPU.PC += 3;
}

// AND A, dp + X
static void Apu34 (void)
{
	APURegisters.YA.B.A &= S9xAPUGetByteZ(OP1 + APURegisters.X);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// AND A, abs + X
static void Apu35 (void)
{
	AbsoluteX();
	APURegisters.YA.B.A &= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// AND A, abs + Y
static void Apu36 (void)
{
	AbsoluteY();
	APURegisters.YA.B.A &= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// AND A, (dp) + Y
static void Apu37 (void)
{
	IndirectIndexedY();
	APURegisters.YA.B.A &= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// AND dp, #00
static void Apu38 (void)
{
	Work8 = OP1;
	Work8 &= S9xAPUGetByteZ(OP2);
	S9xAPUSetByteZ(Work8, OP2);
	APUSetZN8(Work8);
	IAPU.PC += 3;
}

// AND (X), (Y)
static void Apu39 (void)
{
	Work8 = S9xAPUGetByteZ(APURegisters.X) & S9xAPUGetByteZ(APURegisters.YA.B.Y);
	APUSetZN8(Work8);
	S9xAPUSetByteZ(Work8, APURegisters.X);
	IAPU.PC++;
}

// ROL dp
static void Apu2B (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	ROL(Work8);
	S9xAPUSetByteZ(Work8, OP1);
	IAPU.PC += 2;
}

// ROL abs
static void Apu2C (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	ROL(Work8);
	S9xAPUSetByte(Work8, IAPU.Address);
	IAPU.PC += 3;
}

// ROL dp + X
static void Apu3B (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X);
	ROL(Work8);
	S9xAPUSetByteZ(Work8, OP1 + APURegisters.X);
	IAPU.PC += 2;
}

// ROL A
static void Apu3C (void)
{
	ROL(APURegisters.YA.B.A);
	IAPU.PC++;
}

// CBNE dp, rel
static void Apu2E (void)
{
	Work8 = OP1;
	Relative2();

	if (S9xAPUGetByteZ(Work8) != APURegisters.YA.B.A)
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
		APUShutdown();
	}
	else
		IAPU.PC += 3;
}

// CBNE dp + X, rel
static void ApuDE (void)
{
	Work8 = OP1 + APURegisters.X;
	Relative2();

	if (S9xAPUGetByteZ(Work8) != APURegisters.YA.B.A)
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
		APUShutdown();
	}
	else
		IAPU.PC += 3;
}

// INC X
static void Apu3D (void)
{
	APURegisters.X++;
	APUSetZN8(APURegisters.X);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC++;
}

// INC Y
static void ApuFC (void)
{
	APURegisters.YA.B.Y++;
	APUSetZN8(APURegisters.YA.B.Y);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC++;
}

// DEC X
static void Apu1D (void)
{
	APURegisters.X--;
	APUSetZN8(APURegisters.X);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC++;
}

// DEC Y
static void ApuDC (void)
{
	APURegisters.YA.B.Y--;
	APUSetZN8(APURegisters.YA.B.Y);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC++;
}

// INC dp
static void ApuAB (void)
{
	Work8 = S9xAPUGetByteZ(OP1) + 1;
	S9xAPUSetByteZ(Work8, OP1);
	APUSetZN8(Work8);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC += 2;
}

// INC abs
static void ApuAC (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address) + 1;
	S9xAPUSetByte(Work8, IAPU.Address);
	APUSetZN8(Work8);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC += 3;
}

// INC dp + X
static void ApuBB (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X) + 1;
	S9xAPUSetByteZ(Work8, OP1 + APURegisters.X);
	APUSetZN8(Work8);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC += 2;
}

// INC A
static void ApuBC (void)
{
	APURegisters.YA.B.A++;
	APUSetZN8(APURegisters.YA.B.A);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC++;
}

// DEC dp
static void Apu8B (void)
{
	Work8 = S9xAPUGetByteZ(OP1) - 1;
	S9xAPUSetByteZ(Work8, OP1);
	APUSetZN8(Work8);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC += 2;
}

// DEC abs
static void Apu8C (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address) - 1;
	S9xAPUSetByte(Work8, IAPU.Address);
	APUSetZN8(Work8);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC += 3;
}

// DEC dp + X
static void Apu9B (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X) - 1;
	S9xAPUSetByteZ(Work8, OP1 + APURegisters.X);
	APUSetZN8(Work8);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC += 2;
}

// DEC A
static void Apu9C (void)
{
	APURegisters.YA.B.A--;
	APUSetZN8(APURegisters.YA.B.A);
#ifdef SPC700_SHUTDOWN
	IAPU.WaitCounter++;
#endif
	IAPU.PC++;
}

// EOR A, dp
static void Apu44 (void)
{
	APURegisters.YA.B.A ^= S9xAPUGetByteZ(OP1);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// EOR A, abs
static void Apu45 (void)
{
	Absolute();
	APURegisters.YA.B.A ^= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// EOR A, (X)
static void Apu46 (void)
{
	APURegisters.YA.B.A ^= S9xAPUGetByteZ(APURegisters.X);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// EOR A, (dp + X)
static void Apu47 (void)
{
	IndexedXIndirect();
	APURegisters.YA.B.A ^= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// EOR A, #00
static void Apu48 (void)
{
	APURegisters.YA.B.A ^= OP1;
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// EOR dp(dest), dp(src)
static void Apu49 (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	Work8 ^= S9xAPUGetByteZ(OP2);
	S9xAPUSetByteZ(Work8, OP2);
	APUSetZN8(Work8);
	IAPU.PC += 3;
}

// EOR A, dp + X
static void Apu54 (void)
{
	APURegisters.YA.B.A ^= S9xAPUGetByteZ(OP1 + APURegisters.X);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// EOR A, abs + X
static void Apu55 (void)
{
	AbsoluteX();
	APURegisters.YA.B.A ^= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// EOR A, abs + Y
static void Apu56 (void)
{
	AbsoluteY();
	APURegisters.YA.B.A ^= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// EOR A, (dp) + Y
static void Apu57 (void)
{
	IndirectIndexedY();
	APURegisters.YA.B.A ^= S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// EOR dp, #00
static void Apu58 (void)
{
	Work8 = OP1;
	Work8 ^= S9xAPUGetByteZ(OP2);
	S9xAPUSetByteZ(Work8, OP2);
	APUSetZN8(Work8);
	IAPU.PC += 3;
}

// EOR (X), (Y)
static void Apu59 (void)
{
	Work8 = S9xAPUGetByteZ(APURegisters.X) ^ S9xAPUGetByteZ(APURegisters.YA.B.Y);
	APUSetZN8(Work8);
	S9xAPUSetByteZ(Work8, APURegisters.X);
	IAPU.PC++;
}

// LSR dp
static void Apu4B (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	LSR(Work8);
	S9xAPUSetByteZ(Work8, OP1);
	IAPU.PC += 2;
}

// LSR abs
static void Apu4C (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	LSR(Work8);
	S9xAPUSetByte(Work8, IAPU.Address);
	IAPU.PC += 3;
}

// LSR dp + X
static void Apu5B (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X);
	LSR(Work8);
	S9xAPUSetByteZ(Work8, OP1 + APURegisters.X);
	IAPU.PC += 2;
}

// LSR A
static void Apu5C (void)
{
	LSR(APURegisters.YA.B.A);
	IAPU.PC++;
}

// MOV A, X
static void Apu7D (void)
{
	APURegisters.YA.B.A = APURegisters.X;
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// MOV A, Y
static void ApuDD (void)
{
	APURegisters.YA.B.A = APURegisters.YA.B.Y;
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// MOV X, A
static void Apu5D (void)
{
	APURegisters.X = APURegisters.YA.B.A;
	APUSetZN8(APURegisters.X);
	IAPU.PC++;
}

// MOV Y, A
static void ApuFD (void)
{
	APURegisters.YA.B.Y = APURegisters.YA.B.A;
	APUSetZN8(APURegisters.YA.B.Y);
	IAPU.PC++;
}

// MOV X, SP
static void Apu9D (void)
{
	APURegisters.X = APURegisters.S;
	APUSetZN8(APURegisters.X);
	IAPU.PC++;
}

// MOV SP, X
static void ApuBD (void)
{
	APURegisters.S = APURegisters.X;
	IAPU.PC++;
}

// ROR dp
static void Apu6B (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	ROR(Work8);
	S9xAPUSetByteZ(Work8, OP1);
	IAPU.PC += 2;
}

// ROR abs
static void Apu6C (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	ROR(Work8);
	S9xAPUSetByte(Work8, IAPU.Address);
	IAPU.PC += 3;
}

// ROR dp + X
static void Apu7B (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X);
	ROR(Work8);
	S9xAPUSetByteZ(Work8, OP1 + APURegisters.X);
	IAPU.PC += 2;
}

// ROR A
static void Apu7C (void)
{
	ROR(APURegisters.YA.B.A);
	IAPU.PC++;
}

// DBNZ dp, rel
static void Apu6E (void)
{
	Work8 = OP1;
	Relative2();
	W1 = S9xAPUGetByteZ(Work8) - 1;
	S9xAPUSetByteZ(W1, Work8);
	if (W1 != 0)
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
	}
	else
		IAPU.PC += 3;
}

// DBNZ Y, rel
static void ApuFE (void)
{
	Relative();
	APURegisters.YA.B.Y--;
	if (APURegisters.YA.B.Y != 0)
	{
		IAPU.PC = IAPU.RAM + (uint16) Int16;
		APU.Cycles += IAPU.TwoCycles;
	}
	else
		IAPU.PC += 2;
}

// RET
static void Apu6F (void)
{
	PopW(APURegisters.PC);
	IAPU.PC = IAPU.RAM + APURegisters.PC;
}

// RETI
static void Apu7F (void)
{
	//STOP("RETI");
	Pop(APURegisters.P);
	S9xAPUUnpackStatus();
	PopW(APURegisters.PC);
	IAPU.PC = IAPU.RAM + APURegisters.PC;
}

// ADC A, dp
static void Apu84 (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// ADC A, abs
static void Apu85 (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// ADC A, (X)
static void Apu86 (void)
{
	Work8 = S9xAPUGetByteZ(APURegisters.X);
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC++;
}

// ADC A, (dp + X)
static void Apu87 (void)
{
	IndexedXIndirect();
	Work8 = S9xAPUGetByte(IAPU.Address);
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// ADC A, #00
static void Apu88 (void)
{
	Work8 = OP1;
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// ADC dp(dest), dp(src)
static void Apu89 (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	W1 = S9xAPUGetByteZ(OP2);
	ADC(W1, Work8);
	S9xAPUSetByteZ(W1, OP2);
	IAPU.PC += 3;
}

// ADC A, dp + X
static void Apu94 (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X);
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// ADC A, abs + X
static void Apu95 (void)
{
	AbsoluteX();
	Work8 = S9xAPUGetByte(IAPU.Address);
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// ADC A, abs + Y
static void Apu96 (void)
{
	AbsoluteY();
	Work8 = S9xAPUGetByte(IAPU.Address);
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// ADC A, (dp) + Y
static void Apu97 (void)
{
	IndirectIndexedY();
	Work8 = S9xAPUGetByte(IAPU.Address);
	ADC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// ADC dp, #00
static void Apu98 (void)
{
	Work8 = OP1;
	W1 = S9xAPUGetByteZ(OP2);
	ADC(W1, Work8);
	S9xAPUSetByteZ(W1, OP2);
	IAPU.PC += 3;
}

// ADC (X), (Y)
static void Apu99 (void)
{
	W1 = S9xAPUGetByteZ(APURegisters.X);
	Work8 = S9xAPUGetByteZ(APURegisters.YA.B.Y);
	ADC(W1, Work8);
	S9xAPUSetByteZ(W1, APURegisters.X);
	IAPU.PC++;
}

// MOV Y, #00
static void Apu8D (void)
{
	APURegisters.YA.B.Y = OP1;
	APUSetZN8(APURegisters.YA.B.Y);
	IAPU.PC += 2;
}

// MOV dp, #00
static void Apu8F (void)
{
	Work8 = OP1;
	S9xAPUSetByteZ(Work8, OP2);
	IAPU.PC += 3;
}

// DIV YA, X
static void Apu9E (void)
{
	if ((APURegisters.X & 0xf) <= (APURegisters.YA.B.Y & 0x0f))
		APUSetHalfCarry();
	else
		APUClearHalfCarry();

	uint32	yva, x;

	yva = APURegisters.YA.W;
	x = APURegisters.X << 9;

	for (int i = 0; i < 9; i++)
	{
		yva <<= 1;
		if (yva & 0x20000)
			yva = (yva & 0x1ffff) | 1;
		if (yva >= x)
			yva ^= 1;
		if (yva & 1)
			yva = (yva - x) & 0x1ffff;
	}

	if (yva & 0x100)
		APUSetOverflow();
	else
		APUClearOverflow();

	APURegisters.YA.B.Y = (yva >> 9) & 0xff;
	APURegisters.YA.B.A = yva & 0xff;
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// XCN A
static void Apu9F (void)
{
	APURegisters.YA.B.A = (APURegisters.YA.B.A >> 4) | (APURegisters.YA.B.A << 4);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// SBC A, dp
static void ApuA4 (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// SBC A, abs
static void ApuA5 (void)
{
	Absolute();
	Work8 = S9xAPUGetByte(IAPU.Address);
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// SBC A, (X)
static void ApuA6 (void)
{
	Work8 = S9xAPUGetByteZ(APURegisters.X);
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC++;
}

// SBC A, (dp + X)
static void ApuA7 (void)
{
	IndexedXIndirect();
	Work8 = S9xAPUGetByte(IAPU.Address);
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// SBC A, #00
static void ApuA8 (void)
{
	Work8 = OP1;
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// SBC dp(dest), dp(src)
static void ApuA9 (void)
{
	Work8 = S9xAPUGetByteZ(OP1);
	W1 = S9xAPUGetByteZ(OP2);
	SBC(W1, Work8);
	S9xAPUSetByteZ(W1, OP2);
	IAPU.PC += 3;
}

// SBC A, dp + X
static void ApuB4 (void)
{
	Work8 = S9xAPUGetByteZ(OP1 + APURegisters.X);
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// SBC A, abs + X
static void ApuB5 (void)
{
	AbsoluteX();
	Work8 = S9xAPUGetByte(IAPU.Address);
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// SBC A, abs + Y
static void ApuB6 (void)
{
	AbsoluteY();
	Work8 = S9xAPUGetByte(IAPU.Address);
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 3;
}

// SBC A, (dp) + Y
static void ApuB7 (void)
{
	IndirectIndexedY();
	Work8 = S9xAPUGetByte(IAPU.Address);
	SBC(APURegisters.YA.B.A, Work8);
	IAPU.PC += 2;
}

// SBC dp, #00
static void ApuB8 (void)
{
	Work8 = OP1;
	W1 = S9xAPUGetByteZ(OP2);
	SBC(W1, Work8);
	S9xAPUSetByteZ(W1, OP2);
	IAPU.PC += 3;
}

// SBC (X), (Y)
static void ApuB9 (void)
{
	W1 = S9xAPUGetByteZ(APURegisters.X);
	Work8 = S9xAPUGetByteZ(APURegisters.YA.B.Y);
	SBC(W1, Work8);
	S9xAPUSetByteZ(W1, APURegisters.X);
	IAPU.PC++;
}

// MOV (X)+, A
static void ApuAF (void)
{
	S9xAPUSetByteZ(APURegisters.YA.B.A, APURegisters.X++);
	IAPU.PC++;
}

// DAS
static void ApuBE (void)
{
	if (APURegisters.YA.B.A > 0x99 || !IAPU._Carry)
	{
		APURegisters.YA.B.A -= 0x60;
		APUClearCarry();
	}
	else
		APUSetCarry();

	if ((APURegisters.YA.B.A & 0x0f) > 9 || !APUCheckHalfCarry())
		APURegisters.YA.B.A -= 6;

	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// MOV A, (X)+
static void ApuBF (void)
{
	APURegisters.YA.B.A = S9xAPUGetByteZ(APURegisters.X++);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// DI
static void ApuC0 (void)
{
	APUClearInterrupt();
	IAPU.PC++;
}

// EI
static void ApuA0 (void)
{
	APUSetInterrupt();
	IAPU.PC++;
}

// MOV dp, A
static void ApuC4 (void)
{
	S9xAPUSetByteZ(APURegisters.YA.B.A, OP1);
	IAPU.PC += 2;
}

// MOV abs, A
static void ApuC5 (void)
{
	Absolute();
	S9xAPUSetByte(APURegisters.YA.B.A, IAPU.Address);
	IAPU.PC += 3;
}

// MOV (X), A
static void ApuC6 (void)
{
	S9xAPUSetByteZ(APURegisters.YA.B.A, APURegisters.X);
	IAPU.PC++;
}

// MOV (dp + X), A
static void ApuC7 (void)
{
	IndexedXIndirect();
	S9xAPUSetByte(APURegisters.YA.B.A, IAPU.Address);
	IAPU.PC += 2;
}

// MOV abs, X
static void ApuC9 (void)
{
	Absolute();
	S9xAPUSetByte(APURegisters.X, IAPU.Address);
	IAPU.PC += 3;
}

// MOV dp, Y
static void ApuCB (void)
{
	S9xAPUSetByteZ(APURegisters.YA.B.Y, OP1);
	IAPU.PC += 2;
}

// MOV abs, Y
static void ApuCC (void)
{
	Absolute();
	S9xAPUSetByte(APURegisters.YA.B.Y, IAPU.Address);
	IAPU.PC += 3;
}

// MOV X, #00
static void ApuCD (void)
{
	APURegisters.X = OP1;
	APUSetZN8(APURegisters.X);
	IAPU.PC += 2;
}

// MUL YA
static void ApuCF (void)
{
	APURegisters.YA.W = (uint16) APURegisters.YA.B.A * APURegisters.YA.B.Y;
	APUSetZN8(APURegisters.YA.B.Y);
	IAPU.PC++;
}

// MOV dp + X, A
static void ApuD4 (void)
{
	S9xAPUSetByteZ(APURegisters.YA.B.A, OP1 + APURegisters.X);
	IAPU.PC += 2;
}

// MOV abs + X, A
static void ApuD5 (void)
{
	AbsoluteX();
	S9xAPUSetByte(APURegisters.YA.B.A, IAPU.Address);
	IAPU.PC += 3;
}

// MOV abs + Y, A
static void ApuD6 (void)
{
	AbsoluteY();
	S9xAPUSetByte(APURegisters.YA.B.A, IAPU.Address);
	IAPU.PC += 3;
}

// MOV (dp) + Y, A
static void ApuD7 (void)
{
	IndirectIndexedY();
	S9xAPUSetByte(APURegisters.YA.B.A, IAPU.Address);
	IAPU.PC += 2;
}

// MOV dp, X
static void ApuD8 (void)
{
	S9xAPUSetByteZ(APURegisters.X, OP1);
	IAPU.PC += 2;
}

// MOV dp + Y, X
static void ApuD9 (void)
{
	S9xAPUSetByteZ(APURegisters.X, OP1 + APURegisters.YA.B.Y);
	IAPU.PC += 2;
}

// MOV dp + X, Y
static void ApuDB (void)
{
	S9xAPUSetByteZ(APURegisters.YA.B.Y, OP1 + APURegisters.X);
	IAPU.PC += 2;
}

// DAA
static void ApuDF (void)
{
	if (APURegisters.YA.B.A > 0x99 || IAPU._Carry)
	{
		APURegisters.YA.B.A += 0x60;
		APUSetCarry();
	}
	else
		APUClearCarry();

	if ((APURegisters.YA.B.A & 0x0f) > 9 || APUCheckHalfCarry())
	{
		APURegisters.YA.B.A += 6;
		//APUSetHalfCarry();	// Intel procs do this, but this is a Sony proc...
	}
	//else
		//APUClearHalfCarry();	// ditto as above

	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// MOV A, dp
static void ApuE4 (void)
{
	APURegisters.YA.B.A = S9xAPUGetByteZ(OP1);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// MOV A, abs
static void ApuE5 (void)
{
	Absolute();
	APURegisters.YA.B.A = S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// MOV A, (X)
static void ApuE6 (void)
{
	APURegisters.YA.B.A = S9xAPUGetByteZ(APURegisters.X);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC++;
}

// MOV A, (dp + X)
static void ApuE7 (void)
{
	IndexedXIndirect();
	APURegisters.YA.B.A = S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// MOV A, #00
static void ApuE8 (void)
{
	APURegisters.YA.B.A = OP1;
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// MOV X, abs
static void ApuE9 (void)
{
	Absolute();
	APURegisters.X = S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.X);
	IAPU.PC += 3;
}

// MOV Y, dp
static void ApuEB (void)
{
	APURegisters.YA.B.Y = S9xAPUGetByteZ(OP1);
	APUSetZN8(APURegisters.YA.B.Y);
	IAPU.PC += 2;
}

// MOV Y, abs
static void ApuEC (void)
{
	Absolute();
	APURegisters.YA.B.Y = S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.Y);
	IAPU.PC += 3;
}

// MOV A, dp + X
static void ApuF4 (void)
{
	APURegisters.YA.B.A = S9xAPUGetByteZ(OP1 + APURegisters.X);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// MOV A, abs + X
static void ApuF5 (void)
{
	AbsoluteX();
	APURegisters.YA.B.A = S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// MOV A, abs + Y
static void ApuF6 (void)
{
	AbsoluteY();
	APURegisters.YA.B.A = S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 3;
}

// MOV A, (dp) + Y
static void ApuF7 (void)
{
	IndirectIndexedY();
	APURegisters.YA.B.A = S9xAPUGetByte(IAPU.Address);
	APUSetZN8(APURegisters.YA.B.A);
	IAPU.PC += 2;
}

// MOV X, dp
static void ApuF8 (void)
{
	APURegisters.X = S9xAPUGetByteZ(OP1);
	APUSetZN8(APURegisters.X);
	IAPU.PC += 2;
}

// MOV X, dp + Y
static void ApuF9 (void)
{
	APURegisters.X = S9xAPUGetByteZ(OP1 + APURegisters.YA.B.Y);
	APUSetZN8(APURegisters.X);
	IAPU.PC += 2;
}

// MOV dp(dest), dp(src)
static void ApuFA (void)
{
	S9xAPUSetByteZ(S9xAPUGetByteZ(OP1), OP2);
	IAPU.PC += 3;
}

// MOV Y, dp + X
static void ApuFB (void)
{
	APURegisters.YA.B.Y = S9xAPUGetByteZ(OP1 + APURegisters.X);
	APUSetZN8(APURegisters.YA.B.Y);
	IAPU.PC += 2;
}

void (*S9xApuOpcodes[256]) (void) =
{
	Apu00, Apu01, Apu02, Apu03, Apu04, Apu05, Apu06, Apu07, Apu08, Apu09, Apu0A, Apu0B, Apu0C, Apu0D, Apu0E, Apu0F,
	Apu10, Apu11, Apu12, Apu13, Apu14, Apu15, Apu16, Apu17, Apu18, Apu19, Apu1A, Apu1B, Apu1C, Apu1D, Apu1E, Apu1F,
	Apu20, Apu21, Apu22, Apu23, Apu24, Apu25, Apu26, Apu27, Apu28, Apu29, Apu2A, Apu2B, Apu2C, Apu2D, Apu2E, Apu2F,
	Apu30, Apu31, Apu32, Apu33, Apu34, Apu35, Apu36, Apu37, Apu38, Apu39, Apu3A, Apu3B, Apu3C, Apu3D, Apu3E, Apu3F,
	Apu40, Apu41, Apu42, Apu43, Apu44, Apu45, Apu46, Apu47, Apu48, Apu49, Apu4A, Apu4B, Apu4C, Apu4D, Apu4E, Apu4F,
	Apu50, Apu51, Apu52, Apu53, Apu54, Apu55, Apu56, Apu57, Apu58, Apu59, Apu5A, Apu5B, Apu5C, Apu5D, Apu5E, Apu5F,
	Apu60, Apu61, Apu62, Apu63, Apu64, Apu65, Apu66, Apu67, Apu68, Apu69, Apu6A, Apu6B, Apu6C, Apu6D, Apu6E, Apu6F,
	Apu70, Apu71, Apu72, Apu73, Apu74, Apu75, Apu76, Apu77, Apu78, Apu79, Apu7A, Apu7B, Apu7C, Apu7D, Apu7E, Apu7F,
	Apu80, Apu81, Apu82, Apu83, Apu84, Apu85, Apu86, Apu87, Apu88, Apu89, Apu8A, Apu8B, Apu8C, Apu8D, Apu8E, Apu8F,
	Apu90, Apu91, Apu92, Apu93, Apu94, Apu95, Apu96, Apu97, Apu98, Apu99, Apu9A, Apu9B, Apu9C, Apu9D, Apu9E, Apu9F,
	ApuA0, ApuA1, ApuA2, ApuA3, ApuA4, ApuA5, ApuA6, ApuA7, ApuA8, ApuA9, ApuAA, ApuAB, ApuAC, ApuAD, ApuAE, ApuAF,
	ApuB0, ApuB1, ApuB2, ApuB3, ApuB4, ApuB5, ApuB6, ApuB7, ApuB8, ApuB9, ApuBA, ApuBB, ApuBC, ApuBD, ApuBE, ApuBF,
	ApuC0, ApuC1, ApuC2, ApuC3, ApuC4, ApuC5, ApuC6, ApuC7, ApuC8, ApuC9, ApuCA, ApuCB, ApuCC, ApuCD, ApuCE, ApuCF,
	ApuD0, ApuD1, ApuD2, ApuD3, ApuD4, ApuD5, ApuD6, ApuD7, ApuD8, ApuD9, ApuDA, ApuDB, ApuDC, ApuDD, ApuDE, ApuDF,
	ApuE0, ApuE1, ApuE2, ApuE3, ApuE4, ApuE5, ApuE6, ApuE7, ApuE8, ApuE9, ApuEA, ApuEB, ApuEC, ApuED, ApuEE, ApuEF,
	ApuF0, ApuF1, ApuF2, ApuF3, ApuF4, ApuF5, ApuF6, ApuF7, ApuF8, ApuF9, ApuFA, ApuFB, ApuFC, ApuFD, ApuFE, ApuFF
};
