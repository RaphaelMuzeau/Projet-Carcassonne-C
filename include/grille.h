#ifndef grille_h
#define grille_h

#include "tuile.h"
#include "vec.h"

bool placer_tuile(Vec2D grille, int x, int y, Tuile piece);
bool est_vide(Vec2D grille, int x, int y);

#endif
