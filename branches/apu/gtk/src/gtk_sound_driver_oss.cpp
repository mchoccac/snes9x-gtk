#include "gtk_s9x.h"
#include "gtk_sound_driver_oss.h"

#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>

static void
oss_samples_available (void *data)
{
    ((S9xOSSSoundDriver *) data)->samples_available ();
}

S9xOSSSoundDriver::S9xOSSSoundDriver (void)
{
    filedes = -1;
    sound_buffer = NULL;

    return;
}

void
S9xOSSSoundDriver::init (void)
{
    return;
}

void
S9xOSSSoundDriver::terminate (void)
{
    stop ();

    S9xSetSamplesAvailableCallback (NULL, NULL);

    if (filedes >= 0)
    {
        close (filedes);
    }

    if (sound_buffer)
    {
        free (sound_buffer);
        sound_buffer = NULL;
    }

    return;
}

void
S9xOSSSoundDriver::start (void)
{
    return;
}

void
S9xOSSSoundDriver::stop (void)
{
    return;
}

bool8
S9xOSSSoundDriver::open_device (int mode, bool8 stereo, int buffer_size)
{
    int temp;

    printf ("OSS sound driver initializing...\n");

    printf ("    --> (Device: /dev/dsp)...");

    filedes = open ("/dev/dsp", O_WRONLY | O_NONBLOCK);

    if (filedes < 0)
        goto fail;

    printf ("OK\n");


    if (so.sixteen_bit)
    {
        printf ("    --> (Format: 16-bit)...");

        temp = AFMT_S16_LE;
        if (ioctl (filedes, SNDCTL_DSP_SETFMT, &temp) < 0)
            goto close_fail;
    }
    else
    {
        printf ("    --> (Format: 8-bit)...");

        temp = AFMT_U8;
        if (ioctl (filedes, SNDCTL_DSP_SETFMT, &temp) < 0)
            goto close_fail;
    }

    printf ("OK\n");

    if (Settings.Stereo)
    {
        temp = 2;
        printf ("    --> (Stereo)...");
    }
    else
    {
        temp = 1;
        printf ("    --> (Mono)...");
    }

    if (ioctl (filedes, SNDCTL_DSP_CHANNELS, &temp) < 0)
        goto close_fail;

    printf ("OK\n");

    printf ("    --> (Frequency: %d)...", so.playback_rate);
    if (ioctl (filedes, SNDCTL_DSP_SPEED, &so.playback_rate) < 0)
        goto close_fail;

    printf ("OK\n");

    /* Make sure there's at least enough for one frame of sound */
    if (so.buffer_size < 4096)
        so.buffer_size = 4096;

    /* OSS requires a power-of-two buffer size, first 16 bits are the number
     * of fragments to generate, second 16 are the respective power-of-two. */
    temp = (2 << 16) | (base2log (so.buffer_size));

    so.buffer_size = powerof2 (temp & 0xffff);

    printf ("    --> (Buffer size: %d bytes, %dms latency)...",
            so.buffer_size,
            (((so.buffer_size * 1000) >> (so.stereo ? 1 : 0))
                                 >> (so.sixteen_bit ? 1 : 0))
                              / (so.playback_rate));

    if (ioctl (filedes, SNDCTL_DSP_SETFRAGMENT, &temp) < 0)
        goto close_fail;

    printf ("OK\n");

    sound_buffer = (uint8 *) malloc (so.buffer_size);

    S9xSetSamplesAvailableCallback (oss_samples_available, this);

    return TRUE;

close_fail:

    close (filedes);

fail:
    printf ("failed\n");

    return FALSE;
}

void
S9xOSSSoundDriver::mix (void)
{
    return;
}

void
S9xOSSSoundDriver::samples_available (void)
{
    audio_buf_info info;
    int samples_to_write;
    int bytes_to_write;
    int bytes_written;

    S9xFinalizeSamples ();

    ioctl (filedes, SNDCTL_DSP_GETOSPACE, &info);

    samples_to_write = MIN (info.bytes >> (so.sixteen_bit ? 1 : 0),
                            S9xGetSampleCount ());

    if (sizeof (sound_buffer) < ((unsigned int) samples_to_write << (so.sixteen_bit ? 1 : 0)))
    {
        sound_buffer = (uint8 *) realloc (sound_buffer, samples_to_write << (so.sixteen_bit ? 1 : 0));
    }

    S9xMixSamples (sound_buffer, samples_to_write);

    bytes_written = 0;
    bytes_to_write = samples_to_write << (so.sixteen_bit ? 1 : 0);

    while (bytes_to_write > bytes_written)
    {
        int result;

        result = write (filedes,
                        ((char *) sound_buffer) + bytes_written,
                        bytes_to_write - bytes_written);

        if (result < 0)
            break;

        bytes_written += result;
    }

    return;
}
