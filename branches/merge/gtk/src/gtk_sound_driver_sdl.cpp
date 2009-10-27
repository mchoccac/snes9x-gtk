#include "gtk_s9x.h"
#include "gtk_sound_driver_sdl.h"

static void
sdl_audio_callback (void *userdata, Uint8 *stream, int len)
{
    ((S9xSDLSoundDriver *) userdata)->mix ((unsigned char *) stream, len);

    return;
}

static void
samples_available (void *data)
{
    SDL_LockAudio ();
    S9xFinalizeSamples ();
    SDL_UnlockAudio ();

    return;
}

void
S9xSDLSoundDriver::mix (unsigned char *output, int bytes)
{
    SDL_LockAudio ();
    S9xMixSamples (output, bytes >> (so.sixteen_bit ? 1 : 0));
    SDL_UnlockAudio ();

    return;
}

S9xSDLSoundDriver::S9xSDLSoundDriver (void)
{
    audiospec = NULL;

    return;
}

void
S9xSDLSoundDriver::init (void)
{
    SDL_InitSubSystem (SDL_INIT_AUDIO);

    return;
}

void
S9xSDLSoundDriver::terminate (void)
{
    stop ();

    if (audiospec)
    {
        SDL_CloseAudio ();
        free (audiospec);
        audiospec = NULL;
    }

    SDL_QuitSubSystem (SDL_INIT_AUDIO);

    return;
}

void
S9xSDLSoundDriver::start (void)
{
    if (!gui_config->mute_sound)
    {
        if (audiospec)
        {
            SDL_PauseAudio (0);
        }
    }

    return;
}

void
S9xSDLSoundDriver::stop (void)
{
    if (audiospec)
    {
        SDL_PauseAudio (1);
    }

    return;
}

bool8
S9xSDLSoundDriver::open_device (int mode, bool8 stereo, int buffer_size)
{
    audiospec = (SDL_AudioSpec *) malloc (sizeof (SDL_AudioSpec));

    audiospec->freq = playback_rates [Settings.SoundPlaybackRate];
    audiospec->channels = so.stereo ? 2 : 1;
    audiospec->format = so.sixteen_bit ? AUDIO_S16SYS : AUDIO_U8;
    audiospec->samples = gui_config->sound_buffer_size * audiospec->freq / 1000;
    /* Note: divide samples by 16 because SDL thinks it needs a whole buffer on
     * every callback. :-P */
    audiospec->samples = powerof2 (base2log (audiospec->samples)) >> 4;
    audiospec->callback = sdl_audio_callback;
    audiospec->userdata = this;

    printf ("SDL sound driver initializing...\n");
    printf ("    --> (Frequency: %dhz, Latency: %dms)...",
            audiospec->freq,
            (audiospec->samples * 1000 / audiospec->freq) << 4);

    if (SDL_OpenAudio (audiospec, NULL) < 0)
    {
        printf ("Failed\n");

        free (audiospec);
        audiospec = NULL;

        return FALSE;
    }

    printf ("OK\n");

    S9xSetSamplesAvailableCallback (samples_available, NULL);

    return TRUE;
}

void
S9xSDLSoundDriver::mix (void)
{
    return;
}
