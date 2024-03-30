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
#include "game_aux.h"
#include "game_perso.h"



#define FONT "res/arial.ttf"
#define FONTSIZE 36

#define UNCONSTRAINT "res/-1.png"
#define BLACKSQUARE "res/Black.png"
#define WHITESQUARE "res/White.png"
#define EMPTYSQUARE "res/Empty.png"
#define TITLE "res/Mosaic.png"
#define BACKGROUND "res/Background.png"

#define UNDO "res/undo.png"
#define REDO "res/redo.png"

#define TOTALNUMBER 9


/* **************************************************************** */

struct Env_t {
  float game_screen_left_sidex,game_screen_right_sidex,game_screen_up_sidex,game_screen_down_sidex;
  float message_screen_left_sidex,message_screen_right_sidex,message_screen_up_sidex,message_screen_down_sidex;
  float current_left_sidex,current_right_sidex,current_up_sidex,current_down_sidex;
  float current_solve_left_sidex,current_solve_right_sidex,current_solve_up_sidex,current_solve_down_sidex;
  float up_sidey_square,down_sidey_square;
  float undo_redo_up_sidey,undo_redo_left_sidex,undo_redo_right_sidex, undo_right_sidex, undo_left_sidex;
  float help_left_sidex,help_right_sidex,help_up_sidey,help_down_sidey;

  SDL_Texture** green_numbers;
  SDL_Texture** blue_numbers;
  SDL_Texture** red_numbers;
  SDL_Texture* unconstraitSquare;
  SDL_Texture* blackSquare;
  SDL_Texture* whiteSquare;
  SDL_Texture* emptySquare;

  SDL_Rect titleSquare;
  SDL_Texture* title;

  SDL_Rect backgroundSquare;
  SDL_Texture* background;


  game g;
  SDL_Rect * cases;
  SDL_Rect * undo_redo;
  SDL_Texture ** undo_redoSquare;
  SDL_Rect * solve_current; 
  color playColor;

  bool hasWon;
  
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  env->backgroundSquare.x=0;
  env->backgroundSquare.y=0;
  env->backgroundSquare.w=w;
  env->backgroundSquare.h=h;
  SDL_RenderCopy(ren, env->background, NULL, &(env->backgroundSquare));

  //initialisation of the numbers
  env->green_numbers=malloc(sizeof(SDL_Texture*)*TOTALNUMBER+1);
  if(env->green_numbers==NULL){
    ERROR("malloc green numbers");
  } 
  env->blue_numbers=malloc(sizeof(SDL_Texture*)*TOTALNUMBER+1);
  if(env->blue_numbers==NULL){
    ERROR("malloc blue numbers");
  } 
  env->red_numbers=malloc(sizeof(SDL_Texture*)*TOTALNUMBER+1);
  if(env->red_numbers==NULL){
    ERROR("malloc red numbers");
  } 

// numbers images init
  for (int i = 0; i <= TOTALNUMBER; i++){
    char filename[50];

    sprintf(filename, "res/Green/%dv.png", i);
    
    env->green_numbers[i] = IMG_LoadTexture(ren, filename);
    if (!env->green_numbers[i]) ERROR("IMG_LoadTexture: %s\n", filename);
  }
  for (int i = 0; i <= TOTALNUMBER; i++){
    char filename[50];

    sprintf(filename, "res/Blue/%db.png", i);
    
    env->blue_numbers[i] = IMG_LoadTexture(ren, filename);
    if (!env->blue_numbers[i]) ERROR("IMG_LoadTexture: %s\n", filename);
  }
  for (int i = 0; i <= TOTALNUMBER; i++){
    char filename[50];

    sprintf(filename, "res/Red/%dr.png", i);
    
    env->red_numbers[i] = IMG_LoadTexture(ren, filename);
    if (!env->red_numbers[i]) ERROR("IMG_LoadTexture: %s\n", filename);
  }
  env->unconstraitSquare = IMG_LoadTexture(ren, UNCONSTRAINT);
  if (!env->unconstraitSquare) ERROR("IMG_LoadTexture: %s\n", UNCONSTRAINT);

