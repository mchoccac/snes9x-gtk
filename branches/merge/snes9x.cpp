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


#include <ctype.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "snes9x.h"
#include "memmap.h"
#include "controls.h"
#include "crosshairs.h"
#include "cheats.h"
#include "display.h"
#include "conffile.h"
#ifdef NETPLAY_SUPPORT
#include "netplay.h"
#endif

#ifdef DEBUGGER
extern FILE	*trace;
#endif

static char	*rom_filename = NULL;

static bool parse_controller_spec (int, const char *);
static void parse_crosshair_spec (enum crosscontrols, const char *);
static bool try_load_config_file (const char *, ConfigFile &);


static bool parse_controller_spec (int port, const char *arg)
{
	if (!strcasecmp(arg, "none"))
		S9xSetController(port, CTL_NONE,       0, 0, 0, 0);
	else
	if (!strncasecmp(arg, "pad",   3) && arg[3] >= '1' && arg[3] <= '8' && arg[4] == '\0')
		S9xSetController(port, CTL_JOYPAD, arg[3] - '1', 0, 0, 0);
	else
	if (!strncasecmp(arg, "mouse", 5) && arg[5] >= '1' && arg[5] <= '2' && arg[6] == '\0')
		S9xSetController(port, CTL_MOUSE,  arg[5] - '1', 0, 0, 0);
	else
	if (!strcasecmp(arg, "superscope"))
		S9xSetController(port, CTL_SUPERSCOPE, 0, 0, 0, 0);
	else
	if (!strcasecmp(arg, "justifier") || !strcasecmp(arg, "one-justifier"))
		S9xSetController(port, CTL_JUSTIFIER,  0, 0, 0, 0);
	else
	if (!strcasecmp(arg, "two-justifiers"))
		S9xSetController(port, CTL_JUSTIFIER,  1, 0, 0, 0);
	else
	if (!strncasecmp(arg, "mp5:", 4) && ((arg[4] >= '1' && arg[4] <= '8') || arg[4] == 'n') &&
										((arg[5] >= '1' && arg[5] <= '8') || arg[5] == 'n') &&
										((arg[6] >= '1' && arg[6] <= '8') || arg[6] == 'n') &&
										((arg[7] >= '1' && arg[7] <= '8') || arg[7] == 'n') && arg[8] == '\0')
		S9xSetController(port, CTL_MP5, (arg[4] == 'n') ? -1 : arg[4] - '1',
										(arg[5] == 'n') ? -1 : arg[5] - '1',
										(arg[6] == 'n') ? -1 : arg[6] - '1',
										(arg[7] == 'n') ? -1 : arg[7] - '1');
	else
		return (false);

	return (true);
}

static void parse_crosshair_spec (enum crosscontrols ctl, const char *spec)
{
	int			idx = -1, i;
	const char	*fg = NULL, *bg = NULL, *s = spec;

	if (s[0] == '"')
	{
		s++;
		for (i = 0; s[i] != '\0'; i++)
			if (s[i] == '"' && s[i - 1] != '\\')
				break;

		idx = 31 - ctl;

		std::string	fname(s, i);
		if (!S9xLoadCrosshairFile(idx, fname.c_str()))
			return;

		s += i + 1;
	}
	else
	{
		if (isdigit(*s))
		{
			idx = *s - '0';
			s++;
		}

		if (isdigit(*s))
		{
			idx = idx * 10 + *s - '0';
			s++;
		}

		if (idx > 31)
		{
			fprintf(stderr, "Invalid crosshair spec '%s'.\n", spec);
			return;
		}
	}

	while (*s != '\0' && isspace(*s))
		s++;

	if (*s != '\0')
	{
		fg = s;

		while (isalnum(*s))
			s++;

		if (*s != '/' || !isalnum(s[1]))
		{
			fprintf(stderr, "Invalid crosshair spec '%s.'\n", spec);
			return;
		}

		bg = ++s;

		while (isalnum(*s))
			s++;

		if (*s != '\0')
		{
			fprintf(stderr, "Invalid crosshair spec '%s'.\n", spec);
			return;
		}
	}

	S9xSetControllerCrosshair(ctl, idx, fg, bg);
}

static bool try_load_config_file (const char *fname, ConfigFile &conf)
{
	STREAM	fp;

	fp = OPEN_STREAM(fname, "r");
	if (fp)
	{
		fprintf(stdout, "Reading config file %s.\n", fname);
		conf.LoadFile(new fReader(fp));
		CLOSE_STREAM(fp);
		return (true);
	}

	return (false);
}

