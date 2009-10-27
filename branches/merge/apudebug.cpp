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


#ifdef DEBUGGER

#include "snes9x.h"
#include "memmap.h"
#include "apu.h"
#include "soundux.h"

FILE	*apu_trace = NULL;

static const char	*apu_mnemonics[256] =
{
	"NOP",
	"TCALL 0",
	"SET1  $%02X.0",
	"BBS   $%02X.0,$%04X",
	"OR    A,$%02X",
	"OR    A,!$%04X",
	"OR    A,(X)",
	"OR    A,[$%02X+X]",
	"OR    A,#$%02X",
	"OR    $%02X,$%02X",
	"OR1   C,$%04X.%d",
	"ASL   $%02X",
	"MOV   !$%04X,Y",
	"PUSH  PSW",
	"TSET1 !$%04X",
	"BRK",
	"BPL   $%04X",
	"TCALL 1",
	"CLR1  $%02X.0",
	"BBC   $%02X.0,$%04X",
	"OR    A,$%02X+X",
	"OR    A,!$%04X+X",
	"OR    A,!$%04X+Y",
	"OR    A,[$%02X]+Y",
	"OR    $%02X,#$%02X",
	"OR    (X),(Y)",
	"DECW  $%02X",
	"ASL   $%02X+X",
	"ASL   A",
	"DEC   X",
	"CMP   X,!$%04X",
	"JMP   [!$%04X+X]",
	"CLRP",
	"TCALL 2",
	"SET1  $%02X.1",
	"BBS   $%02X.1,$%04X",
	"AND   A,$%02X",
	"AND   A,!$%04X",
	"AND   A,(X)",
	"AND   A,[$%02X+X]",
	"AND   A,#$%02X",
	"AND   $%02X,$%02X",
	"OR1   C,/$%04X.%d",
	"ROL   $%02X",
	"ROL   !$%04X",
	"PUSH  A",
	"CBNE  $%02X,$%04X",
	"BRA   $%04X",
	"BMI   $%04X",
	"TCALL 3",
	"CLR1  $%02X.1",
	"BBC   $%02X.1,$%04X",
	"AND   A,$%02X+X",
	"AND   A,!$%04X+X",
	"AND   A,!$%04X+Y",
	"AND   A,[$%02X]+Y",
	"AND   $%02X,#$%02X",
	"AND   (X),(Y)",
	"INCW  $%02X",
	"ROL   $%02X+X",
	"ROL   A",
	"INC   X",
	"CMP   X,$%02X",
	"CALL  !$%04X",
	"SETP",
	"TCALL 4",
	"SET1  $%02X.2",
	"BBS   $%02X.2,$%04X",
	"EOR   A,$%02X",
	"EOR   A,!$%04X",
	"EOR   A,(X)",
	"EOR   A,[$%02X+X]",
	"EOR   A,#$%02X",
	"EOR   $%02X,$%02X",
	"AND1  C,$%04X.%d",
	"LSR   $%02X",
	"LSR   !$%04X",
	"PUSH  X",
	"TCLR1 !$%04X",
	"PCALL $%02X",
	"BVC   $%04X",
	"TCALL 5",
	"CLR1  $%02X.2",
	"BBC   $%02X.2,$%04X",
	"EOR   A,$%02X+X",
	"EOR   A,!$%04X+X",
	"EOR   A,!$%04X+Y",
	"EOR   A,[$%02X]+Y",
	"EOR   $%02X,#$%02X",
	"EOR   (X),(Y)",
	"CMPW  YA,$%02X",
	"LSR   $%02X+X",
	"LSR   A",
	"MOV   X,A",
	"CMP   Y,!$%04X",
	"JMP   !$%04X",
	"CLRC",
	"TCALL 6",
	"SET1  $%02X.3",
	"BBS   $%02X.3,$%04X",
	"CMP   A,$%02X",
	"CMP   A,!$%04X",
	"CMP   A,(X)",
	"CMP   A,[$%02X+X]",
	"CMP   A,#$%02X",
	"CMP   $%02X,$%02X",
	"AND1  C,/$%04X.%d",
	"ROR   $%02X",
	"ROR   !$%04X",
	"PUSH  Y",
	"DBNZ  $%02X,$%04X",
	"RET",
	"BVS   $%04X",
	"TCALL 7",
	"CLR1  $%02X.3",
	"BBC   $%02X.3,$%04X",
	"CMP   A,$%02X+X",
	"CMP   A,!$%04X+X",
	"CMP   A,!$%04X+Y",
	"CMP   A,[$%02X]+Y",
	"CMP   $%02X,#$%02X",
	"CMP   (X),(Y)",
	"ADDW  YA,$%02X",
	"ROR   $%02X+X",
	"ROR   A",
	"MOV   A,X",
	"CMP   Y,$%02X",
	"RET1",
	"SETC",
	"TCALL 8",
	"SET1  $%02X.4",
	"BBS   $%02X.4,$%04X",
	"ADC   A,$%02X",
	"ADC   A,!$%04X",
	"ADC   A,(X)",
	"ADC   A,[$%02X+X]",
	"ADC   A,#$%02X",
	"ADC   $%02X,$%02X",
	"EOR1  C,$%04X.%d",
	"DEC   $%02X",
	"DEC   !$%04X",
	"MOV   Y,#$%02X",
	"POP   PSW",
	"MOV   $%02X,#$%02X",
	"BCC   $%04X",
	"TCALL 9",
	"CLR1  $%02X.4",
	"BBC   $%02X.4,$%04X",
	"ADC   A,$%02X+X",
	"ADC   A,!$%04X+X",
	"ADC   A,!$%04X+Y",
	"ADC   A,[$%02X]+Y",
	"ADC   $%02X,#$%02X",
	"ADC   (X),(Y)",
	"SUBW  YA,$%02X",
	"DEC   $%02X+X",
	"DEC   A",
	"MOV   X,SP",
	"DIV   YA,X",
	"XCN   A",
	"EI",
	"TCALL 10",
	"SET1  $%02X.5",
	"BBS   $%02X.5,$%04X",
	"SBC   A,$%02X",
	"SBC   A,!$%04X",
	"SBC   A,(X)",
	"SBC   A,[$%02X+X]",
	"SBC   A,#$%02X",
	"SBC   $%02X,$%02X",
	"MOV1  C,$%04X.%d",
	"INC   $%02X",
	"INC   !$%04X",
	"CMP   Y,#$%02X",
	"POP   A",
	"MOV   (X)+,A",
	"BCS   $%04X",
	"TCALL 11",
	"CLR1  $%02X.5",
	"BBC   $%02X.5,$%04X",
	"SBC   A,$%02X+X",
	"SBC   A,!$%04X+X",
	"SBC   A,!$%04X+Y",
	"SBC   A,[$%02X]+Y",
	"SBC   $%02X,#$%02X",
	"SBC   (X),(Y)",
	"MOVW  YA,$%02X",
	"INC   $%02X+X",
	"INC   A",
	"MOV   SP,X",
	"DAS   A",
	"MOV   A,(X)+",
	"DI",
	"TCALL 12",
	"SET1  $%02X.6",
	"BBS   $%02X.6,$%04X",
	"MOV   $%02X,A",
	"MOV   !$%04X,A",
	"MOV   (X),A",
	"MOV   [$%02X+X],A",
	"CMP   X,#$%02X",
	"MOV   !$%04X,X",
	"MOV1  $%04X.%d,C",
	"MOV   $%02X,Y",
	"ASL   !$%04X",
	"MOV   X,#$%02X",
	"POP   X",
	"MUL   YA",
	"BNE   $%04X",
	"TCALL 13",
	"CLR1  $%02X.6",
	"BBC   $%02X.6,$%04X",
	"MOV   $%02X+X,A",
	"MOV   !$%04X+X,A",
	"MOV   !$%04X+Y,A",
	"MOV   [$%02X]+Y,A",
	"MOV   $%02X,X",
	"MOV   $%02X+Y,X",
	"MOVW  $%02X,YA",
	"MOV   $%02X+X,Y",
	"DEC   Y",
	"MOV   A,Y",
	"CBNE  $%02X+X,$%04X",
	"DAA   A",
	"CLRV",
	"TCALL 14",
	"SET1  $%02X.7",
	"BBS   $%02X.7,$%04X",
	"MOV   A,$%02X",
	"MOV   A,!$%04X",
	"MOV   A,(X)",
	"MOV   A,[$%02X+X]",
	"MOV   A,#$%02X",
	"MOV   X,!$%04X",
	"NOT1  $%04X.%d",
	"MOV   Y,$%02X",
	"MOV   Y,!$%04X",
	"NOTC",
	"POP   Y",
	"SLEEP",
	"BEQ   $%04X",
	"TCALL 15",
	"CLR1  $%02X.7",
	"BBC   $%02X.7,$%04X",
	"MOV   A,$%02X+X",
	"MOV   A,!$%04X+X",
	"MOV   A,!$%04X+Y",
	"MOV   A,[$%02X]+Y",
	"MOV   X,$%02X",
	"MOV   X,$%02X+Y",
	"MOV   $%02X,$%02X",
	"MOV   Y,$%02X+X",
	"INC   Y",
	"MOV   Y,A",
	"DBNZ  Y,$%04X",
	"STOP"
};

