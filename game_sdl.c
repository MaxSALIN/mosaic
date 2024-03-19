#include "game_sdl.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

/* **************************************************************** */

struct Env_t {
  float game_screen_left_sidex,game_screen_right_sidex,game_screen_up_sidex,game_screen_down_sidex;
  float message_screen_left_sidex,message_screen_right_sidex,message_screen_up_sidex,message_screen_down_sidex;
  float current_left_sidex,current_right_sidex,current_up_sidex,current_down_sidex;
  float current_solve_left_sidex,current_solve_right_sidex,current_solve_up_sidex,current_solve_down_sidex;
  float up_sidey_square,down_sidey_square;
  float undo_redo_up_sidey,undo_redo_left_sidex,undo_redo_right_sidex;
  float help_left_sidex,help_right_sidex,help_up_sidey,help_down_sidey;
  game g;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */
  env->g=game_default();
  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) { 
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE); /* red */

  env->game_screen_left_sidex=w/20;
  env->game_screen_right_sidex=w-w/5;
  env->game_screen_up_sidex=h/8;
  env->game_screen_down_sidex=h-h/8;

  env->message_screen_left_sidex=w/4;
  env->message_screen_right_sidex=w-w/4;
  env->message_screen_up_sidex=h/50;
  env->message_screen_down_sidex=h/10;

  env->current_solve_left_sidex=w-w/7;
  env->current_solve_right_sidex=w-w/20;
  env->current_solve_up_sidex=h/1.85;
  env->current_solve_down_sidex=h/2.15;

  env->up_sidey_square=h-h/12.5;
  env->down_sidey_square=h-h/25;
  env->undo_redo_up_sidey=h-h/16.5;
  env->undo_redo_left_sidex=w/1.7;
  env->undo_redo_right_sidex=w/1.4;

  env->help_left_sidex=w-w/10;
  env->help_right_sidex=w-w/25;
  env->help_up_sidey=h/10;
  env->help_down_sidey=h/25;

  //game screen 
  SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_up_sidex, env->game_screen_left_sidex, env->game_screen_down_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_up_sidex, env->game_screen_right_sidex, env->game_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_right_sidex, env->game_screen_down_sidex, env->game_screen_right_sidex, env->game_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_down_sidex, env->game_screen_right_sidex, env->game_screen_down_sidex);

  //message screen
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_up_sidex, env->message_screen_left_sidex, env->message_screen_down_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_up_sidex, env->message_screen_right_sidex, env->message_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_right_sidex, env->message_screen_down_sidex, env->message_screen_right_sidex, env->message_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_down_sidex, env->message_screen_right_sidex, env->message_screen_down_sidex);

  //current and solve
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_up_sidex, env->current_solve_left_sidex, env->current_solve_down_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_up_sidex, env->current_solve_right_sidex, env->current_solve_up_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_right_sidex, env->current_solve_down_sidex, env->current_solve_right_sidex, env->current_solve_up_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_down_sidex, env->current_solve_right_sidex, env->current_solve_down_sidex);

  env->current_solve_up_sidex=h-h/1.9;
  env->current_solve_down_sidex=h-h/1.6;
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_up_sidex, env->current_solve_left_sidex, env->current_solve_down_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_up_sidex, env->current_solve_right_sidex, env->current_solve_up_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_right_sidex, env->current_solve_down_sidex, env->current_solve_right_sidex, env->current_solve_up_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_down_sidex, env->current_solve_right_sidex, env->current_solve_down_sidex);

  //undo/redo
  SDL_RenderDrawLine(ren, env->undo_redo_left_sidexx, env->undo_redo_up_sidey, env->undo_redo_right_sidex, env->undo_redo_up_sidey);
  SDL_RenderDrawLine(ren, env->undo_redo_left_sidex, env->down_sidey_square, env->undo_redo_right_sidex, env->undo_redo_up_sidey);
  SDL_RenderDrawLine(ren, env->undo_redo_left_sidex, env->up_sidey_square, env->undo_redo_right_sidex, env->undo_redo_up_sidey);

  env->undo_redo_left_sidex=w-w/1.7;
  env->undo_redo_right_sidex=w-w/1.4;
  SDL_RenderDrawLine(ren, env->undo_redo_right_sidex, env->undo_redo_up_sidey, env->undo_redo_left_sidex, env->undo_redo_up_sidey);
  SDL_RenderDrawLine(ren, env->undo_redo_right_sidex, env->undo_redo_up_sidey, env->undo_redo_left_sidex, env->down_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_redo_right_sidex, env->undo_redo_up_sidey, env->undo_redo_left_sidex, env->up_sidey_square);

  //help
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_up_sidey, env->help_left_sidex, env->help_down_sidey);
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_up_sidey, env->help_right_sidex, env->help_up_sidey);
  SDL_RenderDrawLine(ren, env->help_right_sidex, env->help_down_sidey, env->help_right_sidex, env->help_up_sidey);
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_down_sidey, env->help_right_sidex, env->help_down_sidey);
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }
  /* Android events */
#ifdef __ANDROID__
  else if (e->type == SDL_FINGERDOWN) {
    //env->mario_x = e->tfinger.x * w; /* tfinger.x, normalized in [0..1] */
    //env->mario_y = e->tfinger.y * h; /* tfinger.y, normalized in [0..1] */
  }
  /* other events */
#else
  else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
  } else if (e->type == SDL_KEYDOWN) {
    
    switch (e->key.keysym.sym) {
      case SDLK_LEFT:
        game_undo(env->g);
        game_print(env->g);
        break;
      case SDLK_RIGHT:
        game_redo(env->g);
        game_print(env->g);
        break;
    }
  }
#endif

  return false; /* don't quit */
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  free(env);
}

/* **************************************************************** */
