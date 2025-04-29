#ifndef ENIGME_H_INCLUDED
#define ENIGME_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 1030
#define MAX_QUESTIONS 10
#define NB_REPONSES 3
#define MAX_VIES 3
#define TEMPS_PAR_QUESTION 30

typedef struct {
    // Game data
    char questions[MAX_QUESTIONS][256];
    char reponses[MAX_QUESTIONS][NB_REPONSES][256];
    int correct_answers[MAX_QUESTIONS];
    int questions_used[MAX_QUESTIONS];
    int nb_questions;
    
    // Game state
    int score;
    int vies;
    int level;
    int temps_restant;
    int questions_answered;
    int num_question;
    int pos_selected;
    int hover_state[NB_REPONSES];
   
    
    // Timing
    Uint32 temps_debut;
    
    // Graphics
    SDL_Surface *background;
    SDL_Surface *button;
    SDL_Surface *button_s;
    SDL_Surface *timer_bg;
    SDL_Surface *timer_bar;
    SDL_Surface *question;
    SDL_Surface *reponses_surface[NB_REPONSES];
    SDL_Surface *vies_surface[MAX_VIES];
    SDL_Surface *score_surface;
    SDL_Surface *level_surface;
    
    // Positions
    SDL_Rect pos_timer;
    SDL_Rect pos_question;
    SDL_Rect pos_reponse1;
    SDL_Rect pos_reponse2;
    SDL_Rect pos_reponse3;
    SDL_Rect pos_reponse1txt;
    SDL_Rect pos_reponse2txt;
    SDL_Rect pos_reponse3txt;
    SDL_Rect pos_score;
    SDL_Rect pos_level;
    SDL_Rect pos_vies[MAX_VIES];
    
    // Audio
    Mix_Chunk *sound;
    Mix_Chunk *correct_sound;
    Mix_Chunk *wrong_sound;
    Mix_Chunk *hover_sound;
    Mix_Chunk *timeout_sound;
} enigme;

void load_questions(enigme *e, const char *q_file, const char *a_file, const char *c_file);
void initialiser_enigme(enigme *e);
void generer_enigme(enigme *e, int question_index);
void afficher_enigme(enigme *e, SDL_Surface *ecran, int souris_x, int souris_y);
int verify_enigme(enigme *e, SDL_Surface *ecran);
void mettre_a_jour_timer(enigme *e);
void afficher_resultat(SDL_Surface *ecran, int correct, enigme *e);
void free_enigme(enigme *e);
int afficher_ecran_accueil(SDL_Surface *ecran);
void afficher_game_over(SDL_Surface *ecran);
void afficher_victoire(SDL_Surface *ecran);


#endif


