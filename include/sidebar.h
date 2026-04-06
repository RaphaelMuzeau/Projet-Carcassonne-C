#ifndef sidebar_h
#define sidebar_h

#include "joueur.h"
#include "scrollbar.h"

#define CONTROLES_PADDING 20.0f
#define CONTROLES_APERCU_SIZE 128.0f
#define CONTROLES_HEIGHT (CONTROLES_APERCU_SIZE + 2 * CONTROLES_PADDING)

#define CARTEJOUEUR_WIDTH 250.0f
#define CARTEJOUEUR_HEIGHT 100.0f

#define SIDEBAR_WIDTH (CARTEJOUEUR_WIDTH + SCROLL_DEFAULT_WIDTH + 20.0f)

typedef struct _Controles {
    Rectangle champ;
    Rectangle apercu;
    Bouton rotation;
    Bouton detruire;
} Controles;

Controles creer_controles(void);
void update_controles(Controles *ctrl);
void dessiner_controles(Controles ctrl, RenderTexture2D render_tuile);

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
void rafraichir_cartejoueur(CarteJoueur *carte);
void dessiner_cartejoueur(CarteJoueur carte, bool tour);

typedef struct _BarreJoueurs {
    CarteJoueur *cartes;
    Rectangle champ;
    ListeJoueurs joueurs;
    ScrollBar scrollbar;
    float fin_liste;
} BarreJoueurs;

BarreJoueurs creer_barrejoueurs(ListeJoueurs joueurs);
void detruire_barrejoueurs(BarreJoueurs barre);

void update_barrejoueurs(BarreJoueurs *barre);
// met à jour le compteur de points et de meeple de chaque joueur
void rafraichir_barrejoueurs(BarreJoueurs *barre);
void dessiner_barrejoueurs(BarreJoueurs barre, int id_tour);

#endif
