#ifndef __GTK_SOUND_DRIVER_OSS_H
#define __GTK_SOUND_DRIVER_OSS_H

#include "gtk_sound.h"
#include "gtk_sound_driver.h"

#define OSSTHREAD_OK 0
#define OSSTHREAD_DIE 1

class S9xOSSSoundDriver : public S9xSoundDriver
{
    public:
        S9xOSSSoundDriver (void);
        void init (void);
        void terminate (void);
        bool8 open_device (int mode, bool8 stereo, int buffer_size);
        void start (void);
        void stop (void);
        void mix (void);

    private:
        void mixer_thread (void);
        int filedes;
        uint8 *sound_buffer;
        GThread *thread;
        int thread_die;

    friend gpointer oss_thread (gpointer data);
};



#endif /* __GTK_SOUND_DRIVER_OSS_H */
