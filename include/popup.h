#ifndef popup_h
#define popup_h

#include "raylib.h"
#include "bouton.h"
#include "champsaisie.h"

typedef struct _Popup {
    Rectangle champ;
    ChampSaisie champ_partie;
    Bouton annuler;
    Bouton sauvegarder;
    Bouton quitter;
} Popup;

Popup creer_popup(void);
void centrer_popup(Popup *p);
void dessiner_popup(Popup p);

#endif
