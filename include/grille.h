#ifndef grille_h
#define grille_h

#include "tuile.h"

typedef struct _Grille {
    int taille;
    Tuile **tableau;
} Grille;

Grille init_grille(int taille);
void destruction_grille(Grille grille);

bool est_vide(Grille grille, int x, int y);

#endif
