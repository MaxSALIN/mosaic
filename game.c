#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"

void gameNull(game g);
#define GAME_SIZE (DEFAULT_SIZE*DEFAULT_SIZE)

/**
 * @brief The structure pointer that stores the game state.
 **/
struct game_s{
    color *couleur;
    constraint *contrainte;
    status *statut;

};
typedef struct game_s *game;

/**
 * @brief The structure constant pointer that stores the game state.
 * @details That means that it is not possible to modify the game using this
 * pointer.
 **/
typedef const struct game_s *cgame;

/**
 * @brief Creates a new game with default size and initializes it.
 *
 * @param constraints an array describing the initial constraint of each square using
 * row-major storage (of size DEFAULT_SIZE squared)
 * @param colors an array describing the initial color of each square using
 * row-major storage  (of size DEFAULT_SIZE squared) or NULL to set all colors
 * as EMPTY
 * @pre @p constraints must be an initialized array of default size squared
 * @pre @p colors must be an initialized array of default size squared or NULL
 * @return the created game
 **/
game game_new(constraint *constraints, color *colors)
{
//création de game et vérification de sa création
    game g=malloc(sizeof(game));    
    if(g==NULL || constraints==NULL){
        fprintf(stderr, "Game NULL or inserted parameters in game_new\n");
        exit(EXIT_FAILURE);
    }

     //création des paramètres de game et vérification de leurs créations
    g->couleur=malloc(sizeof(*(g->couleur))*GAME_SIZE);  
    g->contrainte=malloc(sizeof(*(g->contrainte))*GAME_SIZE);
    g->statut=malloc(sizeof(*(g->statut))*GAME_SIZE);
    if(g->couleur==NULL || g->contrainte==NULL || g->statut==NULL){  //safe malloc fonction
        fprintf(stderr, "Game parameters NULL in game_new\n");
        exit(EXIT_FAILURE);
    }

    //mise en place des conditions de game_new
    for(int i=0; i<GAME_SIZE; i++){
        size_t size_constraint=sizeof(g->contrainte); 
        size_t size_color=sizeof(g->couleur);

        // occuper de status
        memcpy(g->contrainte, constraints, size_constraint);
        if(colors==NULL){
          
          memset(g->couleur, EMPTY, size_color);
        }
        else{
          memcpy(g->couleur, colors, size_color);
        }
    }
    return g;
}
    

/**
 * @brief Creates a new empty game with defaut size.
 *
 * @details All squares are initialized with unconstrained squares, with empty
 * color.
 * @return the created game
 **/
game game_new_empty(void)
{
  game g=malloc(sizeof(game));    
  if(g==NULL){
      fprintf(stderr, "Game NULL or inserted parameters in game_new\n");
      exit(EXIT_FAILURE);
  }

   //création des paramètres de game et vérification de leurs créations
  g->couleur=malloc(sizeof(*(g->couleur))*GAME_SIZE);  
  g->contrainte=malloc(sizeof(*(g->contrainte))*GAME_SIZE);
  g->statut=malloc(sizeof(*(g->statut))*GAME_SIZE);
  if(g->couleur==NULL || g->contrainte==NULL || g->statut==NULL){  //safe malloc fonction
      fprintf(stderr, "Game parameters NULL in game_new\n");
      exit(EXIT_FAILURE);
  }
  size_t size_color=sizeof(g->couleur);
  for(int i=0; i<GAME_SIZE; i++){
    g->statut[i]=UNSATISFIED;
    memset(g->couleur, EMPTY, size_color);
    g->contrainte[i]=UNCONSTRAINED;
  }
  return g;
}

/**
 * @brief Duplicates a game.
 *
 * @param g the game to copy
 * @return the copy of the game
 * @pre @p g must be a valid pointer toward a game structure.
 **/
game game_copy(cgame g)
{
  return EXIT_SUCCESS;
}

/**
 * @brief Tests if two games are equal.
 *
 * @param g1 the first game
 * @param g2 the second game
 * @return true if the two games are equal, false otherwise
 * @pre @p g1 must be a valid pointer toward a game structure.
 * @pre @p g2 must be a valid pointer toward a game structure.
 **/
bool game_equal(cgame g1, cgame g2)
{
  return EXIT_SUCCESS;
}

/**
 * @brief Deletes the game and frees the allocated memory.
 * @param g the game to delete
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_delete(game g)
{
  /*gameNull(g);
  if(g!=NULL){
    free(g->couleur);
    free(g->contrainte);
    free(g->statut);
  }
  free(g);*/
}

/**
 * @brief Sets the constraint of a given square.
 *
 * @details This function is useful for initializing the squares of an empty
 * game.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param n the square constraint (or -1 if unconstrained square)
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre @p n >= MIN_CONSTRAINT and @p n <= MAX_CONSTRAINT
 **/
