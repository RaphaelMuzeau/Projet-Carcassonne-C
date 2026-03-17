#ifndef fichier_h
#define fichier_h

#include <stdbool.h>
#include <stdio.h>
#include "vec.h"

// GRILLE
int ecrire_grille(Vec2D *g, int x, int y, FILE *f);
bool sauvegarder_grille(Vec2D *g, FILE *f);
bool charger_grille(Vec2D *g, FILE *f);

#endif