void S9xLoadConfigFiles (char **argv, int argc)
{
	static ConfigFile	conf; // static because some of its functions return pointers
	conf.Clear();

	bool	skip = false;
	for (int i = 0; i < argc; i++)
	{
		if (!strcasecmp(argv[i], "-nostdconf"))
		{
			skip = true;
			break;
		}
	}

	if (!skip)
	{
	#ifdef SYS_CONFIG_FILE
		try_load_config_file(SYS_CONFIG_FILE, conf);
		S9xParsePortConfig(conf, 0);
	#endif

		std::string	fname;

		fname = S9xGetDirectory(DEFAULT_DIR);
		fname += SLASH_STR "snes9x.conf";
		if (!try_load_config_file(fname.c_str(), conf))
		{
			fname = S9xGetDirectory(DEFAULT_DIR);
			fname += SLASH_STR "snes9x.cfg";
			try_load_config_file(fname.c_str(), conf);
		}

		fname = S9xGetDirectory(ROM_DIR);
		fname += SLASH_STR "snes9x.conf";
		if (!try_load_config_file(fname.c_str(), conf))
		{
			fname = S9xGetDirectory(ROM_DIR);
			fname += SLASH_STR "snes9x.cfg";
			try_load_config_file(fname.c_str(), conf);
		}
	}
	else
		fprintf(stderr, "Skipping standard config files.\n");

	for (int i = 0; i < argc - 1; i++)
		if (!strcasecmp(argv[i], "-conf"))
			try_load_config_file(argv[++i], conf);

	/* Parse config file here */

	Settings.BSXBootup                  =  conf.GetBool("Settings::BSXBootup",                 false);
	Settings.MouseMaster                =  conf.GetBool("Controls::MouseMaster",               true);
	Settings.SuperScopeMaster           =  conf.GetBool("Controls::SuperscopeMaster",          true);
	Settings.JustifierMaster            =  conf.GetBool("Controls::JustifierMaster",           true);
	Settings.MultiPlayer5Master         =  conf.GetBool("Controls::MP5Master",                 true);
	Settings.ForceInterleaved2          =  conf.GetBool("ROM::Interleaved2",                   false);
	Settings.ForceInterleaveGD24        =  conf.GetBool("ROM::InterleaveGD24",                 false);
	Settings.DisableSoundEcho           = !conf.GetBool("Sound::Echo",                         true);
	Settings.SixteenBitSound            =  conf.GetBool("Sound::16BitSound",                   true);
	Settings.InterpolatedSound          =  conf.GetBool("Sound::Interpolate",                  true);
	Settings.Stereo                     =  conf.GetBool("Sound::Stereo",                       true);
	Settings.ReverseStereo              =  conf.GetBool("Sound::ReverseStereo",                false);
	Settings.Mute                       =  conf.GetBool("Sound::Mute",                         false);
	Settings.FixFrequency               =  conf.GetBool("Sound::FixFrequency",                 false);
	Settings.SoundSync                  =  conf.GetBool("Sound::Sync",                         false);
	Settings.SupportHiRes               =  conf.GetBool("Display::HiRes",                      true);
	Settings.Transparency               =  conf.GetBool("Display::Transparency",               true);
	Settings.DisableGraphicWindows      = !conf.GetBool("Display::GraphicWindows",             true);
	Settings.DisplayFrameRate           =  conf.GetBool("Display::FrameRate",                  false);
	Settings.DisplayWatchedAddresses    =  conf.GetBool("Display::DisplayWatchedAddresses",    false);
	Settings.DisplayPressedKeys         =  conf.GetBool("Display::DisplayInput",               false);
	Settings.DisplayMovieFrame          =  conf.GetBool("Display::DisplayFrameCount",          false);
	Settings.AutoDisplayMessages        =  conf.GetBool("Display::MessagesInImage",            true);
	Settings.DisableGameSpecificHacks   = !conf.GetBool("Hack::EnableGameSpecificHacks",       true);
	Settings.ShutdownMaster             =  conf.GetBool("Hack::SpeedHacks",                    false);
	Settings.DisableIRQ                 =  conf.GetBool("Hack::DisableIRQ",                    false);
	Settings.DisableHDMA                =  conf.GetBool("Hack::DisableHDMA",                   false);
	Settings.NextAPUEnabled             = !conf.GetBool("Hack::DisableAPUEmulation",           false);
	Settings.BlockInvalidVRAMAccess     = !conf.GetBool("Hack::AllowInvalidVRAMAccess",        false);
	Settings.SoundEnvelopeHeightReading = !conf.GetBool("Hack::DisableEnvelopeHeightReading",  false);
	Settings.TurboMode                  =  conf.GetBool("Settings::TurboMode",                 false);
	Settings.MovieTruncate              =  conf.GetBool("Settings::MovieTruncateAtEnd",        false);
	Settings.MovieNotifyIgnored         =  conf.GetBool("Settings::MovieNotifyIgnored",        false);
	Settings.WrongMovieStateProtection  =  conf.GetBool("Settings::WrongMovieStateProtection", true);
	Settings.SnapshotScreenshots        =  conf.GetBool("Settings::SnapshotScreenshots",       true);
	Settings.ApplyCheats                =  conf.GetBool("ROM::Cheat",                          false);
	Settings.NoPatch                    = !conf.GetBool("ROM::Patch",                          true);
	Settings.DontSaveOopsSnapshot       =  conf.GetBool("Settings::DontSaveOopsSnapshot",      false);
	Settings.UpAndDown                  =  conf.GetBool("Controls::AllowLeftRight",            false);
	Settings.SoundPlaybackRate          =  conf.GetUInt("Sound::Rate",                         5);
	Settings.InitialInfoStringTimeout   =  conf.GetInt ("Display::MessageDisplayTime",         120);
	Settings.SoundSkipMethod            =  conf.GetInt ("Hack::SoundSkip",                     0);
	Settings.TurboSkipFrames            =  conf.GetUInt("Settings::TurboFrameSkip",            15);
	Settings.StretchScreenshots         =  conf.GetInt ("Settings::StretchScreenshots",        1);
	Settings.AutoSaveDelay              =  conf.GetUInt("Settings::AutoSaveDelay",             0);

	Settings.ForceLoROM = conf.GetBool("ROM::LoROM", false);
	Settings.ForceHiROM = conf.GetBool("ROM::HiROM", false);
	if (Settings.ForceLoROM)
		Settings.ForceHiROM = false;

	Settings.ForcePAL   = conf.GetBool("ROM::PAL",  false);
	Settings.ForceNTSC  = conf.GetBool("ROM::NTSC", false);
	if (Settings.ForcePAL)
		Settings.ForceNTSC = false;

	if (conf.Exists("ROM::Header"))
	{
		Settings.ForceHeader = conf.GetBool("ROM::Header", false);
		Settings.ForceNoHeader = !Settings.ForceHeader;
	}

	if (conf.Exists("ROM::Interleaved"))
	{
		Settings.ForceInterleaved = conf.GetBool("ROM::Interleaved", false);
		Settings.ForceNotInterleaved = !Settings.ForceInterleaved;
	}

	if (conf.Exists("Settings::FrameTime"))
	{
		double ft;
		if (sscanf(conf.GetString("Settings::FrameTime"), "%lf", &ft) == 1)
			Settings.FrameTimePAL = Settings.FrameTimeNTSC = (int32) ft;
	}

	int	n = conf.GetInt("Hack::HDMATiming", 100);
	if (n > 0 && n < 200)
		Settings.HDMATimingHack = n;

	if (!strcasecmp(conf.GetString("Settings::FrameSkip", "Auto"), "Auto"))
		Settings.SkipFrames = AUTO_FRAMERATE;
	else
		Settings.SkipFrames = conf.GetUInt("Settings::FrameSkip", 0) + 1;

#ifdef NETPLAY_SUPPORT
	Settings.NetPlay = conf.GetBool("Netplay::Enable");

	Settings.Port = NP_DEFAULT_PORT;
	if (conf.Exists("Netplay::Port"))
		Settings.Port = -(int) conf.GetUInt("Netplay::Port");

	Settings.ServerName[0] = '\0';
	if (conf.Exists("Netplay::Server"))
		conf.GetString("Netplay::Server", Settings.ServerName, 128);
#endif

#ifdef DEBUGGER
	if (conf.GetBool("DEBUG::Debugger", false))
		CPU.Flags |= DEBUG_MODE_FLAG;

	if (conf.GetBool("DEBUG::Trace", false))
	{
		if (!trace)
			trace = fopen("trace.log", "wb");
		CPU.Flags |= TRACE_FLAG;
	}
#endif

	if (conf.Exists("Controls::Port1"))
		parse_controller_spec(0, conf.GetString("Controls::Port1"));
	if (conf.Exists("Controls::Port2"))
		parse_controller_spec(1, conf.GetString("Controls::Port2"));

	if (conf.Exists("Controls::Mouse1Crosshair"))
		parse_crosshair_spec(X_MOUSE1,     conf.GetString("Controls::Mouse1Crosshair"));
	if (conf.Exists("Controls::Mouse2Crosshair"))
		parse_crosshair_spec(X_MOUSE2,     conf.GetString("Controls::Mouse2Crosshair"));
	if (conf.Exists("Controls::SuperscopeCrosshair"))
		parse_crosshair_spec(X_SUPERSCOPE, conf.GetString("Controls::SuperscopeCrosshair"));
	if (conf.Exists("Controls::Justifier1Crosshair"))
		parse_crosshair_spec(X_JUSTIFIER1, conf.GetString("Controls::Justifier1Crosshair"));
	if (conf.Exists("Controls::Justifier2Crosshair"))
		parse_crosshair_spec(X_JUSTIFIER2, conf.GetString("Controls::Justifier2Crosshair"));

	rom_filename = conf.GetStringDup("ROM::Filename", NULL);

	S9xParsePortConfig(conf, 1);
	S9xVerifyControllers();
}

