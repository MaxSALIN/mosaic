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

#endif
