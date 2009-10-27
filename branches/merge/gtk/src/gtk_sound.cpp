#include <errno.h>
#include <unistd.h>

#include "gtk_s9x.h"
#include "gtk_sound.h"
#include "gtk_sound_driver.h"

#ifdef USE_PORTAUDIO
#include "gtk_sound_driver_portaudio.h"
#endif
#ifdef USE_OSS
#include "gtk_sound_driver_oss.h"
#endif
#ifdef USE_JOYSTICK
#include "gtk_sound_driver_sdl.h"
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

int
base2log (int num)
{
    int power;

    if (num < 1)
        return 0;

    for (power = 0; num > 1; power++)
    {
        num >>= 1;
    }

    return power;
}

int
powerof2 (int num)
{
    return (1 << num);
}

void
S9xPortSoundInit (void)
{
    int pao_driver = 0;
    int sdl_driver = 0;
    int oss_driver = 0;
    int max_driver = 0;

    driver = NULL;

#ifdef USE_PORTAUDIO
    sdl_driver++;
    oss_driver++;

    max_driver++;
#endif

#ifdef USE_OSS
    sdl_driver++;

    max_driver++;
#endif

#ifdef USE_JOYSTICK
    max_driver++;
#endif

    if (gui_config->sound_driver >= max_driver)
        gui_config->sound_driver = 0;

#ifdef USE_PORTAUDIO
    if (gui_config->sound_driver == pao_driver)
        driver = new S9xPortAudioSoundDriver ();
#endif

#ifdef USE_OSS
    if (gui_config->sound_driver == oss_driver)
        driver = new S9xOSSSoundDriver ();
#endif

#ifdef USE_JOYSTICK
    if (gui_config->sound_driver == sdl_driver)
        driver = new S9xSDLSoundDriver ();
#endif

    if (driver != NULL)
    {
        driver->init ();

        so.input_rate = CLAMP (gui_config->sound_input_rate, 8000, 48000);

        S9xInitSound (Settings.SoundPlaybackRate,
                      Settings.Stereo,
                      (gui_config->sound_buffer_size * 32000 / 1000) << 2);

        S9xSetSoundMute (FALSE);
    }
    else
    {
        S9xSetSoundMute (gui_config->mute_sound);
    }

    return;
}

void
S9xPortSoundReinit (void)
{
    S9xPortSoundDeinit ();

    /* Ensure the sound device is released before trying to reopen it. */
    sync ();

    S9xPortSoundInit ();
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
S9xMixSound (void)
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

    if (gui_config->sound_buffer_size < 2)
        gui_config->sound_buffer_size = 2;
    if (gui_config->sound_buffer_size > 256)
        gui_config->sound_buffer_size = 256;

    so.buffer_size = (gui_config->sound_buffer_size * so.playback_rate) / 1000;

    if (so.stereo)
        so.buffer_size *= 2;
    if (so.sixteen_bit)
        so.buffer_size *= 2;
    if (so.buffer_size > 65536)
        so.buffer_size = 65536;

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