#undef ABS

enum { DP, ABS, IM, DP2DP, DPIM, DPREL, ABSBIT, REL };

static uint8	modes[256] =
{
	IM , IM, DP, DPREL, DP, ABS, IM , DP, DP  , DP2DP, ABSBIT, DP, ABS, IM, ABS  , IM,
	REL, IM, DP, DPREL, DP, ABS, ABS, DP, DPIM, IM   , DP    , DP, IM , IM, ABS  , ABS,
	IM , IM, DP, DPREL, DP, ABS, IM , DP, DP  , DP2DP, ABSBIT, DP, ABS, IM, DPREL, REL,
	REL, IM, DP, DPREL, DP, ABS, ABS, DP, DPIM, IM   , DP    , DP, IM , IM, DP   , ABS,
	IM , IM, DP, DPREL, DP, ABS, IM , DP, DP  , DP2DP, ABSBIT, DP, ABS, IM, ABS  , DP,
	REL, IM, DP, DPREL, DP, ABS, ABS, DP, DPIM, IM   , DP    , DP, IM , IM, ABS  , ABS,
	IM , IM, DP, DPREL, DP, ABS, IM , DP, DP  , DP2DP, ABSBIT, DP, ABS, IM, DPREL, IM,
	REL, IM, DP, DPREL, DP, ABS, ABS, DP, DPIM, IM   , DP    , DP, IM , IM, DP   , IM,
	IM , IM, DP, DPREL, DP, ABS, IM , DP, DP  , DP2DP, ABSBIT, DP, ABS, DP, IM   , DPIM,
	REL, IM, DP, DPREL, DP, ABS, ABS, DP, DPIM, IM   , DP    , DP, IM , IM, IM   , IM,
	IM , IM, DP, DPREL, DP, ABS, IM , DP, DP  , DP2DP, ABSBIT, DP, ABS, DP, IM   , IM,
	REL, IM, DP, DPREL, DP, ABS, ABS, DP, DPIM, IM   , DP    , DP, IM , IM, IM   , IM,
	IM , IM, DP, DPREL, DP, ABS, IM , DP, DP  , ABS  , ABSBIT, DP, ABS, DP, IM   , IM,
	REL, IM, DP, DPREL, DP, ABS, ABS, DP, DP  , DP   , DP    , DP, IM , IM, DPREL, IM,
	IM , IM, DP, DPREL, DP, ABS, IM , DP, DP  , ABS  , ABSBIT, DP, ABS, IM, IM   , IM,
	REL, IM, DP, DPREL, DP, ABS, ABS, DP, DP  , DP   , DP2DP , DP, IM , IM, REL  , IM
};

