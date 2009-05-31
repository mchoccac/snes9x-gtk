#include "gtk_s9x.h"
#include "gtk_sound_driver_sdl.h"

static void
sdl_audio_callback (void *userdata, Uint8 *stream, int len)
{
    ((S9xSDLSoundDriver *) userdata)->mix ((unsigned char *) stream, len);

    return;
}

void
S9xSDLSoundDriver::mix (unsigned char *output, int bytes)
{
    mixer->write (output, bytes >> 1);
    mixer->write (output + (bytes >> 1), bytes - (bytes >> 1));

    return;
}

S9xSDLSoundDriver::S9xSDLSoundDriver (void)
{
    audiospec = NULL;
    mixer = NULL;

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

    if (mixer)
    {
        SDL_CloseAudio ();
        free (audiospec);
        delete mixer;
        mixer = NULL;
    }

    SDL_QuitSubSystem (SDL_INIT_AUDIO);

    return;
}

void
S9xSDLSoundDriver::start (void)
{
    if (!gui_config->mute_sound)
    {
        if (mixer)
        {
            mixer->start ();
            SDL_PauseAudio (0);
        }
    }

    return;
}

void
S9xSDLSoundDriver::stop (void)
{
    if (mixer)
    {
        mixer->stop ();
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
    audiospec->samples = powerof2 (base2log (audiospec->samples));
    so.buffer_size = (audiospec->samples << (so.stereo ? 1 : 0)) << (so.sixteen_bit ? 1 : 0);
    audiospec->callback = sdl_audio_callback;
    audiospec->userdata = this;

    printf ("SDL sound driver initializing...\n");
    printf ("    --> (Frequency: %dhz, Latency: %dms)...",
            audiospec->freq,
            audiospec->samples * 1000 / audiospec->freq);

    if (SDL_OpenAudio (audiospec, NULL) < 0)
    {
        printf ("Failed\n");

        free (audiospec);
        audiospec = NULL;

        return FALSE;
    }

    printf ("OK\n");

    mixer = new GtkAudioMixer (so.buffer_size);

    return TRUE;
}

void
S9xSDLSoundDriver::mix (void)
{
    return;
}
