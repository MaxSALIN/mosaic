#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"

#define GAME_SIZE (DEFAULT_SIZE*DEFAULT_SIZE)

struct game_s{
    color *couleur;
    constraint *contrainte;
    status *statut;

};

//fonction safe malloc qui vérifie l'allocation de g
void safeMalloc(game g){   
    //vérification de l'allocation pour g
    if(g==NULL){
        fprintf(stderr, "Game NULL\n");
        exit(EXIT_FAILURE);
    }

     //création des paramètres de game et vérification de leurs créations
    g->couleur=malloc(sizeof(*(g->couleur))*GAME_SIZE);  
    g->contrainte=malloc(sizeof(*(g->contrainte))*GAME_SIZE);
    g->statut=malloc(sizeof(*(g->statut))*GAME_SIZE);
    if(g->couleur==NULL || g->contrainte==NULL || g->statut==NULL){ 
        fprintf(stderr, "Game parameters NULL\n");
        exit(EXIT_FAILURE);
    }
}

void checkIJ(uint i, uint j, char* text){
  if (i >= DEFAULT_SIZE || j >= DEFAULT_SIZE) {
    printf("Dans %s, les index entrés sont > à la longueur/largeur du tableau.", text);
    exit(EXIT_FAILURE);
  }
}

void gameNull(game g){
  if(g==NULL){
    fprintf(stderr, "Game NULL\n");
    exit(EXIT_FAILURE);
  }
}
void gameConstNull(cgame g){
  if(g==NULL){
    fprintf(stderr, "Game const NULL\n");
    exit(EXIT_FAILURE);
  }
}

void putStatus(game g, uint i, uint j){
  /*switch (game_get_constraint(g,i,j))
  {
  case -1:
    g->statut[i+DEFAULT_SIZE*j]=SATISFIED;
    break;
  
  case 0:
    if(game_nb_neighbors(g, i, j, BLACK)>=1){
      g->statut[i+DEFAULT_SIZE*j]=ERROR;
    }
    if (game_nb_neighbors(g, i, j, BLACK)==game_get_constraint(g,i, j) && game_nb_neighbors(g,i,j, EMPTY)==0)
    {
      g->statut[i+DEFAULT_SIZE*j]=SATISFIED;
    }
    else{
      g->statut[i+DEFAULT_SIZE*j]=UNSATISFIED;
    }
    break;
  default:
    if((game_nb_neighbors(g, i ,j, BLACK)==game_get_constraint(g, i ,j) && game_nb_neighbors(g, i ,j, WHITE)==MAX_CONSTRAINT-game_get_constraint(g, i ,j))&& game_nb_neighbors(g,i ,j, EMPTY)==0){
      g->statut[i+DEFAULT_SIZE*j]=SATISFIED;
    }
    if((game_nb_neighbors(g, i ,j, BLACK)<game_get_constraint(g, i ,j) && game_nb_neighbors(g, i ,j, WHITE)>MAX_CONSTRAINT-game_get_constraint(g, i ,j))){
      g->statut[i+DEFAULT_SIZE*j]=UNSATISFIED;
    }
    if((game_nb_neighbors(g, i ,j, BLACK)>game_get_constraint(g, i ,j) && game_nb_neighbors(g, i ,j, WHITE)<MAX_CONSTRAINT-game_get_constraint(g, i ,j))){
      g->statut[i+DEFAULT_SIZE*j]=ERROR;
    }
    break;
  }*/
  if(game_get_constraint(g, i, j)==0){
    if(game_nb_neighbors(g, i, j, BLACK)>=1){
      g->statut[i+DEFAULT_SIZE*j]=ERROR;
    }
    if (game_nb_neighbors(g, i, j, BLACK)==game_get_constraint(g,i, j) && game_nb_neighbors(g,i,j, EMPTY)==0)
    {
      g->statut[i+DEFAULT_SIZE*j]=SATISFIED;
    }
    else{
      g->statut[i+DEFAULT_SIZE*j]=UNSATISFIED;
    }
    
  }
  if((game_nb_neighbors(g, i ,j, BLACK)==game_get_constraint(g, i ,j) && game_nb_neighbors(g, i ,j, WHITE)==MAX_CONSTRAINT-game_get_constraint(g, i ,j))&& game_nb_neighbors(g,i ,j, EMPTY)==0){
    g->statut[i+DEFAULT_SIZE*j]=SATISFIED;
  }
  if((game_nb_neighbors(g, i ,j, BLACK)<game_get_constraint(g, i ,j) && game_nb_neighbors(g, i ,j, WHITE)>MAX_CONSTRAINT-game_get_constraint(g, i ,j))){
    g->statut[i+DEFAULT_SIZE*j]=UNSATISFIED;
  }
  if((game_nb_neighbors(g, i ,j, BLACK)>game_get_constraint(g, i ,j) && game_nb_neighbors(g, i ,j, WHITE)<MAX_CONSTRAINT-game_get_constraint(g, i ,j))){
    g->statut[i+DEFAULT_SIZE*j]=ERROR;
  }
  if(game_get_constraint(g, i, j)==-1){
    g->statut[i+DEFAULT_SIZE*j]=SATISFIED;
  }
  
}


