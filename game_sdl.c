#include "game_sdl.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* **************************************************************** */

struct Env_t {
  /* PUT YOUR VARIABLES HERE */
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) { 
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  float left_sidex;
  float right_sidex;
  float up_sidey;
  float down_sidey;
  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE); /* red */

  //game screen
  left_sidex=w/20;
  right_sidex=w-w/5;
  up_sidey=h/8;
  down_sidey=h-h/8;
  
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, left_sidex, down_sidey);
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, right_sidex, down_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, left_sidex, down_sidey, right_sidex, down_sidey);

  //message screen
  left_sidex=w/4;
  right_sidex=w-w/4;
  up_sidey=h/50;
  down_sidey=h/10;
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, left_sidex, down_sidey);
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, right_sidex, down_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, left_sidex, down_sidey, right_sidex, down_sidey);

  //current and solve
  left_sidex=w-w/7;
  right_sidex=w-w/20;

  up_sidey=h/1.9;
  down_sidey=h/1.6;
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, left_sidex, down_sidey);
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, right_sidex, down_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, left_sidex, down_sidey, right_sidex, down_sidey);

  up_sidey=h-h/1.9;
  down_sidey=h-h/1.6;
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, left_sidex, down_sidey);
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, right_sidex, down_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, left_sidex, down_sidey, right_sidex, down_sidey);

  //undo/redo
  float up_sidey_square=h-h/12.5;
  float down_sidey_square=h-h/25;
  up_sidey=h-h/16.5;


  left_sidex=w/1.7;
  right_sidex=w/1.4;
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, left_sidex, down_sidey_square, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, left_sidex, up_sidey_square, right_sidex, up_sidey);

  left_sidex=w-w/1.7;
  right_sidex=w-w/1.4;
  SDL_RenderDrawLine(ren, right_sidex, up_sidey, left_sidex, up_sidey);
  SDL_RenderDrawLine(ren, right_sidex, up_sidey, left_sidex, down_sidey_square);
  SDL_RenderDrawLine(ren, right_sidex, up_sidey, left_sidex, up_sidey_square);

  //help
  left_sidex=w-w/10;
  right_sidex=w-w/25;

  up_sidey=h/10;
  down_sidey=h/25;
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, left_sidex, down_sidey);
  SDL_RenderDrawLine(ren, left_sidex, up_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, right_sidex, down_sidey, right_sidex, up_sidey);
  SDL_RenderDrawLine(ren, left_sidex, down_sidey, right_sidex, down_sidey);
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return true;
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  free(env);
}

/* **************************************************************** */
