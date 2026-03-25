#ifndef scrollbar_h
#define scrollbar_h

#define SCROLL_BACKGROUND_COLOR BLACK

#include "raylib.h"
#include "bouton.h"

typedef struct _ScrollBar {
    Camera2D camera; // espace que la barre est responsable de scroller
    Bouton bouton;
    bool est_tire; // l'utilisateur tire-t-il le bouton ?
} ScrollBar;

// requiere le contexte OpenGL.
ScrollBar creer_scrollbar(Camera2D camera, float largeur);
void update_scrollbar(ScrollBar *barre, float fin_espace);
void dessiner_scrollbar(ScrollBar barre);

#endif
