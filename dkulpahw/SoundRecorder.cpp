#include "SoundRecorder.h"
#include <dkulpaclibs/misc/debug/Log.h>

SoundRecorder::SoundRecorder(size_t frameBuffSize, int chNo) {
    this->frameBuffSize= frameBuffSize;
    this->chNo= chNo;
    buffM= PTHREAD_MUTEX_INITIALIZER;
}

int SoundRecorder::getRecording(char *buff, size_t *len) {
    int flag= overflowFlag;

    pthread_mutex_lock(&buffM);
    memcpy(buff, buffer, buffHead);
    *len= buffHead / (snd_pcm_format_width(format)/8);
    buffHead=0;
    overflowFlag=0;
    pthread_mutex_unlock(&buffM);

    return flag;
}

size_t SoundRecorder::getFrameBuffSize() {
    return frameBuffSize;
}


void SoundRecorder::onStart() {
    unsigned int rate = 16000;

    snd_pcm_hw_params_t *hw_params;
    format = SND_PCM_FORMAT_FLOAT_LE;

    //Open audio interface
    if( snd_pcm_open(&capture_handle, "default", SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK) < 0 ) {
        Log::write("SoundRecorder", "Cannot open audio interface");
        this->stop();
        return;
    }

    //Alloc hardware params
    if( snd_pcm_hw_params_malloc (&hw_params) < 0 ) {
        Log::write("SoundRecorder", "Cannot alloc hw params");
        this->stop();
        return;
    }

    //Initialize hardware params obj
    if(snd_pcm_hw_params_any (capture_handle, hw_params) < 0) {
        Log::write("SoundRecorder", "Cannot initialize hw params obj");
        this->stop();
        return;
    }


    if(snd_pcm_hw_params_set_rate_resample(capture_handle, hw_params, 1)<0){
        Log::write("SoundRecorder", "Cannot enable resampling");
        this->stop();
        return;
    }

    snd_pcm_uframes_t frames=2048;
    int dir;
    snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &frames, &dir);

    Log::write("SoundRecorder", "Dir: %d", dir);


    unsigned int re;
    snd_pcm_hw_params_get_rate_resample(capture_handle, hw_params,&re);
    Log::write("SoundRecorder", "Resample state: %d", re);

    //Set access type
        if(snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
            Log::write("SoundRecorder", "Cannot set access type");
            this->stop();
            return;
    }

    //Set format type
    if(snd_pcm_hw_params_set_format (capture_handle, hw_params, format) < 0) {
        Log::write("SoundRecorder", "Cannot set format type");
        this->stop();
        return;
    }

    //Set sample rate
    if(snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0) < 0) {
        Log::write("SoundRecorder", "Cannot set sample rate");
        this->stop();
        return;
    }

    Log::write("SoundRecorder", "Audio sample rate %dHz", rate);

    //Set channel count
    if (snd_pcm_hw_params_set_channels (capture_handle, hw_params, chNo) < 0) {
        Log::write("SoundRecorder", "Cannot set channel count");
        this->stop();
        return;
    }

    //Finally set hardware params
    if (snd_pcm_hw_params (capture_handle, hw_params) < 0) {
        Log::write("SoundRecorder", "Cannot set hw params");
        this->stop();
        return;
    }


    snd_pcm_hw_params_free (hw_params);

    if (snd_pcm_prepare (capture_handle) < 0) {
        Log::write("SoundRecorder", "Cannot prepare audio interface");
    }

    byteBuffSize= frameBuffSize * (snd_pcm_format_width(format)/8) * chNo;
    buffer = (char*)malloc(byteBuffSize);
    buffHead=0;

    Log::write("SoundRecorder", "Format width %d", snd_pcm_format_width(format));
}

void SoundRecorder::onRun() {
    long fr;

    pthread_mutex_lock(&buffM);
    if( (fr=snd_pcm_readi(capture_handle, buffer+buffHead, 128)) > 0) {
        buffHead += fr*(snd_pcm_format_width(format)/8);
    } else {
    }

    if(buffHead>=byteBuffSize) {
        buffHead = 0;
        overflowFlag=1;
    }
    pthread_mutex_unlock(&buffM);
}

void SoundRecorder::onStop() {
    free(buffer);
    snd_pcm_close (capture_handle);
}
