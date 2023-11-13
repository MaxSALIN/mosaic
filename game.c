#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

/**
 * @brief Standard unsigned integer type.
 **/
typedef unsigned int uint;

/**
 * @brief Size of the default game grid.
 **/
#define DEFAULT_SIZE 5

/** square colors */
typedef enum
{
  EMPTY, /**< empty square */
  WHITE, /**< white square */
  BLACK  /**< blacksquare */
} color;

/** square constraints
 * @details These constants are numerical constraints that indicate the expected
 * number of black squares in the 3x3 neighborhood of a given square (including
 * itself). All valid constant numbers are between 0 and 9. The special constant
 * UNCONSTRAINED (-1) indicates that there is no particular constraint on a
 * square, i.e. there can be any number of black squares in its neighborhood.
 *
 * @warning Values from 0 to 9 are not explicitly listed here, but can be used
 * normally with this enumeration type. For example, `constraint n = 3` is
 * correct.
 **/
typedef enum
{
  UNCONSTRAINED = -1,  /**< unconstrained square */
  MIN_CONSTRAINT = -1, /**< minimum square constraint */
  MAX_CONSTRAINT = 9   /**< maximum square constraint */
} constraint;
/** square status
 * @details These constants are used to indicate the status of a square in the
 * grid, that can be either ERROR, SATISFIED or UNSATISFIED  depending on the
 * colors of its 3x3 neighborhood (including the square itself). More precisely,
 * the ERROR status indicates that the number of black (or white) squares in the
 * neighborhood is too high, so that the numerical square constraint cannot be
 * met. The SATISFIED status indicates that the number of black squares in the
 * neighborhood is equal to the numerical square constraint, and that the other
 * squares are white, i.e. there are no more EMPTY squares in the 3x3
 * neighborhood. Finally, the status UNSATISFIED indicates that the numerical
 * constraint has not yet been satisfied, but may still be. In this case, there
 * are still EMPTY squares in the neighborhood, but there are no obvious errors.
 * 
 * @warning All theses statuses are mutually exclusive, i.e. a square cannot be
 * both ERROR and UNSATISFIED.
**/
typedef enum
{
  ERROR,       /**< error status */
  UNSATISFIED, /**< unsatisfied status */
  SATISFIED    /**< satisfied status */
} status;
/**
 * @brief All possible directions in the game grid to explore the neighbourhood
 * of a given square.
 **/
typedef enum
{
  HERE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT
} direction;

/**
 * @brief The structure pointer that stores the game state.
 **/
typedef struct game_s *game{
    color* couleur;
    constraint* contrainte;
    status* statut;
    uint* tableau;
}