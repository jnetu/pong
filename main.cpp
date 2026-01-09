#define SDL_MAIN_HANDLED true
#include <SDL.h>
#include <SDL_audio.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "player.h"
#include "enemy.h"
#include "Ball.h"
using namespace std::chrono_literals;
int initialPlayerPositionX = 64;
int initialPlayerPositionY = 64 + 128;
int initialEnemyPositionX = 640 - 64 - 32;
int initialEnemyPositionY = 64 + 128;

int ballLarge = 16;
int ballTall = 16;
int initialBallPositionX = 320 - (ballLarge/2);
int initialBallPositionY = 256 - (ballTall/2);
int ballSpeedX = 8;
int ballSpeedY = 8;
bool ballDirection = false; //false -> balls coming2u
int ballX = initialBallPositionX;
int ballY = initialBallPositionY;

int point = 0;
int enemyPoint = 0;

int loaded = false;




Ball ball(initialBallPositionX,initialBallPositionY);
Player player(initialPlayerPositionX,initialPlayerPositionY);
Enemy enemy(initialEnemyPositionX, initialEnemyPositionY, ball);

enum class state { start, gameover, playing, reset };
std::string STATE = "start";
bool reset = false;

bool touchTheBall();
bool enemyTouchTheBall();
void ballsLogicTick();
int playSound();


Uint32 fpsLastTime = 0;
int fpsFrames = 0;

void printFPS() {
    fpsFrames++;
    // Se passou 1 segundo (1000 ms) desde a última contagem
    if (SDL_GetTicks() - fpsLastTime >= 1000) {
        printf("FPS: %d\n", fpsFrames); // Printa no console
        fpsFrames = 0;
        fpsLastTime = SDL_GetTicks();
    }
}
void renderTick(SDL_Renderer *renderer, SDL_Window *window) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    //drawPlayer
    player.PlayerRender(renderer, window);
    //endDrawPLayer

    //drawEnemy
    enemy.EnemyRender(renderer,window);
    //endDrawEnemy

    //Draw ball
    ball.BallRender(renderer, window);
    //end Draw ball
}

void logicTick() {
    if(STATE == "start"){

        player.PlayerTick();
        enemy.enemyXposition = initialEnemyPositionX;
        enemy.enemyYposition = initialEnemyPositionY;
        ball.ballX = initialBallPositionX;
        ball.ballY = initialBallPositionY;
    }

    if(STATE == "playing"){
        player.PlayerTick();

        //enemyLogic
        enemy.EnemyTick(ball);

        //ballLogic
        ballsLogicTick();
    }

}

std::string getCurrentWorkingDirectory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return std::string(cwd);
    } else {
        return std::string("");
    }
}

int playSound(){

    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8 *wav_buffer;

    std::string currentDir = getCurrentWorkingDirectory();
    std::string audioFilePath = currentDir + "/hit.wav";

    if (SDL_LoadWAV(audioFilePath.c_str(), &wav_spec, &wav_buffer, &wav_length) == NULL) {
        fprintf(stderr, "Could not open hit.wav: %s\n", SDL_GetError());
        return 1;
    }
    if(!loaded){

        if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
            fprintf(stderr, "SDL_OpenAudio failed: %s\n", SDL_GetError());
            SDL_FreeWAV(wav_buffer);
            return 1;
        }
    }
    loaded = true;
    SDL_QueueAudio(1, wav_buffer, wav_length);
    SDL_PauseAudio(0);


    // Liberar o buffer após a reprodução
    SDL_FreeWAV(wav_buffer);

    return 0;
}

//playerTouchBall
bool touchTheBall() {
    int x1 = player.playerXposition;
    int y1 = player.playerYposition;
    int w1 = player.playerWidth;
    int h1 = player.playerHeight;
    int x2 = ball.ballX;
    int y2 = ball.ballY;
    int w2 = ball.ballWidth;
    int h2 = ball.ballHeight;
    if(x2 > x1 && x2 < x1 + w1){
        if (y2 > y1 && y2 < y1 + h1) {
            return true;
        }
    }
    return false;
}


//enemyTouchBall
bool enemyTouchTheBall() {
    int x1 = enemy.enemyXposition;
    int y1 = enemy.enemyYposition;
    int w1 = enemy.enemyWidth;
    int h1 = enemy.enemyHeight;
    int x2 = ball.ballX;
    int y2 = ball.ballY;
    int w2 = ball.ballWidth;
    int h2 = ball.ballHeight;

    if(x2 > x1-w2 && x2 < x1 + w1){
        if (y2 > y1 && y2 < y1 + h1) {
            return true;
        }
    }
    return false;
}

void ballsLogicTick() {
    //if ball intersection player -> bounds
    if(touchTheBall()){
        ball.ballSpeedX = -ball.ballSpeedX;
        playSound();
    }
    if(enemyTouchTheBall()){
        ball.ballSpeedX = -ball.ballSpeedX;
        playSound();
    }
    //kick edges
    if(ball.ballY > 512 - ball.ballHeight  || ball.ballY < 0){
        ball.ballSpeedY = -ball.ballSpeedY;
    }
    if(ball.ballX > 640 - ball.ballWidth || ball.ballX < 0){
        ball.ballSpeedX = -ball.ballSpeedX;
    }

    ball.ballX = ball.ballX + ball.ballSpeedX;
    ball.ballY = ball.ballY + ball.ballSpeedY;

    //enemy point
    if(ball.ballX < 32){

        enemyPoint++;
        STATE = "start";
        printf("points: %d  | enemy points: %d\n",point,enemyPoint);
    }
    //player point
    if(ball.ballX > 640 - 64){
        point++;
        STATE = "start";
        printf("points: %d  | enemy points: %d\n",point,enemyPoint);
    }
}


int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);


SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    SDL_Window* window = SDL_CreateWindow(
            "pong",
            0,
            50,
            640,
            512,
            SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
fpsLastTime = SDL_GetTicks();
    for (;;){
        while (SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w){
                        player.Move(1);
                    }
                    if(e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s){
                        player.Move(2);
                    }
                    if(e.key.keysym.sym == SDLK_SPACE){
                        if(STATE == "start"){
                            STATE = "playing";
                        }else if(STATE == "playing"){
                            STATE = "start";
                        }

                    }
                    if(e.key.keysym.sym == SDLK_TAB){
                        ballDirection = !ballDirection;
                    }
                    break;
                case SDL_KEYUP:
                    if(e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w){
                        player.StopMove(1);
                    }
                    if(e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s){
                        player.StopMove(2);
                    }
                    break;
                case SDL_QUIT:
                    return 0;
                default:
                    break;
            }


        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        logicTick();
        renderTick(renderer, window);
printFPS();

        SDL_RenderPresent(renderer);
        //SDL_Delay(1000/70);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

