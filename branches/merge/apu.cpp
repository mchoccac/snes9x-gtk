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
#include "snapshot.h"
#include "display.h"

static uint8	APUROM[64] =
{
	0xCD, 0xEF, 0xBD, 0xE8, 0x00, 0xC6, 0x1D, 0xD0, 0xFC, 0x8F, 0xAA, 0xF4, 0x8F, 0xBB, 0xF5, 0x78,
	0xCC, 0xF4, 0xD0, 0xFB, 0x2F, 0x19, 0xEB, 0xF4, 0xD0, 0xFC, 0x7E, 0xF4, 0xD0, 0x0B, 0xE4, 0xF5,
	0xCB, 0xF4, 0xD7, 0x00, 0xFC, 0xD0, 0xF3, 0xAB, 0x01, 0x10, 0xEF, 0x7E, 0xF4, 0x10, 0xEB, 0xBA,
	0xF6, 0xDA, 0x00, 0xBA, 0xF4, 0xC4, 0xF4, 0xDD, 0x5D, 0xD0, 0xDB, 0x1F, 0x00, 0x00, 0xC0, 0xFF
};

static uint8	spc_dump = 0;

static inline void handle_key_changes (void);


bool8 S9xInitAPU (void)
{
	IAPU.RAM = (uint8 *) malloc(0x10000);
	if (!IAPU.RAM)
		return (FALSE);

	ZeroMemory(IAPU.RAM, 0x10000);

	return (TRUE);
}

void S9xDeinitAPU (void)
{
	if (IAPU.RAM)
	{
		free(IAPU.RAM);
		IAPU.RAM = NULL;
	}
}

void S9xResetAPU (void)
{
	Settings.APUEnabled = Settings.NextAPUEnabled;
	IAPU.APUExecuting = Settings.APUEnabled;
	if (Settings.APUEnabled)
		APU.Flags &= ~HALTED_FLAG;

	ZeroMemory(IAPU.RAM, 0x100);
	memset(IAPU.RAM + 0x20, 0xFF, 0x20);
	memset(IAPU.RAM + 0x60, 0xFF, 0x20);
	memset(IAPU.RAM + 0xA0, 0xFF, 0x20);
	memset(IAPU.RAM + 0xE0, 0xFF, 0x20);
	for (int i = 1; i < 256; i++)
		memcpy(IAPU.RAM + (i << 8), IAPU.RAM, 0x100);

	IAPU.RAM[0xF1] = 0xB0;

	memmove(APU.ExtraRAM, &IAPU.RAM[0xFFC0], sizeof(APUROM));
	memmove(&IAPU.RAM[0xFFC0], APUROM, sizeof(APUROM));
	APU.ShowROM = TRUE;

	ZeroMemory(APU.OutPorts, 4);

	for (int i = 0; i < 3; i++)
	{
		APU.TimerEnabled[i] = FALSE;
		APU.TimerTarget[i] = 0;
		APU.Timer[i] = 0;
	}

	IAPU.DirectPage = IAPU.RAM;
	IAPU.PC = IAPU.RAM + (IAPU.RAM[0xFFFE] | (IAPU.RAM[0xFFFF] << 8));
	APURegisters.YA.W = 0;
	APURegisters.X = 0;
	APURegisters.S = 0xEF;
	APURegisters.PC = 0;
	APURegisters.P = 0x02;
	S9xAPUUnpackStatus();

#ifdef SPC700_SHUTDOWN
	IAPU.WaitAddress1 = NULL;
	IAPU.WaitAddress2 = NULL;
	IAPU.WaitCounter = 0;
#endif

	APU.Cycles = 0;
	APU.NextAPUTimerPos = 0;
	APU.APUTimerCounter = 0;

	for (int i = 0; i < 0x80; i++)
		APU.DSP[i] = 0;
	APU.DSP[APU_FLG] = 0xE0;
	APU.Internal_KON = 0;

	APU.OUTX_ENVX_pos = 0;
	APU.OUTX_ENVX_samples = 1;
	APU.OUTX_ENVX_counter = 0;
	for (int c = 0; c < 8; c++)
	{
		for (int i = 0; i < MAX_OUTX_ENVX_BUFFER; i++)
		{
			APU.OUTX_buffer[c][i] = 0;
			APU.ENVX_buffer[c][i] = 0;
		}
	}

	IAPU.TwoCycles = IAPU.OneCycle * 2;
	for (int i = 0; i < 256; i++)
		S9xAPUCycles[i] = S9xAPUCycleLengths[i] * IAPU.OneCycle;

	S9xResetSound(TRUE);
}

