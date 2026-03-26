#ifndef grille_h
#define grille_h

#include "meeple.h"
#include "tuile.h"
#include "vec.h"

bool placer_tuile(Vec2D *grille, int x, int y, Tuile piece);

int recherche(Vec2D grille, int *nb_meeples, L_meeple *loc_meeple_all, int x, int y, enum Zone z, enum Direction d);
void recherche_is_verified(Vec2D grille, int x, int y);

#endif
