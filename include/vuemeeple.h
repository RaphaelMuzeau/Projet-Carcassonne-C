#ifndef vuemeeple_h
#define vuemeeple_h

#include "raylib.h"
#include "plateau.h"
#include "jeu.h"

/* Conserve la position d'un meeple
 * sur le plateau */
typedef struct _PlacementMeeple {
    Vector2 position;
    Color couleur;
} PlacementMeeple;

PlacementMeeple creer_placement(Jeu *jeu, int x, int y);

#define PLACEMENT_REALLOC_SIZE 8 // nombre de placements ajoutés pour agrandir la liste
_Static_assert(PLACEMENT_REALLOC_SIZE > 0, "PLACEMENT_REALLOC_SIZE doit etre supérieur à 0");

/* Liste de placements à taille variable
 * dont la memoire du tableau est géré
 * similairement à une varstring */
typedef struct _ListePlacements {
    PlacementMeeple *tab;
    int len;
    int capacite;
} ListePlacements;

ListePlacements creer_listeplacements(void);
void detruire_listeplacements(ListePlacements placements);
void rafraichir_listeplacements(ListePlacements *placements, Jeu *jeu);
void dessiner_listeplacements(ListePlacements placements, Plateau plateau);

#endif