static inline void handle_key_changes (void)
{
	if (APU.DSP[APU_FLG] & APU_SOFT_RESET)
	{
		for (int c = 0; c < 8; c++)
			S9xSoundSetSilence(c);
	}
	else
	{
		uint8	mask = 1;
		for (int c = 0; c < 8; c++, mask <<= 1)
		{
			if (APU.DSP[APU_KOFF] & mask)
				S9xSoundSetRelease(c);
			else
			if (APU.Internal_KON & mask)
			{
				APU.DSP[APU_ENDX] &= ~mask;
				APU.Internal_KON &= ~mask;
				S9xSoundSetAttack(c);
			}
		}
	}
}

uint8 S9xGetAPUDSP (void)
{
	uint8	addr = IAPU.RAM[0xF2] & 0x7F;
	uint8	byte;

	switch (addr)
	{
		case APU_ENVX + 0x00:
		case APU_ENVX + 0x10:
		case APU_ENVX + 0x20:
		case APU_ENVX + 0x30:
		case APU_ENVX + 0x40:
		case APU_ENVX + 0x50:
		case APU_ENVX + 0x60:
		case APU_ENVX + 0x70:
		{
			byte = Settings.SoundEnvelopeHeightReading ? APU.ENVX_buffer[addr >> 4][APU.OUTX_ENVX_pos] : 0;
			break;
		}

		case APU_OUTX + 0x00:
		case APU_OUTX + 0x10:
		case APU_OUTX + 0x20:
		case APU_OUTX + 0x30:
		case APU_OUTX + 0x40:
		case APU_OUTX + 0x50:
		case APU_OUTX + 0x60:
		case APU_OUTX + 0x70:
		{
			byte = APU.OUTX_buffer[addr >> 4][APU.OUTX_ENVX_pos];
			break;
		}

		default:
		{
			byte = APU.DSP[addr];
			break;
		}
	}

	return (byte);
}

