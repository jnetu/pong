//
// Created by jnetn on 13/02/2026.
//

#ifndef PONG_CONSTANTS_H
#define PONG_CONSTANTS_H

namespace Constants {
    // Screen
    constexpr int SCREEN_WIDTH = 640;
    constexpr int SCREEN_HEIGHT = 512;
    constexpr int TARGET_FPS = 60;
    constexpr float FIXED_TIMESTEP = 1.0f / TARGET_FPS;

    // Player
    constexpr int PLAYER_WIDTH = 32;
    constexpr int PLAYER_HEIGHT = 128;
    constexpr int PLAYER_SPEED = 400; // pixels per second
    constexpr int PLAYER_START_X = 64;
    constexpr int PLAYER_START_Y = 192;

    // Enemy
    constexpr int ENEMY_WIDTH = 32;
    constexpr int ENEMY_HEIGHT = 128;
    constexpr int ENEMY_SPEED = 320;
    constexpr int ENEMY_START_X = SCREEN_WIDTH - 96;
    constexpr int ENEMY_START_Y = 192;

    // Ball
    constexpr int BALL_SIZE = 16;
    constexpr int BALL_SPEED = 300;
    constexpr int BALL_START_X = (SCREEN_WIDTH - BALL_SIZE) / 2;
    constexpr int BALL_START_Y = (SCREEN_HEIGHT - BALL_SIZE) / 2;
}

enum class GameState {
    START,
    PLAYING,
    PAUSED
};

#endif
