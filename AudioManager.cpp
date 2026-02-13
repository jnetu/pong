//
// Created by jnetn on 13/02/2026.
//
#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager()
    : wavBuffer(nullptr), wavLength(0), initialized(false) {}

AudioManager::~AudioManager() {
    Cleanup();
}

bool AudioManager::LoadSound(const std::string& filepath) {
    if (SDL_LoadWAV(filepath.c_str(), &wavSpec, &wavBuffer, &wavLength) == nullptr) {
        std::cerr << "Failed to load WAV: " << SDL_GetError() << std::endl;
        return false;
    }

    if (SDL_OpenAudio(&wavSpec, nullptr) < 0) {
        std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
        SDL_FreeWAV(wavBuffer);
        return false;
    }

    initialized = true;
    return true;
}

void AudioManager::PlaySound() {
    if (initialized && wavBuffer) {
        SDL_ClearQueuedAudio(1);
        SDL_QueueAudio(1, wavBuffer, wavLength);
        SDL_PauseAudio(0);
    }
}

void AudioManager::Cleanup() {
    if (wavBuffer) {
        SDL_FreeWAV(wavBuffer);
        wavBuffer = nullptr;
    }
    if (initialized) {
        SDL_CloseAudio();
        initialized = false;
    }
}
