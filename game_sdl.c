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

#define TOTALNUMBER 9

bool min_number(float a, float b);

bool min_number(float a, float b){
  return a<=b;
}

float min_number_return(float a, float b){
  if(a<=b){
    return a;
  }
  return b;
}


/* **************************************************************** */

struct Env_t {
  //[0] = left, [1] = right, [2] = up, [3] = down   
  float * game_screen;
  float * message_screen;
  float * current_color;
  float * solve;
  float * undo;
  float * redo;
  float * help;
  SDL_Texture** numbers;
  game g;
  SDL_Rect * cases;
  SDL_Rect * undo_redo;
  SDL_Rect * solve_current; 
  color playColor;
  
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  env->numbers=malloc(sizeof(SDL_Texture*)*6);
  if(env->numbers==NULL){
    ERROR("malloc numbers");
  } 
// numbers images init
  int count=0;
  for (int i = 0; i <= 1; i++) {//TOTALNUMBER; i++) {
    char filename[50];

    sprintf(filename, "res/%db.png", i);
    
    env->numbers[count] = IMG_LoadTexture(ren, filename);
    if (!env->numbers[count]) ERROR("IMG_LoadTexture: %s\n", filename);
    count++;

    sprintf(filename, "res/%de.png", i);
    env->numbers[count] = IMG_LoadTexture(ren, filename);
    if (!env->numbers[count]) ERROR("IMG_LoadTexture: %s\n", filename);
    count++;

    sprintf(filename, "res/%dw.png", i);
    env->numbers[count] = IMG_LoadTexture(ren, filename);
    if (!env->numbers[count]) ERROR("IMG_LoadTexture: %s\n", filename);
    count++;
  }

