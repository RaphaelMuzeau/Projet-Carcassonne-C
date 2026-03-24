#ifndef champsaisie_h
#define champsaisie_h

#include <raylib.h>
#include <varstring.h>
#include "texte.h"

#define CHAMP_BORDER_SIZE 4

typedef struct _ChampSaisie {
    Rectangle champ;
    VarString saisie;
    Texte texte;
    RenderTexture vue;
    bool focus;
    bool numerique;
} ChampSaisie;

ChampSaisie creer_champsaisie(float x, float y, float largeur, float hauteur, bool est_numerique);
void detruire_champsaisie(ChampSaisie cs);
void update_champsaisie(ChampSaisie *cs);
void update_champsaisie_camera(ChampSaisie *cs, Camera2D camera);
void dessiner_champsaisie(ChampSaisie cs);

#endif
