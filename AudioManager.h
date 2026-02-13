//
// Created by jnetn on 13/02/2026.
//

#ifndef PONG_AUDIOMANAGER_H
#define PONG_AUDIOMANAGER_H

#include <SDL_audio.h>
#include <string>

class AudioManager {
private:
    SDL_AudioSpec wavSpec;
    Uint8* wavBuffer;
    Uint32 wavLength;
    bool initialized;

public:
    AudioManager();
    ~AudioManager();

    bool LoadSound(const std::string& filepath);
    void PlaySound();
    void Cleanup();
};

#endif
