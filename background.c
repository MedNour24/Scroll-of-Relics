#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "background.h"

// Initialize the background structure for split-screen
void initialize_background(background *bg, int level) {
    if (bg == NULL) {
        fprintf(stderr, "Error: Background pointer is NULL\n");
        return;
    }

    // Load the background image based on the level
    const char *bg_path = (level == 1) ? "outils/level1_bg.jpg" : "outils/level2_bg.jpg";
    bg->img_bg = IMG_Load(bg_path);
    if (bg->img_bg == NULL) {
        fprintf(stderr, "Error loading background image: %s\n", SDL_GetError());
        return;
    }

    printf("Background image loaded: %dx%d\n", bg->img_bg->w, bg->img_bg->h);

    // Initialize positions for split-screen (assuming 800x600 screen)
    bg->posback1.x = 0;
    bg->posback1.y = 0;
    bg->posback1.w = bg->img_bg->w < 800 ? bg->img_bg->w : 800;
    bg->posback1.h = bg->img_bg->h < 300 ? bg->img_bg->h : 300; // Top half

    bg->posback2.x = 0;
    bg->posback2.y = 0;
    bg->posback2.w = bg->img_bg->w < 800 ? bg->img_bg->w : 800;
    bg->posback2.h = bg->img_bg->h < 300 ? bg->img_bg->h : 300; // Bottom half

    // Destination positions on screen
    bg->posScreen1.x = 0;
    bg->posScreen1.y = 0;
    bg->posScreen1.w = 800;
    bg->posScreen1.h = 300;

    bg->posScreen2.x = 0;
    bg->posScreen2.y = 300;
    bg->posScreen2.w = 800;
    bg->posScreen2.h = 300;

    // Independent scroll
    bg->scroll.x = 0;
    bg->scroll.y = 0;
    bg->scroll.w = bg->img_bg->w;
    bg->scroll.h = bg->img_bg->h;

    bg->speed = 5;
}

// Initialize a player with animation images
void initialize_player(Player *p, const char *image_path, const char *image_format) {
    if (p == NULL) {
        fprintf(stderr, "Error: Player pointer is NULL\n");
        return;
    }

    p->position.x = 50;
    p->position.y = 50;
    p->score = 0;
    p->lives = 3;
    p->puzzles_solved = 0;
    p->current_frame = 0;

    // Load animation frames (assuming simple naming: 1.jpg, 2.jpg, etc.)
    char filepath[256];
    for (int i = 0; i < 10; i++) {
        snprintf(filepath, sizeof(filepath), "%s/%d.%s", image_path, i + 1, image_format);
        p->animation[i] = IMG_Load(filepath);
        if (p->animation[i] == NULL) {
            fprintf(stderr, "Error loading animation frame %s: %s\n", filepath, SDL_GetError());
            p->animation[i] = IMG_Load("outils/default.jpg"); // Fallback image
        }
    }
}

// Display the background on the screen
void display_background(background bg, SDL_Surface *ecran) {
    if (bg.img_bg == NULL || ecran == NULL) {
        fprintf(stderr, "Error: Invalid surface for displaying background\n");
        return;
    }

    SDL_BlitSurface(bg.img_bg, &(bg.scroll), ecran, NULL);
}

// Display a player on the screen with a specific viewport
void display_player(Player p, SDL_Surface *ecran, SDL_Rect *viewport) {
    if (p.animation[p.current_frame] == NULL || ecran == NULL) {
        fprintf(stderr, "Error: Invalid surface for displaying player\n");
        return;
    }

    SDL_SetClipRect(ecran, viewport);
    SDL_BlitSurface(p.animation[p.current_frame], NULL, ecran, &(p.position));
    SDL_SetClipRect(ecran, NULL);
}

// Handle scrolling in four directions
void scrolling_bg(background *bg, int numBack, int direction, int speed) {
    if (bg == NULL || bg->img_bg == NULL) {
        fprintf(stderr, "Error: Background pointer or image is NULL in scrolling_bg\n");
        return;
    }

    SDL_Rect *target = (numBack == 1) ? &(bg->posback1) : &(bg->posback2);
    int max_scroll_x = bg->img_bg->w - target->w;
    int max_scroll_y = bg->img_bg->h - target->h;
    int min_scroll_x = 0, min_scroll_y = 0;

    switch (direction) {
        case 2: // Right
            target->x += speed;
            break;
        case 1: // Left
            target->x -= speed;
            break;
        case 3: // Up
            target->y -= speed;
            break;
        case 4: // Down
            target->y += speed;
            break;
        default:
            break;
    }

    if (target->x < min_scroll_x) target->x = min_scroll_x;
    if (target->x > max_scroll_x) target->x = max_scroll_x;
    if (target->y < min_scroll_y) target->y = min_scroll_y;
    if (target->y > max_scroll_y) target->y = max_scroll_y;
}

// Update player position based on input
void update_player(Player *p, int keysHeld[], int playerNum, background *bg) {
    int speed = 5;
    int direction = 0;

    if (playerNum == 1) {
        if (keysHeld[SDLK_RIGHT]) { p->position.x += speed; direction = 2; }
        if (keysHeld[SDLK_LEFT]) { p->position.x -= speed; direction = 1; }
        if (keysHeld[SDLK_UP]) { p->position.y -= speed; direction = 3; }
        if (keysHeld[SDLK_DOWN]) { p->position.y += speed; direction = 4; }
    } else {
        if (keysHeld[SDLK_d]) { p->position.x += speed; direction = 2; }
        if (keysHeld[SDLK_a]) { p->position.x -= speed; direction = 1; }
        if (keysHeld[SDLK_w]) { p->position.y -= speed; direction = 3; }
        if (keysHeld[SDLK_s]) { p->position.y += speed; direction = 4; }
    }

    // Update animation frame (simple cycling)
    p->current_frame = (p->current_frame + 1) % 10;

    // Scroll the background based on player movement
    if (direction != 0) {
        scrolling_bg(bg, playerNum, direction, speed);
    }

    // Boundary checks for player position
    if (p->position.x < 0) p->position.x = 0;
    if (p->position.y < 0) p->position.y = 0;
    if (p->position.x > 800 - 50) p->position.x = 800 - 50;
    if (p->position.y > (playerNum == 1 ? 300 : 600) - 50) p->position.y = (playerNum == 1 ? 300 : 600) - 50;
}