static uint8	modes_to_bytes[8] = { 2, 3, 1, 3, 3, 3, 3, 2 };

static const char	*dsp_registers[0x80] =
{
	"V0VOLL", "V0VOLR", "V0PITCHL", "V0PITCHH", "V0SRCN", "V0ADSR1", "V0ADSR2", "V0GAIN", "V0ENVX", "V0OUTX", "", "", "MVOLL", "EFB" , "", "FIR0",
	"V1VOLL", "V1VOLR", "V1PITCHL", "V1PITCHH", "V1SRCN", "V1ADSR1", "V1ADSR2", "V1GAIN", "V1ENVX", "V1OUTX", "", "", "MVOLR", ""    , "", "FIR1",
	"V2VOLL", "V2VOLR", "V2PITCHL", "V2PITCHH", "V2SRCN", "V2ADSR1", "V2ADSR2", "V2GAIN", "V2ENVX", "V2OUTX", "", "", "EVOLL", "PMON", "", "FIR2",
	"V3VOLL", "V3VOLR", "V3PITCHL", "V3PITCHH", "V3SRCN", "V3ADSR1", "V3ADSR2", "V3GAIN", "V3ENVX", "V3OUTX", "", "", "EVOLR", "NON" , "", "FIR3",
	"V4VOLL", "V4VOLR", "V4PITCHL", "V4PITCHH", "V4SRCN", "V4ADSR1", "V4ADSR2", "V4GAIN", "V4ENVX", "V4OUTX", "", "", "KON"  , "EON" , "", "FIR4",
	"V5VOLL", "V5VOLR", "V5PITCHL", "V5PITCHH", "V5SRCN", "V5ADSR1", "V5ADSR2", "V5GAIN", "V5ENVX", "V5OUTX", "", "", "KOFF" , "DIR" , "", "FIR5",
	"V6VOLL", "V6VOLR", "V6PITCHL", "V6PITCHH", "V6SRCN", "V6ADSR1", "V6ADSR2", "V6GAIN", "V6ENVX", "V6OUTX", "", "", "FLG"  , "ESA" , "", "FIR6",
	"V7VOLL", "V7VOLR", "V7PITCHL", "V7PITCHH", "V7SRCN", "V7ADSR1", "V7ADSR2", "V7GAIN", "V7ENVX", "V7OUTX", "", "", "ENDX" , "EDL" , "", "FIR7"
};

