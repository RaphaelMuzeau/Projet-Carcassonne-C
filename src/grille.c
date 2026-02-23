#include <stdlib.h>
#include "grille.h"
#include "tuile.h"

Grille init_grille(int taille)
{
    Grille grille;
    grille.taille = taille;
    grille.tableau = (Tuile **) malloc(taille*sizeof(Tuile *));

    for(int i = 0; i < taille; i++) {
        grille.tableau[i] = (Tuile *) malloc(taille*sizeof(Tuile));
        for(int j = 0; j < taille; j++)
            grille.tableau[i][j] = NULL;
    }

    return grille;
}

void destruction_grille(Grille grille)
{
    int taille = grille.taille;
    int i;

    for(i = 0; i < taille; i++) {
        free(grille.tableau[i]);
    }
    free(grille.tableau);
}