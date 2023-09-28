#define SDL_MAIN_HANDLED true
#include <SDL.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
struct tile{
    int x;
    int y;
};
class Tile{
    int x;
    int y;
public:
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
};
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
    for (int i = 0; i < actualWidth; ++i) {
        if(i % 64 == 0){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawLine(renderer,i,0,i,10);
            SDL_RenderDrawLine(renderer,i,actualHeight,i,actualHeight-10);
        }
    }
    for (int i = 0; i < SDL_GetWindowSurface(window)->h; ++i) {
        if(i % 64 == 0){
            SDL_RenderDrawLine(renderer,0,i,10,i);
            SDL_RenderDrawLine(renderer,actualWidth,i,actualWidth-10,i);
        }
    }
    //end draw graphic lines

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect r = {64,64,32,32};
    SDL_RenderFillRect(renderer, &r);

}

void logicTick() {

}

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
            "pong",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
    for (;;){

        while (SDL_PollEvent(&e)){
            switch(e.type ){
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_LEFT){
                        printf( "press left\n" );
                    }
                    if(e.key.keysym.sym == SDLK_RIGHT){
                        printf( "press right\n" );
                    }
                    if(e.key.keysym.sym == SDLK_UP){
                        printf( "press up\n" );
                    }
                    if(e.key.keysym.sym == SDLK_DOWN){
                        printf( "press down\n" );
                    }
                    break;

                case SDL_KEYUP:
                    if(e.key.keysym.sym == SDLK_LEFT){
                        printf( "release left\n" );
                    }
                    if(e.key.keysym.sym == SDLK_RIGHT){
                        printf( "release right\n" );
                    }
                    if(e.key.keysym.sym == SDLK_UP){
                        printf( "release up\n" );
                    }
                    if(e.key.keysym.sym == SDLK_DOWN){
                        printf( "release down\n" );
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
        SDL_Delay(0);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


