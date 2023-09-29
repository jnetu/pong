#define SDL_MAIN_HANDLED true
#include <SDL.h>
#include <chrono>
#include <iostream>
#include "player.h"

using namespace std::chrono_literals;
int initialPlayerPositionX = 64;
int initialPlayerPositionY = 64 + 128;

int ballLarge = 16;
int ballTall = 16;
int initialBallPositionX = 320 - (ballLarge/2);
int initialBallPositionY = 256 - (ballTall/2);
int ballSpeed = 8;
bool ballDirection = false; //false -> balls coming2u
int ballX = initialBallPositionX;
int ballY = initialBallPositionY;

Player player(initialPlayerPositionX,initialPlayerPositionY);
bool reset = false;

bool touchTheBall();

void ballsLogicTick();

void renderTick(SDL_Renderer *renderer, SDL_Window *window) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    /*SDL_Rect r = {0,0,100,100};
    SDL_RenderFillRect(renderer, &r);*/
    //draw graphic lines
    int actualWidth = SDL_GetWindowSurface(window)->w;
    int actualHeight = SDL_GetWindowSurface(window)->h;
    SDL_RenderDrawLine(renderer,0,0,actualWidth,0); //up
    SDL_RenderDrawLine(renderer,0,actualHeight-1,actualWidth,actualHeight -1); //down
    SDL_RenderDrawLine(renderer,0,0,0,actualHeight);//left
    SDL_RenderDrawLine(renderer,actualWidth-1,0,actualWidth-1,actualHeight);//right
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderDrawLine(renderer,0,0,actualWidth,actualHeight);
    SDL_RenderDrawLine(renderer,0,actualHeight,actualWidth,0);
    for (int i = 0; i < actualWidth; ++i) {
        if(i % 64 == 0){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawLine(renderer,i,0,i,10);
            SDL_RenderDrawLine(renderer,i,actualHeight,i,actualHeight-10);
        }
    }
    for (int i = 0; i < actualHeight; ++i) {
        if(i % 64 == 0){
            SDL_RenderDrawLine(renderer,0,i,10,i);
            SDL_RenderDrawLine(renderer,actualWidth,i,actualWidth-10,i);
        }
    }
    //end draw graphic lines

    //drawPlayer
    player.PlayerRender(renderer, window);
    //endDrawPLayer

    //drawEnemy
    //set color white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rE = {640 - 64 - 32,64 + 128,32,128};
    SDL_RenderFillRect(renderer, &rE);
    //endDrawEnemy

    //Draw ball
    SDL_Rect ball = {ballX,ballY,ballLarge,ballTall};
    SDL_RenderFillRect(renderer, &ball);
    //end Draw ball
}

void logicTick() {
    if(reset){
        reset = false;
        player.playerXposition = initialPlayerPositionX;
        player.playerYposition = initialPlayerPositionY;

    }
    //playerLogic
    player.PlayerTick();
    //if ball intersection player -> bounds
    if(touchTheBall()){
        ballDirection = !ballDirection;
    }
    ballsLogicTick();

}

void ballsLogicTick() {
    if(ballDirection){
        ballX = ballX + ballSpeed;
    }else{
        ballX = ballX - ballSpeed;
    }
}

//playerTouchBall
bool touchTheBall() {
    int x1 = player.playerXposition;
    int y1 = player.playerYposition;
    int w1 = player.playerWidth;
    int h1 = player.playerHeight;
    int x2 = ballX;
    int y2 = ballY;
    int w2 = ballLarge;
    int h2 = ballTall;
    if ((x1 + w1) >= x2 && ((y1+h1) >= y2 && y2 > y1)) { //see the png to not forget
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);

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
                        reset = true;
                        ballDirection = !ballDirection;
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


        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


