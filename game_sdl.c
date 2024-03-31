#include "game_sdl.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_perso.h"
#include "game_struct.h"
#include "game_tools.h"

// cell textures
#define UNCONSTRAINT "res/-1.png"
#define BLACKTEXTURE "res/Black.png"
#define WHITETEXTURE "res/White.png"
#define EMPTYTEXTURE "res/Empty.png"

// environment textures
#define TITLE "res/Mosaic.png"
#define BACKGROUND "res/Background.png"
#define VICTORY "res/VictoryScreen.png"
#define SOLVETEXTE "res/SolveTexte.png"
#define CURRENTTEXTE "res/CurrentTexte.png"

// button textures
#define SOLVE "res/Solve.png"
#define UNDO "res/undo.png"
#define REDO "res/redo.png"
#define CURRENTWHITE "res/CurrentWhite.png"
#define CURRENTBLACK "res/CurrentBlack.png"
#define CURRENTEMPTY "res/CurrentEmpty.png"

#define TOTALNUMBER 9

/* **************************************************************** */

struct Env_t {
  // declares the rectangle sides
  float game_screen_left_sidex, game_screen_right_sidex, game_screen_up_sidey, game_screen_down_sidey;
  float title_screen_left_sidex, title_screen_right_sidex, title_screen_up_sidex, title_screen_down_sidex;
  float current_left_sidex, current_right_sidex, current_up_sidex, current_down_sidex;
  float current_solve_left_sidex, current_solve_right_sidex, solve_up_sidey, solve_down_sidey;
  float undo_redo_up_sidey, undo_redo_down_sidey, undo_undo_redo_up_sidey, undo_undo_redo_down_sidey;
  float redo_left_sidex, redo_right_sidex, undo_right_sidex, undo_left_sidex;
  float help_left_sidex, help_right_sidex, help_up_sidey, help_down_sidey;

  // declares the textures
  SDL_Texture** green_numbers;
  SDL_Texture** blue_numbers;
  SDL_Texture** red_numbers;
  SDL_Texture* unconstraitTexture;
  SDL_Texture* blackTexture;
  SDL_Texture* whiteTexture;
  SDL_Texture* emptyTexture;
  SDL_Texture* titleTexture;
  SDL_Texture* backgroundTexture;
  SDL_Texture* victoryScreen;
  SDL_Texture* currentColorWhiteTexture;
  SDL_Texture* currentColorBlackTexture;
  SDL_Texture* currentColorEmptyTexture;
  SDL_Texture* currentColorTexteTexture;
  SDL_Texture** undo_redoTextures;
  SDL_Texture* solveTexteTexture;
  SDL_Texture* solveTexture;

  // declares the rectangles
  SDL_Rect titleRect;
  SDL_Rect backgroundRect;
  SDL_Rect currentColorRect;
  SDL_Rect currentColorTexte;
  SDL_Rect* cells;
  SDL_Rect* undo_redoRects;
  SDL_Rect solve;
  SDL_Rect SolveTexteRect;

  // declares the rest
  game g;
  color playColor;

  bool hasWon;
};

/* **************************************************************** */