void S9xSetAPUDSP (uint8 byte)
{
	uint8	addr = IAPU.RAM[0xF2];

	switch (addr)
	{
		case APU_VOLL + 0x00:
		case APU_VOLL + 0x10:
		case APU_VOLL + 0x20:
		case APU_VOLL + 0x30:
		case APU_VOLL + 0x40:
		case APU_VOLL + 0x50:
		case APU_VOLL + 0x60:
		case APU_VOLL + 0x70:
		{
			APU.DSP[addr] = byte;
			S9xSoundSetVolume(addr >> 4, (int8) byte, (int8) APU.DSP[addr + 1]);
			break;
		}

		case APU_VOLR + 0x00:
		case APU_VOLR + 0x10:
		case APU_VOLR + 0x20:
		case APU_VOLR + 0x30:
		case APU_VOLR + 0x40:
		case APU_VOLR + 0x50:
		case APU_VOLR + 0x60:
		case APU_VOLR + 0x70:
		{
			APU.DSP[addr] = byte;
			S9xSoundSetVolume(addr >> 4, (int8) APU.DSP[addr - 1], (int8) byte);
			break;
		}

		case APU_PITCHL + 0x00:
		case APU_PITCHL + 0x10:
		case APU_PITCHL + 0x20:
		case APU_PITCHL + 0x30:
		case APU_PITCHL + 0x40:
		case APU_PITCHL + 0x50:
		case APU_PITCHL + 0x60:
		case APU_PITCHL + 0x70:
		{
			APU.DSP[addr] = byte;
			S9xSoundSetPitch(addr >> 4, (byte | (APU.DSP[addr + 1] << 8)) & 0x3FFF);
			break;
		}

		case APU_PITCHH + 0x00:
		case APU_PITCHH + 0x10:
		case APU_PITCHH + 0x20:
		case APU_PITCHH + 0x30:
		case APU_PITCHH + 0x40:
		case APU_PITCHH + 0x50:
		case APU_PITCHH + 0x60:
		case APU_PITCHH + 0x70:
		{
			APU.DSP[addr] = byte;
			S9xSoundSetPitch(addr >> 4, ((byte << 8) | APU.DSP[addr - 1]) & 0x3FFF);
			break;
		}
		
		case APU_SRCN + 0x00:
		case APU_SRCN + 0x10:
		case APU_SRCN + 0x20:
		case APU_SRCN + 0x30:
		case APU_SRCN + 0x40:
		case APU_SRCN + 0x50:
		case APU_SRCN + 0x60:
		case APU_SRCN + 0x70:
		{
			APU.DSP[addr] = byte;
			break;
		}

		case APU_ADSR1 + 0x00:
		case APU_ADSR1 + 0x10:
		case APU_ADSR1 + 0x20:
		case APU_ADSR1 + 0x30:
		case APU_ADSR1 + 0x40:
		case APU_ADSR1 + 0x50:
		case APU_ADSR1 + 0x60:
		case APU_ADSR1 + 0x70:
		{
			APU.DSP[addr] = byte;
			S9xSoundFinishRapidAttack(addr >> 4);
			S9xSoundAdjustEnvelope(addr >> 4, APU.DSP[addr + 2], byte, APU.DSP[addr + 1]);
			break;
		}

		case APU_ADSR2 + 0x00:
		case APU_ADSR2 + 0x10:
		case APU_ADSR2 + 0x20:
		case APU_ADSR2 + 0x30:
		case APU_ADSR2 + 0x40:
		case APU_ADSR2 + 0x50:
		case APU_ADSR2 + 0x60:
		case APU_ADSR2 + 0x70:
		{
			APU.DSP[addr] = byte;
			S9xSoundFinishRapidAttack(addr >> 4);
			S9xSoundSetSustainLevel(addr >> 4, (byte >> 5) & 0x07);
			S9xSoundAdjustEnvelope(addr >> 4, APU.DSP[addr + 1], APU.DSP[addr - 1], byte);
			break;
		}

		case APU_GAIN + 0x00:
		case APU_GAIN + 0x10:
		case APU_GAIN + 0x20:
		case APU_GAIN + 0x30:
		case APU_GAIN + 0x40:
		case APU_GAIN + 0x50:
		case APU_GAIN + 0x60:
		case APU_GAIN + 0x70:
		{
			APU.DSP[addr] = byte;
			S9xSoundFinishRapidAttack(addr >> 4);
			S9xSoundAdjustEnvelope(addr >> 4, byte, APU.DSP[addr - 2], APU.DSP[addr - 1]);
			break;
		}

		case APU_ENVX + 0x00:
		case APU_ENVX + 0x10:
		case APU_ENVX + 0x20:
		case APU_ENVX + 0x30:
		case APU_ENVX + 0x40:
		case APU_ENVX + 0x50:
		case APU_ENVX + 0x60:
		case APU_ENVX + 0x70:
		{
			APU.DSP[addr] = byte;
			break;
		}

		case APU_OUTX + 0x00:
		case APU_OUTX + 0x10:
		case APU_OUTX + 0x20:
		case APU_OUTX + 0x30:
		case APU_OUTX + 0x40:
		case APU_OUTX + 0x50:
		case APU_OUTX + 0x60:
		case APU_OUTX + 0x70:
		{
			APU.DSP[addr] = byte;
			break;
		}

		case APU_MVOLL:
		{
			APU.DSP[APU_MVOLL] = byte;
			S9xSoundSetMasterVolume((int8) byte, (int8) APU.DSP[APU_MVOLR]);
			break;
		}

		case APU_MVOLR:
		{
			APU.DSP[APU_MVOLR] = byte;
			S9xSoundSetMasterVolume((int8) APU.DSP[APU_MVOLL], (int8) byte);
			break;
		}

		case APU_EVOLL:
		{
			APU.DSP[APU_EVOLL] = byte;
			S9xSoundSetEchoVolume((int8) byte, (int8) APU.DSP[APU_EVOLR]);
			break;
		}

		case APU_EVOLR:
		{
			APU.DSP[APU_EVOLR] = byte;
			S9xSoundSetEchoVolume((int8) APU.DSP[APU_EVOLL], (int8) byte);
			break;
		}

		case APU_KON:
		{
			if (Settings.TakeSPCShapshot)
			{
				if (byte & ~spc_dump)
				{
					APURegisters.PC = IAPU.PC - IAPU.RAM;
					S9xAPUPackStatus();
					S9xSPCDump(S9xGetFilenameInc((".spc"), SPC_DIR));
					Settings.TakeSPCShapshot = FALSE;
				}
			}

			spc_dump = byte;

			APU.DSP[APU_KON] = byte;
			APU.Internal_KON = byte;
			handle_key_changes();
			break;
		}

		case APU_KOFF:
		{
			APU.DSP[APU_KOFF] = byte;
			handle_key_changes();
			break;
		}

		case APU_FLG:
		{
			APU.DSP[APU_FLG] = byte;
			S9xSoundSetMute(byte & APU_MUTE);
			S9xSoundSetEchoWriteEnable(byte & APU_ECHO_DISABLED);
			S9xSoundSetNoiseRate(byte & 0x1F);
			handle_key_changes();
			break;
		}

		case APU_ENDX:
		{
			// Any writes clear all bits of ENDX
			APU.DSP[APU_ENDX] = 0;
			break;
		}

		case APU_EFB:
		{
			APU.DSP[APU_EFB] = byte;
			S9xSoundSetEchoFeedback((int8) byte);
			break;
		}

		case APU_PMON:
		{
			APU.DSP[APU_PMON] = byte;
			break;
		}

		case APU_NON:
		{
			APU.DSP[APU_NON] = byte;
			S9xSoundSetType(byte);
			break;
		}

		case APU_EON:
		{
			APU.DSP[APU_EON] = byte;
			S9xSoundSetEchoEnable(byte);
			break;
		}

		case APU_DIR:
		{
			APU.DSP[APU_DIR] = byte;
			break;
		}

		case APU_ESA:
		{
			APU.DSP[APU_ESA] = byte;
			S9xSoundSetEchoRingBufferAddress(byte);
			break;
		}

		case APU_EDL:
		{
			APU.DSP[APU_EDL] = byte;
			S9xSoundSetEchoDelay(byte & 0x0F);
			break;
		}

		case APU_FIR0:
		case APU_FIR1:
		case APU_FIR2:
		case APU_FIR3:
		case APU_FIR4:
		case APU_FIR5:
		case APU_FIR6:
		case APU_FIR7:
		{
			APU.DSP[addr] = byte;
			S9xSoundSetFilterCoefficient(addr >> 4, (int8) byte);
			break;
		}

		default:
		{
			if (addr < 0x80)
				APU.DSP[addr] = byte;
			break;
		}
	}

#ifdef DEBUGGER
	if (addr < 0x80 && Settings.TraceSoundDSP)
		S9xTraceSoundDSP(addr, byte);
#endif
}

