//
// Created by j_net on 9/28/2023.
//

#ifndef PONG_PLAYER_H
#define PONG_PLAYER_H

#include "SDL_render.h"

class Player {
public:

    int playerWidth;
    int playerHeight;
    int playerSpeed;
    int playerXposition;
    int playerYposition;
    bool playerMoveUP;
    bool playerMoveDOWN;

    Player(int x, int y);

    void Move(int direction);

    void StopMove(int direction);

    void PlayerRender(SDL_Renderer *renderer, SDL_Window *window);

    void PlayerTick();
};

#endif //PONG_PLAYER_H
