#define SDL_MAIN_HANDLED true
#include <SDL.h>

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
            if (e.type == SDL_QUIT){
                return 0;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        SDL_Rect r = {0,0,50,50};
        SDL_SetRenderDrawColor(renderer,200,200,200, 255);
        SDL_RenderFillRect(renderer, &r);



        SDL_Rect r2 = {0,
                       0,
                       SDL_GetWindowSurface(window)->w,
                       SDL_GetWindowSurface(window)->h/2
        };
        SDL_SetRenderDrawColor(renderer,200,200,200, 255);

        SDL_RenderFillRect(renderer, &r2);

        SDL_RenderPresent(renderer);
        SDL_Delay(0);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