void S9xSetAPUControl (uint8 byte)
{
	if ((byte & 1) && !APU.TimerEnabled[0])
	{
		APU.Timer[0] = 0;
		IAPU.RAM[0xFD] = 0;
	}

	if ((byte & 2) && !APU.TimerEnabled[1])
	{
		APU.Timer[1] = 0;
		IAPU.RAM[0xFE] = 0;
	}

	if ((byte & 4) && !APU.TimerEnabled[2])
	{
		APU.Timer[2] = 0;
		IAPU.RAM[0xFF] = 0;
	}

	APU.TimerEnabled[0] = (byte & 1);
	APU.TimerEnabled[1] = (byte & 2) >> 1;
	APU.TimerEnabled[2] = (byte & 4) >> 2;

	if (byte & 0x10)
		IAPU.RAM[0xF4] = IAPU.RAM[0xF5] = 0;

	if (byte & 0x20)
		IAPU.RAM[0xF6] = IAPU.RAM[0xF7] = 0;

	if (byte & 0x80)
	{
		if (!APU.ShowROM)
		{
			memmove(&IAPU.RAM[0xFFC0], APUROM, sizeof(APUROM));
			APU.ShowROM = TRUE;
		}
	}
	else
	{
		if (APU.ShowROM)
		{
			APU.ShowROM = FALSE;
			memmove(&IAPU.RAM[0xFFC0], APU.ExtraRAM, sizeof(APUROM));
		}
	}

	IAPU.RAM[0xF1] = byte;
}

