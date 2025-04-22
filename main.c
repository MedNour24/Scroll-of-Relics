#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    Game game = {0}; // Initialize all fields to 0/NULL
    SDL_Event event;
    int i;
    char filename[50];
    int isWaiting = 0;
    Mix_Music *winSound = NULL;
    Mix_Music *loseSound = NULL;

    // Initialize SDL and SDL_mixer
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        fprintf(stderr, "SDL_mixer initialization failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    // Set up video mode
    game.screen = SDL_SetVideoMode(1280, 720, 32, SDL_SWSURFACE);
    if (!game.screen) {
        fprintf(stderr, "Video mode setup failed: %s\n", SDL_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    // Load images
    game.background = IMG_Load("background.png");
    game.cardBack = IMG_Load("back.png");
    game.cardImages[0] = IMG_Load("card1.png");
    game.cardImages[1] = IMG_Load("card2.png");
    game.cardImages[2] = IMG_Load("card3.png");
    game.winImage = IMG_Load("VICTOIRE.png");
    game.loseImage = IMG_Load("echec.png");

    if (!game.background || !game.cardBack || !game.cardImages[0] ||
        !game.cardImages[1] || !game.cardImages[2] || !game.winImage || !game.loseImage) {
        fprintf(stderr, "Image loading failed: %s\n", IMG_GetError());
        cleanup(&game, winSound, loseSound);
        return 1;
    }

    // Load clock images
    for (i = 0; i < CLOCK_IMAGE_COUNT; i++) {
        sprintf(filename, "clock/clock %d.png", i);
        game.clockImages[i] = IMG_Load(filename);
        if (!game.clockImages[i]) {
            fprintf(stderr, "Failed to load %s: %s\n", filename, IMG_GetError());
            cleanup(&game, winSound, loseSound);
            return 1;
        }
    }

    // Load sounds
    winSound = Mix_LoadMUS("win_sound");
    loseSound = Mix_LoadMUS("lose_sound");
    if (!winSound || !loseSound) {
        fprintf(stderr, "Sound loading failed: %s\n", Mix_GetError());
        cleanup(&game, winSound, loseSound);
        return 1;
    }

    // Initialize game state
    srand(time(NULL));
    shuffleCards(game.cardOrder);
    for (i = 0; i < NUM_CARDS; i++) {
        game.cardPositions[i].x = (i % 3) * (CARD_WIDTH + 100) + 186;
        game.cardPositions[i].y = (i / 3) * (CARD_HEIGHT + 100) + 72;
        game.revealed[i] = 0;
    }
    game.selected[0] = -1;
    game.selected[1] = -1;
    game.pairsFound = 0;
    game.startTime = time(NULL);
    game.clockPos.x = 10;
    game.clockPos.y = 10;
    game.resPos.x = 440;
    game.resPos.y = 94;
    game.running = 1;

    // Main game loop
    while (game.running) {
        time_t currentTime = time(NULL);
        int elapsedTime = (int)(currentTime - game.startTime);

        if (elapsedTime > TIME_LIMIT) {
            SDL_BlitSurface(game.loseImage, NULL, game.screen, &game.resPos);
            SDL_Flip(game.screen);
            Mix_PlayMusic(loseSound, 1);
            SDL_Delay(3000);
            cleanup(&game, winSound, loseSound);
            return 0;
        }

        drawCards(&game);
        drawClock(&game, elapsedTime);
        SDL_Flip(game.screen);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game.running = 0;
            } else if (!isWaiting && event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int cardIndex = getCardIndex(event.button.x, event.button.y, game.cardPositions);
                    if (cardIndex != -1 && !game.revealed[cardIndex]) {
                        if (game.selected[0] == -1) {
                            game.selected[0] = cardIndex;
                        } else if (game.selected[1] == -1 && cardIndex != game.selected[0]) {
                            game.selected[1] = cardIndex;
                            drawCards(&game);
                            drawClock(&game, elapsedTime);
                            SDL_Flip(game.screen);
                            isWaiting = 1;
                            SDL_Delay(500);
                            if (game.cardOrder[game.selected[0]] == game.cardOrder[game.selected[1]]) {
                                game.revealed[game.selected[0]] = 1;
                                game.revealed[game.selected[1]] = 1;
                                game.pairsFound++;
                            }
                            game.selected[0] = -1;
                            game.selected[1] = -1;
                            isWaiting = 0;
                        }
                    }
                }
            }
        }

        if (game.pairsFound == 3) {
            SDL_BlitSurface(game.winImage, NULL, game.screen, &game.resPos);
            SDL_Flip(game.screen);
            Mix_PlayMusic(winSound, 1);
            SDL_Delay(3000);
            cleanup(&game, winSound, loseSound);
            return 0;
        }
    }

    cleanup(&game, winSound, loseSound);
    return 0;
}
