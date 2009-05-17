#include <errno.h>

#include "gtk_s9x.h"
#include "gtk_sound.h"
#include "gtk_sound_driver.h"

#ifdef USE_PORTAUDIO
#include "gtk_sound_driver_portaudio.h"
#endif
#ifdef USE_OSS
#include "gtk_sound_driver_oss.h"
#endif

int playback_rates[8] =
{
    0, 8000, 11025, 16000, 22050, 32000, 44100, 48000
};

double d_playback_rates[8] =
{
    0.0, 8000.0, 11025.0, 16000.0, 22050.0, 32000.0, 44100.0, 48000.0
};

S9xSoundDriver *driver;

void
S9xPortSoundInit (void)
{
#ifdef USE_PORTAUDIO
#ifdef USE_OSS
    if (gui_config->sound_driver == 0)
        driver = new S9xPortAudioSoundDriver ();
    else
        driver = new S9xOSSSoundDriver ();
#else
    driver = new S9xPortAudioSoundDriver ();
#endif
#endif
#ifdef USE_OSS
#ifndef USE_PORTAUDIO
    driver = new S9xOSSSoundDriver ();
#endif
#endif

    driver->init ();

    S9xInitSound (Settings.SoundPlaybackRate,
                  Settings.Stereo,
                  Settings.SoundBufferSize);

    S9xSetSoundMute (FALSE);

    return;
}

void
S9xPortSoundDeinit (void)
{
    S9xSoundStop ();

    driver->terminate ();

    delete driver;

    return;
}

void
S9xSoundStart (void)
{
    driver->start ();

    return;
}

void
S9xSoundStop (void)
{
    driver->stop ();

    return;
}

void
S9xGenerateSound (void)
{
    driver->mix ();

    return;
}

bool8
S9xOpenSoundDevice (int mode, bool8 stereo, int buffer_size)
{
    if (Settings.APUEnabled == FALSE)
        return FALSE;

    so.playback_rate = playback_rates[Settings.SoundPlaybackRate];
    S9xSetPlaybackRate (so.playback_rate);
    so.stereo = Settings.Stereo ? 1 : 0;
    so.sixteen_bit = Settings.SixteenBitSound;
    so.encoded = 1;

    so.buffer_size = (gui_config->sound_buffer_size * so.playback_rate) / 1000;

    if (so.stereo)
        so.buffer_size *= 2;
    if (so.sixteen_bit)
        so.buffer_size *= 2;
    if (so.buffer_size >= MAX_BUFFER_SIZE)
        so.buffer_size = MAX_BUFFER_SIZE;

    if (gui_config->mute_sound)
        return FALSE;

    return driver->open_device (mode, stereo, buffer_size);
}

/* This really shouldn't be in the port layer */
void
S9xToggleSoundChannel (int c)
{
    if (c == 8)
        so.sound_switch = 255;
    else
        so.sound_switch ^= 1 << c;

    S9xSetSoundControl (so.sound_switch);

    return;
}
