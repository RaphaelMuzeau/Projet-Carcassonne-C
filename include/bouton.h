#ifndef bouton_h
#define bouton_h

#include <stdbool.h>
#include "raylib.h"
#include "texte.h"

#define BOUTON_COLOR_NEUTRAL GRAY
#define BOUTON_COLOR_HOVERED DARKGRAY
#define BOUTON_COLOR_PUSHED  MAROON

// un Bouton s'adapte à la taille de son texte
typedef struct _Bouton {
    Rectangle champ;
    Color couleur;
    Texte texte;
} Bouton;

// requiere le contexte OpenGL.
// Le bouton doit avoir été mis à jour au moins une fois
// avant de pouvoir être dessiné.
Bouton creer_bouton(float x, float y, char *texte);

bool update_bouton(Bouton *b);
void dessiner_bouton(Bouton b);

#endif