void game_set_constraint(game g, uint i, uint j, constraint n)
{
  g->contrainte[i+DEFAULT_SIZE*j]=n;
}

/**
 * @brief Sets the color of a given square.
 *
 * @details This function is useful for initializing the squares of an empty
 * game.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param c the square color
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre @p c must be a valid color (BLACK, WHITE or EMPTY)
 **/
void game_set_color(game g, uint i, uint j, color c)
{
  g->couleur[i+DEFAULT_SIZE*j]=c;
}

/**
 * @brief Gets the constraint of a given square.
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the square constraint or -1 if unconstrained square
 **/
constraint game_get_constraint(cgame g, uint i, uint j)
{

  return g->contrainte[i+DEFAULT_SIZE*j];
}

/**
 * @brief Gets the color of a given square.
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the square color
 **/
color game_get_color(cgame g, uint i, uint j)
{
  return g->couleur[i+DEFAULT_SIZE*j];
}

/**
 * @brief Gets the coordinate of the next square in a given direction.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param dir the direction
 * @param[out] pi_next the row index of the next square (output)
 * @param[out] pj_next the column index of the next square (output)
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return true if the next square is inside the grid, false otherwise
 */
bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next, uint *pj_next)
{
  if(i>DEFAULT_SIZE || j>DEFAULT_SIZE){
    fprintf(stderr, "i ou j mauvais\n");
    return EXIT_FAILURE;
  }
  if(g == NULL){
    fprintf(stderr, "Game NULL\n");
    return EXIT_FAILURE;
  }
  if((j==DEFAULT_SIZE-1 && ((dir == RIGHT) || (dir == UP_RIGHT) || (dir==DOWN_RIGHT)))
  || (i==DEFAULT_SIZE-1 && ((dir == DOWN) || (dir == DOWN_RIGHT) || (dir==DOWN_LEFT))) 
  || (j==0 && ((dir == LEFT) || (dir == DOWN_LEFT) || (dir==UP_LEFT)))
  || (i==0 && ((dir == UP) || (dir == UP_RIGHT) || (dir==UP_LEFT)))){
    printf("dir mauvais %d %d %d\n",i,j,dir);
    return false;
  }else{
    printf("dir correst %d %d %d\n",i,j, dir);
  } 

  switch (dir)
  {
  case UP:
    *pi_next = i;
    *pj_next = j-1;
    break;
  case DOWN:
    *pi_next = i;
    *pj_next = j+1;
    break;
  case LEFT:
    *pi_next = i-1;
    *pj_next = j;
    break;
  case RIGHT:
    *pi_next = i+1;
    *pj_next = j;
    break;
  case UP_LEFT:
    *pi_next = i-1;
    *pj_next = j-1;
    break;
  case UP_RIGHT:
    *pi_next = i+1;
    *pj_next = j-1;
    break;
  case DOWN_LEFT:
    *pi_next = i-1;
    *pj_next = j+1;
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

/**
 * @brief Gets the square status.
 *
 * @param g the game
 * @param i the row index
 * @param j the column index
 * @return the square status, either ERROR, UNSATISFIED or SATISFIED.
 */
status game_get_status(cgame g, uint i, uint j)
{
  return g->statut[i+DEFAULT_SIZE*j];
  
}

/**
 * @brief Returns the number of neighbors of the same color.
 *
 * @details This function counts the number of squares of color @p c in the 3x3
 * neighborhood of the square at coordinate (i, j), including the square itself
 * in this count.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param c the choosen color
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the number of neighbors of the same color
 */
int game_nb_neighbors(cgame g, uint i, uint j, color c)
{
  return EXIT_SUCCESS;
}

/**
 * @brief Plays a move in a given square.
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @param c the color to play
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre @p c must be a valid color (BLACK, WHITE or EMPTY)
 **/
void game_play_move(game g, uint i, uint j, color c)
{
  game_set_color(g, i, j, c);
  if(game_nb_neighbors(g, i ,j, BLACK)==game_get_constraint(g, i ,j)){
      g->statut[i+DEFAULT_SIZE*j]=SATISFIED;
  }
}

/**
 * @brief Checks if the game is won.
 *
 * @param g the game
 * @details This function checks that all the game rules are satisfied, i.e. all
 * squares have color WHITE or BLACK with status SATISFIED.
 * @return true if the game is won, false otherwise
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 **/
bool game_won(cgame g)
{
  for(int j=0; j<DEFAULT_SIZE; j++){
    for(int i=0; i<DEFAULT_SIZE; i++){
      if(game_get_color(g, i, j)==EMPTY || game_get_status(g, i, j)!=SATISFIED){
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Restarts a game.
 *
 * @details All the game is reset to its initial state. In particular, all the
 * colors are removed.
 * @param g the game
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_restart(game g)
{

}