void S9xUsage (void)
{
	S9xMessage(S9X_INFO, S9X_USAGE, "Snes9x " VERSION);
	S9xMessage(S9X_INFO, S9X_USAGE, "");
	S9xMessage(S9X_INFO, S9X_USAGE, "usage: snes9x [options] <ROM image filename>");
	S9xMessage(S9X_INFO, S9X_USAGE, "");

	// SOUND OPTIONS
	S9xMessage(S9X_INFO, S9X_USAGE, "-soundquality, -sq, or -r <num> Set sound playback rate");
	S9xMessage(S9X_INFO, S9X_USAGE, "                                   0 -   off, 1 -  8192, 2 - 11025, 3 - 16000,");
	S9xMessage(S9X_INFO, S9X_USAGE, "                                   4 - 22050, 5 - 32000, 6 - 44100, 7 - 48000");
	S9xMessage(S9X_INFO, S9X_USAGE, "-reversestereo                  Reverse stereo sound output");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nostereo                       Disable stereo sound output");
	S9xMessage(S9X_INFO, S9X_USAGE, "-noecho or -ne                  Disable sound echo effects");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nois                           Turn off interpolated sound");
	S9xMessage(S9X_INFO, S9X_USAGE, "-fix                            Fix sound frequencies for some sound devices");
	S9xMessage(S9X_INFO, S9X_USAGE, "-mute                           Don't output audio to sound card");
	S9xMessage(S9X_INFO, S9X_USAGE, "-soundsync or -sy               Enable sound sync to CPU");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nosound or -ns                 (Not recommended) Disable whole sound emulation");
	S9xMessage(S9X_INFO, S9X_USAGE, "-soundskip or -sk <0-3>         (Not recommended) Sound CPU skip-waiting method (use with -nosound)");
	S9xMessage(S9X_INFO, S9X_USAGE, "-noenvx                         (Not recommended) Disable volume envelope height reading");
	S9xMessage(S9X_INFO, S9X_USAGE, "");

	// DISPLAY OPTIONS
	S9xMessage(S9X_INFO, S9X_USAGE, "-displayframerate or -dfr       Display the frame rate counter");
	S9xMessage(S9X_INFO, S9X_USAGE, "-displaykeypress                Display output of all controllers and peripherals");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nohires                        (Not recommended) Disable support for hi-res and interlace modes");
	S9xMessage(S9X_INFO, S9X_USAGE, "-notransparency or -nt          (Not recommended) Disable transparency effects");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nowindows or -nw               (Not recommended) Disable graphic window effects");
	S9xMessage(S9X_INFO, S9X_USAGE, "");

	// CONTROLLER OPTIONS
	S9xMessage(S9X_INFO, S9X_USAGE, "-nomp5                          Disable emulation of the Multiplayer 5 adapter");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nomouse                        Disable emulation of the SNES mouse");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nosuperscope                   Disable emulation of the Superscope");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nojustifier                    Disable emulation of the Konami Justifier");
	S9xMessage(S9X_INFO, S9X_USAGE, "-port# <control>                Specify which controller to emulate in port 1/2");
	S9xMessage(S9X_INFO, S9X_USAGE, "    Controllers: none              No controller");
	S9xMessage(S9X_INFO, S9X_USAGE, "                 pad#              Joypad number 1-8");
	S9xMessage(S9X_INFO, S9X_USAGE, "                 mouse#            Mouse number 1-2");
	S9xMessage(S9X_INFO, S9X_USAGE, "                 superscope        Superscope (not useful with -port1)");
	S9xMessage(S9X_INFO, S9X_USAGE, "                 justifier         Blue Justifier (not useful with -port1)");
	S9xMessage(S9X_INFO, S9X_USAGE, "                 one-justifier     ditto");
	S9xMessage(S9X_INFO, S9X_USAGE, "                 two-justifiers    Blue & Pink Justifiers");
	S9xMessage(S9X_INFO, S9X_USAGE, "                 mp5:####          MP5 with the 4 named pads (1-8 or n)");
	S9xMessage(S9X_INFO, S9X_USAGE, "");

	// ROM OPTIONS
	S9xMessage(S9X_INFO, S9X_USAGE, "-hirom, -hr, or -fh             Force Hi-ROM memory map");
	S9xMessage(S9X_INFO, S9X_USAGE, "-lorom, -lr, or -fl             Force Lo-ROM memory map");
	S9xMessage(S9X_INFO, S9X_USAGE, "-ntsc or -n                     Force NTSC timing (60 frames/sec)");
	S9xMessage(S9X_INFO, S9X_USAGE, "-pal or -p                      Force PAL timing (50 frames/sec)");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nointerleave or -ni            Assume the ROM image is not in interleaved format");
	S9xMessage(S9X_INFO, S9X_USAGE, "-interleaved or -i              Assume the ROM image is in interleaved format");
	S9xMessage(S9X_INFO, S9X_USAGE, "-interleaved2 or -i2            Assume the ROM image is in interleaved 2 format");
	S9xMessage(S9X_INFO, S9X_USAGE, "-interleavedgd24 or -gd24       Assume the ROM image is in interleaved gd24 format");
	S9xMessage(S9X_INFO, S9X_USAGE, "-noheader or -nhd               Assume the ROM image doesn't have a header of a copier");
	S9xMessage(S9X_INFO, S9X_USAGE, "-header, -he, or -hd            Assume the ROM image has a header of a copier");
	S9xMessage(S9X_INFO, S9X_USAGE, "-bsxbootup                      Boot up BS games from BS-X");
	S9xMessage(S9X_INFO, S9X_USAGE, "");

	// PATCH/CHEAT OPTIONS
	S9xMessage(S9X_INFO, S9X_USAGE, "-nopatch                        Do not apply any available IPS patches");
	S9xMessage(S9X_INFO, S9X_USAGE, "-cheat                          Apply saved cheats");
	S9xMessage(S9X_INFO, S9X_USAGE, "-gamegenie or -gg <code>        Supply a Game Genie code");
	S9xMessage(S9X_INFO, S9X_USAGE, "-actionreplay or -ar <code>     Supply a Pro-Action Reply code");
	S9xMessage(S9X_INFO, S9X_USAGE, "-goldfinger or -gf <code>       Supply a Gold Finger code");
	S9xMessage(S9X_INFO, S9X_USAGE, "");

