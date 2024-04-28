#ifndef PONG_ENEMY_H
#define PONG_ENEMY_H

#include "SDL_render.h"
#include "Ball.h"

class Enemy{
public:
    int enemyWidth;
    int enemyHeight;
    int enemySpeed;
    int enemyXposition;
    int enemyYposition;
    bool enemyMoveUP;
    bool enemyMoveDOWN;
    Ball referenceBall;

    Enemy(int x, int y, Ball referenceBall);

    void EnemyRender(SDL_Renderer *renderer, SDL_Window *window);

    void EnemyTick(Ball ball);
};


#endif //PONG_ENEMY_H