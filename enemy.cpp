#include "enemy.h"
#include <random>
Enemy::Enemy(int x, int y, Ball referenceBall) : referenceBall(referenceBall) {
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
    //drawEnemy
    //set color white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rE = {enemyXposition,enemyYposition,enemyWidth,enemyHeight};
    SDL_RenderFillRect(renderer, &rE);
    //endDrawEnemy
}
void Enemy::EnemyTick(Ball ball) {
    //follow the ball in vertical position
    // - needs a balls reference


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.7, 0.9);


    i++;
    if(i > 100){
        randomSpeed = dis(gen);
        printf("%f", randomSpeed);
        i = 0;
    }


    // Gere o número aleatório
    float randomNum = dis(gen);
    if (enemyYposition + enemyHeight / 2 <= ball.ballY) {
        enemyYposition += enemySpeed * randomSpeed;
    }
    if (enemyYposition + enemyHeight / 2 > ball.ballY) {
        enemyYposition -= enemySpeed * randomSpeed;
    }

    if(enemyYposition < 0){
        enemyYposition = 0;
    }
    if(enemyYposition + enemyHeight > 512){
        enemyYposition = 512 - enemyHeight;

    }
}