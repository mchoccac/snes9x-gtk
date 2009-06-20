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
#include "2xsai.h"
#include "epx.h"
#include "hq2x.h"
#include "blit.h"

static uint32	colorMask = 0x3DEF3DEF, lowPixelMask = 0x0421;
static uint8	XDelta[SNES_WIDTH * SNES_HEIGHT_EXTENDED * 4];


void S9xBlitInit (int pixelformat)
{
	S9xBlitClearDelta();

	if (pixelformat == 555)
	{
		colorMask = 0x3DEF3DEF;
		lowPixelMask = 0x0421;
	}
	else
	if (pixelformat == 565)
	{
		colorMask = 0x7BEF7BEF;
		lowPixelMask = 0x0821;
	}
	else
	{
		colorMask = 0;
		lowPixelMask = 0;
	}

	Init_2xSaI(pixelformat);
	InitLUTs(); // FIXME
}

void S9xBlitClearDelta (void)
{
	uint32	*d = (uint32 *) XDelta;

	for (int y = 0; y < SNES_HEIGHT_EXTENDED; y++)
		for (int x = 0; x < SNES_WIDTH; x++)
			*d++ = 0x80008000;
}

void S9xBlitPixSmall16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	width <<= 1;

	for (; height; height--)
	{
		memcpy(dstPtr, srcPtr, width);
		srcPtr += srcRowBytes;
		dstPtr += dstRowBytes;
	}
}

void S9xBlitPixScaled16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	uint8	*dstPtr2 = dstPtr + dstRowBytes, *deltaPtr = XDelta;
	dstRowBytes <<= 1;

	for (; height; height--)
	{
		uint32	*dP1 = (uint32 *) dstPtr, *dP2 = (uint32 *) dstPtr2, *bP = (uint32 *) srcPtr, *xP = (uint32 *) deltaPtr;
		uint32	currentPixel, lastPixel, currentPixA, currentPixB, colorA, colorB;

		for (int i = 0; i < (width >> 1); i++)
		{
			currentPixel = *bP;
			lastPixel    = *xP;

			if (currentPixel != lastPixel)
			{
			#ifdef MSB_FIRST
				colorA = (currentPixel >> 16) & 0xFFFF;
				colorB = (currentPixel      ) & 0xFFFF;
			#else
				colorA = (currentPixel      ) & 0xFFFF;
				colorB = (currentPixel >> 16) & 0xFFFF;
			#endif

				currentPixA = (colorA << 16) | colorA;
				currentPixB = (colorB << 16) | colorB;

				dP1[0] = currentPixA;
				dP1[1] = currentPixB;
				dP2[0] = currentPixA;
				dP2[1] = currentPixB;

				*xP = *bP;
			}

			bP++;
			xP++;
			dP1 += 2;
			dP2 += 2;
		}

		srcPtr   += srcRowBytes;
		deltaPtr += srcRowBytes;
		dstPtr   += dstRowBytes;
		dstPtr2  += dstRowBytes;
	}
}

void S9xBlitPixHiRes16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	width <<= 1;

	for (; height; height--)
	{
		memcpy(dstPtr, srcPtr, width);
		dstPtr += dstRowBytes;
		memcpy(dstPtr, srcPtr, width);
		srcPtr += srcRowBytes;
		dstPtr += dstRowBytes;
	}
}