static const char	*env_string[11] =
{
	"silent", "attack", "decay", "sustain", "release", "direct", "inc_linear", "inc_bent", "dec_linear", "dec_exp", "(adjust)"
};

static const char * as_binary (uint8);


void S9xTraceSoundDSP (uint8 addr, uint8 byte)
{
	if (apu_trace)
	{
		fprintf(apu_trace, "--- S-DSP write: %-9s %02X HC:%03ld VC:%03ld APU:%03ld FC:%02ld\n",
			dsp_registers[addr], byte, (long) CPU.Cycles, (long) CPU.V_Counter, (long) (APU.Cycles >> SNES_APU_ACCURACY), (long) IPPU.FrameCount);
	}
}

void S9xTraceAPU (void)
{
	char	buffer[200];

	if (apu_trace == NULL)
		apu_trace = fopen("apu_trace.log", "w");

	S9xAPUOPrint(buffer, IAPU.PC - IAPU.RAM);
	fprintf(apu_trace, "%s\n", buffer);
}

void S9xAPUOPrint (char *buffer, uint16 Address)
{
	char	mnem[100];
	uint8	*p = IAPU.RAM + Address;
	int		mode = modes[*p];
	int		bytes = modes_to_bytes[mode];

	switch (bytes)
	{
		case 1:
			sprintf(buffer, "%04X %02X       ", p - IAPU.RAM, *p);
			break;
		case 2:
			sprintf(buffer, "%04X %02X %02X    ", p - IAPU.RAM, *p, *(p + 1));
			break;
		case 3:
			sprintf(buffer, "%04X %02X %02X %02X ", p - IAPU.RAM, *p, *(p + 1), *(p + 2));
			break;
	}

	switch (mode)
	{
		case DP:
			sprintf(mnem, apu_mnemonics[*p], *(p + 1));
			break;
		case ABS:
			sprintf(mnem, apu_mnemonics[*p], *(p + 1) + (*(p + 2) << 8));
			break;
		case IM:
			sprintf(mnem, apu_mnemonics[*p]);
			break;
		case DP2DP:
			sprintf(mnem, apu_mnemonics[*p], *(p + 2), *(p + 1));
			break;
		case DPIM:
			sprintf(mnem, apu_mnemonics[*p], *(p + 2), *(p + 1));
			break;
		case DPREL:
			sprintf(mnem, apu_mnemonics[*p], *(p + 1), (int) (p + 3 - IAPU.RAM) + (signed char) *(p + 2));
			break;
		case ABSBIT:
			sprintf(mnem, apu_mnemonics[*p], (*(p + 1) + (*(p + 2) << 8)) & 0x1FFF, *(p + 2) >> 5);
			break;
		case REL:
			sprintf(mnem, apu_mnemonics[*p], (int) (p + 2 - IAPU.RAM) + (signed char) *(p + 1));
			break;
	}

	sprintf(buffer, "%s %-23s A:%02X X:%02X Y:%02X S:%02X P:%c%c%c%c%c%c%c%c HC:%04d APU:%04d VC:%03d FC:%02d",
		buffer, mnem,
		APURegisters.YA.B.A, APURegisters.X, APURegisters.YA.B.Y, APURegisters.S,
		APUCheckNegative()   ? 'N' : 'n',
		APUCheckOverflow()   ? 'V' : 'v',
		APUCheckDirectPage() ? 'P' : 'p',
		APUCheckBreak()      ? 'B' : 'b',
		APUCheckHalfCarry()  ? 'H' : 'h',
		APUCheckInterrupt()  ? 'I' : 'i',
		APUCheckZero()       ? 'Z' : 'z',
		APUCheckCarry()      ? 'C' : 'c',
		CPU.Cycles, APU.Cycles >> SNES_APU_ACCURACY, CPU.V_Counter, IPPU.FrameCount);
}

