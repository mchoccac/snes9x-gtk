PACKAGE="snes9x-gtk"
VERSION="1.51"

AC_SUBST(PACKAGE)
AC_SUBST(VERSION)

AC_INIT([snes9x-gtk], 1.51)
dnl Set CFLAGS, CXXFLAGS to something 
dnl before doing AC_PROG_CC, AC_PROG_CXX to avoid default -g
CFLAGS="-O2 -fomit-frame-pointer -Wall -W -pedantic -Wno-unused-parameter -pipe $CFLAGS"
CXXFLAGS="$CFLAGS"

AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_LANG_CPLUSPLUS

AC_PROG_INSTALL
AM_INIT_AUTOMAKE

AM_PROG_CC_C_O
AC_PROG_INTLTOOL([0.23])
GETTEXT_PACKAGE=[snes9x-gtk]
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE")

ALL_LINGUAS="fr_FR"
AM_GLIB_GNU_GETTEXT

snes9xlocaledir='${prefix}/${DATADIRNAME}/locale'
AC_SUBST(snes9xlocaledir)


AC_ARG_WITH(debug,
  [AS_HELP_STRING([--with(out)-debug],
    [Leave debug information in the final binary (default: without)])],
  [],
  [with_debug=no])

AC_ARG_WITH(opengl,
  [AS_HELP_STRING([--with(out)-opengl],
    [Use OpenGL for output (default: with)])],
  [],
  [with_opengl=yes])

AC_ARG_WITH(gtk,
  [AS_HELP_STRING([--with(out)-gtk],
    [Use GTK GUI as frontend. (default: with)])],
  [],
  [with_gtk=yes])

AC_ARG_WITH(joystick, 
  [AS_HELP_STRING([--with(out)-joystick],
    [Enable joystick support if available (default: with)])],
  [],
  [with_joystick=yes])

AC_ARG_WITH(xv, 
  [AS_HELP_STRING([--with(out)-xv],
    [Enable XVideo output on GTK (default: with)])],
  [],
  [with_xv=yes])

AC_ARG_WITH(xrandr, 
  [AS_HELP_STRING([--with(out)-xrandr],
    [Enable XRandR support on GTK (default: with)])],
  [],
  [with_xrandr=yes])

AC_ARG_WITH(assembler,
  [AS_HELP_STRING([--with(out)-assembler],
    [Use assembler cores if available (default: with)])],
  [],
  [with_assembler=yes])

AC_ARG_WITH(zsnesc4,
  [AS_HELP_STRING([--with(out)-zsnesc4],
    [Use ZSNES C4 core if available (default: same as --with-assembler)])],
  [],
  [with_zsnesc4=$with_assembler])

AC_ARG_WITH(zsnesfx,
  [AS_HELP_STRING([--with(out)-zsnesfx],
    [Use ZSNES SuperFX core if available (default: same as --with-assembler)])],
  [],
  [with_zsnesfx=$with_assembler])

AC_ARG_WITH(correct-vram-reads,
  [AS_HELP_STRING([--with(out)-correct-vram-reads],
    [Use Correct VRAM read logic (default: with)])],
  [],
  [with_correct_vram_reads=yes])

AC_ARG_WITH(debugger, 
  [AS_HELP_STRING([--with(out)-debugger],
    [Enable debugger (default: without)])],
  [],
  [with_debugger=no])

AC_ARG_WITH(sdd1-decomp,
  [AS_HELP_STRING([--with(out)-sdd1-decomp],
    [Use SDD1 decompression (default: with)])],
  [],
  [with_sdd1_decomp=yes])

AC_ARG_WITH(netplay,
  [AS_HELP_STRING([--with(out)-netplay],
    [Enable netplay support (default: without)])],
  [],
  [with_netplay=yes])

AC_ARG_WITH(jma-decomp, 
  [AS_HELP_STRING([--with(out)-jma-decomp],
    [Enable JMA archive decompression (default: with)])],
  [],
  [with_jma_decomp=yes])

AC_ARG_WITH(newblend,
  [AS_HELP_STRING([--with(out)-newblend],
    [New colour blender (default: with)])],
  [],
  [with_newblend=yes])