void S9xBlitPixScaledTV16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	uint8	*dstPtr2 = dstPtr + dstRowBytes, *deltaPtr = XDelta;
	dstRowBytes <<= 1;

	for (; height; height--)
	{
		uint32	*dP1 = (uint32 *) dstPtr, *dP2 = (uint32 *) dstPtr2, *bP = (uint32 *) srcPtr, *xP = (uint32 *) deltaPtr;
		uint32	currentPixel, nextPixel, currentDelta, nextDelta, colorA, colorB, product, darkened;

		for (int i = 0; i < (width >> 1) - 1; i++)
		{
			currentPixel = *bP;
			currentDelta = *xP;
			nextPixel    = *(bP + 1);
			nextDelta    = *(xP + 1);

			if ((currentPixel != currentDelta) || (nextPixel != nextDelta))
			{
				*xP = *bP;

			#ifdef MSB_FIRST
				colorA = (currentPixel >> 16) & 0xFFFF;
				colorB = (currentPixel      ) & 0xFFFF;
			#else
				colorA = (currentPixel      ) & 0xFFFF;
				colorB = (currentPixel >> 16) & 0xFFFF;
			#endif

			#ifdef MSB_FIRST
				*dP1       = product = (colorA << 16) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask))      );
			#else
				*dP1       = product = (colorA      ) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask)) << 16);
			#endif

				darkened  = (product = ((product >> 1) & colorMask));
				darkened += (product = ((product >> 1) & colorMask));
				darkened +=             (product >> 1) & colorMask;

				*dP2       = darkened;

			#ifdef MSB_FIRST
				colorA = (nextPixel    >> 16) & 0xFFFF;
			#else
				colorA = (nextPixel         ) & 0xFFFF;
			#endif

			#ifdef MSB_FIRST
				*(dP1 + 1) = product = (colorB << 16) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask))      );
			#else
				*(dP1 + 1) = product = (colorB      ) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask)) << 16);
			#endif

				darkened  = (product = ((product >> 1) & colorMask));
				darkened += (product = ((product >> 1) & colorMask));
				darkened +=             (product >> 1) & colorMask;

				*(dP2 + 1) = darkened;
			}

			bP++;
			xP++;
			dP1 += 2;
			dP2 += 2;
		}

		// Last 2 Pixels

		currentPixel = *bP;
		currentDelta = *xP;

		if (currentPixel != currentDelta)
		{
			*xP = *bP;

		#ifdef MSB_FIRST
			colorA = (currentPixel >> 16) & 0xFFFF;
			colorB = (currentPixel      ) & 0xFFFF;
		#else
			colorA = (currentPixel      ) & 0xFFFF;
			colorB = (currentPixel >> 16) & 0xFFFF;
		#endif

		#ifdef MSB_FIRST
			*dP1       = product = (colorA << 16) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask))      );
		#else
			*dP1       = product = (colorA      ) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask)) << 16);
		#endif

			darkened  = (product = ((product >> 1) & colorMask));
			darkened += (product = ((product >> 1) & colorMask));
			darkened +=             (product >> 1) & colorMask;

			*dP2       = darkened;

			*(dP1 + 1) = product = (colorB << 16) | colorB;

			darkened  = (product = ((product >> 1) & colorMask));
			darkened += (product = ((product >> 1) & colorMask));
			darkened +=             (product >> 1) & colorMask;

			*(dP2 + 1) = darkened;
		}

		srcPtr   += srcRowBytes;
		deltaPtr += srcRowBytes;
		dstPtr   += dstRowBytes;
		dstPtr2  += dstRowBytes;
	}
}

void S9xBlitPixHiResTV16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	uint8	*dstPtr2 = dstPtr + dstRowBytes;
	dstRowBytes <<= 1;

	for (; height; height--)
	{
		uint32	*dP1 = (uint32 *) dstPtr, *dP2 = (uint32 *) dstPtr2, *bP = (uint32 *) srcPtr;
		uint32	product, darkened;

		for (int i = 0; i < (width >> 1); i++)
		{
			product = *dP1++ = *bP++;
			darkened  = (product = (product >> 1) & colorMask);
			darkened += (product = (product >> 1) & colorMask);
			*dP2++  = darkened;
		}

		srcPtr  += srcRowBytes;
		dstPtr  += dstRowBytes;
		dstPtr2 += dstRowBytes;
	}
}

