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

  // grid game
  float game_width = right_sidex - left_sidex;
  float game_height = down_sidey - up_sidey;

  int window_width = (game_width) + 1;
  int window_height = (game_height) + 1;

  int cell_required = 9;

  int grid_cell_size = game_height / cell_required;

  //printf("cell_size: %d", grid_cell_size);

  // Dark theme.
  SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
  SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
   //SDL_Color grid_cursor_color = {255, 255, 255, 255}; // White

    // Draw grid background.
    /*SDL_SetRenderDrawColor(ren, grid_background.r, grid_background.g,
                          grid_background.b, grid_background.a);  
    SDL_RenderClear(ren);

    // Draw grid lines.
    SDL_SetRenderDrawColor(ren, grid_line_color.r, grid_line_color.g,
                          grid_line_color.b, grid_line_color.a);*/

  for (int x = left_sidex; x <= game_width; x += grid_cell_size) {
      
       SDL_RenderDrawLine(ren, x, up_sidey, x, game_height);
  }

  for (int y = up_sidey; y <= game_height; y += grid_cell_size) {
    SDL_RenderDrawLine(ren, left_sidex, y, game_width, y);
       
  }

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
  up_sidey=h/1.85;
  down_sidey=h/2.15;
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
