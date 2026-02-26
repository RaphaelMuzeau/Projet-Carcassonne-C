#ifndef grille_h
#define grille_h

#include "tuile.h"

typedef struct _Grille {
    Tuile **tableau;
    int taille;
} Grille;

Grille init_grille(int taille);
void destruction_grille(Grille grille);

bool placer_tuile(Grille grille, int x, int y, Tuile piece);
bool est_vide(Grille grille, int x, int y);

#endif
