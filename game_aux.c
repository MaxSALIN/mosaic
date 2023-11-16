#ifndef __GAME_AUX_H__
#define __GAME_AUX_H__

#include "game.h"
#include "game_aux.h"


/**
 * @brief Prints a game as text on the standard output stream.
 * @details The different squares are respectively displayed as text, as
 * described in @ref index.
 * @param g the game
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_print(cgame g) {
    // Afficher les indices des colonnes en haut
    printf("   ");
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        printf("%d  ", i);
    }
    printf("\n");

    // Afficher les indices des lignes et les éléments du tableau
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        // Afficher l'indice de la ligne sur le côté gauche
        printf("%d |", i);

        // Afficher les éléments du tableau
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            printf(" %d ", g.tableau[i][j]);
        }

        printf("\n");
    }
}

/**
 * @brief Creates the default game.
 * @details See the description of the default game in @ref index.
 * @return the created game
 **/
game game_default(void) {

}

/**
 * @brief Creates the default game solution.
 * @details See the description of the default game in @ref index.
 * @return the created game
 **/
game game_default_solution(void) {
    
}

#endif  // __GAME_AUX_H__
