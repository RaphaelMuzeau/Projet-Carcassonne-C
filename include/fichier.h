#ifndef fichier_h
#define fichier_h

#include <stdbool.h>
#include <stdio.h>
#include "vec.h"
#include "pile.h"

void sauvegarder_grille(Vec2D *g, FILE *f);
Vec2D charger_grille(FILE *f);

void sauvegarder_pile(Pile p, FILE *f);
Pile charger_pile(FILE *f);

#endif