Env* init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]) {
  Env* env = malloc(sizeof(struct Env_t));

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // initialisation of the numbers
  env->green_numbers = malloc(sizeof(SDL_Texture*) * TOTALNUMBER + 1);
  if (env->green_numbers == NULL) {
    ERROR("malloc green numbers");
  }
  env->blue_numbers = malloc(sizeof(SDL_Texture*) * TOTALNUMBER + 1);
  if (env->blue_numbers == NULL) {
    ERROR("malloc blue numbers");
  }
  env->red_numbers = malloc(sizeof(SDL_Texture*) * TOTALNUMBER + 1);
  if (env->red_numbers == NULL) {
    ERROR("malloc red numbers");
  }

  // initialisation of the images of the numbers
  // for green numbers
  for (int i = 0; i <= TOTALNUMBER; i++) {
    char filename[50];

    sprintf(filename, "res/Green/%dv.png", i);

    env->green_numbers[i] = IMG_LoadTexture(ren, filename);
    if (!env->green_numbers[i]) ERROR("IMG_LoadTexture: %s\n", filename);
  }
  // for blue numbers
  for (int i = 0; i <= TOTALNUMBER; i++) {
    char filename[50];

    sprintf(filename, "res/Blue/%db.png", i);

    env->blue_numbers[i] = IMG_LoadTexture(ren, filename);
    if (!env->blue_numbers[i]) ERROR("IMG_LoadTexture: %s\n", filename);
  }
  // for red numbers
  for (int i = 0; i <= TOTALNUMBER; i++) {
    char filename[50];

    sprintf(filename, "res/Red/%dr.png", i);

    env->red_numbers[i] = IMG_LoadTexture(ren, filename);
    if (!env->red_numbers[i]) ERROR("IMG_LoadTexture: %s\n", filename);
  }

  // load the game
  if (argc == 2) {
    env->g = game_load(argv[1]);
    /*if(env->g==NULL){
      fprintf(stderr, "failed to load game");
      exit(EXIT_FAILURE);
    }*/
  } else {
    env->g = game_default();
  }

  // allocate the memory for some rectangles or textures
  env->cells = malloc(sizeof(SDL_Rect) * game_nb_rows(env->g) * game_nb_cols(env->g));
  if (env->cells == NULL) {
    ERROR("malloc cells");
  }
  env->undo_redoRects = malloc(sizeof(SDL_Rect) * 2);
  if (env->undo_redoRects == NULL) {
    ERROR("malloc undo_redo");
  }

  env->undo_redoTextures = malloc(sizeof(SDL_Texture*) * 2);
  if (env->undo_redoTextures == NULL) {
    ERROR("malloc undo_redo");
  }

  // initialise all of the textures
  env->unconstraitTexture = IMG_LoadTexture(ren, UNCONSTRAINT);
  if (!env->unconstraitTexture) ERROR("IMG_LoadTexture: %s\n", UNCONSTRAINT);

  env->blackTexture = IMG_LoadTexture(ren, BLACKTEXTURE);
  if (!env->blackTexture) ERROR("IMG_LoadTexture: %s\n", BLACKTEXTURE);

  env->whiteTexture = IMG_LoadTexture(ren, WHITETEXTURE);
  if (!env->whiteTexture) ERROR("IMG_LoadTexture: %s\n", WHITETEXTURE);

  env->emptyTexture = IMG_LoadTexture(ren, EMPTYTEXTURE);
  if (!env->emptyTexture) ERROR("IMG_LoadTexture: %s\n", EMPTYTEXTURE);

  env->titleTexture = IMG_LoadTexture(ren, TITLE);
  if (!env->titleTexture) ERROR("IMG_LoadTexture: %s\n", TITLE);

  env->backgroundTexture = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->backgroundTexture) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  env->victoryScreen = IMG_LoadTexture(ren, VICTORY);
  if (!env->victoryScreen) ERROR("IMG_LoadTexture: %s\n", VICTORY);

  env->solveTexture = IMG_LoadTexture(ren, SOLVE);
  if (!env->solveTexture) ERROR("IMG_LoadTexture: %s\n", SOLVE);

  env->currentColorTexteTexture = IMG_LoadTexture(ren, CURRENTTEXTE);
  if (!env->currentColorTexteTexture) ERROR("IMG_LoadTexture: %s\n", CURRENTTEXTE);

  env->solveTexteTexture = IMG_LoadTexture(ren, SOLVETEXTE);
  if (!env->solveTexteTexture) ERROR("IMG_LoadTexture: %s\n", SOLVETEXTE);

  env->undo_redoTextures[0] = IMG_LoadTexture(ren, UNDO);
  if (!env->undo_redoTextures[0]) ERROR("IMG_LoadTexture: %s\n", UNDO);

  env->undo_redoTextures[1] = IMG_LoadTexture(ren, REDO);
  if (!env->undo_redoTextures[1]) ERROR("IMG_LoadTexture: %s\n", REDO);

  env->currentColorWhiteTexture = IMG_LoadTexture(ren, CURRENTWHITE);
  if (!env->currentColorWhiteTexture) ERROR("IMG_LoadTexture: %s\n", CURRENTWHITE);

  env->currentColorBlackTexture = IMG_LoadTexture(ren, CURRENTBLACK);
  if (!env->currentColorBlackTexture) ERROR("IMG_LoadTexture: %s\n", CURRENTBLACK);

  env->currentColorEmptyTexture = IMG_LoadTexture(ren, CURRENTEMPTY);
  if (!env->currentColorEmptyTexture) ERROR("IMG_LoadTexture: %s\n", CURRENTEMPTY);

  // sets up the first playable color
  env->playColor = WHITE;

  return env;
}

