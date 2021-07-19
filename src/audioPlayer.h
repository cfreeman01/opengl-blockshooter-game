#pragma once
#include "miniaudio.h"
#include <iostream>

class audioPlayer {
private:
    ma_result result;
    ma_decoder decoder;
    ma_decoder_config decoderConfig;
    ma_device_config deviceConfig;
    ma_device device;

    //callback which writes data to the device
    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        if (pDecoder == NULL) {
            return;
        }

        ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

        (void)pInput;
    }
public:
    audioPlayer() {
        //configure the device
        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.format = decoder.outputFormat;
        deviceConfig.playback.channels = decoder.outputChannels;
        deviceConfig.sampleRate = decoder.outputSampleRate;
        deviceConfig.dataCallback = data_callback;
        deviceConfig.pUserData = &decoder;
        //set encoding format as mp3
        decoderConfig.encodingFormat = ma_encoding_format_mp3;
        //initialize the device and check for errors
        if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
            std::cout << "failed to initialize audio player";
            ma_decoder_uninit(&decoder);
            return;
        }
    }

    //plays audio from specified file
    void play(const char* filepath) {
            result = ma_decoder_init_file(filepath, NULL, &decoder);
            if (result != MA_SUCCESS) {
                std::cout << "could not load audio file " << filepath << std::endl;
                return;
            }
            ma_device_start(&device);
    }
};