//Note 
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

typedef struct Player Player;

typedef struct {
    char player_name[32];
    int score;
} HighScore;

typedef enum {
    FIXED,
    MOBILE,
    DESTRUCTIBLE
} PlatformType;

typedef struct {
    SDL_Rect position;
    PlatformType type;
    int speed; // For mobile platforms
    int direction; // For mobile platforms (1 for right/down, -1 for left/up)
    int active; // For destructible platforms (0 if destroyed)
} Platform;

typedef struct {
    SDL_Surface *img_bg;
    SDL_Rect posback1;
    SDL_Rect posback2;
    SDL_Rect posScreen1;
    SDL_Rect posScreen2;
    SDL_Rect scroll;
    SDL_Rect camera_pos;
    int speed;
    int direction;
    Platform platforms[10]; // Array of platforms (adjust size as needed)
    int num_platforms;
} background;

void initialize_background(background *bg, int level);
void display_background(background bg, SDL_Surface *ecran);
void scrolling_bg(background *bg, int numBack, int direction, int speed); // Renamed for consistency
void afficher_split_screen(background bg, SDL_Surface *ecran, Player *p1, Player *p2);
void afficher_temps(SDL_Surface *ecran, TTF_Font *font, Uint32 start_time);
void save_high_score(const char *player_name, int score);
void display_high_scores(SDL_Surface *ecran, TTF_Font *font);
void update_platforms(background *bg);
void display_platforms(background bg, SDL_Surface *ecran);
void display_game_guide(SDL_Surface *ecran, TTF_Font *font);

#endif
//.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "background.h"
#include "player.h"

void initialize_background(background *bg, int level) {
    if (bg == NULL) {
        fprintf(stderr, "Error: Background pointer is NULL\n");
        return;
    }
    const char *bg_path = (level == 1) ? "outils/level1_bg.jpg" : "outils/level2_bg.jpg";
    bg->img_bg = IMG_Load(bg_path);
    if (bg->img_bg == NULL) {
        fprintf(stderr, "Error loading background image: %s\n", SDL_GetError());
        return;
    }
    bg->posback1.x = 0; bg->posback1.y = 0; bg->posback1.w = bg->img_bg->w < 400 ? bg->img_bg->w : 400; bg->posback1.h = bg->img_bg->h < 600 ? bg->img_bg->h : 600;
    bg->posback2.x = 0; bg->posback2.y = 0; bg->posback2.w = bg->img_bg->w < 400 ? bg->img_bg->w : 400; bg->posback2.h = bg->img_bg->h < 600 ? bg->img_bg->h : 600;
    bg->posScreen1.x = 0; bg->posScreen1.y = 0; bg->posScreen1.w = 400; bg->posScreen1.h = 600;
    bg->posScreen2.x = 400; bg->posScreen2.y = 0; bg->posScreen2.w = 400; bg->posScreen2.h = 600;
    bg->scroll.x = 0; bg->scroll.y = 0; bg->scroll.w = bg->img_bg->w; bg->scroll.h = bg->img_bg->h;
    bg->speed = 5;

    // Initialize platforms
    bg->num_platforms = 3;
    // Fixed platform
    bg->platforms[0].position = (SDL_Rect){100, 500, 100, 20};
    bg->platforms[0].type = FIXED;
    bg->platforms[0].active = 1;
    // Mobile platform
    bg->platforms[1].position = (SDL_Rect){200, 400, 80, 20};
    bg->platforms[1].type = MOBILE;
    bg->platforms[1].speed = 2;
    bg->platforms[1].direction = 1;
    bg->platforms[1].active = 1;
    // Destructible platform
    bg->platforms[2].position = (SDL_Rect){300, 300, 60, 20};
    bg->platforms[2].type = DESTRUCTIBLE;
    bg->platforms[2].active = 1;
}

void display_background(background bg, SDL_Surface *ecran) {
    if (bg.img_bg == NULL || ecran == NULL) {
        fprintf(stderr, "Error: Invalid surface for displaying background\n");
        return;
    }
    SDL_BlitSurface(bg.img_bg, &(bg.scroll), ecran, NULL);
    display_platforms(bg, ecran);
}