// Display split-screen (horizontal split)
void afficher_split_screen(background bg, SDL_Surface *ecran, Player *p1, Player *p2) {
    SDL_Rect viewport1 = {0, 0, 800, 300};    // Top half
    SDL_Rect viewport2 = {0, 300, 800, 300};  // Bottom half

    // Display Player 1's background and player
    SDL_SetClipRect(ecran, &viewport1);
    SDL_BlitSurface(bg.img_bg, &(bg.posback1), ecran, &(bg.posScreen1));
    display_player(*p1, ecran, &viewport1);

    // Display Player 2's background and player
    SDL_SetClipRect(ecran, &viewport2);
    SDL_BlitSurface(bg.img_bg, &(bg.posback2), ecran, &(bg.posScreen2));
    display_player(*p2, ecran, &viewport2);

    SDL_SetClipRect(ecran, NULL);
}

// Display elapsed time
void afficher_temps(SDL_Surface *ecran, TTF_Font *font, Uint32 start_time) {
    Uint32 current_time = SDL_GetTicks();
    int seconds = (current_time - start_time) / 1000;
    char time_str[32];
    snprintf(time_str, sizeof(time_str), "Temps: %d s", seconds);

    SDL_Color color = {255, 255, 255, 0};
    SDL_Surface *text = TTF_RenderText_Solid(font, time_str, color);
    if (!text) {
        fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());
        return;
    }

    SDL_Rect text_pos = {10, 10, 0, 0};
    SDL_BlitSurface(text, NULL, ecran, &text_pos);
    SDL_FreeSurface(text);
}

// Save high score to file
void save_high_score(const char *player_name, int score) {
    FILE *file = fopen("highscores.txt", "a");
    if (file) {
        fprintf(file, "%s %d\n", player_name, score);
        fclose(file);
    }
}

// Display high scores
void display_high_scores(SDL_Surface *ecran, TTF_Font *font) {
    FILE *file = fopen("highscores.txt", "r");
    if (!file) return;

    HighScore scores[5];
    int num_scores = 0;
    char name[32];
    int score;

    while (fscanf(file, "%s %d", name, &score) == 2 && num_scores < 5) {
        strcpy(scores[num_scores].player_name, name);
        scores[num_scores].score = score;
        num_scores++;
    }
    fclose(file);

    SDL_Color color = {255, 255, 255, 0};
    for (int i = 0; i < num_scores; i++) {
        char score_str[64];
        snprintf(score_str, sizeof(score_str), "%d. %s: %d", i + 1, scores[i].player_name, scores[i].score);
        SDL_Surface *text = TTF_RenderText_Solid(font, score_str, color);
        SDL_Rect text_pos = {300, 200 + i * 30, 0, 0};
        SDL_BlitSurface(text, NULL, ecran, &text_pos);
        SDL_FreeSurface(text);
    }
}

// Main function
int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG) {
        fprintf(stderr, "SDL_image initialization failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *ecran = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    if (!ecran) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Initialize background
    background bg;
    initialize_background(&bg, 1); // Start with Level 1
    if (!bg.img_bg) {
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Initialize players
    Player player1, player2;
    initialize_player(&player1, "outils/player1", "jpg");
    initialize_player(&player2, "outils/player2", "jpg");
    player2.position.y = 350; // Start Player 2 in bottom half for split-screen

    // Input handling
    int keysHeld[3232] = {0}; // As per workshop suggestion
    int split_screen = 0;     // Toggle for split-screen mode
    int running = 1;
    SDL_Event event;
    Uint32 start_time = SDL_GetTicks();
    int show_high_scores = 0;
    char player_name[32] = "";

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                keysHeld[event.key.keysym.sym] = 1;
                if (event.key.keysym.sym == SDLK_w) {
                    split_screen = !split_screen; // Toggle split-screen on 'W'
                }
                if (event.key.keysym.sym == SDLK_h) {
                    show_high_scores = !show_high_scores;
                }
            }
            if (event.type == SDL_KEYUP) {
                keysHeld[event.key.keysym.sym] = 0;
            }
        }

        // Update players
        update_player(&player1, keysHeld, 1, &bg);
        if (split_screen) {
            update_player(&player2, keysHeld, 2, &bg);
        }

        // Clear screen
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

        // Display based on mode
        if (split_screen) {
            afficher_split_screen(bg, ecran, &player1, &player2);
        } else {
            display_background(bg, ecran);
            SDL_Rect full_viewport = {0, 0, 800, 600};
            display_player(player1, ecran, &full_viewport);
        }

        // Display additional UI
        afficher_temps(ecran, font, start_time);
        if (show_high_scores) display_high_scores(ecran, font);

        SDL_Flip(ecran);
    }

    // Cleanup
    if (bg.img_bg) SDL_FreeSurface(bg.img_bg);
    for (int i = 0; i < 10; i++) {
        if (player1.animation[i]) SDL_FreeSurface(player1.animation[i]);
        if (player2.animation[i]) SDL_FreeSurface(player2.animation[i]);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
