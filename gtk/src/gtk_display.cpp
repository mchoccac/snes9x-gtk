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

#include <gdk/gdk.h>
#include <sched.h>

#include "gtk_s9x.h"
#include "gtk_display.h"
#include "gtk_display_driver.h"
#include "gtk_display_driver_gtk.h"
#ifdef USE_XV
#include "gtk_display_driver_xv.h"
#endif
#include "snes_ntsc.h"
#ifdef USE_OPENGL
#include "gtk_display_driver_opengl.h"
#endif

static S9xDisplayDriver  *driver;
static snes_ntsc_t       snes_ntsc;
static snes_ntsc_setup_t snes_ntsc_setup;
static thread_job_t      job[8];
static GThreadPool       *pool;
static uint8             *y_table, *u_table, *v_table;
static int               endianess = ENDIAN_LSB;

/* Scanline constants for the NTSC filter */
static unsigned int scanline_offsets[] =
{
        0,     /* 0%    */
        3,     /* 12.5% */
        2,     /* 25%   */
        1,     /* 50%   */
        0,     /* 100%  */
};

static unsigned short scanline_masks[] =
{
        0x0000,     /* 0%    */
        0x0C63,     /* 12.5% */
        0x1CE7,     /* 25%   */
        0x3DEF,     /* 50%   */
        0xffff,     /* 100%  */
};

extern unsigned int scanline_offset;
extern unsigned short scanline_mask;

/* Scanline constants for standard scanline filter */
static uint8 scanline_denominators[] =
{
        8,     /* 12.5% */
        4,     /* 25%   */
        2,     /* 50%   */
        1      /* 100%  */
};

static uint8 scanline_shifts[] =
{
        3,
        2,
        1,
        0
};

void
S9xSetEndianess (int type)
{
    endianess = type;

    return;
}

void
S9xRegisterYUVTables (uint8 *y, uint8 *u, uint8 *v)
{
    y_table = y;
    u_table = u;
    v_table = v;

    return;
}

/* YUY2 in LSB first format */
static void
internal_convert_16_yuv (void *src_buffer,
                         void *dst_buffer,
                         int  src_pitch,
                         int  dst_pitch,
                         int  width,
                         int  height)
{
    register int    x, y;
    register uint16 *src;
    register uint8  *dst;
    register uint16 p0, p1;

    if (endianess == ENDIAN_MSB)
    {
        for (y = 0; y < height; y++)
        {
            src = (uint16 *) (((uint8 *) src_buffer) + src_pitch * y);
            dst = (uint8  *) (((uint8 *) dst_buffer) + dst_pitch * y);

            for (x = 0; x < width >> 1; x++)
            {
                p0 = *src++;
                p1 = *src++;

                *dst++ = (v_table[p0] + v_table[p1]) >> 1;
                *dst++ = y_table[p1];
                *dst++ = (u_table[p0] + u_table[p1]) >> 1;
                *dst++ = y_table[p0];
            }
        }
    }

    else
    {
        for (y = 0; y < height; y++)
        {
            src = (uint16 *) (((uint8 *) src_buffer) + src_pitch * y);
            dst = (uint8  *) (((uint8 *) dst_buffer) + dst_pitch * y);

            for (x = 0; x < width >> 1; x++)
            {
                p0 = *src++;
                p1 = *src++;

                *dst++ = y_table[p0];
                *dst++ = (u_table[p0] + u_table[p1]) >> 1;
                *dst++ = y_table[p1];
                *dst++ = (v_table[p0] + v_table[p1]) >> 1;
            }
        }
    }

    return;
}

