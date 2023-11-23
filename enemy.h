//
// Created by j_net on 11/23/2023.
//

#ifndef PONG_ENEMY_H
#define PONG_ENEMY_H

#include "SDL_render.h"

class Enemy{
public:
    int enemyWidth;
    int enemyHeight;
    int enemySpeed;
    int enemyXposition;
    int enemyYposition;
    bool enemyMoveUP;
    bool enemyMoveDOWN;

    Enemy(int x, int y);

    void EnemyRender(SDL_Renderer *renderer, SDL_Window *window);

    void EnemyTick();
};


#endif //PONG_ENEMY_H
