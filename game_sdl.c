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



#define FONT "res/arial.ttf"
#define FONTSIZE 36
#define TEST1 "res/mario.png"
#define TEST2 "res/test2.png"


/* **************************************************************** */

struct Env_t {
  float game_screen_left_sidex,game_screen_right_sidex,game_screen_up_sidex,game_screen_down_sidex;
  float message_screen_left_sidex,message_screen_right_sidex,message_screen_up_sidex,message_screen_down_sidex;
  float current_left_sidex,current_right_sidex,current_up_sidex,current_down_sidex;
  float current_solve_left_sidex,current_solve_right_sidex,current_solve_up_sidex,current_solve_down_sidex;
  float up_sidey_square,down_sidey_square;
  float undo_redo_up_sidey,undo_redo_left_sidex,undo_redo_right_sidex, undo_right_sidex, undo_left_sidex;
  float help_left_sidex,help_right_sidex,help_up_sidey,help_down_sidey;
  SDL_Texture* text[10];
  SDL_Texture* test1;
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
  
// numbers images init
  for (int i = 0; i < 10; i++) {
    char filename[50];

    sprintf(filename, "res/%db.png", i);
    
    env->numbers[i] = IMG_LoadTexture(ren, filename);

    sprintf(filename, "res/%de.png", i);
    env->numbers[i] = IMG_LoadTexture(ren, filename);

    sprintf(filename, "res/%dw.png", i);
    env->numbers[i] = IMG_LoadTexture(ren, filename);
  }
  printf(env->numbers[4]);

  env->playColor=WHITE;

  env->g=game_default();
  env->cases=malloc(sizeof(SDL_Rect)*game_nb_rows(env->g)*game_nb_cols(env->g));
  env->test1 = IMG_LoadTexture(ren, TEST1);
  if (!env->test1) ERROR("IMG_LoadTexture: %s\n", TEST1);

