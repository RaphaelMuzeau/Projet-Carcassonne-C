#include <stdlib.h>
#include <stdbool.h>
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
    int i, j;

    for(i = 0; i < taille; i++) {
        for (j = 0; j < taille; j++)
            free(grille.tableau[i][j]);
        free(grille.tableau[i]);
    }
    free(grille.tableau);
}

bool placer_tuile(Grille grille, int x, int y, Tuile piece)
{
    // tuile hors champ
    if (x < 0 || x >= grille.taille || y < 0 || y >= grille.taille)
        return false;

    // tuile occupé
    if (!est_vide(grille,x,y))
        return false;

    // tuile sans connexion
    if (est_vide(grille, x-1, y) && est_vide(grille, x+1, y) && est_vide(grille, x, y-1) && est_vide(grille, x, y+1))
        return false;

    if (compatibilite_tuile(grille.tableau[x][y],grille.tableau[x-1][y],D_OUEST)
        && compatibilite_tuile(grille.tableau[x][y],grille.tableau[x+1][y],D_EST)
        && compatibilite_tuile(grille.tableau[x][y],grille.tableau[x][y+1],D_NORD)
        && compatibilite_tuile(grille.tableau[x][y],grille.tableau[x][y-1],D_SUD)) {
        grille.tableau[x][y] = piece;
        return true;
    }
    return false;
}

bool est_vide(Grille grille, int x, int y)
{
    // Une case hors champs est considéré comme vide
    return x < 0 || x >= grille.taille || y < 0 || y >= grille.taille || grille.tableau[x][y] == NULL;
}