static void
internal_convert_mask (void         *src_buffer,
                       void         *dst_buffer,
                       int          src_pitch,
                       int          dst_pitch,
                       int          width,
                       int          height,
                       unsigned int inv_rshift,
                       unsigned int inv_gshift,
                       unsigned int inv_bshift,
                       unsigned int bpp)
{
#ifdef __BIG_ENDIAN__
    if (endianess == ENDIAN_MSB)
#else
    if (endianess == ENDIAN_LSB)
#endif
    {
        switch (bpp)
        {
            case 15:
            case 16:

                for (register int y = 0; y < height; y++)
                {
                    register uint16 *data =
                        (uint16 *) ((uint8 *) dst_buffer + y * dst_pitch);

                    register uint16 *snes =
                        (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                    for (register int x = 0; x < width; x++)
                    {
                        uint32 pixel = *snes++;

                        *data++ = ((pixel & 0x7c00) << 1) >> inv_rshift
                                | ((pixel & 0x03e0) << 6) >> inv_gshift
                                | ((pixel & 0x001f) << 11) >> inv_bshift;
                    }
                }

                break;

            case 24:

#ifdef __BIG_ENDIAN__
                if (inv_rshift > 8)
#else
                if (!(inv_rshift > 8))
#endif
                {
                    for (register int y = 0; y < height; y++)
                    {
                        register uint8 *data =
                            (uint8 *) dst_buffer + y * dst_pitch;

                        register uint16 *snes =
                            (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                        for (register int x = 0; x < width; x++)
                        {
                            uint32 pixel = *snes++;
                            *data++ = ((pixel & 0x001f) << 3);
                            *data++ = ((pixel & 0x03e0) >> 2);
                            *data++ = ((pixel & 0x7c00) >> 7);
                        }
                    }
                }

                else
                {
                    for (register int y = 0; y < height; y++)
                    {
                        register uint8 *data =
                            (uint8 *) dst_buffer + y * dst_pitch;

                        register uint16 *snes =
                            (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                        for (register int x = 0; x < width; x++)
                        {
                            uint32 pixel = *snes++;
                            *data++ = ((pixel & 0x7c00) >> 7);
                            *data++ = ((pixel & 0x03e0) >> 2);
                            *data++ = ((pixel & 0x001f) << 3);
                        }
                    }
                }

                break;

            case 32:

                for (register int y = 0; y < height; y++)
                {
                    register uint32 *data =
                        (uint32 *) ((uint8 *) dst_buffer + y * dst_pitch);

                    register uint16 *snes =
                        (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                    for (register int x = 0; x < width; x++)
                    {
                        uint32 pixel = *snes++;

                        *data++ = ((uint32) (pixel & 0x7c00) << 17) >> inv_rshift
                                | ((uint32) (pixel & 0x03e0) << 22) >> inv_gshift
                                | ((uint32) (pixel & 0x001f) << 27) >> inv_bshift;
                    }
                }

                break;
        }
    }

    else /* Byte-order is inverted from native */
    {
        switch (bpp)
        {
            case 15:
            case 16:

                for (register int y = 0; y < height; y++)
                {
                    register uint16 *data =
                        (uint16 *) ((uint8 *) dst_buffer + y * dst_pitch);

                    register uint16 *snes =
                        (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                    for (register int x = 0; x < width; x++)
                    {
                        uint32 pixel = *snes++;
                        register uint16 value;

                        value   = ((pixel & 0x7c00) << 1) >> inv_rshift
                                | ((pixel & 0x03e0) << 6) >> inv_gshift
                                | ((pixel & 0x001f) << 11) >> inv_bshift;

                        *data++ = ((value & 0xff) << 8)
                                | ((value & 0xff00) >> 8);
                    }
                }

                break;

            case 24:

#ifdef __BIG_ENDIAN__
                if (inv_rshift > 8)
#else
                    if (!(inv_rshift > 8))
#endif
                    {
                        for (register int y = 0; y < height; y++)
                        {
                            register uint8 *data =
                                (uint8 *) dst_buffer + y * dst_pitch;

                            register uint16 *snes =
                                (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                            for (register int x = 0; x < width; x++)
                            {
                                uint32 pixel = *snes++;
                                *data++ = ((pixel & 0x7c00) >> 7);
                                *data++ = ((pixel & 0x03e0) >> 2);
                                *data++ = ((pixel & 0x001f) << 3);
                            }
                        }
                    }

                    else
                    {
                        for (register int y = 0; y < height; y++)
                        {
                            register uint8 *data =
                                (uint8 *) dst_buffer + y * dst_pitch;

                            register uint16 *snes =
                                (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                            for (register int x = 0; x < width; x++)
                            {
                                uint32 pixel = *snes++;
                                *data++ = ((pixel & 0x001f) << 3);
                                *data++ = ((pixel & 0x03e0) >> 2);
                                *data++ = ((pixel & 0x7c00) >> 7);
                            }
                        }
                    }

                break;

            case 32:

                for (register int y = 0; y < height; y++)
                {
                    register uint32 *data =
                        (uint32 *) ((uint8 *) dst_buffer + y * dst_pitch);

                    register uint16 *snes =
                        (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                    for (register int x = 0; x < width; x++)
                    {
                        uint32 pixel = *snes++;
                        register uint32 value;

                        value   = ((uint32) (pixel & 0x7c00) << 17) >> inv_rshift
                                | ((uint32) (pixel & 0x03e0) << 22) >> inv_gshift
                                | ((uint32) (pixel & 0x001f) << 27) >> inv_bshift;

                        *data++ = ((value & 0x000000ff) << 24)
                                | ((value & 0x0000ff00) << 8)
                                | ((value & 0x00ff0000) >> 8)
                                | ((value & 0xff000000) >> 24);
                    }
                }

                break;
        }
    }

    return;
}

static void
internal_convert (void *src_buffer,
                  void *dst_buffer,
                  int  src_pitch,
                  int  dst_pitch,
                  int  width,
                  int  height,
                  int  bpp)
{
    if (endianess == ENDIAN_MSB)
    {
        if (bpp == 15)
        {
            /* Format in fourcc is xrrrrrgg gggbbbbb */
            for (register int y = 0; y < height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                for (register int x = 0; x < width; x++)
                {
                    uint32 pixel = *snes++;
                    *data++ = (pixel & 0x7c00) >> 8
                            | (pixel & 0x0300) >> 8; /* Top 2 green, 5 red */
                    *data++ = (pixel & 0x00c0)
                            | (pixel & 0x001f); /* Top 3 of last 4 green 5 blue */
                }
            }
        }

        else if (bpp == 16)
        {
            /* Format in fourcc is rrrrrggg gggbbbbb */
            for (register int y = 0; y < height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                for (register int x = 0; x < width; x++)
                {
                    uint32 pixel = *snes++;

                    *data++ = (pixel & 0x7c00) >> 7
                            | (pixel & 0x0300) >> 7; /* 5 red, first 3 green */
                    *data++ = (pixel & 0x00c0)
                            | (pixel & 0x001f); /* last 3 green, 5 blue */
                }
            }
        }

        else if (bpp == 24)
        {
            /* Format in fourcc is rrrrrrrr gggggggg bbbbbbbb */
            for (register int y = 0; y < height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                for (register int x = 0; x < width; x++)
                {
                    uint32 pixel = *snes++;

                    *data++ = (pixel & 0x7c00) >> 7; /* Red */
                    *data++ = (pixel & 0x03e0) >> 2; /* Green */
                    *data++ = (pixel & 0x001f) << 3; /* Blue */
                }
            }
        }

        else if (bpp == 32)
        {
            /* Format in fourcc is xxxxxxxx rrrrrrrr gggggggg bbbbbbbb */
            for (register int y = 0; y < height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                for (register int x = 0; x < width; x++)
                {
                    uint32 pixel = *snes++;

                    *data++ = 0xff; /* Null */
                    *data++ = (pixel & 0x7c00) >> 7; /* Red */
                    *data++ = (pixel & 0x03e0) >> 2; /* Green */
                    *data++ = (pixel & 0x001f) << 3; /* Blue */
                }
            }
        }
    }

    else /* Least significant byte first :-P */
    {
        if (bpp == 15)
        {
            /* Format in fourcc is xrrrrrgg gggbbbbb */

            for (register int y = 0; y < height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                for (register int x = 0; x < width; x++)
                {
                    uint32 pixel = *snes++;
                    *data++ = (pixel & 0x00c0)
                            | (pixel & 0x001f); /* Top 3 of last 4 green 5 blue */
                    *data++ = (pixel & 0x7c00) >> 8
                            | (pixel & 0x0300) >> 8; /* Top 2 green, 5 red */
                }
            }
        }

        else if (bpp == 16)
        {
            /* Format in fourcc is rrrrrggg gggbbbbb */
            for (register int y = 0; y < height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                for (register int x = 0; x < width; x++)
                {
                    uint32 pixel = *snes++;

                    *data++ = (pixel & 0x00c0)
                            | (pixel & 0x001f); /* last 3 green, 5 blue */
                    *data++ = (pixel & 0x7c00) >> 7
                            | (pixel & 0x0300) >> 7; /* 5 red, first 3 green */
                }
            }
        }

        else if (bpp == 24)
        {
            /* Format in fourcc is rrrrrrrr gggggggg bbbbbbbb */
            for (register int y = 0; y < height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                for (register int x = 0; x < width; x++)
                {
                    uint32 pixel = *snes++;

                    *data++ = (pixel & 0x001f) << 3; /* Blue */
                    *data++ = (pixel & 0x03e0) >> 2; /* Green */
                    *data++ = (pixel & 0x7c00) >> 7; /* Red */
                }
            }
        }

        else if (bpp == 32)
        {
            /* Format in fourcc is xxxxxxxx rrrrrrrr gggggggg bbbbbbbb */
            for (register int y = 0; y < height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + y * src_pitch);

                for (register int x = 0; x < width; x++)
                {
                    uint32 pixel = *snes++;

                    *data++ = (pixel & 0x001f) << 3; /* Blue */
                    *data++ = (pixel & 0x03e0) >> 2; /* Green */
                    *data++ = (pixel & 0x7c00) >> 7; /* Red */
                    *data++ = 0xff; /* Null */
                }
            }
        }
    }

    return;
}

static void
internal_convert_scale (void *src_buffer,
                        void *dst_buffer,
                        int  src_pitch,
                        int  dst_pitch,
                        int  width,
                        int  height,
                        int  dest_width,
                        int  dest_height,
                        int  bpp)
{
    register uint32 x_error = 0, x_fraction;
    uint32          y_error = 0, y_fraction;
    int             yy = 0;

    x_fraction = (width * 0x10000) / dest_width;
    y_fraction = (height * 0x10000) / dest_height;

    if (endianess == ENDIAN_MSB)
    {
        if (bpp == 15)
        {
            /* Format in fourcc is xrrrrrgg gggbbbbb */
            for (register int y = 0; y < dest_height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + yy * src_pitch);

                y_error += y_fraction;

                while (y_error >= 0x10000)
                {
                    yy++;
                    y_error -= 0x10000;
                }
                x_error = 0;

                for (register int x = 0; x < dest_width; x++)
                {
                    uint32 pixel = *snes;
                    *data++ = (pixel & 0x7c00) >> 8 |
                              (pixel & 0x0300) >> 8; /* Top 2 green, 5 red */
                    *data++ = (pixel & 0x00c0) |
                              (pixel & 0x001f); /* Top 3 of last 4 green 5 blue */

                    x_error += x_fraction;

                    while (x_error >= 0x10000)
                    {
                        snes++;
                        x_error -= 0x10000;
                    }
                }
            }
        }

        else if (bpp == 16)
        {
            /* Format in fourcc is rrrrrggg gggbbbbb */
            for (register int y = 0; y < dest_height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + yy * src_pitch);

                y_error += y_fraction;

                while (y_error >= 0x10000)
                {
                    yy++;
                    y_error -= 0x10000;
                }
                x_error = 0;

                for (register int x = 0; x < dest_width; x++)
                {
                    uint32 pixel = *snes;

                    *data++ = (pixel & 0x7c00) >> 7 |
                              (pixel & 0x0300) >> 7; /* 5 red, first 3 green */
                    *data++ = (pixel & 0x00c0) |
                              (pixel & 0x001f); /* last 3 green, 5 blue */

                    x_error += x_fraction;

                    while (x_error >= 0x10000)
                    {
                        snes++;
                        x_error -= 0x10000;
                    }
                }
            }
        }

        else if (bpp == 24)
        {
            /* Format in fourcc is rrrrrrrr gggggggg bbbbbbbb */
            for (register int y = 0; y < dest_height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + yy * src_pitch);

                y_error += y_fraction;

                while (y_error >= 0x10000)
                {
                    yy++;
                    y_error -= 0x10000;
                }
                x_error = 0;

                for (register int x = 0; x < dest_width; x++)
                {
                    uint32 pixel = *snes;

                    *data++ = (pixel & 0x7c00) >> 7; /* Red */
                    *data++ = (pixel & 0x03e0) >> 2; /* Green */
                    *data++ = (pixel & 0x001f) << 3; /* Blue */

                    x_error += x_fraction;

                    while (x_error >= 0x10000)
                    {
                        snes++;
                        x_error -= 0x10000;
                    }
                }
            }
        }

        else if (bpp == 32)
        {
            /* Format in fourcc is xxxxxxxx rrrrrrrr gggggggg bbbbbbbb */
            for (register int y = 0; y < dest_height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + yy * src_pitch);

                y_error += y_fraction;

                while (y_error >= 0x10000)
                {
                    yy++;
                    y_error -= 0x10000;
                }
                x_error = 0;

                for (register int x = 0; x < dest_width; x++)
                {
                    uint32 pixel = *snes;

                    *data++ = 0xff; /* Null */
                    *data++ = (pixel & 0x7c00) >> 7; /* Red */
                    *data++ = (pixel & 0x03e0) >> 2; /* Green */
                    *data++ = (pixel & 0x001f) << 3; /* Blue */

                    x_error += x_fraction;

                    while (x_error >= 0x10000)
                    {
                        snes++;
                        x_error -= 0x10000;
                    }
                }
            }
        }
    }

    else /* Least significant byte first :-P */
    {
        if (bpp == 15)
        {
            /* Format in fourcc is xrrrrrgg gggbbbbb */

            for (register int y = 0; y < dest_height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + yy * src_pitch);

                y_error += y_fraction;

                while (y_error >= 0x10000)
                {
                    yy++;
                    y_error -= 0x10000;
                }
                x_error = 0;

                for (register int x = 0; x < dest_width; x++)
                {
                    uint32 pixel = *snes;
                    *data++ = (pixel & 0x00c0) |
                              (pixel & 0x001f); /* Top 3 of last 4 green 5 blue */
                    *data++ = (pixel & 0x7c00) >> 8 |
                              (pixel & 0x0300) >> 8; /* Top 2 green, 5 red */

                    x_error += x_fraction;

                    while (x_error >= 0x10000)
                    {
                        snes++;
                        x_error -= 0x10000;
                    }
                }
            }
        }

        else if (bpp == 16)
        {
            /* Format in fourcc is rrrrrggg gggbbbbb */
            for (register int y = 0; y < dest_height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + yy * src_pitch);

                y_error += y_fraction;

                while (y_error >= 0x10000)
                {
                    yy++;
                    y_error -= 0x10000;
                }
                x_error = 0;

                for (register int x = 0; x < dest_width; x++)
                {
                    uint32 pixel = *snes;

                    *data++ = (pixel & 0x00c0) |
                              (pixel & 0x001f); /* last 3 green, 5 blue */
                    *data++ = (pixel & 0x7c00) >> 7 |
                              (pixel & 0x0300) >> 7; /* 5 red, first 3 green */

                    x_error += x_fraction;

                    while (x_error >= 0x10000)
                    {
                        snes++;
                        x_error -= 0x10000;
                    }
                }
            }
        }

        else if (bpp == 24)
        {
            /* Format in fourcc is rrrrrrrr gggggggg bbbbbbbb */
            for (register int y = 0; y < dest_height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + yy * src_pitch);

                y_error += y_fraction;

                while (y_error >= 0x10000)
                {
                    yy++;
                    y_error -= 0x10000;
                }
                x_error = 0;

                for (register int x = 0; x < dest_width; x++)
                {
                    uint32 pixel = *snes;

                    *data++ = (pixel & 0x001f) << 3; /* Blue */
                    *data++ = (pixel & 0x03e0) >> 2; /* Green */
                    *data++ = (pixel & 0x7c00) >> 7; /* Red */

                    x_error += x_fraction;

                    while (x_error >= 0x10000)
                    {
                        snes++;
                        x_error -= 0x10000;
                    }
                }
            }
        }

        else if (bpp == 32)
        {
            /* Format in fourcc is xxxxxxxx rrrrrrrr gggggggg bbbbbbbb */
            for (register int y = 0; y < dest_height; y++)
            {
                register uint8 *data =
                    (uint8 *) dst_buffer + y * dst_pitch;

                register uint16 *snes =
                    (uint16 *) (((uint8 *) src_buffer) + yy * src_pitch);

                y_error += y_fraction;

                while (y_error >= 0x10000)
                {
                    yy++;
                    y_error -= 0x10000;
                }

                for (register int x = 0; x < dest_width; x++)
                {
                    uint32 pixel = *snes;

                    *data++ = (pixel & 0x001f) << 3; /* Blue */
                    *data++ = (pixel & 0x03e0) >> 2; /* Green */
                    *data++ = (pixel & 0x7c00) >> 7; /* Red */
                    *data++ = 0xff; /* Null */

                    x_error += x_fraction;

                    while (x_error >= 0x10000)
                    {
                        snes++;
                        x_error -= 0x10000;
                    }
                }
            }
        }
    }

    return;
}