/* **************************************************************** */

void render(SDL_Window* win, SDL_Renderer* ren, Env* env) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // renders the background
  env->backgroundRect.x = 0;
  env->backgroundRect.y = 0;
  env->backgroundRect.w = w;
  env->backgroundRect.h = h;
  SDL_RenderCopy(ren, env->backgroundTexture, NULL, &(env->backgroundRect));

  if (!game_won(env->g)) {
    env->hasWon = false;

    SDL_RenderCopy(ren, env->backgroundTexture, NULL, &(env->backgroundRect));
  }

  // size of the possible playable area

  env->game_screen_left_sidex = w / 20;
  env->game_screen_right_sidex = w - w / 5;
  env->game_screen_up_sidey = h / 8;
  env->game_screen_down_sidey = h - h / 8;

  // creates the offset and the cell sizes

  float game_width = env->game_screen_right_sidex - env->game_screen_left_sidex;
  float game_height = env->game_screen_down_sidey - env->game_screen_up_sidey;

  float cell_size = 0;
  float offsetx_count = 0;
  float offsety_count = 0;
  float cell_size1 = game_width / game_nb_cols(env->g);
  float cell_size2 = game_height / game_nb_rows(env->g);
  if (game_width >= cell_size2 * game_nb_cols(env->g)) {
    cell_size = cell_size2;
    offsetx_count = (game_width - game_nb_cols(env->g) * cell_size) / 2;
    offsety_count = 0;
  } else if (game_height > cell_size1 * game_nb_rows(env->g)) {
    cell_size = cell_size1;
    offsetx_count = 0;
    offsety_count = (game_height - game_nb_rows(env->g) * cell_size) / 2;
  }

  // renders the title

  // set up dimensions of the rectangle
  env->title_screen_left_sidex = env->game_screen_left_sidex + offsetx_count;
  env->title_screen_right_sidex = env->game_screen_left_sidex + game_width - offsetx_count;
  env->title_screen_up_sidex = h / 50;
  env->title_screen_down_sidex = h / 10;

  env->titleRect.x = env->title_screen_left_sidex;
  env->titleRect.y = env->title_screen_up_sidex;
  env->titleRect.w = env->title_screen_right_sidex - env->title_screen_left_sidex;
  env->titleRect.h = env->title_screen_down_sidex - env->title_screen_up_sidex;
  SDL_RenderCopy(ren, env->titleTexture, NULL, &(env->titleRect));

  // renders the solve button and its text

  // set up dimensions of the rectangles
  env->current_solve_left_sidex =
      env->game_screen_left_sidex + offsetx_count + (game_nb_cols(env->g) * cell_size) + w / 20;
  env->current_solve_right_sidex =
      env->game_screen_left_sidex + offsetx_count + (game_nb_cols(env->g) * cell_size) + w / 8;
  env->current_up_sidex = h / 1.9;
  env->current_down_sidex = h / 1.7;

  // renders the current button and its text
  env->currentColorRect.x = env->current_solve_left_sidex;
  env->currentColorRect.y = env->current_up_sidex;
  env->currentColorRect.w = env->current_solve_right_sidex - env->current_solve_left_sidex;
  env->currentColorRect.h = env->current_down_sidex - env->current_up_sidex;

  env->currentColorTexte = env->currentColorRect;
  env->currentColorTexte.y = env->currentColorRect.y + env->currentColorRect.h;
  SDL_RenderCopy(ren, env->currentColorTexteTexture, NULL, &(env->currentColorTexte));

  // loads a different image depending on the current playable color
  if (env->playColor == WHITE) {
    SDL_RenderCopy(ren, env->currentColorWhiteTexture, NULL, &(env->currentColorRect));
  } else if (env->playColor == BLACK) {
    SDL_RenderCopy(ren, env->currentColorBlackTexture, NULL, &(env->currentColorRect));
  } else {
    SDL_RenderCopy(ren, env->currentColorEmptyTexture, NULL, &(env->currentColorRect));
  }

  // renders the solve button and its text

  // set up dimensions of the rectangle
  env->solve_up_sidey = h / 2.1;
  env->solve_down_sidey = h / 2.4;

  env->solve.x = env->current_solve_left_sidex;
  env->solve.y = env->solve_down_sidey;
  env->solve.w = env->current_solve_right_sidex - env->current_solve_left_sidex;
  env->solve.h = env->solve_up_sidey - env->solve_down_sidey;

  env->SolveTexteRect = env->solve;
  env->SolveTexteRect.y = env->solve.y - env->solve.h;
  SDL_RenderCopy(ren, env->solveTexteTexture, NULL, &(env->SolveTexteRect));

  SDL_RenderCopy(ren, env->solveTexture, NULL, &(env->solve));

  // renders the undo and redo buttons

  // set up dimensions of the rectangles
  env->undo_redo_up_sidey = h - h / 12.5;
  env->undo_redo_down_sidey = h - h / 25;

  // for undo
  env->undo_left_sidex = env->game_screen_left_sidex + offsetx_count + (cell_size * game_nb_cols(env->g)) / 3;
  env->undo_right_sidex = env->game_screen_left_sidex + offsetx_count;

  // for redo
  env->redo_left_sidex = env->game_screen_right_sidex - offsetx_count - (cell_size * game_nb_cols(env->g)) / 3;
  env->redo_right_sidex = env->game_screen_right_sidex - offsetx_count;

  // creates and renders buttons
  env->undo_redoRects[0].x = env->undo_right_sidex;
  env->undo_redoRects[0].y = env->undo_redo_up_sidey;
  env->undo_redoRects[0].w = env->undo_left_sidex - env->undo_right_sidex;
  env->undo_redoRects[0].h = env->undo_redo_down_sidey - env->undo_redo_up_sidey;
  SDL_RenderCopy(ren, env->undo_redoTextures[0], NULL, &(env->undo_redoRects[0]));

  env->undo_redoRects[1].x = env->redo_left_sidex;
  env->undo_redoRects[1].y = env->undo_redo_up_sidey;
  env->undo_redoRects[1].w = env->redo_right_sidex - env->redo_left_sidex;
  env->undo_redoRects[1].h = env->undo_redo_down_sidey - env->undo_redo_up_sidey;
  SDL_RenderCopy(ren, env->undo_redoTextures[1], NULL, &(env->undo_redoRects[1]));

  // creates the playable cells

  // initialise the playable area
  float x1 = env->game_screen_left_sidex + offsetx_count;
  float y1 = env->game_screen_up_sidey + offsety_count;

  for (int x = 0; x < game_nb_rows(env->g); x++) {
    // reset the x coordinate
    x1 = env->game_screen_left_sidex + offsetx_count;
    for (int y = 0; y < game_nb_cols(env->g); y++) {
      // initialise the cells
      env->cells[game_nb_cols(env->g) * x + y].x = x1;
      env->cells[game_nb_cols(env->g) * x + y].y = y1;
      env->cells[game_nb_cols(env->g) * x + y].w = cell_size + 1;
      env->cells[game_nb_cols(env->g) * x + y].h = cell_size + 1;
      // check what color is the cell
      if (game_get_color(env->g, x, y) == WHITE) {
        SDL_RenderCopy(ren, env->whiteTexture, NULL, &(env->cells[game_nb_cols(env->g) * x + y]));

      } else if (game_get_color(env->g, x, y) == BLACK) {
        SDL_RenderCopy(ren, env->blackTexture, NULL, &(env->cells[game_nb_cols(env->g) * x + y]));
      } else {
        SDL_RenderCopy(ren, env->emptyTexture, NULL, &(env->cells[game_nb_cols(env->g) * x + y]));
      }
      // set up the constraints and status of the cell
      for (int i = 0; i <= TOTALNUMBER; i++) {
        if (game_get_constraint(env->g, x, y) == i) {
          if (game_get_status((env->g), x, y) == SATISFIED) {
            SDL_RenderCopy(ren, env->green_numbers[i], NULL, &(env->cells[game_nb_cols(env->g) * x + y]));
          } else if (game_get_status((env->g), x, y) == UNSATISFIED) {
            SDL_RenderCopy(ren, env->blue_numbers[i], NULL, &(env->cells[game_nb_cols(env->g) * x + y]));
          } else if (game_get_status((env->g), x, y) == ERROR) {
            SDL_RenderCopy(ren, env->red_numbers[i], NULL, &(env->cells[game_nb_cols(env->g) * x + y]));
          }
        }
      }
      x1 += cell_size;
    }
    y1 += cell_size;
  }

  // if the game is won, makes it that you can't play any moves and put the victory screen
  if (game_won(env->g)) {
    env->hasWon = true;

    SDL_RenderCopy(ren, env->victoryScreen, NULL, &(env->backgroundRect));
  }
}

