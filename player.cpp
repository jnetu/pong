#include "player.h"


//1 ups
//2 down
void Player::Move(int direction) {
    switch (direction) {
        case 1:
            playerMoveUP = true;
            break;
        case 2:
            playerMoveDOWN = true;
        default:
            break;
    }
}

//1 ups
//2 down
void Player::StopMove(int direction) {
    switch (direction) {
        case 1:
            playerMoveUP = false;
            break;
        case 2:
            playerMoveDOWN = false;
        default:
            break;
    }
}

Player::Player(int x, int y) {
    playerSpeed = 16;
    playerXposition = x;
    playerYposition = y;
    playerWidth = 32;
    playerHeight = 128;
    playerMoveUP = false;
    playerMoveDOWN = false;
}

void Player::PlayerRender(SDL_Renderer *renderer, SDL_Window *window) {
    //draw player
    //set color white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rP = {playerXposition, playerYposition, playerWidth, playerHeight};
    SDL_RenderFillRect(renderer, &rP);
    //end draw Player
}

void Player::PlayerTick() {
    //move logic
    if (playerMoveUP && playerYposition > 0) {
        playerYposition = playerYposition - playerSpeed;
    } else if (playerMoveDOWN && playerYposition < 512 - playerHeight) {
        playerYposition = playerYposition + playerSpeed;
    }

}