static void
S9xForceHires (void *buffer,
               int  pitch,
               int  &width,
               int  &height)
{
    int double_width  = 0,
        double_height = 0;

    if (width <= 256)
        double_width++;

    /*if (height <= 224)
        double_height++; */

    if (double_width && double_height)
    {
        for (register int y = (height * 2) - 1; y >= 0; y--)
        {
            register uint16 *src_line = (uint16 *) ((uint8 *) buffer + (y >> 1) * pitch);
            register uint16 *dst_line = (uint16 *) ((uint8 *) buffer + y * pitch);

            for (register int x = (width * 2) - 1; x >= 0; x--)
            {
                *(dst_line + x) = *(src_line + (x >> 1));
            }
        }

        width *= 2; height *= 2;
    }
    else if (double_width && !double_height)
    {
        for (register int y = (height) - 1; y >= 0; y--)
        {
            register uint16 *line = (uint16 *) ((uint8 *) buffer + y * pitch);

            for (register int x = (width * 2) - 1; x >= 0; x--)
            {
                *(line + x) = *(line + (x >> 1));
            }
        }

        width *= 2;
    }
    else if (!double_width && double_height)
    {
        for (register int y = (height * 2) - 1; y >= 0; y--)
        {
            register uint16 *src_line = (uint16 *) ((uint8 *) buffer + (y >> 1) * pitch);
            register uint16 *dst_line = (uint16 *) ((uint8 *) buffer + y * pitch);

            memcpy (dst_line, src_line, width * 2);
        }

        height *= 2;
    }

    return;
}

