#include <ma/miniaudio.h>
#include "holder.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
}

void initAudioDevice(ma_device* pDevice, ma_decoder* pDecoder){
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = outputFormat;
    deviceConfig.playback.channels = outputChannels;
    deviceConfig.sampleRate = outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = pDecoder;

    if (ma_device_init(NULL, &deviceConfig, pDevice) != MA_SUCCESS){
        printf("failed to initialize miniaudio device\n");
        exit(0);
    }

    ma_device_start(pDevice);
}