  SDL_Color color = {0, 0, 255, 255}; /* blue color in RGBA */
  TTF_Font* font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD); // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
  SDL_Surface* surf;
  for(int i=0; i<10;i++){
    switch (i)
    {
    case 0:
      surf = TTF_RenderText_Blended(font, "0", color);
      break;
    case 1:
      surf = TTF_RenderText_Blended(font, "1", color);
      break;
    case 2:
      surf = TTF_RenderText_Blended(font, "2", color);
      break;
    case 3:
      surf = TTF_RenderText_Blended(font, "3", color);
      break;
    case 4:
      surf = TTF_RenderText_Blended(font, "4", color);
      break;
    case 5:
      surf = TTF_RenderText_Blended(font, "5", color);
      break;
    case 6:
      surf = TTF_RenderText_Blended(font, "6", color);
      break;
    case 7:
      surf = TTF_RenderText_Blended(font, "7", color);
      break;
    case 8:
      surf = TTF_RenderText_Blended(font, "8", color);
      break;
    case 9:
      surf = TTF_RenderText_Blended(font, "9", color);
      break;
    default:
      break;
    }
        // blended rendering for ultra nice text
    env->text[i] = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    
  }
                           
  
  
  TTF_CloseFont(font);
  
  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) { 
  
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  SDL_Rect rect;
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
  /*SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_up_sidex, env->game_screen_left_sidex, env->game_screen_down_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_up_sidex, env->game_screen_right_sidex, env->game_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_right_sidex, env->game_screen_down_sidex, env->game_screen_right_sidex, env->game_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->game_screen_left_sidex, env->game_screen_down_sidex, env->game_screen_right_sidex, env->game_screen_down_sidex);*/

  // grid game
  float game_width = env->game_screen_right_sidex - env->game_screen_left_sidex;
  float game_height = env->game_screen_down_sidex - env->game_screen_up_sidex;

  int cell_required = 5;
  float min=0;
  if (game_height<game_width){
    min=game_height;
  }
  else{
    min=game_width;
  }
  float grid_cell_size = min / cell_required;

  float offsetx=env->game_screen_left_sidex+((game_width-min)/2);
  float offsety=env->game_screen_up_sidex+((game_height-min)/2);

  for (float x = offsetx; x <= offsetx+min+1; x += grid_cell_size){
       SDL_RenderDrawLine(ren, x, offsety, x, offsety+min);
  }

  for (float y = offsety; y <= offsety+min+1; y += grid_cell_size) {
    SDL_RenderDrawLine(ren, offsetx, y, offsetx+min, y);
       
  }

  //message screen
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_up_sidex, env->message_screen_left_sidex, env->message_screen_down_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_up_sidex, env->message_screen_right_sidex, env->message_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_right_sidex, env->message_screen_down_sidex, env->message_screen_right_sidex, env->message_screen_up_sidex);
  SDL_RenderDrawLine(ren, env->message_screen_left_sidex, env->message_screen_down_sidex, env->message_screen_right_sidex, env->message_screen_down_sidex);

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
  env->undo_redo[0].y=env->down_sidey_square;
  env->undo_redo[0].w=env->undo_redo_right_sidex-env->undo_redo_left_sidex;
  env->undo_redo[0].h=env->down_sidey_square-env->up_sidey_square;

  SDL_RenderDrawLine(ren, env->undo_left_sidex, env->down_sidey_square, env->undo_right_sidex, env->down_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_right_sidex, env->down_sidey_square, env->undo_right_sidex, env->up_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_left_sidex, env->up_sidey_square, env->undo_left_sidex, env->down_sidey_square);
  SDL_RenderDrawLine(ren, env->undo_right_sidex, env->up_sidey_square, env->undo_left_sidex, env->up_sidey_square);

  env->undo_redo[1].x=env->undo_left_sidex;
  env->undo_redo[1].y=env->down_sidey_square;
  env->undo_redo[1].w=env->undo_right_sidex-env->undo_left_sidex;
  env->undo_redo[1].h=env->down_sidey_square-env->up_sidey_square;

  //help
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_up_sidey, env->help_left_sidex, env->help_down_sidey);
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_up_sidey, env->help_right_sidex, env->help_up_sidey);
  SDL_RenderDrawLine(ren, env->help_right_sidex, env->help_down_sidey, env->help_right_sidex, env->help_up_sidey);
  SDL_RenderDrawLine(ren, env->help_left_sidex, env->help_down_sidey, env->help_right_sidex, env->help_down_sidey);


  env->undo_redo[0];



  /* render text texture */
  for(int i=0; i<10;i++){
    SDL_QueryTexture(env->text[i], NULL, NULL, &rect.w, &rect.h);
  } 
  int x1=0;
  int y1=0;
  for(float x = offsetx; x < offsetx+min-1; x += grid_cell_size) {
      for (float y = offsety; y < offsety+min-1; y += grid_cell_size) {
          rect.x =  x;
          rect.y = y;
          rect.w=grid_cell_size;
          rect.h=grid_cell_size;
          env->cases[game_nb_cols(env->g)*x1+y1]=rect;
              for(int i=0; i<10;i++){        
                if(game_get_constraint(env->g, y1,x1)==-1){
                  break;
                  }
                if(game_get_constraint(env->g, y1,x1)==i){
                   SDL_RenderCopy(ren, env->text[i], NULL, &(env->cases[game_nb_cols(env->g)*x1+y1]));
                }
              }
              
              y1=(y1+1)%game_nb_cols(env->g);
             
            }
        x1=(x1+1)%game_nb_rows(env->g);
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
        SDL_RenderCopy(ren, env->text[0], NULL, &(env->cases[i]));
        printf("%d\n", game_get_constraint(env->g, i%5, i/5));
        game_set_color(env->g, i%5, i/5, env->playColor);
        game_print(env->g);

        int number = game_get_constraint(env->g, i%5, i/5);
        char filename[50];

        if (env->playColor == WHITE) {
          sprintf(filename, "res/%dw.png", number);
          SDL_RenderCopy(ren, env->numbers[number+2], NULL, &(env->cases[i]));

          // render

          
        } else if (env->playColor == BLACK) {

        } else if (env->playColor == EMPTY) {

        }

      }
    }
    if(e->button.button == SDL_BUTTON_RIGHT){
      env->playColor=(env->playColor+1)%3;
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
  
  for(int i=0; i<10;i++){
    SDL_DestroyTexture(env->text[i]);
  } 
  SDL_DestroyTexture(env->test1);


  free(env);
}

/* **************************************************************** */