/* **************************************************************** */

bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }
  /* Android events */
#ifdef __ANDROID__

#else
  // checks the events when the mouse is pressed
  else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    // plays the move
    for (int i = 0; i < game_nb_cols(env->g) * game_nb_rows(env->g); i++) {
      if (SDL_PointInRect(&mouse, &(env->cells[i])) && e->button.button == SDL_BUTTON_LEFT && env->hasWon == false) {
        game_play_move(env->g, i / game_nb_cols(env->g), i % game_nb_cols(env->g), env->playColor);
      }
    }
    // switches the color
    if (e->button.button == SDL_BUTTON_RIGHT) {
      env->playColor = (env->playColor + 1) % 3;
    }
    // presses the undo/redo buttons
    if (SDL_PointInRect(&mouse, &(env->undo_redoRects[0]))) {
      game_undo(env->g);
    }
    if (SDL_PointInRect(&mouse, &(env->undo_redoRects[1]))) {
      game_redo(env->g);
    }
    if (SDL_PointInRect(&mouse, &(env->solve))) {
      game_solve(env->g);
    }

    // checks the vent when a key on the keyboard is pressed
  } else if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      // left arrow
      case SDLK_LEFT:
        game_undo(env->g);
        game_print(env->g);
        break;
      // right arrow
      case SDLK_RIGHT:
        game_redo(env->g);
        game_print(env->g);
        break;
      // escape
      case SDLK_ESCAPE:
        return true;
        break;
    }
    // s key
    if (e->key.keysym.sym == SDLK_s) {
      game_save(env->g, "game_saved");
    }
    // r key
    else if (e->key.keysym.sym == SDLK_r) {
      env->hasWon = false;
      game_restart(env->g);
    }
  }