void
filter_scanlines (void *src_buffer,
                  int src_pitch,
                  void *dst_buffer,
                  int dst_pitch,
                  int width,
                  int height)
{
    register int x, y;

    uint8 dnm   = scanline_denominators[gui_config->scanline_filter_intensity];
    uint8 shift = scanline_shifts[gui_config->scanline_filter_intensity];

    for (y = 0; y < height; y++)
    {
        register uint16 *src   = (uint16 *) ((uint8 *) src_buffer + y * src_pitch);
        register uint16 *dst_a = (uint16 *) ((uint8 *) dst_buffer + (y * 2) * dst_pitch);
        register uint16 *dst_b = (uint16 *) ((uint8 *) dst_buffer + ((y * 2) + 1) * dst_pitch);

        for (x = 0; x < width; x++)
        {
            register uint8 rs, gs, bs, /* Source components */
                           rh, gh, bh; /* High (bright) components */

            rs = ((*(src + x) >> 10) & 0x1f);
            gs = ((*(src + x) >> 5)  & 0x1f);
            bs = ((*(src + x))       & 0x1f);

            rh = ((rs * (dnm + 1)) >> shift);
            gh = ((gs * (dnm + 1)) >> shift);
            bh = ((bs * (dnm + 1)) >> shift);

            rh = (rh > 31) ? 31 : rh;
            gh = (gh > 31) ? 31 : gh;
            bh = (bh > 31) ? 31 : bh;

            *(dst_a + x) = (rh << 10) | (gh << 5) | (bh);
            *(dst_b + x) = ((rs + rs - rh) << 10) |
                           ((gs + gs - gh) << 5)  |
                           (bs + bs - bh);
        }
    }

    return;
}


