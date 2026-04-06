#ifndef gentest_h
#define gentest_h

#include "vec.h"

Vec2D generer_recherche_ville_incomplete(void);
Vec2D generer_recherche_ville_complete(void);
Vec2D generer_recherche_ville_blason(void);
Vec2D generer_route_village(void);
Vec2D generer_route_ville(void);
Vec2D generer_recherche_abbaye_complete(void);
Vec2D generer_recherche_abbaye_non_complete(void);

void generer_fichier_grille(void);
void generer_fichier_pile(void);
void generer_fichier_joueur(void);
void generer_fichier_joueur_vide(void);
void generer_fichier_liste_joueurs(void);
void generer_fichier_pile_aleatoire(void);

#endif
