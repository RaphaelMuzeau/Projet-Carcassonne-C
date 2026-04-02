#ifndef fichier_h
#define fichier_h

#include <stdbool.h>
#include <stdio.h>
#include "vec.h"
#include "pile.h"
#include "joueur.h"

void sauvegarder_grille(Vec2D *g, FILE *f);
Vec2D charger_grille(FILE *f);

void sauvegarder_pile(Pile p, FILE *f);
Pile charger_pile(FILE *f);

void sauvegarder_liste_joueurs(ListeJoueurs tab, FILE *f);
ListeJoueurs charger_liste_joueurs(FILE *f);

void sauvegarder_joueur(Joueur joueur, FILE *f);
Joueur charger_joueur(FILE *f);

#endif
