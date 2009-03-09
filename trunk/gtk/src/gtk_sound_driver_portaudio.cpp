#include "gtk_s9x.h"
#include "gtk_sound_driver_portaudio.h"

static int
port_audio_callback (const void *input,
                     void *output,
                     unsigned long frameCount,
                     const PaStreamCallbackTimeInfo* timeInfo,
                     PaStreamCallbackFlags statusFlags,
                     void *userData)
{
    S9xMixSamples ((uint8 *) output, frameCount * (so.stereo ? 2 : 1));

    return 0;
}

S9xPortAudioSoundDriver::S9xPortAudioSoundDriver(void)
{
    audio_stream = NULL;

    return;
}

void
S9xPortAudioSoundDriver::init (void)
{
    PaError err;

    err = Pa_Initialize ();

    if (err != paNoError)
        fprintf (stderr,
                 "Couldn't initialize PortAudio: %s\n",
                 Pa_GetErrorText (err));

    return;
}

void
S9xPortAudioSoundDriver::terminate (void)
{
    Pa_Terminate ();

    return;
}

void
S9xPortAudioSoundDriver::start (void)
{
    PaError err;

    if (audio_stream != NULL && !(gui_config->mute_sound))
    {
        if ((Pa_IsStreamActive (audio_stream)))
            return;

        err = Pa_StartStream (audio_stream);

        if (err != paNoError)
        {
            fprintf (stderr, "Error: %s\n", Pa_GetErrorText (err));
        }
    }

    return;
}

void
S9xPortAudioSoundDriver::stop (void)
{
    if (audio_stream != NULL)
    {
        Pa_StopStream (audio_stream);
    }

    return;
}

bool8
S9xPortAudioSoundDriver::open_device (int mode, bool8 stereo, int buffer_size)
{
    int                 sample_count;
    PaStreamParameters  param;
    const PaDeviceInfo  *device_info;
    const PaHostApiInfo *hostapi_info;
    PaError             err = paNoError;

    if (audio_stream != NULL)
    {
        printf ("Shutting down sound for reset\n");
        err = Pa_CloseStream (audio_stream);

        if (err != paNoError)
        {
            fprintf (stderr,
                     "Couldn't reset audio stream.\nError: %s\n",
                     Pa_GetErrorText (err));
            return TRUE;
        }

        audio_stream = NULL;
    }

    sample_count = buffer_size;

    param.channelCount = Settings.Stereo ? 2 : 1;
    param.sampleFormat = Settings.SixteenBitSound ? paInt16 : paUInt8;
    param.hostApiSpecificStreamInfo = NULL;

    printf ("PortAudio sound driver initializing...\n");

    for (int i = 0; i < Pa_GetHostApiCount (); i++)
    {
        printf ("    --> ");

        hostapi_info = Pa_GetHostApiInfo (i);
        if (!hostapi_info)
        {
            printf ("Host API #%d has no info\n", i);
            err = paNotInitialized;
            continue;
        }

        device_info = Pa_GetDeviceInfo (hostapi_info->defaultOutputDevice);
        if (!device_info)
        {
            printf ("(%s)...No device info available.\n", hostapi_info->name);
            err = paNotInitialized;
            continue;
        }

        param.device = hostapi_info->defaultOutputDevice;
        param.suggestedLatency = gui_config->sound_buffer_size * 0.001;

        printf ("(%s : %s, latency %dms)...",
                hostapi_info->name,
                device_info->name,
                (int) (param.suggestedLatency * 1000.0));

        fflush (stdout);

        err = Pa_OpenStream (&audio_stream,
                             NULL,
                             &param,
                             d_playback_rates[Settings.SoundPlaybackRate],
                             sample_count,
                             paNoFlag,
                             port_audio_callback,
                             NULL);

        if (err == paNoError)
        {
            printf ("OK\n");
            break;
        }
        else
        {
            printf ("Failed (%s)\n",
                    Pa_GetErrorText (err));
        }
    }

    if (err != paNoError || Pa_GetHostApiCount () < 1)
    {
        fprintf (stderr,
                 "Couldn't initialize sound\n");
        return FALSE;
    }

    fflush (stdout);
    fflush (stderr);

    return TRUE;
}

void
S9xPortAudioSoundDriver::mix (void)
{
    return;
}
