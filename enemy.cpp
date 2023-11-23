//
// Created by j_net on 11/23/2023.
//

#include "enemy.h"

Enemy::Enemy(int x, int y) {
    enemySpeed = 16;
    enemyXposition = x;
    enemyYposition = y;
    enemyWidth = 32;
    enemyHeight = 128;
    enemyMoveUP = false;
    enemyMoveDOWN = false;
}

void Enemy::EnemyRender(SDL_Renderer *renderer, SDL_Window *window) {
    //drawEnemy
    //set color white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rE = {640 - 64 - 32,64 + 128,32,128};
    SDL_RenderFillRect(renderer, &rE);
    //endDrawEnemy
}
void Enemy::EnemyTick() {
    //follow the ball in vertical position
    // - needs a balls reference

}