AC_ARG_WITH(zlib,
  [AS_HELP_STRING([--with(out)-zlib],
    [Support gzip decompression])],
  [],
  [with_zlib=yes])

AC_ARG_WITH(png,
  [AS_HELP_STRING([--with(out)-screenshot],
    [Screenshot support through libpng if available (default: with)])],
  [],
  [with_screenshot=yes])

if test yes = "$with_debug" ; then
  CFLAGS="$CFLAGS -g"
fi

ZSNESFX='#ZSNESFX=yes'
ZSNESC4='#ZSNESC4=yes'
ASMCPU=''
SDD1_DECOMP='#SDD1_DECOMP=yes'
JMA='#JMA=yes'
SPC700ASM=''
I386SPC=''
CPUINC=''
NETPLAY='#NETPLAY=yes'

SYSLIBS=''
NASM='not_found'

OPENGL=0

GTK=0

if test yes = "$with_opengl" ; then
  OPENGL=yes
fi

if test yes = "$with_gtk" ; then
    GTK=yes
    snes9x_info_display="GTK"
    
    
    PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.10])
    PKG_CHECK_MODULES([GLIB], [gthread-2.0 >= 2.6 gobject-2.0 >= 2.6])
    PKG_CHECK_MODULES([GLADE], [libglade-2.0 >= 2.0])
    PKG_CHECK_MODULES([LIBXML], [libxml-2.0 >= 2.0])
    PKG_CHECK_MODULES([PORTAUDIO], [portaudio-2.0 >= 10])
    PKG_CHECK_MODULES([XTEST], [xtst])
                              
    if test yes = "$with_opengl" ; then
        PKG_CHECK_EXISTS([gtkglext-1.0 >= 1.0],[OPENGL=yes],[OPENGL=0])

        if test yes = "$OPENGL" ; then
    	    PKG_CHECK_MODULES([GTKGL], [gtkglext-1.0 > 1.0])
            CFLAGS="$CFLAGS -DUSE_OPENGL"
        fi
    fi

    GTKJOY=0
    SDL_CFLAGS=""
    SDL_LDFLAGS=""
    if test yes = "$with_joystick"; then
        PKG_CHECK_MODULES([SDL],[sdl],[
            GTKJOY=yes
            CFLAGS="$CFLAGS -DUSE_JOYSTICK"
        ],[
            AC_CHECK_PROG(SDLCONFIGFOUND,[sdl-config],yes,no)
            
            if test yes = "$SDLCONFIGFOUND" ; then
                SDL_CFLAGS="-DUSE_JOYSTICK `sdl-config --cflags`"
                SDL_LDFLAGS="`sdl-config --libs`"
                GTKJOY=yes
            else
                echo "Cannot find SDL. Disabling joystick support."
            fi
        ])
    fi

    XV=0
    XV_CFLAGS=""
    XV_LDFLAGS=""
    if test yes = "$with_xv" ; then
        PKG_CHECK_MODULES([XV],[xv],[
            XV=yes
            CFLAGS="$CFLAGS -DUSE_XV"
        ],[
            echo "Cannot find libXv. Make sure the X11 development headers are installed."
            echo "--> Disabling Xv support."
        ])
    fi
   
    XRANDR=0
    XRANDR_CFLAGS=""
    XRANDR_LDFLAGS=""
    if test yes = "$with_xrandr" ; then
        PKG_CHECK_MODULES([XRANDR],[xrandr],[
            XRANDR=yes
            CFLAGS="$CFLAGS -DUSE_XRANDR"
        ],[
            echo "Cannot find libXrandr. Make sure the X11 development headers are installed."
            echo "--> Disabling Xrandr support."
        ])
    fi
    PKG_CHECK_MODULES([LIBPNG],libpng)

    CFLAGS="$CFLAGS $GTK_CFLAGS $GLIB_CFLAGS $GLADE_CFLAGS $LIBXML_CFLAGS $PORTAUDIO_CFLAGS $GTKGL_CFLAGS $LIBPNG_CFLAGS $SDL_CFLAGS $XV_CFLAGS $XRANDR_CFLAGS $XTEST_CFLAGS"
    LDFLAGS="$LDFLAGS $GTK_LIBS $GLIB_LIBS $GLADE_LIBS $LIBXML_LIBS $PORTAUDIO_LIBS $GTKGL_LIBS $LIBPNG_LIBS $SDL_LIBS $XV_LIBS $XRANDR_LIBS $XTEST_LIBS"
