#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#define CARD_WIDTH 236
#define CARD_HEIGHT 236
#define NUM_CARDS 6
#define TIME_LIMIT 30
#define CLOCK_IMAGE_COUNT 12

typedef struct {
    SDL_Surface *screen;
    SDL_Surface *background;
    SDL_Surface *cardBack;
    SDL_Surface *cardImages[3];
    SDL_Surface *winImage;
    SDL_Surface *loseImage;
    SDL_Surface *clockImages[CLOCK_IMAGE_COUNT];
    SDL_Rect cardPositions[NUM_CARDS];
    SDL_Rect clockPos;
    SDL_Rect resPos;
    int cardOrder[NUM_CARDS];
    int revealed[NUM_CARDS];
    int selected[2];
    int pairsFound;
    int running;
    time_t startTime;
} Game;

void shuffleCards(int cardOrder[]);
void drawCards(Game *game);
int getCardIndex(int x, int y, SDL_Rect cardPositions[]);
void drawClock(Game *game, int elapsedTime);
void cleanup(Game *game, Mix_Music *winSound, Mix_Music *loseSound);

#endif
