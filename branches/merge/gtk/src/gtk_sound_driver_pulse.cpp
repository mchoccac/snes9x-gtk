#include "gtk_s9x.h"
#include "gtk_sound_driver_pulse.h"

#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>

static void
pulse_samples_available (void *data)
{
    ((S9xPulseSoundDriver *) data)->samples_available ();
}

S9xPulseSoundDriver::S9xPulseSoundDriver (void)
{
    pulse = NULL;
    buffer = NULL;
    buffer_size = 0;

    return;
}

void
S9xPulseSoundDriver::init (void)
{
    return;
}

void
S9xPulseSoundDriver::terminate (void)
{
    stop ();

    S9xSetSamplesAvailableCallback (NULL, NULL);

    if (pulse)
    {
        pa_simple_free (pulse);
    }

    if (buffer)
    {
        delete[] buffer;
        buffer = NULL;
    }

    return;
}

void
S9xPulseSoundDriver::start (void)
{
    return;
}

void
S9xPulseSoundDriver::stop (void)
{
    return;
}

bool8
S9xPulseSoundDriver::open_device (int mode, bool8 stereo, int buffer_size)
{
    int err;
    pa_sample_spec ss;
    pa_buffer_attr buffer_attr;

    ss.channels = stereo ? 2 : 1;
    ss.format   = so.sixteen_bit ? PA_SAMPLE_S16NE : PA_SAMPLE_U8;
    ss.rate     = so.playback_rate;

    buffer_attr.maxlength = -1;
    buffer_attr.fragsize  = -1;
    buffer_attr.prebuf    = -1;
    buffer_attr.minreq    = -1;
    buffer_attr.tlength   = ((so.playback_rate <<
                             (so.stereo ? 1 : 0)) <<
                             (so.sixteen_bit ? 1 : 0)) *
                             (gui_config->sound_buffer_size + 16) /
                             1000;

    printf ("PulseAudio sound driver initializing...\n");

    printf ("    --> (%dhz, %s %s, %dms (%d+16ms))...",
            so.playback_rate,
            so.sixteen_bit ? "16-bit" : "8-bit",
            so.stereo ? "Stereo" : "Mono",
            gui_config->sound_buffer_size + 16,
            gui_config->sound_buffer_size);

    pulse = pa_simple_new (NULL,
                           "Snes9x",
                           PA_STREAM_PLAYBACK,
                           NULL,
                           "Game",
                           &ss,
                           NULL,
                           &buffer_attr,
                           &err);

    if (!pulse)
    {
        printf ("Failed: %s\n", pa_strerror (err));
        return FALSE;
    }

    printf ("OK\n");

    S9xSetSamplesAvailableCallback (pulse_samples_available, this);

    return TRUE;
}

void
S9xPulseSoundDriver::mix (void)
{
    return;
}

void
S9xPulseSoundDriver::samples_available (void)
{
    int bytes, err;

    S9xFinalizeSamples ();

    bytes = (S9xGetSampleCount () << (so.sixteen_bit ? 1 : 0));

    if (bytes <= 128)
        return;

    if (buffer_size < bytes || buffer == NULL)
    {
        delete[] buffer;
        buffer = new uint8[bytes];
        buffer_size = bytes;
    }

    S9xMixSamples (buffer, bytes >> (so.sixteen_bit ? 1 : 0));

    pa_simple_write (pulse, buffer, bytes, &err);

    return;
}
