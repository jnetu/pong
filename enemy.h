#ifndef PONG_ENEMY_H
#define PONG_ENEMY_H

#include "SDL_render.h"
#include "Ball.h"

class Enemy{
public:
    int enemyWidth;
    int enemyHeight;
    int enemySpeed;
    float enemyXposition;
    float enemyYposition;
    bool enemyMoveUP;
    bool enemyMoveDOWN;
    Ball& referenceBall;

    Enemy(int x, int y, Ball& referenceBall);
    void EnemyRender(SDL_Renderer *renderer, SDL_Window *window);
    void EnemyTick(const Ball& ball, float deltaTime); // const ref + deltaTime
};

#endif
