#ifndef controles_h
#define controles_h

#include "raylib.h"
#include "bouton.h"

#define CONTROLES_DEFAULT_PADDING 20

typedef struct _Controles {
    Rectangle champ;
    Rectangle apercu;
    Bouton rotation;
    Bouton detruire;
} Controles;

Controles creer_controles(void);
void update_controles(Controles *ctrl);
void dessiner_controles(Controles ctrl);

#endif