#endif

  return false; /* don't quit */
}

/* **************************************************************** */

void clean(SDL_Window* win, SDL_Renderer* ren, Env* env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  // frees the numbers
  for (int i = 0; i <= TOTALNUMBER; i++) {
    SDL_DestroyTexture(env->green_numbers[i]);
    SDL_DestroyTexture(env->blue_numbers[i]);
    SDL_DestroyTexture(env->red_numbers[i]);
  }
  free(env->green_numbers);
  free(env->blue_numbers);
  free(env->red_numbers);

  // free the textures
  SDL_DestroyTexture(env->unconstraitTexture);
  SDL_DestroyTexture(env->emptyTexture);
  SDL_DestroyTexture(env->whiteTexture);
  SDL_DestroyTexture(env->blackTexture);
  SDL_DestroyTexture(env->backgroundTexture);
  SDL_DestroyTexture(env->undo_redoTextures[0]);
  SDL_DestroyTexture(env->undo_redoTextures[1]);
  SDL_DestroyTexture(env->victoryScreen);
  SDL_DestroyTexture(env->currentColorBlackTexture);
  SDL_DestroyTexture(env->currentColorEmptyTexture);
  SDL_DestroyTexture(env->currentColorWhiteTexture);
  SDL_DestroyTexture(env->currentColorTexteTexture);
  SDL_DestroyTexture(env->solveTexture);
  SDL_DestroyTexture(env->solveTexteTexture);
  SDL_DestroyTexture(env->titleTexture);

  free(env->undo_redoTextures);
  free(env->undo_redoRects);
  free(env->cells);

  game_delete(env->g);

  free(env);
}

/* **************************************************************** */
