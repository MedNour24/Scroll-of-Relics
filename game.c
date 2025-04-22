#include "game.h"
#include <stdlib.h>
#include <stdio.h>

void shuffleCards(int cardOrder[]) {
    int indices[] = {0, 0, 1, 1, 2, 2};
    int i, j, temp;

    for (i = 0; i < NUM_CARDS; i++) {
        j = rand() % NUM_CARDS;
        temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    for (i = 0; i < NUM_CARDS; i++) {
        cardOrder[i] = indices[i];
    }
}

void drawCards(Game *game) {
    int i;
    SDL_Surface *img;
    SDL_BlitSurface(game->background, NULL, game->screen, NULL);
    for (i = 0; i < NUM_CARDS; i++) {
        if (game->revealed[i] || i == game->selected[0] || i == game->selected[1])
            img = game->cardImages[game->cardOrder[i]];
        else
            img = game->cardBack;
        SDL_BlitSurface(img, NULL, game->screen, &game->cardPositions[i]);
    }
}

int getCardIndex(int x, int y, SDL_Rect cardPositions[]) {
    int i;
    for (i = 0; i < NUM_CARDS; i++) {
        if (x >= cardPositions[i].x && x <= cardPositions[i].x + CARD_WIDTH &&
            y >= cardPositions[i].y && y <= cardPositions[i].y + CARD_HEIGHT)
            return i;
    }
    return -1;
}

void drawClock(Game *game, int elapsedTime) {
    int remaining = TIME_LIMIT - elapsedTime;
    if (remaining < 0) remaining = 0;
    int index = (remaining * (CLOCK_IMAGE_COUNT - 1)) / TIME_LIMIT;
    if (index < 0 || index >= CLOCK_IMAGE_COUNT) {
        fprintf(stderr, "Clock index error: %d\n", index);
        return;
    }
    SDL_BlitSurface(game->clockImages[index], NULL, game->screen, &game->clockPos);
}

void cleanup(Game *game, Mix_Music *winSound, Mix_Music *loseSound) {
    int i;
    if (game->background) SDL_FreeSurface(game->background);
    if (game->cardBack) SDL_FreeSurface(game->cardBack);
    if (game->winImage) SDL_FreeSurface(game->winImage);
    if (game->loseImage) SDL_FreeSurface(game->loseImage);
    for (i = 0; i < 3; i++) {
        if (game->cardImages[i]) SDL_FreeSurface(game->cardImages[i]);
    }
    for (i = 0; i < CLOCK_IMAGE_COUNT; i++) {
        if (game->clockImages[i]) SDL_FreeSurface(game->clockImages[i]);
    }
    if (winSound) Mix_FreeMusic(winSound);
    if (loseSound) Mix_FreeMusic(loseSound);
    Mix_CloseAudio();
    SDL_Quit();
}
