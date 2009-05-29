#ifndef __GTK_SOUND_DRIVER_PORTAUDIO_H
#define __GTK_SOUND_DRIVER_PORTAUDIO_H

#include <portaudio.h>
#include <errno.h>

#include "gtk_sound.h"
#include "gtk_sound_driver.h"
#include "gtk_audio_mixer.h"

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
        void mix (unsigned char *output, int bytes);

    private:
        PaStream *audio_stream;
        GtkAudioMixer *mixer;
};


#endif /* __GTK_SOUND_DRIVER_PORTAUDIO_H */