void scrolling_bg(background *bg, int numBack, int direction, int speed) {
    if (bg == NULL || bg->img_bg == NULL) return;
    SDL_Rect *target = (numBack == 1) ? &(bg->posback1) : &(bg->posback2);
    int max_scroll_x = bg->img_bg->w - target->w;
    int max_scroll_y = bg->img_bg->h - target->h;
    int min_scroll_x = 0, min_scroll_y = 0;

    switch (direction) {
        case 2: target->x += speed; break; // Right
        case 1: target->x -= speed; break; // Left
        case 3: target->y -= speed; break; // Up
        case 4: target->y += speed; break; // Down
    }
    if (target->x < min_scroll_x) target->x = min_scroll_x;
    if (target->x > max_scroll_x) target->x = max_scroll_x;
    if (target->y < min_scroll_y) target->y = min_scroll_y;
    if (target->y > max_scroll_y) target->y = max_scroll_y;
}

void update_platforms(background *bg) {
    for (int i = 0; i < bg->num_platforms; i++) {
        if (bg->platforms[i].type == MOBILE && bg->platforms[i].active) {
            if (bg->platforms[i].direction == 1) { // Moving right
                bg->platforms[i].position.x += bg->platforms[i].speed;
                if (bg->platforms[i].position.x > 700) bg->platforms[i].direction = -1;
            } else { // Moving left
                bg->platforms[i].position.x -= bg->platforms[i].speed;
                if (bg->platforms[i].position.x < 0) bg->platforms[i].direction = 1;
            }
        }
    }
}

void display_platforms(background bg, SDL_Surface *ecran) {
    for (int i = 0; i < bg.num_platforms; i++) {
        if (bg.platforms[i].active) {
            SDL_Rect pos = bg.platforms[i].position;
            Uint32 color;
            switch (bg.platforms[i].type) {
                case FIXED: color = SDL_MapRGB(ecran->format, 0, 255, 0); break; // Green
                case MOBILE: color = SDL_MapRGB(ecran->format, 0, 0, 255); break; // Blue
                case DESTRUCTIBLE: color = SDL_MapRGB(ecran->format, 255, 0, 0); break; // Red
            }
            SDL_FillRect(ecran, &pos, color);
        }
    }
}

void afficher_split_screen(background bg, SDL_Surface *ecran, Player *p1, Player *p2) {
    SDL_Rect viewport1 = {0, 0, 400, 600};
    SDL_Rect viewport2 = {400, 0, 400, 600};
    SDL_SetClipRect(ecran, &viewport1);
    SDL_BlitSurface(bg.img_bg, &(bg.posback1), ecran, &(bg.posScreen1));
    display_player(*p1, ecran);
    SDL_SetClipRect(ecran, &viewport2);
    SDL_BlitSurface(bg.img_bg, &(bg.posback2), ecran, &(bg.posScreen2));
    display_player(*p2, ecran);
    SDL_SetClipRect(ecran, NULL);
}

void afficher_temps(SDL_Surface *ecran, TTF_Font *font, Uint32 start_time) {
    Uint32 current_time = SDL_GetTicks();
    int seconds = (current_time - start_time) / 1000;
    char time_str[32];
    snprintf(time_str, sizeof(time_str), "Temps: %d s", seconds);
    SDL_Color color = {255, 255, 255, 0};
    SDL_Surface *text = TTF_RenderText_Solid(font, time_str, color);
    if (!text) return;
    SDL_Rect text_pos = {10, 10, 0, 0};
    SDL_BlitSurface(text, NULL, ecran, &text_pos);
    SDL_FreeSurface(text);
}

void save_high_score(const char *player_name, int score) {
    FILE *file = fopen("highscores.txt", "a");
    if (file) {
        fprintf(file, "%s %d\n", player_name, score);
        fclose(file);
    }
}

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

void display_game_guide(SDL_Surface *ecran, TTF_Font *font) {
    SDL_Color color = {255, 255, 255, 0};
    const char *guide_lines[] = {
        "Game Guide:",
        "W: Toggle split-screen",
        "H: Show high scores",
        "G: Show this guide",
        "Arrow keys: Move player"
    };
    for (int i = 0; i < 5; i++) {
        SDL_Surface *text = TTF_RenderText_Solid(font, guide_lines[i], color);
        if (text) {
            SDL_Rect text_pos = {200, 150 + i * 30, 0, 0};
            SDL_BlitSurface(text, NULL, ecran, &text_pos);
            SDL_FreeSurface(text);
        }
    }
}