  env->playColor=WHITE;
  if(argc==2){
    env->g=game_load(argv[1]);
  }
  else{
    env->g=game_default();
  }
  env->game_screen=malloc(sizeof(float)*4);
  if(env->game_screen==NULL){
    ERROR("malloc game_screen");
  }
  env->message_screen=malloc(sizeof(float)*4);
  if(env->message_screen==NULL){
    ERROR("malloc message_screen");
  }
  env->current_color=malloc(sizeof(float)*4);
  if(env->current_color==NULL){
    ERROR("malloc current_color");
  }
  env->solve=malloc(sizeof(float)*4);
  if(env->solve==NULL){
    ERROR("malloc solve");
  }
  env->undo=malloc(sizeof(float)*4);
  if(env->undo==NULL){
    ERROR("malloc undo");
  }
  env->redo=malloc(sizeof(float)*4);
  if(env->redo==NULL){
    ERROR("malloc redo");
  }
  env->help=malloc(sizeof(float)*4);
  if(env->help==NULL){
    ERROR("malloc help");
  }
  env->cases=malloc(sizeof(SDL_Rect)*game_nb_rows(env->g)*game_nb_cols(env->g));
  if(env->cases==NULL){
    ERROR("malloc cases");
  } 
  env->undo_redo=malloc(sizeof(SDL_Rect)*2);
  if(env->undo_redo==NULL){
    ERROR("malloc undo_redo");
  } 
  
  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) { 
  
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  SDL_Rect rect;
  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
  env->game_screen[0]=w/20;
  env->game_screen[1]=w-w/5;
  env->game_screen[2]=h/8;
  env->game_screen[3]=h-h/8;

  env->message_screen[0]=w/4;
  env->message_screen[1]=w-w/4;
  env->message_screen[2]=h/50;
  env->message_screen[3]=h/10;

  env->current_color[0]=w-w/7;
  env->current_color[1]=w-w/20;
  env->current_color[2]=h/1.9;
  env->current_color[3]=h/1.6;

  env->solve[0]=w-w/7;;
  env->solve[1]=w-w/20;
  env->solve[2]=h-h/1.9;
  env->solve[3]=h-h/1.6;

  env->redo[0]=w-w/1.7;
  env->redo[1]=w-w/1.4;
  env->redo[2]=h-h/12.5;
  env->redo[3]=h-h/25;

  env->undo[0]=w/1.7;
  env->undo[1]=w/1.4;
  env->undo[2]=h-h/12.5;
  env->undo[3]=h-h/25;

  env->help[0]=w-w/10;
  env->help[1]=w-w/25;
  env->help[2]=h/10;
  env->help[3]=h/25;


  //game screen 
  drawRect(ren, env->game_screen);

  // grid game

  float game_width = env->game_screen[1] - env->game_screen[0];
  float game_height = env->game_screen[3] - env->game_screen[2];

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
    SDL_RenderDrawLine(ren, env->game_screen[0]+offsetx_count, env->game_screen[2]+offsety_count, env->game_screen[0]+offsetx_count, env->game_screen[3]-offsety_count);
    offsetx_count+=cell_size;
  }
  offsetx_count=offsetx_count-(cell_size*(game_nb_cols(env->g)+1));
  for(int i=0; i<=game_nb_rows(env->g); i++){
    SDL_RenderDrawLine(ren, env->game_screen[0]+offsetx_count, env->game_screen[2]+offsety_count, env->game_screen[1]-offsetx_count, env->game_screen[2]+offsety_count);
    offsety_count+=cell_size;
  }
  offsety_count=offsety_count-(cell_size*(game_nb_rows(env->g)+1));



  //message screen
  drawRect(ren,env->message_screen);

  //current and solve
  drawRect(ren,env->current_color);

  
  drawRect(ren,env->solve);

  //undo/redo
  drawRect(ren,env->redo);

  env->undo_redo[0].x=env->redo[0];
  env->undo_redo[0].y=env->redo[3];
  env->undo_redo[0].w=env->redo[1]-env->redo[0];
  env->undo_redo[0].h=env->redo[3]-env->redo[2];

  drawRect(ren,env->undo);

  env->undo_redo[1].x=env->undo[0];
  env->undo_redo[1].y=env->redo[3];
  env->undo_redo[1].w=env->undo[1]-env->undo[0];
  env->undo_redo[1].h=env->redo[3]-env->redo[2];

  //help
  drawRect(ren,env->help);





  /* render text texture */
  for(int i=0; i<=5; i++){//TOTALNUMBER*3; i++){
    SDL_QueryTexture(env->numbers[i], NULL, NULL, &rect.w, &rect.h);
  }

  
  float x1=env->game_screen[0]+offsetx_count;
  float y1=env->game_screen[2]+offsety_count;
  int test=0;
  for(int x=0; x<game_nb_rows(env->g); x++){
    x1=env->game_screen[0]+offsetx_count;
    for(int y=0; y<game_nb_cols(env->g); y++){
      env->cases[game_nb_cols(env->g)*x+y].x =  x1;
      env->cases[game_nb_cols(env->g)*x+y].y = y1;
      env->cases[game_nb_cols(env->g)*x+y].w=cell_size+1;
      env->cases[game_nb_cols(env->g)*x+y].h=cell_size+1;
      //env->cases[game_nb_rows(env->g)*x+y]=rect;
      for(int i=-1; i<10;i++){        
                if(game_get_constraint(env->g, x,y)==i){
                  if(game_get_constraint(env->g, x,y)==1){
                    SDL_RenderCopy(ren, env->numbers[4], NULL, &(env->cases[game_nb_cols(env->g)*x+y]));
                  }
                  if(game_get_constraint(env->g, x,y)==0){
                    SDL_RenderCopy(ren, env->numbers[1], NULL, &(env->cases[game_nb_cols(env->g)*x+y]));
                  }
                  else{
                   SDL_RenderCopy(ren, env->numbers[3], NULL, &(env->cases[game_nb_cols(env->g)*x+y]));
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
      if (SDL_PointInRect(&mouse, &(env->cases[i])) && e->button.button == SDL_BUTTON_LEFT){
        game_play_move(env->g, i/game_nb_cols(env->g), i%game_nb_cols(env->g), env->playColor);
        if(checkStatus(env->g, i/game_nb_cols(env->g), i%game_nb_cols(env->g))==true){
          printf("error"); //ajouter les coordonnées de la case via checkStatus
        }
        game_print(env->g);

        /*int number = game_get_constraint(env->g, i%5, i/5);
        char filename[50];

        if (env->playColor == WHITE) {
          sprintf(filename, "res/%dw.png", number);
          SDL_RenderCopy(ren, env->numbers[number+2], NULL, &(env->cases[i]));

          // render

          
        } else if (env->playColor == BLACK) {

        } else if (env->playColor == EMPTY) {

        }*/

      }
    }
    if(e->button.button == SDL_BUTTON_RIGHT){
      env->playColor=(env->playColor+1)%3;
    }
    if (SDL_PointInRect(&mouse, &(env->undo_redo[0])) && e->button.button == SDL_BUTTON_LEFT){
      game_undo(env->g);
      game_print(env->g);
    } 
    if (SDL_PointInRect(&mouse, &(env->undo_redo[1])) && e->button.button == SDL_BUTTON_LEFT){
      game_redo(env->g);
      game_print(env->g);
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
  }
#endif

  return false; /* don't quit */
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */
  

  for(int i=0; i<6; i++){
    SDL_DestroyTexture(env->numbers[i]);
  }
  game_delete(env->g);
  free(env->numbers);
  
  free(env->cases);

  free(env->game_screen);

  free(env->current_color);

  free(env->help);

  free(env->undo);

  free(env->redo);

  free(env->message_screen);

  free(env->solve);

  free(env);
}

void drawRect(SDL_Renderer *ren, float * rect){
  SDL_RenderDrawLine(ren, rect[0], rect[2], rect[0], rect[3]);
  SDL_RenderDrawLine(ren, rect[0], rect[2], rect[1], rect[2]);
  SDL_RenderDrawLine(ren, rect[1], rect[3], rect[1], rect[2]);
  SDL_RenderDrawLine(ren, rect[0], rect[3], rect[1], rect[3]);
}


/* **************************************************************** */