void
get_filter_scale (int &width, int &height)
{
    switch (gui_config->scale_method)
    {
        case FILTER_SUPEREAGLE:
            width *= 2;
            height *= 2;
            break;

        case FILTER_2XSAI:
            width *= 2;
            height *= 2;
            break;

        case FILTER_SUPER2XSAI:
            width *= 2;
            height *= 2;
            break;

        case FILTER_HQ4X:
            if (((width * 4) <= S9xDisplayDriver::scaled_max_width) &&
                ((height * 4) <= S9xDisplayDriver::scaled_max_height))
            {
                width *= 4;
                height *= 4;
                break;
            }

        case FILTER_HQ3X:
            if (width * 3 <= S9xDisplayDriver::scaled_max_width &&
                    height * 3 <= S9xDisplayDriver::scaled_max_height)
            {
                width *= 3;
                height *= 3;
                break;
            }

        case FILTER_HQ2X:
            width *= 2;
            height *= 2;
            break;

        case FILTER_EPX:
            width *= 2;
            height *= 2;
            break;

        case FILTER_NTSC:
            if (width > 256)
                width = SNES_NTSC_OUT_WIDTH (width / 2);
            else
                width = SNES_NTSC_OUT_WIDTH (width);
            height *= 2;
            break;

        case FILTER_SCANLINES:
            height *= 2;
            break;
    }

    return;
}

static void
internal_filter (uint8 *src_buffer,
                 int   src_pitch,
                 uint8 *dst_buffer,
                 int   dst_pitch,
                 int   &width,
                 int   &height)
{
    switch (gui_config->scale_method)
    {
        case FILTER_SUPEREAGLE:
            Mac_SuperEagle (src_buffer,
                            src_pitch,
                            dst_buffer,
                            dst_pitch,
                            width,
                            height);

            break;

        case FILTER_2XSAI:
            Mac_2xSaI (src_buffer,
                       src_pitch,
                       dst_buffer,
                       dst_pitch,
                       width,
                       height);

            break;

        case FILTER_SUPER2XSAI:
            Mac_Super2xSaI (src_buffer,
                            src_pitch,
                            dst_buffer,
                            dst_pitch,
                            width,
                            height);

            break;


        case FILTER_HQ4X:

            if (((width * 4) <= S9xDisplayDriver::scaled_max_width) &&
                    ((height * 4) <= S9xDisplayDriver::scaled_max_height))
            {
                HQ4X_16 (src_buffer,
                         src_pitch,
                         dst_buffer,
                         dst_pitch,
                         width,
                         height);

                break;
            }
            /* Fall back to 3x */

        case FILTER_HQ3X:

            if (width * 3 <= S9xDisplayDriver::scaled_max_width &&
                    height * 3 <= S9xDisplayDriver::scaled_max_height)
            {
                HQ3X_16 (src_buffer,
                         src_pitch,
                         dst_buffer,
                         dst_pitch,
                         width,
                         height);

                break;
            }
            /* Fall back to 2x */

        case FILTER_HQ2X:

            HQ2X_16 (src_buffer,
                     src_pitch,
                     dst_buffer,
                     dst_pitch,
                     width,
                     height);

            break;

        case FILTER_EPX:

            EPX_16_unsafe (src_buffer,
                           src_pitch,
                           dst_buffer,
                           dst_pitch,
                           width,
                           height);

            break;

        case FILTER_NTSC:
            if (width > 256)
                snes_ntsc_blit_hires (&snes_ntsc,
                                      (SNES_NTSC_IN_T *) src_buffer,
                                      src_pitch >> 1,
                                      0, /* Burst_phase */
                                      width,
                                      height,
                                      (void *) dst_buffer,
                                      dst_pitch);
            else
                snes_ntsc_blit (&snes_ntsc,
                                (SNES_NTSC_IN_T *) src_buffer,
                                src_pitch >> 1,
                                0, /* Burst_phase */
                                width,
                                height,
                                (void *) dst_buffer,
                                dst_pitch);
            break;

        case FILTER_SCANLINES:
            filter_scanlines (src_buffer,
                              src_pitch,
                              dst_buffer,
                              dst_pitch,
                              width,
                              height);
            break;

    }

    get_filter_scale (width, height);

    return;
}