game game_new(constraint *constraints, color *colors)
{
  //création de game
    game g=malloc(sizeof(game)); 
    safeMalloc(g);

    //mise en place des conditions de game_new
    size_t size_color=sizeof(g->couleur);
    for(int j=0; j<DEFAULT_SIZE; j++){
        for(int i=0; i<DEFAULT_SIZE; i++){

          if(game_nb_neighbors(g, i, j, BLACK)==game_get_constraint(g, i ,j) || game_nb_neighbors(g, i, j, WHITE)==MAX_CONSTRAINT-game_get_constraint(g, i ,j)){
            g->statut[j+DEFAULT_SIZE*i]=SATISFIED;
          }
          else{
            g->statut[j+DEFAULT_SIZE*i]=UNSATISFIED;
          }
          g->contrainte[j+DEFAULT_SIZE*i]=constraints[i+DEFAULT_SIZE*j]; //à regarder pourquoi
          if(colors==NULL){
            
            memset(g->couleur, EMPTY, size_color);
          }
          else{
            g->couleur[j+DEFAULT_SIZE*i]=colors[i+DEFAULT_SIZE*j];
          }
        }
    }
    return g;
}
    

game game_new_empty(void)
{
  game g=malloc(sizeof(game)); 
  safeMalloc(g);

  size_t size_color=sizeof(g->couleur);
  for(int i=0; i<GAME_SIZE; i++){
    g->statut[i]=UNSATISFIED;
    memset(g->couleur, EMPTY, size_color);
    g->contrainte[i]=UNCONSTRAINED;
  }
  return g;
}


game game_copy(cgame g)
{
  gameConstNull(g);
  game g2=malloc(sizeof(game)); 
  safeMalloc(g2);
  for(int i=0; i<GAME_SIZE; i++){
    g2->contrainte[i]=g->contrainte[i];
    g2->statut[i]=g->statut[i];
    g2->couleur[i]=g->couleur[i];
  }
  
  return g2;
}


bool game_equal(cgame g1, cgame g2)
{
  gameConstNull(g2);
  gameConstNull(g1);
  /*size_t size_constraint=sizeof(g1->contrainte); 
  size_t size_color=sizeof(g1->couleur);
  size_t size_status=sizeof(g1->statut);

  for(int i=0; i<GAME_SIZE; i++){
      if(memcmp(g1->contrainte, g2->contrainte, size_constraint)!=0 || memcmp(g1->couleur, g2->couleur, size_color)!=0 || memcmp(g1->statut, g2->statut, size_status)!=0)
      {
        return false;
      }
    }*/
  for(int i=0; i<GAME_SIZE; i++){
    if(g2->contrainte[i]!=g1->contrainte[i] || g2->statut[i]!=g1->statut[i] || g2->couleur[i]!=g1->couleur[i]){
      return false;
    }
  }
  
  return true;
}


void game_delete(game g)
{
  gameNull(g);
  if(g!=NULL){
    free(g->couleur);
    free(g->contrainte);
    free(g->statut);
  }
  free(g);
}


void game_set_constraint(game g, uint i, uint j, constraint n)
{
  gameNull(g);
  checkIJ(i,j, "game_set_constraint");
  g->contrainte[i+DEFAULT_SIZE*j]=n;
}


void game_set_color(game g, uint i, uint j, color c)
{
  gameNull(g);
  checkIJ(i,j, "game_set_color");
  if (g != NULL) {
    g->couleur[i+DEFAULT_SIZE*j]=c;
  }
}


constraint game_get_constraint(cgame g, uint i, uint j)
{
  gameConstNull(g);
  checkIJ(i,j, "game_get,constraint");
  return g->contrainte[i+DEFAULT_SIZE*j];
}


color game_get_color(cgame g, uint i, uint j)
{
  gameConstNull(g);
  checkIJ(i,j, "game_get_color");
  return g->couleur[i+DEFAULT_SIZE*j];
}


bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next, uint *pj_next)
{
  gameConstNull(g);
  checkIJ(i,j, "game_get_next_square");
  if(g == NULL){
    fprintf(stderr, "Game NULL\n");
    return EXIT_FAILURE;
  }
  if((j==DEFAULT_SIZE-1 && ((dir == RIGHT) || (dir == UP_RIGHT) || (dir==DOWN_RIGHT)))
  || (i==DEFAULT_SIZE-1 && ((dir == DOWN) || (dir == DOWN_RIGHT) || (dir==DOWN_LEFT))) 
  || (j==0 && ((dir == LEFT) || (dir == DOWN_LEFT) || (dir==UP_LEFT)))
  || (i==0 && ((dir == UP) || (dir == UP_RIGHT) || (dir==UP_LEFT)))){
    return false;
  }else{
  } 

  switch (dir)
  {
  case UP:
    *pi_next = i-1;
    *pj_next = j;
    break;
  case DOWN:
    *pi_next = i+1;
    *pj_next = j;
    break;
  case LEFT:
    *pi_next = i;
    *pj_next = j-1;
    break;
  case RIGHT:
    *pi_next = i;
    *pj_next = j+1;
    break;
  case UP_LEFT:
    *pi_next = i-1;
    *pj_next = j-1;
    break;
  case UP_RIGHT:
    *pi_next = i-1;
    *pj_next = j+1;
    break;
  case DOWN_LEFT:
    *pi_next = i+1;
    *pj_next = j-1;
    break;
  case DOWN_RIGHT:
    *pi_next = i+1;
    *pj_next = j+1;
    break;
  case HERE:
    *pi_next = i;
    *pj_next = j;
    break;
  default:
    break;
  }

  return true;
}


status game_get_status(cgame g, uint i, uint j)
{
  gameConstNull(g);
  checkIJ(i,j, "game_get_status");
  return g->statut[i+DEFAULT_SIZE*j];
}


int game_nb_neighbors(cgame g, uint i, uint j, color c)
{
  gameConstNull(g);
  
  checkIJ(i,j, "game_nb_neighbors");
  uint count=0;
  uint pi_next[DEFAULT_SIZE+2];
  uint pj_next[DEFAULT_SIZE+2];
  
  if(game_get_next_square(g, i, j, HERE, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  if(game_get_next_square(g, i, j, UP, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  if(game_get_next_square(g, i, j, DOWN, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  if(game_get_next_square(g, i, j, LEFT, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  if(game_get_next_square(g, i, j, RIGHT, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  if(game_get_next_square(g, i, j, UP_LEFT, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  if(game_get_next_square(g, i, j, UP_RIGHT, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  if(game_get_next_square(g, i, j, DOWN_LEFT, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  if(game_get_next_square(g, i, j, DOWN_RIGHT, pi_next, pj_next)==true){
    if(game_get_color(g, *pi_next, *pj_next)==c){
      count++;
    }
  }
  return count;
}


void game_play_move(game g, uint i, uint j, color c)
{
  gameNull(g);
  checkIJ(i,j, "game_play_move");
  game_set_color(g, i, j, c);

  putStatus(g,i,j);

  //Ajouter que cela valide aussi si les voisins de i j peuvent être satisfait
  uint pi_next[DEFAULT_SIZE+2];
  uint pj_next[DEFAULT_SIZE+2];
  if(game_get_next_square(g, i, j, HERE, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  
  }
  if(game_get_next_square(g, i, j, UP, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  }
  if(game_get_next_square(g, i, j, DOWN, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  }
  if(game_get_next_square(g, i, j, LEFT, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  }
  if(game_get_next_square(g, i, j, RIGHT, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  }
  if(game_get_next_square(g, i, j, UP_LEFT, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  }
  if(game_get_next_square(g, i, j, UP_RIGHT, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  }
  if(game_get_next_square(g, i, j, DOWN_LEFT, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  }
  if(game_get_next_square(g, i, j, DOWN_RIGHT, pi_next, pj_next)==true){
    putStatus(g,*pi_next,*pj_next);
  }
}


bool game_won(cgame g)
{
  gameConstNull(g);
  for(int j=0; j<DEFAULT_SIZE; j++){
    for(int i=0; i<DEFAULT_SIZE; i++){
      if(game_get_color(g, i, j)==EMPTY || game_get_status(g, i, j)!=SATISFIED){
        return false;
      }
    }
  }
  return true;
}


void game_restart(game g)
{
  gameNull(g);
  for(int j=0; j<DEFAULT_SIZE; j++){
    for(int i=0; i<DEFAULT_SIZE; i++){
      game_set_color(g, i, j, EMPTY);
    }
  }
}
