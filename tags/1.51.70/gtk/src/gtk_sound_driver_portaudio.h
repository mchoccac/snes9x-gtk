#ifndef __GTK_SOUND_DRIVER_PORTAUDIO_H
#define __GTK_SOUND_DRIVER_PORTAUDIO_H

#include <portaudio.h>
#include <errno.h>

#include "gtk_sound.h"
#include "gtk_sound_driver.h"

class S9xPortAudioSoundDriver : public S9xSoundDriver
{
    public:
        S9xPortAudioSoundDriver (void);
        void init (void);
        void terminate (void);
        bool8 open_device (int mode, bool8 stereo, int buffer_size);
        void start (void);
        void stop (void);
        void mix (void);

    private:
        PaStream *audio_stream;
};


#endif /* __GTK_SOUND_DRIVER_PORTAUDIO_H */
