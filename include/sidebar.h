#ifndef sidebar_h
#define sidebar_h

#include "scrollbar.h"

#define CONTROLES_PADDING 20
#define CONTROLES_APERCU_SIZE 128
#define CONTROLES_HEIGHT (CONTROLES_APERCU_SIZE + 2 * CONTROLES_PADDING)

#define CARTEJOUEUR_WIDTH 250
#define CARTEJOUEUR_HEIGHT 100

#define SIDEBAR_WIDTH (CARTEJOUEUR_WIDTH + SCROLL_DEFAULT_WIDTH + 20.0f)

typedef struct _Controles {
    Rectangle champ;
    Rectangle apercu;
    Bouton rotation;
    Bouton detruire;
} Controles;

Controles creer_controles(void);
void update_controles(Controles *ctrl);
void dessiner_controles(Controles ctrl);

typedef struct _Joueur {
    char *nom;
    char id;
    int pts;
    int nb_meeple_restant;
} Joueur;

typedef struct _CarteJoueur {
    Joueur *joueur;
    Color couleur;
    Rectangle champ;
    Texte texte_nom;
    Texte texte_pts;
    Texte texte_meeple;
} CarteJoueur;

CarteJoueur creer_cartejoueur(float x, float y, Joueur *joueur, Color couleur);
void detruire_cartejoueur(CarteJoueur carte);
void update_cartejoueur(CarteJoueur *carte);
void dessiner_cartejoueur(CarteJoueur carte, bool tour);

#endif