static void
thread_worker (gpointer data,
               gpointer user_data)
{
    thread_job_t *job = ((thread_job_t *) data);

    switch (job->operation_type)
    {
        case JOB_FILTER:
            internal_filter (job->src_buffer,
                             job->src_pitch,
                             job->dst_buffer,
                             job->dst_pitch,
                             job->width,
                             job->height);
            break;

        case JOB_CONVERT:
            internal_convert (job->src_buffer,
                              job->dst_buffer,
                              job->src_pitch,
                              job->dst_pitch,
                              job->width,
                              job->height,
                              job->bpp);
            break;

        case JOB_CONVERT_YUV:
            internal_convert_16_yuv (job->src_buffer,
                                     job->dst_buffer,
                                     job->src_pitch,
                                     job->dst_pitch,
                                     job->width,
                                     job->height);
            break;

        case JOB_CONVERT_MASK:
            internal_convert_mask (job->src_buffer,
                                   job->dst_buffer,
                                   job->src_pitch,
                                   job->dst_pitch,
                                   job->width,
                                   job->height,
                                   job->inv_rmask,
                                   job->inv_bmask,
                                   job->inv_gmask,
                                   job->bpp);

        case JOB_SCALE_AND_CONVERT:
            internal_convert_scale (job->src_buffer,
                                    job->dst_buffer,
                                    job->src_pitch,
                                    job->dst_pitch,
                                    job->width,
                                    job->height,
                                    job->dst_width,
                                    job->dst_height,
                                    job->bpp);
            break;
    }

    job->complete = 1;

    return;
}

static void
create_thread_pool (void)
{
    if (pool == NULL)
    {
        pool = g_thread_pool_new (thread_worker,
                                  NULL,
                                  gui_config->num_threads - 1,
                                  TRUE,
                                  NULL);
    }

    return;
}

static void
internal_threaded_convert_scale (void *src_buffer,
                                 void *dst_buffer,
                                 int src_pitch,
                                 int dst_pitch,
                                 int width,
                                 int height,
                                 int dst_width,
                                 int dst_height,
                                 int bpp)
{
    int i, flag;

    /* If the threadpool doesn't exist, create it */
    create_thread_pool ();

    for (i = 0; i < gui_config->num_threads - 1; i++)
    {
        job[i].operation_type = JOB_SCALE_AND_CONVERT;
        job[i].src_buffer =
            ((uint8 *) src_buffer) + (src_pitch * i * (height / gui_config->num_threads));
        job[i].src_pitch = src_pitch;
        job[i].dst_buffer =
            ((uint8 *) dst_buffer) + (dst_pitch * i * (dst_height / gui_config->num_threads));
        job[i].dst_pitch = dst_pitch;
        job[i].width = width;
        job[i].height = height / gui_config->num_threads;
        job[i].dst_width = dst_width;
        job[i].dst_height = dst_height / gui_config->num_threads;
        job[i].bpp = bpp;
        job[i].complete = 0;

        g_thread_pool_push (pool, (gpointer) &(job[i]), NULL);
    }

    i = gui_config->num_threads - 1;

    job[i].operation_type = JOB_SCALE_AND_CONVERT;
    job[i].src_buffer =
        ((uint8 *) src_buffer) + (src_pitch * i * (height / gui_config->num_threads));
    job[i].src_pitch = src_pitch;
    job[i].dst_buffer =
        ((uint8 *) dst_buffer) + (dst_pitch * i * (dst_height / gui_config->num_threads));
    job[i].dst_pitch = dst_pitch;
    job[i].width = width;
    job[i].height =
        height - ((gui_config->num_threads - 1) * (height / gui_config->num_threads));
    job[i].dst_width = dst_width;
    job[i].dst_height =
        dst_height - ((gui_config->num_threads - 1) * (dst_height / gui_config->num_threads));
    job[i].bpp = bpp;

    thread_worker ((gpointer) &(job[i]), NULL);

    while (1)
    {
        flag = 1;
        for (i = 0; i < gui_config->num_threads - 1; i++)
            flag = flag && job[i].complete;

        if (flag)
            break;

        sched_yield ();
    }

    return;
}

static void
internal_threaded_convert (void *src_buffer,
                           void *dst_buffer,
                           int  src_pitch,
                           int  dst_pitch,
                           int  width,
                           int  height,
                           int  bpp)
{
    int i, flag;

    create_thread_pool ();

    for (i = 0; i < gui_config->num_threads - 1; i++)
    {
        job[i].operation_type = (bpp == -1 ? JOB_CONVERT_YUV : JOB_CONVERT);
        job[i].src_buffer =
            ((uint8 *) src_buffer) + (src_pitch * i * (height / gui_config->num_threads));
        job[i].src_pitch = src_pitch;
        job[i].dst_buffer =
            ((uint8 *) dst_buffer) + (dst_pitch * i * (height / gui_config->num_threads));
        job[i].dst_pitch = dst_pitch;
        job[i].width = width;
        job[i].height = height / gui_config->num_threads;
        job[i].bpp = bpp;
        job[i].complete = 0;

        g_thread_pool_push (pool, (gpointer) &(job[i]), NULL);
    }

    i = gui_config->num_threads - 1;

    job[i].operation_type = (bpp == -1 ? JOB_CONVERT_YUV : JOB_CONVERT);
    job[i].src_buffer =
        ((uint8 *) src_buffer) + (src_pitch * i * (height / gui_config->num_threads));
    job[i].src_pitch = src_pitch;
    job[i].dst_buffer =
        ((uint8 *) dst_buffer) + (dst_pitch * i * (height / gui_config->num_threads));
    job[i].dst_pitch = dst_pitch;
    job[i].width = width;
    job[i].height = height - ((gui_config->num_threads - 1) * (height / gui_config->num_threads));
    job[i].bpp = bpp;

    thread_worker ((gpointer) &(job[i]), NULL);

    while (1)
    {
        flag = 1;
        for (i = 0; i < gui_config->num_threads - 1; i++)
            flag = flag && job[i].complete;

        if (flag)
            break;

        sched_yield ();
    }

    return;
}