#ifdef NETPLAY_SUPPORT
	// NETPLAY OPTIONS
	S9xMessage(S9X_INFO, S9X_USAGE, "-net                            Enable netplay");
	S9xMessage(S9X_INFO, S9X_USAGE, "-port or -po <num>              Use port <num> for netplay (use with -net)");
	S9xMessage(S9X_INFO, S9X_USAGE, "-server or -srv <string>        Use the specified server for netplay (use with -net)");
	S9xMessage(S9X_INFO, S9X_USAGE, "");
#endif

	// HACKING OR DEBUGGING OPTIONS
#ifdef DEBUGGER
	S9xMessage(S9X_INFO, S9X_USAGE, "-debug                          Set the Debugger flag");
	S9xMessage(S9X_INFO, S9X_USAGE, "-trace                          Begin CPU instruction tracing");
#endif
	S9xMessage(S9X_INFO, S9X_USAGE, "-noirq                          (Not recommended) Disable IRQ emulation");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nohdma                         (Not recommended) Disable HDMA emulation");
	S9xMessage(S9X_INFO, S9X_USAGE, "-hdmatiming <1-199>             (Not recommended) Changes HDMA transfer timings");
	S9xMessage(S9X_INFO, S9X_USAGE, "-cpushutdown                    (Not recommended) Skip emulation until the next event comes");
	S9xMessage(S9X_INFO, S9X_USAGE, "-invalidvramaccess              (Not recommended) Allow invalid VRAM access");
	S9xMessage(S9X_INFO, S9X_USAGE, "");

	// OTHER OPTIONS
	S9xMessage(S9X_INFO, S9X_USAGE, "-frameskip or -f <num>          Screen update frame skip rate");
	S9xMessage(S9X_INFO, S9X_USAGE, "-frametime or -ft <float>       Milliseconds per frame for frameskip auto-adjust");
	S9xMessage(S9X_INFO, S9X_USAGE, "-upanddown                      Override protection from pressing left+right or up+down together");
	S9xMessage(S9X_INFO, S9X_USAGE, "-conf <filename>                Use specified conf file (after standard files)");
	S9xMessage(S9X_INFO, S9X_USAGE, "-nostdconf                      Do not load the standard config files");
	S9xMessage(S9X_INFO, S9X_USAGE, "");

	S9xExtraUsage();

	S9xMessage(S9X_INFO, S9X_USAGE, "");
	S9xMessage(S9X_INFO, S9X_USAGE, "ROM image can be compressed with zip, gzip, JMA, or compress.");

	exit(1);
}

