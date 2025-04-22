#include "enigme.h"
#include <stdio.h>

int main() {
    printf("Starting game...\n");
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer initialization failed: %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Set video mode
    SDL_Surface *ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!ecran) {
        printf("Failed to set video mode: %s\n", SDL_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_WM_SetCaption("Quiz Game", NULL);

    // Load background music
    printf("Loading background music...\n");
    Mix_Music *bg_music = Mix_LoadMUS("sonc.mp3");
    if (bg_music) {
        Mix_PlayMusic(bg_music, -1); // Loop indefinitely
    } else {
        printf("Warning: Failed to load background music: %s\n", Mix_GetError());
    }

    // Initialize game structure
    enigme e;
    initialiser_enigme(&e);

    // Show main menu
    int choice = afficher_ecran_accueil(ecran);
    if (choice != 1) { // If user didn't choose quiz, quit
        if (bg_music) Mix_FreeMusic(bg_music);
        free_enigme(&e);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Main game loop
    int running = 1;
    int mouse_x = 0, mouse_y = 0;
    
    while (running && e.vies > 0 && e.level <= 10) {
        // Select a random unused question
        int q_index;
        int attempts = 0;
        do {
            q_index = rand() % e.nb_questions;
            attempts++;
            if (attempts > e.nb_questions * 2) {
                printf("Warning: Couldn't find unused question after %d attempts\n", attempts);
                break;
            }
        } while (e.questions_used[q_index] && e.questions_answered < e.nb_questions);
        
        if (e.questions_answered >= e.nb_questions) {
            printf("All questions have been used!\n");
            break;
        }
        
        e.questions_used[q_index] = 1;
        e.questions_answered++;
        
        // Generate the selected question
        generer_enigme(&e, q_index);
        
        // Question display loop
        int answered = 0;
        while (!answered && e.vies > 0 && running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = 0;
                    answered = 1;
                }
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = 0;
                        answered = 1;
                    }
                }
                else if (event.type == SDL_MOUSEMOTION) {
                    mouse_x = event.motion.x;
                    mouse_y = event.motion.y;
                }
                else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                    // Check which answer was clicked
                    if (e.button && 
                        mouse_x >= e.pos_reponse1.x && mouse_x <= e.pos_reponse1.x + e.button->w &&
                        mouse_y >= e.pos_reponse1.y && mouse_y <= e.pos_reponse1.y + e.button->h) {
                        e.pos_selected = 1;
                        if (e.sound) Mix_PlayChannel(-1, e.sound, 0);
                        int correct = verify_enigme(&e, ecran);
                        afficher_resultat(ecran, correct, &e);
                        if (correct) e.level++;
                        answered = 1;
                    }
                    else if (e.button && 
                             mouse_x >= e.pos_reponse2.x && mouse_x <= e.pos_reponse2.x + e.button->w &&
                             mouse_y >= e.pos_reponse2.y && mouse_y <= e.pos_reponse2.y + e.button->h) {
                        e.pos_selected = 2;
                        if (e.sound) Mix_PlayChannel(-1, e.sound, 0);
                        int correct = verify_enigme(&e, ecran);
                        afficher_resultat(ecran, correct, &e);
                        if (correct) e.level++;
                        answered = 1;
                    }
                    else if (e.button && 
                             mouse_x >= e.pos_reponse3.x && mouse_x <= e.pos_reponse3.x + e.button->w &&
                             mouse_y >= e.pos_reponse3.y && mouse_y <= e.pos_reponse3.y + e.button->h) {
                        e.pos_selected = 3;
                        if (e.sound) Mix_PlayChannel(-1, e.sound, 0);
                        int correct = verify_enigme(&e, ecran);
                        afficher_resultat(ecran, correct, &e);
                        if (correct) e.level++;
                        answered = 1;
                    }
                }
            }
            
            // Update timer
            mettre_a_jour_timer(&e);
            if (e.temps_restant <= 0) {
                e.vies--;
                afficher_resultat(ecran, 0, &e);
                answered = 1;
            }
            
            // Display the current question
            afficher_enigme(&e, ecran, mouse_x, mouse_y);
            SDL_Delay(16); // Cap at ~60fps
        }
    }
    
    // Game over screens
    if (e.vies <= 0) {
        afficher_game_over(ecran);
    } 
    else if (e.level > 5) {
        afficher_victoire(ecran);
    }
    
    // Clean up resources
    printf("Cleaning up resources...\n");
    if (bg_music) {
        Mix_HaltMusic();
        Mix_FreeMusic(bg_music);
    }
    free_enigme(&e);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    
    printf("Game exited successfully\n");
    return 0;
}