  env->blackSquare = IMG_LoadTexture(ren, BLACKSQUARE);
  if (!env->blackSquare) ERROR("IMG_LoadTexture: %s\n", BLACKSQUARE);
  env->whiteSquare = IMG_LoadTexture(ren, WHITESQUARE);
  if (!env->whiteSquare) ERROR("IMG_LoadTexture: %s\n", WHITESQUARE);
  env->emptySquare = IMG_LoadTexture(ren, EMPTYSQUARE);
  if (!env->emptySquare) ERROR("IMG_LoadTexture: %s\n", EMPTYSQUARE);

  env->title = IMG_LoadTexture(ren, TITLE);
  if (!env->title) ERROR("IMG_LoadTexture: %s\n", TITLE);

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  env->playColor=WHITE;
  if(argc==2){
    env->g=game_load(argv[1]);
  }
  else{
    env->g=game_default();
  }
  env->cases=malloc(sizeof(SDL_Rect)*game_nb_rows(env->g)*game_nb_cols(env->g));
  if(env->cases==NULL){
    ERROR("malloc cases");
  } 
  env->undo_redo=malloc(sizeof(SDL_Rect)*2);
  if(env->undo_redo==NULL){
    ERROR("malloc undo_redo");
  } 
  
  env->undo_redoSquare=malloc(sizeof(SDL_Texture *)*2);
  if(env->undo_redoSquare==NULL){
    ERROR("malloc undo_redo");
  }
  env->undo_redoSquare[1] = IMG_LoadTexture(ren, UNDO);
  if (!env->undo_redoSquare[1]) ERROR("IMG_LoadTexture: %s\n", UNDO);
  env->undo_redoSquare[0] = IMG_LoadTexture(ren, REDO);
  if (!env->undo_redoSquare[0]) ERROR("IMG_LoadTexture: %s\n", REDO);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) { 
  
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  

  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
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
  env->current_up_sidex=h/1.9;
  env->current_down_sidex=h/1.6;

  env->current_solve_up_sidex=h-h/1.9;
  env->current_solve_down_sidex=h-h/1.6;

  env->up_sidey_square=h-h/12.5;
  env->down_sidey_square=h-h/25;
  env->undo_redo_up_sidey=h-h/16.5;
  env->undo_redo_left_sidex=w/1.7;
  env->undo_redo_right_sidex=w/1.4;

  env->help_left_sidex=w-w/10;
  env->help_right_sidex=w-w/25;
  env->help_up_sidey=h/10;
  env->help_down_sidey=h/25;

  env->undo_left_sidex=w-w/1.7;
  env->undo_right_sidex=w-w/1.4;

  //game screen 
  SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_up_sidex, env->game_screen_left_sidex, env->game_screen_down_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_up_sidex, env->game_screen_right_sidex, env->game_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_right_sidex, env->game_screen_down_sidex, env->game_screen_right_sidex, env->game_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_down_sidex, env->game_screen_right_sidex, env->game_screen_down_sidex);

  // grid game

  float game_width = env->game_screen_right_sidex - env->game_screen_left_sidex;
  float game_height = env->game_screen_down_sidex - env->game_screen_up_sidex;

  float cell_size=0;
  float offsetx_count=0;
  float offsety_count=0;
    float cell_size1=game_width/game_nb_cols(env->g);
    float cell_size2=game_height/game_nb_rows(env->g);
    if(game_width>=cell_size2*game_nb_cols(env->g)){
      cell_size=cell_size2;
      offsetx_count=(game_width-game_nb_cols(env->g)*cell_size)/2;
      offsety_count=0;
    }
    else if(game_height>cell_size1*game_nb_rows(env->g)){
      cell_size=cell_size1;
      offsetx_count=0;
      offsety_count=(game_height-game_nb_rows(env->g)*cell_size)/2;

    }


  for(int i=0; i<=game_nb_cols(env->g); i++){
    SDL_RenderDrawLine(ren, env->game_screen_left_sidex+offsetx_count, env->game_screen_up_sidex+offsety_count, env->game_screen_left_sidex+offsetx_count, env->game_screen_down_sidex-offsety_count);
    offsetx_count+=cell_size;
  }
  offsetx_count=offsetx_count-(cell_size*(game_nb_cols(env->g)+1));
  for(int i=0; i<=game_nb_rows(env->g); i++){
    SDL_RenderDrawLine(ren, env->game_screen_left_sidex+offsetx_count, env->game_screen_up_sidex+offsety_count, env->game_screen_right_sidex-offsetx_count, env->game_screen_up_sidex+offsety_count);
    offsety_count+=cell_size;
  }
  offsety_count=offsety_count-(cell_size*(game_nb_rows(env->g)+1));



  //message screen
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_up_sidex, env->message_screen_left_sidex, env->message_screen_down_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_up_sidex, env->message_screen_right_sidex, env->message_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_right_sidex, env->message_screen_down_sidex, env->message_screen_right_sidex, env->message_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_down_sidex, env->message_screen_right_sidex, env->message_screen_down_sidex);

  env->titleSquare.x=env->message_screen_left_sidex;
  env->titleSquare.y=env->message_screen_up_sidex;
  env->titleSquare.w=env->message_screen_right_sidex-env->message_screen_left_sidex;
  env->titleSquare.h=env->message_screen_down_sidex-env->message_screen_up_sidex;
  SDL_RenderCopy(ren, env->title, NULL, &(env->titleSquare));


  //current and solve
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_up_sidex, env->current_solve_left_sidex, env->current_down_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_up_sidex, env->current_solve_right_sidex, env->current_up_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_right_sidex, env->current_down_sidex, env->current_solve_right_sidex, env->current_up_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_down_sidex, env->current_solve_right_sidex, env->current_down_sidex);

  
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_up_sidex, env->current_solve_left_sidex, env->current_solve_down_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_up_sidex, env->current_solve_right_sidex, env->current_solve_up_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_right_sidex, env->current_solve_down_sidex, env->current_solve_right_sidex, env->current_solve_up_sidex);
  SDL_RenderDrawLine(ren, env->current_solve_left_sidex, env->current_solve_down_sidex, env->current_solve_right_sidex, env->current_solve_down_sidex);

  //undo/redo
  SDL_RenderDrawLine(ren, env->undo_redo_left_sidex, env->down_sidey_square, env->undo_redo_right_sidex, env->down_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_redo_right_sidex, env->down_sidey_square, env->undo_redo_right_sidex, env->up_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_redo_left_sidex, env->up_sidey_square, env->undo_redo_left_sidex, env->down_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_redo_right_sidex, env->up_sidey_square, env->undo_redo_left_sidex, env->up_sidey_square);

  env->undo_redo[0].x=env->undo_redo_left_sidex;
  env->undo_redo[0].y=env->undo_redo_up_sidey;
  env->undo_redo[0].w=env->undo_redo_right_sidex-env->undo_redo_left_sidex;
  env->undo_redo[0].h=env->down_sidey_square-env->undo_redo_up_sidey;
  SDL_RenderCopy(ren, env->undo_redoSquare[0], NULL, &(env->undo_redo[0]));

  SDL_RenderDrawLine(ren, env->undo_left_sidex, env->down_sidey_square, env->undo_right_sidex, env->down_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_right_sidex, env->down_sidey_square, env->undo_right_sidex, env->up_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_left_sidex, env->up_sidey_square, env->undo_left_sidex, env->down_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_right_sidex, env->up_sidey_square, env->undo_left_sidex, env->up_sidey_square);

  env->undo_redo[1].x=env->undo_left_sidex;
  env->undo_redo[1].y=env->undo_redo_up_sidey;
  env->undo_redo[1].w=env->undo_right_sidex-env->undo_left_sidex;
  env->undo_redo[1].h=env->down_sidey_square-env->up_sidey_square;
  SDL_RenderCopy(ren, env->undo_redoSquare[1], NULL, &( env->undo_redo[1]));

  //help
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_up_sidey, env->help_left_sidex, env->help_down_sidey);
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_up_sidey, env->help_right_sidex, env->help_up_sidey);
  SDL_RenderDrawLine(ren, env->help_right_sidex, env->help_down_sidey, env->help_right_sidex, env->help_up_sidey);
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_down_sidey, env->help_right_sidex, env->help_down_sidey);





  /* render text texture */


  if(game_won(env->g)){
    env->hasWon =true;
  }
  else{
    env->hasWon=false;
  }
  
  float x1=env->game_screen_left_sidex+offsetx_count;
  float y1=env->game_screen_up_sidex+offsety_count;
  int test=0;
  for(int x=0; x<game_nb_rows(env->g); x++){
    x1=env->game_screen_left_sidex+offsetx_count;
    for(int y=0; y<game_nb_cols(env->g); y++){
      env->cases[game_nb_cols(env->g)*x+y].x =  x1;
      env->cases[game_nb_cols(env->g)*x+y].y = y1;
      env->cases[game_nb_cols(env->g)*x+y].w=cell_size+1;
      env->cases[game_nb_cols(env->g)*x+y].h=cell_size+1;
      //env->cases[game_nb_rows(env->g)*x+y]=rect;
      if(game_get_color(env->g, x, y)==WHITE){
        SDL_RenderCopy(ren, env->whiteSquare, NULL, &(env->cases[game_nb_cols(env->g)*x+y]));

      }
      else if(game_get_color(env->g, x, y)==BLACK){
        SDL_RenderCopy(ren, env->blackSquare, NULL, &(env->cases[game_nb_cols(env->g)*x+y]));
      }
      else{
        SDL_RenderCopy(ren, env->emptySquare, NULL, &(env->cases[game_nb_cols(env->g)*x+y]));
      }

      for(int i=0; i<=TOTALNUMBER;i++){        
        
          if(game_get_constraint(env->g, x,y)==i){

            if(game_get_status((env->g), x, y)==SATISFIED){
              SDL_RenderCopy(ren, env->green_numbers[i], NULL, &(env->cases[game_nb_cols(env->g)*x+y]));
            }
            else if(game_get_status((env->g), x, y)==UNSATISFIED){
              SDL_RenderCopy(ren, env->blue_numbers[i], NULL, &(env->cases[game_nb_cols(env->g)*x+y]));
            }
            else if(game_get_status((env->g), x, y)==ERROR){
              SDL_RenderCopy(ren, env->red_numbers[i], NULL, &(env->cases[game_nb_cols(env->g)*x+y]));
            }
          }
        }
        x1+=cell_size;
      
    }
    y1+=cell_size;
    
  }
  
  
       
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
    for(int i=0; i<game_nb_cols(env->g)*game_nb_rows(env->g); i++){
      if (SDL_PointInRect(&mouse, &(env->cases[i])) && e->button.button == SDL_BUTTON_LEFT && env->hasWon==false){
        game_play_move(env->g, i/game_nb_cols(env->g), i%game_nb_cols(env->g), env->playColor);
      }
    }
    if(e->button.button == SDL_BUTTON_RIGHT){
      env->playColor=(env->playColor+1)%3;
    }
    if (SDL_PointInRect(&mouse, &(env->undo_redo[1]))){
      game_undo(env->g);
    }
    if (SDL_PointInRect(&mouse, &(env->undo_redo[0]))){
      game_redo(env->g);
    }
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
      case SDLK_ESCAPE:
        return true;
        break;
    }
    if(e->key.keysym.sym==SDLK_s){
      game_solve(env->g);
    }
    else if(e->key.keysym.sym==SDLK_r){
        env->hasWon=false;
        game_restart(env->g);
    }

      
  }
#endif

  return false; /* don't quit */
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */
  

  for(int i=0; i<=TOTALNUMBER; i++){
    SDL_DestroyTexture(env->green_numbers[i]);
    SDL_DestroyTexture(env->blue_numbers[i]);
    SDL_DestroyTexture(env->red_numbers[i]);
  }
  free(env->green_numbers);
  free(env->blue_numbers);
  free(env->red_numbers);

  SDL_DestroyTexture(env->unconstraitSquare);

  game_delete(env->g);

  
  free(env->cases);

  free(env);
}

/* **************************************************************** */