char * S9xParseArgs (char **argv, int argc)
{
	for (int i = 1; i < argc; i++)
	{
		if (*argv[i] == '-')
		{
			if (strcasecmp(argv[i], "-help") == 0)
				S9xUsage();
			else

			// SOUND OPTIONS

			if (strcasecmp(argv[i], "-soundquality") == 0 ||
				strcasecmp(argv[i], "-sq") == 0 ||
				strcasecmp(argv[i], "-r") == 0)
			{
				if (i + 1 < argc)
					Settings.SoundPlaybackRate = atoi(argv[++i]) & 7;
				else
					S9xUsage();
			}
			else
			if (strcasecmp(argv[i], "-reversestereo") == 0)
				Settings.ReverseStereo = TRUE;
			else
			if (strcasecmp(argv[i], "-nostereo") == 0)
				Settings.Stereo = FALSE;
			else
			if (strcasecmp(argv[i], "-noecho") == 0 ||
				strcasecmp(argv[i], "-ne") == 0)
				Settings.DisableSoundEcho = TRUE;
			else
			if (strcasecmp(argv[i], "-nois") == 0)
				Settings.InterpolatedSound = FALSE;
			else
			if (strcasecmp(argv[i], "-fix") == 0)
				Settings.FixFrequency = TRUE;
			else
			if (strcasecmp(argv[i], "-mute") == 0)
				Settings.Mute = TRUE;
			else
			if (strcasecmp(argv[i], "-soundsync") == 0 ||
				strcasecmp(argv[i], "-sy") == 0)
				Settings.SoundSync = TRUE;
			else
			if (strcasecmp(argv[i], "-nosound") == 0 ||
				strcasecmp(argv[i], "-ns") == 0)
				Settings.NextAPUEnabled = FALSE;
			else
			if (strcasecmp(argv[i], "-soundskip") == 0 ||
				strcasecmp(argv[i], "-sk") == 0)
			{
				if (i + 1 < argc)
					Settings.SoundSkipMethod = atoi(argv[++i]);
				else
					S9xUsage();
			}
			else
			if (strcasecmp(argv[i], "-noenvx") == 0)
				Settings.SoundEnvelopeHeightReading = FALSE;
			else

			// DISPLAY OPTIONS

			if (strcasecmp(argv[i], "-displayframerate") == 0 ||
				strcasecmp(argv[i], "-dfr") == 0)
				Settings.DisplayFrameRate = TRUE;
			else
			if (strcasecmp(argv[i], "-displaykeypress") == 0)
				Settings.DisplayPressedKeys = TRUE;
			else
			if (strcasecmp(argv[i], "-nohires") == 0)
				Settings.SupportHiRes = FALSE;
			else
			if (strcasecmp(argv[i], "-notransparency") == 0 ||
				strcasecmp(argv[i], "-nt") == 0)
				Settings.Transparency = FALSE;
			else
			if (strcasecmp(argv[i], "-nowindows") == 0 ||
				strcasecmp(argv[i], "-nw") == 0)
				Settings.DisableGraphicWindows = TRUE;
			else

			// CONTROLLER OPTIONS

			if (strcasecmp(argv[i], "-nomp5") == 0)
				Settings.MultiPlayer5Master = FALSE;
			else
			if (strcasecmp(argv[i], "-nomouse") == 0)
				Settings.MouseMaster = FALSE;
			else
			if (strcasecmp(argv[i], "-nosuperscope") == 0)
				Settings.SuperScopeMaster = FALSE;
			else
			if (strcasecmp(argv[i], "-nojustifier") == 0)
				Settings.JustifierMaster = FALSE;
			else
			if (strcasecmp(argv[i], "-port1") == 0 ||
				strcasecmp(argv[i], "-port2") == 0)
			{
				if (i + 1 < argc)
				{
					i++;
					if (!parse_controller_spec(argv[i - 1][5] - '1', argv[i]))
						S9xUsage();
				}
				else
					S9xUsage();
			}
			else

			// ROM OPTIONS

			if (strcasecmp(argv[i], "-hirom") == 0 ||
				strcasecmp(argv[i], "-hr") == 0 ||
				strcasecmp(argv[i], "-fh") == 0)
				Settings.ForceHiROM = TRUE;
			else
			if (strcasecmp(argv[i], "-lorom") == 0 ||
				strcasecmp(argv[i], "-lr") == 0 ||
				strcasecmp(argv[i], "-fl") == 0)
				Settings.ForceLoROM = TRUE;
			else
			if (strcasecmp(argv[i], "-ntsc") == 0 ||
				strcasecmp(argv[i], "-n") == 0)
				Settings.ForceNTSC = TRUE;
			else
			if (strcasecmp(argv[i], "-pal") == 0 ||
				strcasecmp(argv[i], "-p") == 0)
				Settings.ForcePAL = TRUE;
			else
			if (strcasecmp(argv[i], "-nointerleave") == 0 ||
				strcasecmp(argv[i], "-ni") == 0)
				Settings.ForceNotInterleaved = TRUE;
			else
			if (strcasecmp(argv[i], "-interleaved") == 0 ||
				strcasecmp(argv[i], "-i") == 0)
				Settings.ForceInterleaved = TRUE;
			else
			if (strcasecmp(argv[i], "-interleaved2") == 0 ||
				strcasecmp(argv[i], "-i2") == 0)
				Settings.ForceInterleaved2 = TRUE;
			else
			if (strcasecmp(argv[i], "-interleavedgd24") == 0 ||
				strcasecmp(argv[i], "-gd24") == 0)
				Settings.ForceInterleaveGD24 = TRUE;
			else
			if (strcasecmp(argv[i], "-noheader") == 0 ||
				strcasecmp(argv[i], "-nhd") == 0)
				Settings.ForceNoHeader = TRUE;
			else
			if (strcasecmp(argv[i], "-header") == 0 ||
				strcasecmp(argv[i], "-he") == 0 ||
				strcasecmp(argv[i], "-hd") == 0)
				Settings.ForceHeader = TRUE;
			else
			if (strcasecmp(argv[i], "-bsxbootup") == 0)
				Settings.BSXBootup = TRUE;
			else

			// PATCH/CHEAT OPTIONS

			if (strcasecmp(argv[i], "-nopatch") == 0)
				Settings.NoPatch = TRUE;
			else
			if (strcasecmp(argv[i], "-cheat") == 0)
				Settings.ApplyCheats = TRUE;
			else
			if (strcasecmp(argv[i], "-gamegenie") == 0 ||
				strcasecmp(argv[i], "-gg") == 0)
			{
				if (i + 1 < argc)
				{
					uint32		address;
					uint8		byte;
					const char	*error;

					if ((error = S9xGameGenieToRaw(argv[++i], address, byte)) == NULL)
						S9xAddCheat(TRUE, FALSE, address, byte);
					else
						S9xMessage(S9X_ERROR, S9X_GAME_GENIE_CODE_ERROR, error);
				}
				else
					S9xUsage();
			}
			else
			if (strcasecmp(argv[i], "-actionreplay") == 0 ||
				strcasecmp(argv[i], "-ar") == 0)
			{
				if (i + 1 < argc)
				{
					uint32		address;
					uint8		byte;
					const char	*error;

					if ((error = S9xProActionReplayToRaw(argv[++i], address, byte)) == NULL)
						S9xAddCheat(TRUE, FALSE, address, byte);
					else
						S9xMessage(S9X_ERROR, S9X_ACTION_REPLY_CODE_ERROR, error);
				}
				else
					S9xUsage();
			}
			else
			if (strcasecmp(argv[i], "-goldfinger") == 0 ||
				strcasecmp(argv[i], "-gf") == 0)
			{
				if (i + 1 < argc)
				{
					uint32		address;
					uint8		bytes[3];
					bool8		sram;
					uint8		num_bytes;
					const char	*error;

					if ((error = S9xGoldFingerToRaw(argv[++i], address, sram, num_bytes, bytes)) == NULL)
					{
						for (int c = 0; c < num_bytes; c++)
							S9xAddCheat(TRUE, FALSE, address + c, bytes[c]);
					}
					else
						S9xMessage(S9X_ERROR, S9X_GOLD_FINGER_CODE_ERROR, error);
				}
				else
					S9xUsage();
			}
			else

			// NETPLAY OPTIONS

		#ifdef NETPLAY_SUPPORT
			if (strcasecmp(argv[i], "-net") == 0)
				Settings.NetPlay = TRUE;
			else
			if (strcasecmp(argv[i], "-port") == 0 ||
				strcasecmp(argv[i], "-po") == 0)
			{
				if (i + 1 < argc)
					Settings.Port = -atoi(argv[++i]);
				else
					S9xUsage();
			}
			else
			if (strcasecmp(argv[i], "-server") == 0 ||
				strcasecmp(argv[i], "-srv") == 0)
			{
				if (i + 1 < argc)
				{
					strncpy(Settings.ServerName, argv[++i], 127);
					Settings.ServerName[127] = 0;
				}
				else
					S9xUsage();
			}
			else
		#endif

			// HACKING OR DEBUGGING OPTIONS
		
		#ifdef DEBUGGER
			if (strcasecmp(argv[i], "-debug") == 0)
				CPU.Flags |= DEBUG_MODE_FLAG;
			else
			if (strcasecmp(argv[i], "-trace") == 0)
			{
				if (!trace)
					trace = fopen("trace.log", "wb");
				CPU.Flags |= TRACE_FLAG;
			}
			else
		#endif

			if (strcasecmp(argv[i], "-noirq") == 0)
				Settings.DisableIRQ = TRUE;
			else
			if (strcasecmp(argv[i], "-nohdma") == 0)
				Settings.DisableHDMA = TRUE;
			else
			if (strcasecmp(argv[i], "-hdmatiming") == 0)
			{
				if (i + 1 < argc)
				{
					int	p = atoi(argv[++i]);

					if (p > 0 && p < 200)
						Settings.HDMATimingHack = p;
				}
				else
					S9xUsage();
			}
			else
			if (strcasecmp(argv[i], "-cpushutdown") == 0)
				Settings.ShutdownMaster = TRUE;
			else
			if (strcasecmp(argv[i], "-invalidvramaccess") == 0)
				Settings.BlockInvalidVRAMAccess = FALSE;
			else

			// OTHER OPTIONS

			if (strcasecmp(argv[i], "-frameskip") == 0 ||
				strcasecmp(argv[i], "-f") == 0)
			{
				if (i + 1 < argc)
					Settings.SkipFrames = atoi(argv[++i]) + 1;
				else
					S9xUsage();
			}
			else
			if (strcasecmp(argv[i], "-frametime") == 0 ||
				strcasecmp(argv[i], "-ft") == 0)
			{
				if (i + 1 < argc)
				{
					double	ft;

					if (sscanf(argv[++i], "%lf", &ft) == 1)
						Settings.FrameTimePAL = Settings.FrameTimeNTSC = (int32) ft;
				}
				else
					S9xUsage();
			}
			else
			if (strcasecmp(argv[i], "-upanddown") == 0)
				Settings.UpAndDown = TRUE;
			else
			if (strcasecmp(argv[i], "-conf") == 0)
			{
				if (++i >= argc)
					S9xUsage();
				// Else do nothing, S9xLoadConfigFiles() handled it.
			}
			else
			if (strcasecmp(argv[i], "-nostdconf") == 0)
			{
				// Do nothing, S9xLoadConfigFiles() handled it.
			}
			else
				S9xParseArg(argv, i, argc);
		}
		else
			rom_filename = argv[i];
	}

	S9xVerifyControllers();

	return (rom_filename);
}
