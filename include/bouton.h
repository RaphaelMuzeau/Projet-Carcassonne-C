#ifndef bouton_h
#define bouton_h

#include <stdbool.h>
#include "raylib.h"
#include "texte.h"

#define BOUTON_COLOR_NEUTRAL LIGHTGRAY
#define BOUTON_COLOR_HOVERED GRAY
#define BOUTON_COLOR_PUSHED  DARKGRAY

typedef struct _Bouton {
    Rectangle champ;
    Color couleur;
    Texte texte;
} Bouton;

// requiere le contexte OpenGL.
Bouton creer_bouton(float x, float y, char *texte);
Bouton creer_bouton_adapte(float x, float y, char *texte);

// adapte la taille du bouton et centre le texte
// en laissant une marge de 10px sur chaque bord.
void adapter_bouton(Bouton *b);
// centre le texte dans le bouton sans changer sa taille.
// si le texte est trop grand, il depassera.
void centrer_texte(Bouton *b);

// modifie l'apparence du bouton selon l'interaction du curseur.
// renvoit vrai si le bouton a été cliqué.
bool update_bouton(Bouton *b);
// update un bouton present dans un espace 2D.
// (par exemple un espace scrollable)
bool update_bouton_camera(Bouton *b, Camera2D camera);

bool update_bouton_adapte(Bouton *b);
bool update_bouton_centre(Bouton *b);

void dessiner_bouton(Bouton b);

#endif
