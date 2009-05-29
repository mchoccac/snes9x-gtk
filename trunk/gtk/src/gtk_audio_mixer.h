#ifndef __GTK_AUDIO_MIXER_H
#define __GTK_AUDIO_MIXER_H

class GtkAudioMixer
{
    public:
        GtkAudioMixer (int buffer_size);
        ~GtkAudioMixer (void);
        void start (void);
        void stop (void);
        bool write (unsigned char *output, int bytes);
        bool is_ready (void);

    private:
        void mixer_thread (void);
        int space_available (void);
        int bytes_available (void);
        unsigned char *internal_buffer;
        int thread_die;
        int start_byte;
        int end_byte;
        int buffer_size;
        GMutex *buffer_mutex;
        GThread *thread;

    friend gpointer gtk_audio_mixer_thread (gpointer data);
};


#endif /* __GTK_AUDIO_MIXER_H */
