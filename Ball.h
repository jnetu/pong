#ifndef UNTITLED_BALL_H
#define UNTITLED_BALL_H

#include "SDL_render.h"

class Ball {
public:
    int ballWidth;
    int ballHeight;
    int ballSpeedX;
    int ballSpeedY;
    int ballX;
    int ballY;

    Ball(int x, int y);
    void BallRender(SDL_Renderer *renderer, SDL_Window *window);
    void BallTick();
};


#endif //UNTITLED_BALL_H
