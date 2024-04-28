#include "Ball.h"

Ball::Ball(int x, int y){
    ballX = x;
    ballY = y;
    ballSpeedX = 8;
    ballSpeedY = 8;
    ballHeight = 16;
    ballWidth = 16;
}

void Ball::BallRender(SDL_Renderer *renderer, SDL_Window *window){
    SDL_Rect ball = {ballX,ballY,ballWidth,ballHeight};
    SDL_RenderFillRect(renderer, &ball);
}



void Ball::BallTick() {



}
