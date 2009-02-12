SUBDIRS = data po

bin_PROGRAMS = snes9x-gtk
snes9x_gtk_LDADD =
snes9x_gtk_CXXFLAGS = -fno-exceptions -fno-rtti
noinst_LIBRARIES =

INCLUDES = -I$(top_srcdir) -I.. -DSNES9XLOCALEDIR=\""$(snes9xlocaledir)"\"

EXTRA_DIST = intltool-extract.in intltool-merge.in intltool-update.in

CLEANFILES = src/gtk_glade.cpp src/sourcify

snes9x_gtk_SOURCES = src/filter_2xsai.cpp src/filter_2xsai.h src/filter_epx.cpp src/filter_epx.h src/filter_hq2x.cpp src/filter_hq2x.h src/gtk_binding.cpp src/gtk_binding.h src/gtk_cheat.cpp src/gtk_cheat.h src/gtk_config.cpp src/gtk_config.h src/gtk_control.cpp src/gtk_control.h src/gtk_display.cpp src/gtk_display_driver_gtk.cpp src/gtk_display_driver_gtk.h src/gtk_display_driver.h src/gtk_display.h src/gtk_file.cpp src/gtk_file.h src/gtk_glade.cpp src/gtk_glade_window.cpp src/gtk_glade_window.h src/gtk_icon.h src/gtk_preferences.cpp src/gtk_preferences.h src/gtk_s9xcore.h src/gtk_s9x.cpp src/gtk_s9x.h src/gtk_s9xwindow.cpp src/gtk_s9xwindow.h src/gtk_sound.cpp src/gtk_sound.h src/gtk_splash.h src/snes_ntsc_config.h src/snes_ntsc.h src/snes_ntsc_impl.h src/snes_ntsc.c

if ZSNESFX
snes9x_gtk_SOURCES += ../i386/fxemu2b.asm ../i386/fxemu2.asm ../i386/fxemu2c.asm ../i386/fxtable.asm ../i386/sfxproc.asm ../i386/ZSNES.ASM
else
snes9x_gtk_SOURCES += ../fxinst.cpp ../fxemu.cpp ../fxdbg.cpp
endif

if ZSNESC4
snes9x_gtk_SOURCES += ../i386/C4.ASM ../i386/zsnesc4.asm ../c4.cpp
else
snes9x_gtk_SOURCES +=../c4.cpp ../c4emu.cpp
endif

snes9x_gtk_SOURCES += ../spc700.cpp ../soundux.cpp ../apu.cpp ../spc7110.cpp ../obc1.cpp ../seta.cpp ../seta010.cpp ../seta011.cpp ../seta018.cpp ../apudebug.cpp ../controls.cpp ../crosshairs.cpp ../cpu.cpp ../sa1.cpp ../debug.cpp ../sdd1.cpp ../tile.cpp ../srtc.cpp ../gfx.cpp ../memmap.cpp ../clip.cpp ../dsp1.cpp ../ppu.cpp ../dma.cpp ../snes9x.cpp ../data.cpp ../globals.cpp ../reader.cpp ../conffile.cpp ../bsx.cpp ../logger.cpp ../snaporig.cpp ../snapshot.cpp ../screenshot.cpp ../movie.cpp

#if ASMCPU
#snes9x_gtk_SOURCES += ../i386/cpuops.S ../i386/cpuexec.S ../i386/sa1ops.S
#else
snes9x_gtk_SOURCES += ../cpuops.cpp ../cpuexec.cpp ../sa1cpu.cpp
#endif

snes9x_gtk_SOURCES += ../cheats.cpp ../cheats2.cpp

if SDD1_DECOMP
snes9x_gtk_SOURCES += ../sdd1emu.cpp
endif

if NETPLAY
snes9x_gtk_SOURCES += ../netplay.cpp ../server.cpp
endif

snes9x_gtk_SOURCES += ../loadzip.cpp ../unzip/unzip.c ../unzip/explode.c ../unzip/unreduce.c ../unzip/unshrink.c
UNZIPDEFINES=-DUNZIP_SUPPORT

if JMA
snes9x_gtk_LDADD += libjma.a
noinst_LIBRARIES += libjma.a
libjma_a_SOURCES = ../jma/s9x-jma.cpp ../jma/7zlzma.cpp ../jma/crc32.cpp ../jma/iiostrm.cpp ../jma/inbyte.cpp ../jma/jma.cpp ../jma/lzma.cpp ../jma/lzmadec.cpp ../jma/winout.cpp
libjma_a_CPPFLAGS = -fexceptions
endif

if OPENGL
snes9x_gtk_SOURCES += src/gtk_display_driver_opengl.cpp src/gtk_display_driver_opengl.h
endif

if XV
snes9x_gtk_SOURCES += src/gtk_display_driver_xv.cpp src/gtk_display_driver_xv.h
endif

if NETPLAY
snes9x_gtk_SOURCES += src/gtk_netplay_dialog.cpp src/gtk_netplay.cpp
endif

SUFFIXES = .asm .ASM

src/sourcify: src/sourcify.c
	$(CC) src/sourcify.c -o src/sourcify

src/gtk_glade.cpp: src/sourcify src/snes9x.glade
	src/sourcify src/snes9x.glade src/gtk_glade.cpp snes9x_glade

.asm.o:
	$(NASM) -f elf -DELF $(NASM_FLAGS) -i ./ -i ../ -ii386 -o $@ $*.asm

.ASM.o:
	$(NASM) -f elf -DELF $(NASM_FLAGS) -i ./ -i ../ -i i386 $*.ASM -o $@

