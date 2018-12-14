#ifndef KOPYTKO_SOUNDRECORDER_H
#define KOPYTKO_SOUNDRECORDER_H

#include <dkulpaclibs/misc/Thread.h>
#include <iostream>
#include <alsa/asoundlib.h>
#include <fstream>

using namespace std;

class SoundRecorder : public Thread {
public:

    SoundRecorder(size_t frameBuffSize, int chNo);
    int getRecording(char* buff, size_t *len);
    size_t getFrameBuffSize();

protected:
    void onStart() override;
    void onRun() override;
    void onStop() override;

private:
    int chNo;               //Number of channels
    char *buffer;           //Byte array
    snd_pcm_format_t format;

    size_t buffHead;        //Byte pointer
    size_t byteBuffSize;    //Bytes in buffer
    size_t frameBuffSize;   //Frames in buffer

    int overflowFlag;       //Buffer overflow flag (1 - overflow occurred)

    snd_pcm_t *capture_handle;

    pthread_mutex_t buffM;
};


#endif //KOPYTKO_SOUNDRECORDER_H
