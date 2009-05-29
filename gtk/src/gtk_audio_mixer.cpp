#include "gtk_s9x.h"
#include "gtk_audio_mixer.h"
#include <sys/time.h>

gpointer
gtk_audio_mixer_thread (gpointer data)
{
    ((GtkAudioMixer *) data)->mixer_thread ();

    return NULL;
}

GtkAudioMixer::GtkAudioMixer (int buffer_size)
{
    this->buffer_size = buffer_size + 1;
    internal_buffer = new unsigned char[this->buffer_size];
    buffer_mutex = NULL;
    thread = NULL;

    return;
}

GtkAudioMixer::~GtkAudioMixer (void)
{
    delete[] internal_buffer;

    return;
}

void
GtkAudioMixer::start (void)
{
    if (thread)
        return;

    thread_die = 0;

    start_byte = 0;
    end_byte = 0;

    buffer_mutex = g_mutex_new ();

    thread = g_thread_create (gtk_audio_mixer_thread,
                              (gpointer) this,
                              TRUE,
                              NULL);

    return;
}

void
GtkAudioMixer::stop (void)
{
    if (thread)
    {
        thread_die = 1;
        g_thread_join (thread);
        thread = NULL;

        g_mutex_free (buffer_mutex);
        buffer_mutex = NULL;
    }

    return;
}

bool
GtkAudioMixer::write (unsigned char *output, int bytes)
{
    while (!thread_die && (bytes_available () < bytes))
    {
        usleep (100);
    }

    if (!thread_die && buffer_mutex)
    {
        g_mutex_lock (buffer_mutex);
        memcpy (output,
                internal_buffer + start_byte,
                MIN (bytes, buffer_size - start_byte));
        if (bytes > buffer_size - start_byte)
        {
            memcpy (output + (buffer_size - start_byte),
                    internal_buffer,
                    bytes - (buffer_size - start_byte));
        }

        start_byte = (start_byte + bytes) % buffer_size;

        g_mutex_unlock (buffer_mutex);

        return true;
    }

    return false;
}

void
GtkAudioMixer::mixer_thread (void)
{
    int            samples_to_mix;
    int            space_needed;
    int            bytes_to_write;
    struct timeval now, next;

    /* Mix at 2ms intervals */
    samples_to_mix = (2 * so.playback_rate) / 1000 << (so.stereo ? 1 : 0);
    space_needed   = samples_to_mix << (so.sixteen_bit ? 1 : 0);

    unsigned char *temp = new unsigned char[space_needed];

    gettimeofday (&now, NULL);
    next.tv_sec = now.tv_sec;
    next.tv_usec = now.tv_usec;

    while (!thread_die)
    {
        while (!thread_die && timercmp (&now, &next, <))
        {
            usleep ((next.tv_sec - now.tv_sec) * 1000000 + (next.tv_usec - now.tv_usec));
            gettimeofday (&now, NULL);
        }

        while (!thread_die && (space_available () < space_needed))
        {
            usleep (100);
        }

        if (thread_die)
            break;

        S9xMixSamples (temp, samples_to_mix);

        g_mutex_lock (buffer_mutex);

        bytes_to_write = MIN (space_needed, buffer_size - end_byte);

        memcpy (internal_buffer + end_byte, temp, bytes_to_write);

        if (space_needed > bytes_to_write)
        {
            memcpy (internal_buffer,
                    temp + bytes_to_write,
                    space_needed - bytes_to_write);
        }

        end_byte = (end_byte + space_needed) % buffer_size;

        g_mutex_unlock (buffer_mutex);

        next.tv_usec += 2000;
        if (next.tv_usec >= 1000000)
        {
            next.tv_sec++;
            next.tv_usec -= 1000000;
        }
    }

    delete[] temp;

    return;
}

bool
GtkAudioMixer::is_ready (void)
{
    int bytes = bytes_available ();

    return (bytes > (buffer_size * 9 / 10));
}

int
GtkAudioMixer::bytes_available (void)
{
    int bytes;

    g_mutex_lock (buffer_mutex);
    bytes = end_byte - start_byte;
    g_mutex_unlock (buffer_mutex);

    while (bytes < 0)
        bytes += buffer_size;

    return bytes;
}

int
GtkAudioMixer::space_available (void)
{
    return buffer_size - bytes_available ();
}
