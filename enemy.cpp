#include "enemy.h"
#include <random>

static std::mt19937 rng(std::random_device{}());
static std::uniform_real_distribution<float> dis(0.7, 0.9);

Enemy::Enemy(int x, int y, Ball& referenceBall) : referenceBall(referenceBall) {
    enemySpeed = 8;
    enemyXposition = x;
    enemyYposition = y;
    enemyWidth = 32;
    enemyHeight = 128;
    enemyMoveUP = false;
    enemyMoveDOWN = false;
}
int i = 0;
float randomSpeed = 0.8f;
void Enemy::EnemyRender(SDL_Renderer *renderer, SDL_Window *window) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rE = {(int)enemyXposition, (int)enemyYposition, enemyWidth, enemyHeight};
    SDL_RenderFillRect(renderer, &rE);
}

void Enemy::EnemyTick(const Ball& ball, float deltaTime) {
    i++;
    if(i > 100){
        randomSpeed = dis(rng);
        i = 0;
    }

    if (enemyYposition + enemyHeight / 2 <= ball.ballY) {
        enemyYposition += enemySpeed * randomSpeed * 60 * deltaTime;
    }
    if (enemyYposition + enemyHeight / 2 > ball.ballY) {
        enemyYposition -= enemySpeed * randomSpeed * 60 * deltaTime;
    }

    if(enemyYposition < 0){
        enemyYposition = 0;
    }
    if(enemyYposition + enemyHeight > 512){
        enemyYposition = 512 - enemyHeight;
    }
}