static const char * as_binary (uint8 data)
{
	static char	buf[9];

	for (int c = 0; c < 8; c++)
		buf[c] = (data & (1 << c)) ? '1' : '0';
	buf[8] = 0;

	return (buf);
}

void S9xPrintSoundDSPState (void)
{
	printf("Master volume:\n");
	printf("  volume left   : %d\n", (int8) APU.DSP[APU_MVOLL]);
	printf("  volume right  : %d\n", (int8) APU.DSP[APU_MVOLR]);
	printf("\n");
	printf("Echo:\n");
	printf("  channel       : %s\n", as_binary(APU.DSP[APU_EON]));
	printf("  volume left   : %d\n", (int8) APU.DSP[APU_EVOLL]);
	printf("  volume right  : %d\n", (int8) APU.DSP[APU_EVOLR]);
	printf("  delay         : %d\n", APU.DSP[APU_EDL] & 0x0F);
	printf("  feedback      : %d\n", (int8) APU.DSP[APU_EFB]);
	printf("  write disabled: %s\n", (APU.DSP[APU_FLG] & APU_ECHO_DISABLED) ? "yes" : "no");
	printf("\n");
	printf("Filter:\n");
	printf("  tap 0         : %d\n", (int8) APU.DSP[APU_FIR0]);
	printf("  tap 1         : %d\n", (int8) APU.DSP[APU_FIR1]);
	printf("  tap 2         : %d\n", (int8) APU.DSP[APU_FIR2]);
	printf("  tap 3         : %d\n", (int8) APU.DSP[APU_FIR3]);
	printf("  tap 4         : %d\n", (int8) APU.DSP[APU_FIR4]);
	printf("  tap 5         : %d\n", (int8) APU.DSP[APU_FIR5]);
	printf("  tap 6         : %d\n", (int8) APU.DSP[APU_FIR6]);
	printf("  tap 7         : %d\n", (int8) APU.DSP[APU_FIR7]);
	printf("\n");
	printf("Noise:\n");
	printf("  channel       : %s\n", as_binary(APU.DSP[APU_NON]));
	printf("  frequency     : %d\n", APU.DSP[APU_FLG] & 0x1F);
	printf("\n");
	printf("Pitch modulation:\n");
	printf("  channel       : %s\n", as_binary(APU.DSP[APU_PMON]));
	printf("\n");
	printf("Other FLG status:\n");
	printf("  reset         : %s\n", (APU.DSP[APU_FLG] & APU_SOFT_RESET) ? "yes" : "no");
	printf("  mute          : %s\n", (APU.DSP[APU_FLG] & APU_MUTE) ? "yes" : "no");

	for (int c = 0; c < 8; c++)
	{
		Channel	*ch = &SoundData.channels[c];

		printf("\n");
		printf("Channel %d:\n", c);
		printf("  type          : %s\n", (APU.DSP[APU_NON] & (1 << c)) ? "noise" : "sample");
		printf("  sample number : %d\n", APU.DSP[APU_SRCN + (c << 4)]);
		printf("  pitch         : 0x%04X\n", ((APU.DSP[APU_PITCHH + (c << 4)] << 8) | APU.DSP[APU_PITCHL + (c << 4)]) & 0x3FFF);
		printf("  modulation    : %s\n", (APU.DSP[APU_PMON] & (1 << c)) ? "on" : "off");
		printf("  echo          : %s\n", (APU.DSP[APU_EON] & (1 << c)) ? "on" : "off");
		printf("  volume left   : %d\n", (int8) APU.DSP[APU_VOLL + (c << 4)]);
		printf("  volume right  : %d\n", (int8) APU.DSP[APU_VOLR + (c << 4)]);
		printf("  state         : %s\n", env_string[ch->state]);
		printf("  envelope      :\n");
		printf("    envx        : 0x%03X\n", ch->xenvx);
		printf("    target      : 0x%03X\n", ch->xenvx_target);
		printf("    rate        : 0x%04X\n", ch->xenv_rate);
	}
}

#endif