fi


AC_MSG_CHECKING(if the CPU is IA32)
AC_CACHE_VAL(snes9x_cv_i386_cpu, [
  AC_TRY_COMPILE([
#if (defined (__i386__) || defined (__i486__) ||\
     defined (__i586__) || defined (__i686__) || defined (__WIN32__) || defined (__DJGPP))
/* */
#else
#error Not i386.
#endif
  ], [], snes9x_cv_i386_cpu=yes, snes9x_cv_i386_cpu=no)])

dnl Enable assembler optimizations if CPU is x86

if test yes = "$with_assembler"; then
  AC_PATH_PROG(prog_nasm, nasm, no)

  if test "x$ac_cv_path_prog_nasm" = "xno"; then
    if test yes = "$snes9x_cv_i386_cpu"; then
        AC_MSG_WARN([No nasm found. Will build without assembler optimizations.])
    fi
  else
    if test yes = "$snes9x_cv_i386_cpu"; then
      NASM="$ac_cv_path_prog_nasm"
    fi
  fi
fi

if test $NASM != "not_found"; then
  if test yes = "$with_zsnesc4"; then
    ZSNESC4=yes
    CFLAGS="$CFLAGS -DZSNES_C4"
    NASM_FLAGS="$NASM_FLAGS -DZSNES_C4"
  fi
  if test yes = "$with_zsnesfx"; then
    ZSNESFX=yes
    CFLAGS="$CFLAGS -DZSNES_FX -DEXECUTE_SUPERFX_PER_LINE"
    NASM_FLAGS="$NASM_FLAGS -DZSNES_FX -DEXECUTE_SUPERFX_PER_LINE"
  fi
  CPUINC='-Ii386'
fi

if test yes = "$with_correct_vram_reads"; then
  CFLAGS="$CFLAGS -DCORRECT_VRAM_READS"
  CORRECT_VRAM_READS=1
fi

if test yes = "$with_debugger"; then
   CFLAGS="$CFLAGS -DDEBUGGER'"
fi


dnl Enable SDD1 decompression if requested
if test yes = "$with_sdd1_decomp"; then
   CFLAGS="$CFLAGS -DSDD1_DECOMP"
   SDD1_DECOMP=yes
fi

dnl Enable Netplay support if requested
if test yes = "$with_netplay"; then
   CFLAGS="$CFLAGS -DNETPLAY_SUPPORT"
   NETPLAY=yes
fi

dnl Enable jma decompression (nachs own format)
if test yes = "$with_jma_decomp"; then
   CFLAGS="$CFLAGS -DJMA_SUPPORT"
   JMA=yes
fi

dnl FIXME: There is a third case (OLD) that is missed with this.
if test yes = "$with_newblend"; then
   CFLAGS="$CFLAGS -DNEW_COLOUR_BLENDING"
else
   #OLD_COLOUR_BLENDING hasn't been default for a while
   # SYSDEFINES="$SYSDEFINES"' -DOLD_COLOUR_BLENDING'
   :
fi

if test x$with_zlib = xyes ; then
  AC_CHECK_HEADERS(zlib.h)
  AC_CHECK_LIB(z, gzread, [
        CFLAGS="$CFLAGS -DZLIB"
        LDFLAGS="$LDFLAGS -lz"
      ], ,)
fi

if test yes = "$with_screenshot"; then
  AC_CHECK_LIB(png, png_init_io, [
    CFLAGS="$CFLAGS -DHAVE_LIBPNG"
    LDFLAGS="$LDFLAGS -lpng"
  ], , -lz -lm)
fi

dnl Check for functions
AC_CHECK_FUNC(mkstemp, [
  CFLAGS="$CFLAGS -DHAVE_MKSTEMP"
])

dnl Check for headers

AC_CHECK_HEADER(strings.h, [
  CFLAGS="$CFLAGS -DHAVE_STRINGS_H"
])

AC_CHECK_HEADER(sys/ioctl.h, [
  CFLAGS="$CFLAGS -DHAVE_SYS_IOCTL_H"
])

