#ifndef PONG_PLAYER_H
#define PONG_PLAYER_H

#include "SDL_render.h"

class Player {
public:

    int playerWidth;
    int playerHeight;
    int playerSpeed;
    float  playerXposition;
    float  playerYposition;
    bool playerMoveUP;
    bool playerMoveDOWN;

    Player(int x, int y);

    void Move(int direction);

    void StopMove(int direction);

    void PlayerRender(SDL_Renderer *renderer, SDL_Window *window);

    void PlayerTick(float deltaTime);
};

#endif //PONG_PLAYER_H