static void
internal_threaded_convert_mask (void *src_buffer,
                                void *dst_buffer,
                                int  src_pitch,
                                int  dst_pitch,
                                int  width,
                                int  height,
                                int  inv_rmask,
                                int  inv_gmask,
                                int  inv_bmask,
                                int  bpp)
{
    int i, flag;

    create_thread_pool ();

    for (i = 0; i < gui_config->num_threads - 1; i++)
    {
        job[i].operation_type = (bpp == -1 ? JOB_CONVERT_YUV : JOB_CONVERT);
        job[i].src_buffer =
            ((uint8 *) src_buffer) + (src_pitch * i * (height / gui_config->num_threads));
        job[i].src_pitch = src_pitch;
        job[i].dst_buffer =
            ((uint8 *) dst_buffer) + (dst_pitch * i * (height / gui_config->num_threads));
        job[i].dst_pitch = dst_pitch;
        job[i].width = width;
        job[i].height = height / gui_config->num_threads;
        job[i].bpp = bpp;
        job[i].inv_rmask = inv_rmask;
        job[i].inv_gmask = inv_gmask;
        job[i].inv_bmask = inv_bmask;
        job[i].complete = 0;

        g_thread_pool_push (pool, (gpointer) &(job[i]), NULL);
    }

    i = gui_config->num_threads - 1;

    job[i].operation_type = (bpp == -1 ? JOB_CONVERT_YUV : JOB_CONVERT);
    job[i].src_buffer =
        ((uint8 *) src_buffer) + (src_pitch * i * (height / gui_config->num_threads));
    job[i].src_pitch = src_pitch;
    job[i].dst_buffer =
        ((uint8 *) dst_buffer) + (dst_pitch * i * (height / gui_config->num_threads));
    job[i].dst_pitch = dst_pitch;
    job[i].width = width;
    job[i].height = height - ((gui_config->num_threads - 1) * (height / gui_config->num_threads));
    job[i].bpp = bpp;
    job[i].inv_rmask = inv_rmask;
    job[i].inv_gmask = inv_gmask;
    job[i].inv_bmask = inv_bmask;

    thread_worker ((gpointer) &(job[i]), NULL);

    while (1)
    {
        flag = 1;
        for (i = 0; i < gui_config->num_threads - 1; i++)
            flag = flag && job[i].complete;

        if (flag)
            break;

        sched_yield ();
    }

    return;
}

static void
internal_threaded_filter (uint8 *src_buffer,
                          int   src_pitch,
                          uint8 *dst_buffer,
                          int   dst_pitch,
                          int   &width,
                          int   &height)
{
    int i, flag;
    int dwidth = width, dheight = height;

    get_filter_scale (dwidth, dheight);

    /* If the threadpool doesn't exist, create it */
    create_thread_pool ();

    for (i = 0; i < gui_config->num_threads - 1; i++)
    {
        job[i].operation_type = JOB_FILTER;
        job[i].src_buffer =
            src_buffer + (src_pitch * i * (height / gui_config->num_threads));
        job[i].src_pitch = src_pitch;
        job[i].dst_buffer =
            dst_buffer + (dst_pitch * i * (dheight / gui_config->num_threads));
        job[i].dst_pitch = dst_pitch;
        job[i].width = width;
        job[i].height = height / gui_config->num_threads;
        job[i].complete = 0;
        job[i].dst_width = width;
        job[i].dst_height = height;

        g_thread_pool_push (pool, (gpointer) &(job[i]), NULL);
    }

    i = gui_config->num_threads - 1;

    job[i].operation_type = JOB_FILTER;
    job[i].src_buffer =
        src_buffer + (src_pitch * i * (height / gui_config->num_threads));
    job[i].src_pitch = src_pitch;
    job[i].dst_buffer =
        dst_buffer + (dst_pitch * i * (dheight / gui_config->num_threads));
    job[i].dst_pitch = dst_pitch;
    job[i].width = width;
    job[i].height = height - ((gui_config->num_threads - 1) * (height / gui_config->num_threads));
    job[i].dst_width = width;
    job[i].dst_height = height;

    thread_worker ((gpointer) &(job[i]), NULL);

    while (1)
    {
        flag = 1;
        for (i = 0; i < gui_config->num_threads - 1; i++)
            flag = flag && job[i].complete;

        if (flag)
            break;

        sched_yield ();
    }

    get_filter_scale (width, height);

    return;
}

void
S9xFilter (uint8 *src_buffer,
           int   src_pitch,
           uint8 *dst_buffer,
           int   dst_pitch,
           int   &width,
           int   &height)
{

    if (gui_config->multithreading)
        internal_threaded_filter (src_buffer,
                                  src_pitch,
                                  dst_buffer,
                                  dst_pitch,
                                  width,
                                  height);
    else
        internal_filter (src_buffer,
                         src_pitch,
                         dst_buffer,
                         dst_pitch,
                         width,
                         height);

    return;
}

void
S9xConvertYUV (void *src_buffer,
               void *dst_buffer,
               int  src_pitch,
               int  dst_pitch,
               int  width,
               int  height)
{
    if (gui_config->multithreading)
        internal_threaded_convert (src_buffer,
                                   dst_buffer,
                                   src_pitch,
                                   dst_pitch,
                                   width,
                                   height,
                                   -1);
    else
        internal_convert_16_yuv (src_buffer,
                                 dst_buffer,
                                 src_pitch,
                                 dst_pitch,
                                 width,
                                 height);
    return;
}

void
S9xConvert (void *src,
            void *dst,
            int  src_pitch,
            int  dst_pitch,
            int  width,
            int  height,
            int  bpp)
{
    if (gui_config->multithreading)
        internal_threaded_convert (src,
                                   dst,
                                   src_pitch,
                                   dst_pitch,
                                   width,
                                   height,
                                   bpp);
    else
        internal_convert (src,
                          dst,
                          src_pitch,
                          dst_pitch,
                          width,
                          height,
                          bpp);
    return;
}

void
S9xConvertMask (void *src,
                void *dst,
                int  src_pitch,
                int  dst_pitch,
                int  width,
                int  height,
                int  rshift,
                int  gshift,
                int  bshift,
                int  bpp)
{
    if (gui_config->multithreading)
        internal_threaded_convert_mask (src,
                                        dst,
                                        src_pitch,
                                        dst_pitch,
                                        width,
                                        height,
                                        rshift,
                                        gshift,
                                        bshift,
                                        bpp);
    else
        internal_convert_mask (src,
                               dst,
                               src_pitch,
                               dst_pitch,
                               width,
                               height,
                               rshift,
                               gshift,
                               bshift,
                               bpp);
    return;
}

