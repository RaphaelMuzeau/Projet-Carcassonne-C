#ifndef grille_h
#define grille_h

#include "meeple.h"
#include "joueur.h"
#include "tuile.h"
#include "vec.h"

bool placer_tuile(Vec2D *grille, int x, int y, Tuile t);

int recherche(Vec2D grille, int *nb_meeple, L_meeple *loc_meeple, int x, int y, enum Zone z, enum Direction d, bool fin);
void recherche_est_verifie(Vec2D grille, int x, int y);

void verification_abbaye(Vec2D grille, ListeJoueurs joueurs, int x, int y);
void recherche_abbaye(Vec2D grille, ListeJoueurs joueurs, int x, int y, bool fin);

bool placer_meeple(Vec2D grille, Joueur *joueur, int x, int y, enum Direction d);
void retirer_meeple(Vec2D grille, ListeJoueurs listejoueurs, int x, int y);

#endif