AC_CHECK_HEADER(stdint.h, [
  CFLAGS="$CFLAGS -DHAVE_STDINT_H"
])


dnl Check SAR

AC_DEFUN(AC_MY_CHECK_SAR,
[
AC_MSG_CHECKING([if rightshift $1 is arithmetic])
AC_CACHE_VAL(ac_cv_my_sar_$1,
[
AC_TRY_RUN([
#include "../port.h"

int main(void){
    $1 i;

    i=-1;
    i>>=1;
    return(i<0?0:1);
}
],ac_cv_my_sar_$1=yes,ac_cv_my_sar_$1=no,ac_cv_my_sar_%1=no)
])

if test $ac_cv_my_sar_$1 = no; then
  AC_MSG_RESULT(no)
else
  CFLAGS="$CFLAGS -DRIGHTSHIFT_$1_IS_SAR"
  AC_MSG_RESULT(yes)
fi
])

AC_MSG_CHECKING(if sizeof(void *) == sizeof(int))
AC_TRY_RUN([
  int main()
  {
   return(!(sizeof(void *) == sizeof(int)));
  }
], snes9x_ptr_is_int=yes)
if test x$snes9x_ptr_is_int = xyes; then
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
  CFLAGS="$CFLAGS -DPTR_NOT_INT"
fi

AC_MY_CHECK_SAR(int8)
AC_MY_CHECK_SAR(int16)
AC_MY_CHECK_SAR(int32)
AC_MY_CHECK_SAR(int64)
if test $ac_cv_my_sar_int8 = yes && \
   test $ac_cv_my_sar_int16 = yes && \
   test $ac_cv_my_sar_int32 = yes && \
   test $ac_cv_my_sar_int64 = yes ; then
  CFLAGS="$CFLAGS -DRIGHTSHIFT_IS_SAR"
fi

CFLAGS="$CFLAGS -DCPU_SHUTDOWN -DSPC700_SHUTDOWN -DNO_INLINE_SET_GET -DUNZIP_SUPPORT -DSPC700_C -I. -I.. -I../unzip"

CXXFLAGS="$CFLAGS"

CCASFLAGS="$CFLAGS -I.. -I../i386"

AM_CONDITIONAL(ZSNESFX, [test yes = "$ZSNESFX"])
AM_CONDITIONAL(ZSNESC4, [test yes = "$ZSNESC4"])
AM_CONDITIONAL(ASMCPU, [test yes = "$ASMCPU"])
AM_CONDITIONAL(SDD1_DECOMP, [test yes = "$SDD1_DECOMP"])
AM_CONDITIONAL(NETPLAY, [test yes = "$NETPLAY"])
AM_CONDITIONAL(JMA, [test yes = "$JMA"])
AM_CONDITIONAL(OPENGL, [test yes = "$OPENGL"])
AM_CONDITIONAL(XV, [test yes = "$XV"])

AC_SUBST(NASM)
AC_SUBST(NASM_FLAGS)

AC_OUTPUT(Makefile data/Makefile po/Makefile.in)

echo ""
echo "Snes9x will build with support for the following:"
echo ""

if test yes = "$OPENGL" ; then
    echo "    OpenGL"
else
    if test yes = "$with_opengl" ; then
        echo "!!! NOT able to use OpenGL support"
        echo "--- Needs gtkglext and OpenGL libraries and headers"
    fi
fi

if test yes = "$XV" ; then
    echo "    XVideo"
else
    if test yes = "$with_xv" ; then
        echo "!!! NOT able to use XVideo support"
        echo "--- Needs recent X11 libraries and headers"
    fi
fi

if test yes = "$XRANDR" ; then
    echo "    XRandR"
else
    if test yes = "$with_xrandr" ; then
        echo "!!! NOT able to use XRandR support"
        echo "--- Needs recent X11 libraries and headers"
    fi
fi

if test yes = "$GTKJOY" ; then
    echo "    Joysticks"
else
    if test yes = "$with_joystick" ; then
        echo "!!! NOT able to use joystick support"
        echo "--- Needs SDL libraries and headers"
    fi
fi

if test yes = "$NETPLAY" ; then
    echo "    NetPlay"
else
    if test yes = "$with_netplay" ; then
        echo "!!! NOT able to build NetPlay support"

    fi
fi

echo ""