void
S9xConvertScale (void *src,
                 void *dst,
                 int  src_pitch,
                 int  dst_pitch,
                 int  width,
                 int  height,
                 int  dest_width,
                 int  dest_height,
                 int  bpp)
{
    if (gui_config->multithreading)
        internal_threaded_convert_scale (src,
                                         dst,
                                         src_pitch,
                                         dst_pitch,
                                         width,
                                         height,
                                         dest_width,
                                         dest_height,
                                         bpp);
    else
        internal_convert_scale (src,
                                dst,
                                src_pitch,
                                dst_pitch,
                                width,
                                height,
                                dest_width,
                                dest_height,
                                bpp);
    return;
}

void
S9xDisplayRefresh (int width, int height)
{
    driver->refresh (width, height);

    return;
}

static void
ntsc_filter_init (void)
{
    switch (gui_config->ntsc_format)
    {
        case NTSC_COMPOSITE:
            memcpy (&snes_ntsc_setup,
                    &snes_ntsc_composite,
                    sizeof (snes_ntsc_setup_t));
            break;

        case NTSC_SVIDEO:
            memcpy (&snes_ntsc_setup,
                    &snes_ntsc_svideo,
                    sizeof (snes_ntsc_setup_t));
            break;

        case NTSC_RGB:
            memcpy (&snes_ntsc_setup,
                    &snes_ntsc_rgb,
                    sizeof (snes_ntsc_setup_t));
            break;
    }

    snes_ntsc_setup.hue += (double) gui_config->ntsc_hue;
    snes_ntsc_setup.saturation += (double) gui_config->ntsc_saturation;
    snes_ntsc_setup.contrast += (double) gui_config->ntsc_contrast;
    snes_ntsc_setup.brightness += (double) gui_config->ntsc_brightness;
    snes_ntsc_setup.sharpness += (double) gui_config->ntsc_sharpness;
    snes_ntsc_setup.artifacts += (double) gui_config->ntsc_warping;

    scanline_offset = scanline_offsets [gui_config->ntsc_scanline_intensity];
    scanline_mask   = scanline_masks [gui_config->ntsc_scanline_intensity];

    snes_ntsc_init (&snes_ntsc, &snes_ntsc_setup);

    return;
}

void
S9xDisplayReconfigure (void)
{
    ntsc_filter_init ();

    driver->reconfigure ();

    if (pool)
    {
        g_thread_pool_set_max_threads (pool, gui_config->num_threads - 1, NULL);
    }

    return;
}

bool8
S9xDeinitUpdate (int width, int height)
{
    if (!Settings.Paused
#ifdef NETPLAY_SUPPORT
            && !NetPlay.Paused
#endif
    )

    {
        if (gui_config->overscan)
            height = (height > SNES_HEIGHT_EXTENDED) ?
                         SNES_HEIGHT_EXTENDED * 2 :
                         SNES_HEIGHT_EXTENDED;
        else
            if (height > SNES_HEIGHT_EXTENDED)
                height = 448;
            else
                height = 224;

        if (gui_config->force_hires)
        {
            S9xForceHires (GFX.Screen,
                           S9xDisplayDriver::image_width *
                           S9xDisplayDriver::image_bpp,
                           width,
                           height);
        }

        GFX.Screen = driver->get_next_buffer ();
    }

    top_level->last_width = width;
    top_level->last_height = height;

    driver->update (width, height);

    return TRUE;
}

static void
S9xInitDriver (void)
{
    switch (gui_config->hw_accel)
    {
#ifdef USE_OPENGL
        case HWA_OPENGL:

            driver = new S9xOpenGLDisplayDriver (top_level,
                                                 gui_config);

            break;
#endif
#ifdef USE_XV
        case HWA_XV:

            driver = new S9xXVDisplayDriver (top_level, gui_config);

            break;
#endif
        default:

            driver = new S9xGTKDisplayDriver (top_level, gui_config);
    }

    if (driver->init ())
    {
        if (gui_config->hw_accel > 0)
        {
            driver->deinit ();
            delete driver;

            gui_config->hw_accel = HWA_NONE;

            S9xInitDriver ();
        }
        else
        {
            fprintf (stderr, "Error: Couldn't initialize any display output.\n");
            exit (1);
        }
    }

    pool = NULL;

    return;
}

void
S9xInitDisplay (int argc, char **argv)
{
    Settings.SupportHiRes = TRUE;
    S9xSetRenderPixelFormat (RGB555);
    InitLUTs ();
    S9xInitDriver ();
    S9xGraphicsInit ();
    S9xDisplayReconfigure ();

    return;
}

void
S9xDisplayClearBuffers (void)
{
    driver->clear_buffers ();

    return;
}

void
S9xDeinitDisplay (void)
{
    driver->deinit ();
    delete driver;

    driver = NULL;
    GFX.Screen = NULL;

    if (pool)
        g_thread_pool_free (pool, FALSE, TRUE);

    return;
}

void
S9xReinitDisplay (void)
{
    uint16 *buffer = NULL;
    int    width, height;

    buffer = (uint16 *) malloc (S9xDisplayDriver::image_size);
    memmove (buffer,
             driver->get_current_buffer (),
             S9xDisplayDriver::image_size);

    width = top_level->last_width;
    height = top_level->last_height;

    S9xDeinitDisplay ();
    S9xInitDriver ();
    S9xDisplayReconfigure ();

    top_level->last_width = width;
    top_level->last_height = height;

    driver->push_buffer (buffer);

    free (buffer);

    return;
}

bool8
S9xContinueUpdate (int width, int height)
{
    return TRUE;
}

bool8
S9xInitUpdate (void)
{
    return TRUE;
}

void
S9xSetPalette (void)
{
    return;
}


