/**#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

// Structure for high scores
typedef struct {
    char player_name[32];  // Player name
    int score;             // Player score
} HighScore;

// Main background structure
typedef struct {
    SDL_Surface *img_bg;    // Background image surface
    SDL_Rect position_bg;   // Position on screen (destination)
    SDL_Rect scroll;        // Source rectangle for scrolling
    int speed;              // Scrolling speed
} background;

// Function prototypes
void initialiser_background(background *bg, int level);
void afficher_background(background bg, SDL_Surface *ecran);
void scrolling_bg(int clic, background *bg);
void afficher_split_screen(background bg, SDL_Surface *ecran);
void afficher_temps(SDL_Surface *ecran, TTF_Font *font, Uint32 start_time);
void afficher_guide(SDL_Surface *ecran, SDL_Surface *guide_img, int show_guide);
void save_high_score(const char *player_name, int score);
void display_high_scores(SDL_Surface *ecran, TTF_Font *font);

#endif#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

// Structure for high scores
typedef struct {
    char player_name[32];  // Player name
    int score;             // Player score
} HighScore;

// Main background structure
typedef struct {
    SDL_Surface *img_bg;    // Background image surface
    SDL_Rect position_bg;   // Position on screen (destination)
    SDL_Rect scroll;        // Source rectangle for scrolling
    int speed;              // Scrolling speed
} background;

// Function prototypes
void initialiser_background(background *bg, int level);
void afficher_background(background bg, SDL_Surface *ecran);
void scrolling_bg(int clic, background *bg);
void afficher_split_screen(background bg, SDL_Surface *ecran);
void afficher_temps(SDL_Surface *ecran, TTF_Font *font, Uint32 start_time);
void afficher_guide(SDL_Surface *ecran, SDL_Surface *guide_img, int show_guide);
void save_high_score(const char *player_name, int score);
void display_high_scores(SDL_Surface *ecran, TTF_Font *font);

#endif*/
/*#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

// Structure for high scores
typedef struct {
    char player_name[32];  // Player name
    int score;             // Player score
} HighScore;

// Main background structure
typedef struct {
    SDL_Surface *img_bg;    // Background image surface
    SDL_Rect position_bg;   // Position on screen (destination)
    SDL_Rect scroll;        // Source rectangle for scrolling
    int speed;              // Scrolling speed
} background;

// Function prototypes
void initialiser_background(background *bg, int level);
void afficher_background(background bg, SDL_Surface *ecran);
void scrolling_bg(int clic, background *bg);
void afficher_split_screen(background bg, SDL_Surface *ecran);
void afficher_temps(SDL_Surface *ecran, TTF_Font *font, Uint32 start_time);
void afficher_guide(SDL_Surface *ecran, SDL_Surface *guide_img, int show_guide);
void save_high_score(const char *player_name, int score);
void display_high_scores(SDL_Surface *ecran, TTF_Font *font);

#endif*/
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

// Structure for high scores
typedef struct {
    char player_name[32];  // Player name
    int score;             // Player score
} HighScore;

// Player structure
typedef struct {
    SDL_Surface *animation[10]; // Array for animation frames
    SDL_Rect position;          // Player position on screen
    int score;                  // Player score
    int lives;                  // Number of lives
    int puzzles_solved;         // Number of puzzles solved
    int current_frame;          // Current animation frame
} Player;

// Main background structure for split-screen
typedef struct {
    SDL_Surface *img_bg;    // Background image surface
    SDL_Rect posback1;      // Source rectangle for player 1's background
    SDL_Rect posback2;      // Source rectangle for player 2's background
    SDL_Rect posScreen1;    // Destination rectangle for player 1's screen
    SDL_Rect posScreen2;    // Destination rectangle for player 2's screen
    SDL_Rect scroll;        // Independent scroll for dynamic updates
    int speed;              // Scrolling speed
} background;

// Function prototypes
void initialize_background(background *bg, int level);
void initialize_player(Player *p, const char *image_path, const char *image_format);
void display_background(background bg, SDL_Surface *ecran);
void display_player(Player p, SDL_Surface *ecran, SDL_Rect *viewport);
void scrolling_bg(background *bg, int numBack, int direction, int speed);
void update_player(Player *p, int keysHeld[], int playerNum, background *bg);
void afficher_split_screen(background bg, SDL_Surface *ecran, Player *p1, Player *p2);
void afficher_temps(SDL_Surface *ecran, TTF_Font *font, Uint32 start_time);
void save_high_score(const char *player_name, int score);
void display_high_scores(SDL_Surface *ecran, TTF_Font *font);

#endif
