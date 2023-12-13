#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_ext.h"
#include "game_aux.h"
#include "queue.h"

typedef struct game_s game_s;
typedef struct move_s move;
typedef game_s * game;

struct game_s {
  color *couleur;
  constraint *contrainte;
  status *statut;
  neighbourhood neighbour;
  bool wrapping;
  uint row;
  uint col;
  queue *queue;
  queue *queue_saves;
};

struct move_s {
  uint i;
  uint j;
  color c;
};

#endif