void S9xSetAPUTimer (uint16 Address, uint8 byte)
{
	IAPU.RAM[Address] = byte;

	switch (Address)
	{
		case 0xFA:
			if ((APU.TimerTarget[0] = IAPU.RAM[0xFA]) == 0)
				APU.TimerTarget[0] = 0x100;
			break;

		case 0xFB:
			if ((APU.TimerTarget[1] = IAPU.RAM[0xFB]) == 0)
				APU.TimerTarget[1] = 0x100;
			break;

		case 0xFC:
			if ((APU.TimerTarget[2] = IAPU.RAM[0xFC]) == 0)
				APU.TimerTarget[2] = 0x100;
			break;
	}
}

void S9xAPUExecute (void)
{
	while ((CPU.Cycles << SNES_APU_ACCURACY) >= APU.NextAPUTimerPos)
	{
		// catch up the APU timers
		if (IAPU.APUExecuting)
		{
			while (APU.Cycles < APU.NextAPUTimerPos)
				APU_EXECUTE1();
		}
		else
			APU.Cycles = APU.NextAPUTimerPos;

		APU.NextAPUTimerPos += SNES_APUTIMER2_CYCLE_SCALED;

		APU.OUTX_ENVX_counter += (1 << SNES_APU_ACCURACY);
		if (APU.OUTX_ENVX_counter >= APU.OUTX_ENVX_counter_max)
		{
			APU.OUTX_ENVX_counter -= APU.OUTX_ENVX_counter_max;
			if (++APU.OUTX_ENVX_pos == APU.OUTX_ENVX_samples)
				APU.OUTX_ENVX_pos--;
		}

		if (APU.TimerEnabled[2])
		{
			APU.Timer[2]++;
			if (APU.Timer[2] >= APU.TimerTarget[2])
			{
				IAPU.RAM[0xFF] = (IAPU.RAM[0xFF] + 1) & 0xF;
				APU.Timer[2] = 0;
			#ifdef SPC700_SHUTDOWN
				IAPU.WaitCounter++;
				IAPU.APUExecuting = TRUE;
			#endif
			}
		}

		if (++APU.APUTimerCounter == 8)
		{
			APU.APUTimerCounter = 0;

			if (APU.TimerEnabled[0])
			{
				APU.Timer[0]++;
				if (APU.Timer[0] >= APU.TimerTarget[0])
				{
					IAPU.RAM[0xFD] = (IAPU.RAM[0xFD] + 1) & 0xF;
					APU.Timer[0] = 0;
				#ifdef SPC700_SHUTDOWN
					IAPU.WaitCounter++;
					IAPU.APUExecuting = TRUE;
				#endif
				}
			}

			if (APU.TimerEnabled[1])
			{
				APU.Timer[1]++;
				if (APU.Timer[1] >= APU.TimerTarget[1])
				{
					IAPU.RAM[0xFE] = (IAPU.RAM[0xFE] + 1) & 0xF;
					APU.Timer[1] = 0;
				#ifdef SPC700_SHUTDOWN
					IAPU.WaitCounter++;
					IAPU.APUExecuting = TRUE;
				#endif
				}
			}
		}
	}

	// catch up the current cycles
	if (IAPU.APUExecuting)
	{
		while (APU.Cycles < (CPU.Cycles << SNES_APU_ACCURACY))
			APU_EXECUTE1();
	}
	else
		APU.Cycles = (CPU.Cycles << SNES_APU_ACCURACY);
}
