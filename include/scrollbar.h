#ifndef scrollbar_h
#define scrollbar_h

#define SCROLL_BACKGROUND_COLOR BLACK
#define SCROLL_DEFAULT_WIDTH 20.0f

#include "raylib.h"
#include "bouton.h"

typedef struct _ScrollBar {
    Camera2D vue; // espace que la barre est responsable de scroller
    Bouton bouton;
    bool est_tire; // l'utilisateur tire-t-il le bouton ?
    float offset_tire; // la position du curseur dans le bouton
} ScrollBar;

// requiere le contexte OpenGL.
ScrollBar creer_scrollbar(Camera2D vue);
void update_scrollbar(ScrollBar *barre, Rectangle ecran, float hauteur_espace);
void dessiner_scrollbar(ScrollBar barre, Rectangle ecran);

#endif
