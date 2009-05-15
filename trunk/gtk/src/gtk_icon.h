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

/* GdkPixbuf RGBA C-Source image dump 1-byte-run-length-encoded */

#ifdef __SUNPRO_C
#pragma align 4 (app_icon)
#endif
#ifdef __GNUC__
static const guint8 app_icon[] __attribute__ ((__aligned__ (4))) = 
#else
static const guint8 app_icon[] = 
#endif
{ ""
  /* Pixbuf magic (0x47646b50) */
  "GdkP"
  /* length: header (24) + pixel_data (623) */
  "\0\0\2\207"
  /* pixdata_type (0x2010002) */
  "\2\1\0\2"
  /* rowstride (64) */
  "\0\0\0@"
  /* width (16) */
  "\0\0\0\20"
  /* height (16) */
  "\0\0\0\20"
  /* pixel_data: */
  "\205\0\0\0\0\6\320\315\345\24\216\213\237\234a^s\367ZWo\367sn\217\226"
  "\231\224\302\16\212\0\0\0\0\2\216\213\237\234\215\212\243\377\202\305"
  "\302\332\377\2\212\206\241\377jf\207\210\212\0\0\0\0\2b_r\367\262\255"
  "\316\377\202\247\240\315\377\2\262\255\316\377OKd\355\212\0\0\0\0\2["
  "Xp\367\230\221\276\377\202\241\232\312\377\2\230\221\276\377JG`\360\212"
  "\0\0\0\0\2sn\220\233oj\213\377\202\230\221\276\377\2gc\202\377VRs\224"
  "\205\0\0\0\0\22\320\315\345\24\216\213\237\234a^s\367ZWo\367sn\217\226"
  "\232\224\304\"mh\210\235QLe\361JH`\357XRt\223\217\203\261%l]\210\234"
  "@1`\3677#^\367A&y\226V*\246\16\216\213\237\234\215\212\243\377\202\305"
  "\302\332\377\2\212\206\241\377jf\207\210\204\0\0\0\0\2l]\210\234jV\223"
  "\377\202\240\214\311\377\4cM\217\3777\31t\210b_r\367\262\255\316\377"
  "\202\247\240\315\377\2\262\255\316\377OKd\355\204\0\0\0\0\2@1`\367\201"
  "f\270\377\202e@\257\377\4\201f\270\377*\20W\355[Xp\367\230\221\276\377"
  "\202\241\232\312\377\2\230\221\276\377JG`\360\204\0\0\0\0\2""9&`\367"
  "V1\240\377\202[4\252\377\4V1\240\377$\13S\360sn\220\233oj\213\377\202"
  "\230\221\276\377\2gc\202\377VRs\224\204\0\0\0\0\2F)z\233\77$u\377\202"
  "V1\240\377\22""7\32q\377(\14\\\224\234\226\304\25mh\210\235QLe\361JH"
  "`\357XRt\223\217\203\261%l]\210\234@1`\3677#^\367A&y\226T-\245\":\34"
  "v\235*\21W\361#\14U\357(\14\\\2233\21y\22\205\0\0\0\0\2l]\210\234jV\223"
  "\377\202\240\214\311\377\2cM\217\3777\31t\210\212\0\0\0\0\2@1`\367\201"
  "f\270\377\202e@\257\377\2\201f\270\377*\20W\355\212\0\0\0\0\2""9&`\367"
  "V1\240\377\202[4\252\377\2V1\240\377$\13S\360\212\0\0\0\0\2F)z\233\77"
  "$u\377\202V1\240\377\2""7\32q\377(\14\\\224\212\0\0\0\0\6W/\247\25:\34"
  "v\235*\21W\361#\14U\357(\14\\\2233\21y\22\205\0\0\0\0"};


