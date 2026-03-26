#ifndef grille_h
#define grille_h

#include "tuile.h"
#include "vec.h"
#include "jeu.h"

bool placer_tuile(Vec2D grille, int x, int y, Tuile piece);
bool est_vide(Vec2D grille, int x, int y);
int recherche(Vec2D grille, int x, int y, char nb_meeples[], enum Zone z, enum Direction d);
void recherche_is_verified(Vec2D grille, int x, int y);
int maximal(int nb_meeples[], int taille);
#endif
