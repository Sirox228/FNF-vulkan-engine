#ifndef AUDIO_H
#define AUDIO_H

#include <ma/miniaudio.h>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
void initAudioDevice(ma_device* pDevice, ma_decoder* pDecoder);

#endif