void S9xBlitPixSmooth16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	uint8	*dstPtr2 = dstPtr + dstRowBytes, *deltaPtr = XDelta;
	uint32	lastLinePix[SNES_WIDTH << 1];
	uint8	lastLineChg[SNES_WIDTH >> 1];
	int		lineBytes = width << 1;

	dstRowBytes <<= 1;

	memset(lastLinePix, 0, sizeof(lastLinePix));
	memset(lastLineChg, 0, sizeof(lastLineChg));

	for (; height; height--)
	{
		uint32	*dP1 = (uint32 *) dstPtr, *dP2 = (uint32 *) dstPtr2, *bP = (uint32 *) srcPtr, *xP = (uint32 *) deltaPtr;
		uint32	*lL = lastLinePix;
		uint8	*lC = lastLineChg;
		uint32	currentPixel, nextPixel, currentDelta, nextDelta, lastPix, lastChg, thisChg, currentPixA, currentPixB, colorA, colorB, colorC;
		uint16	savePixel;

		savePixel = *(uint16 *) (srcPtr + lineBytes);
		*(uint16 *) (srcPtr   + lineBytes) = *(uint16 *) (srcPtr + lineBytes - 2);
		*(uint32 *) (deltaPtr + lineBytes) = *(uint32 *) (srcPtr + lineBytes);

		nextPixel = *bP++;
		nextDelta = *xP++;

		for (int i = 0; i < (width >> 1); i++)
		{
			currentPixel = nextPixel;
			currentDelta = nextDelta;
			nextPixel    = *bP++;
			nextDelta    = *xP++;
			lastChg      = *lC;
			thisChg      = (nextPixel - nextDelta) | (currentPixel - currentDelta);

		#ifdef MSB_FIRST
			colorA = (currentPixel >> 16) & 0xFFFF;
			colorB = (currentPixel      ) & 0xFFFF;
			colorC = (nextPixel    >> 16) & 0xFFFF;

			currentPixA = (colorA << 16) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask))      );
			currentPixB = (colorB << 16) | ((((colorC >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorC & colorB & lowPixelMask))      );
		#else
			colorA = (currentPixel      ) & 0xFFFF;
			colorB = (currentPixel >> 16) & 0xFFFF;
			colorC = (nextPixel         ) & 0xFFFF;

			currentPixA = (colorA      ) | ((((colorA >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorA & colorB & lowPixelMask)) << 16);
			currentPixB = (colorB      ) | ((((colorC >> 1) & colorMask) + ((colorB >> 1) & colorMask) + (colorC & colorB & lowPixelMask)) << 16);
		#endif

			if (thisChg | lastChg)
			{
				xP[-2]  = currentPixel;

				lastPix = lL[0];
				dP1[0]  = ((currentPixA >> 1) & colorMask) + ((lastPix >> 1) & colorMask) + (currentPixA & lastPix & lowPixelMask);
				dP2[0]  = currentPixA;
				lL[0]   = currentPixA;

				lastPix = lL[1];
				dP1[1]  = ((currentPixB >> 1) & colorMask) + ((lastPix >> 1) & colorMask) + (currentPixB & lastPix & lowPixelMask);
				dP2[1]  = currentPixB;
				lL[1]   = currentPixB;

				*lC++   = (thisChg != 0);
			}
			else
			{
				lL[0]   = currentPixA;
				lL[1]   = currentPixB;
				*lC++   = 0;
			}

			lL  += 2;
			dP2 += 2;
			dP1 += 2;
		}

		*(uint16 *) (srcPtr + lineBytes) = savePixel;

		srcPtr   += srcRowBytes;
		deltaPtr += srcRowBytes;
		dstPtr   += dstRowBytes;
		dstPtr2  += dstRowBytes;
	}
}

void S9xBlitPixSuper2xSaI16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	Super2xSaI(srcPtr, srcRowBytes, dstPtr, dstRowBytes, width, height);
}

void S9xBlitPix2xSaI16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	_2xSaI(srcPtr, srcRowBytes, dstPtr, dstRowBytes, width, height);
}

void S9xBlitPixSuperEagle16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	SuperEagle(srcPtr, srcRowBytes, dstPtr, dstRowBytes, width, height);
}

void S9xBlitPixEPX16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	EPX_16(srcPtr, srcRowBytes, dstPtr, dstRowBytes, width, height);
}

void S9xBlitPixHQ2x16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	HQ2X_16(srcPtr, srcRowBytes, dstPtr, dstRowBytes, width, height);
}

void S9xBlitPixHQ3x16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	HQ3X_16(srcPtr, srcRowBytes, dstPtr, dstRowBytes, width, height);
}

void S9xBlitPixHQ4x16 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	HQ4X_16(srcPtr, srcRowBytes, dstPtr, dstRowBytes, width, height);
}
