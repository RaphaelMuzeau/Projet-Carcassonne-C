#ifndef listeparties
#define listeparties

#include "raylib.h"
#include "bouton.h"

#define LISTEPARTIES_SPACING 10.0f // espace en pixels entre chaque bouton

/* Liste verticale de boutons de parties,
 * Les boutons occupent 80% de la largeur de l'ecran. */
typedef struct _ListeParties {
    FilePathList fichiers;
    Bouton *boutons;
    float fin_liste; // point y delimitant graphiquement la fin de la liste
} ListeParties;

// requiere le contexte OpenGL.
ListeParties creer_listeparties(float y);
void detruire_listeparties(ListeParties parties);

// renvoit le chemin du fichier d'une partie si l'une d'entre
// elles est selectionnée, NULL sinon.
char *update_listeparties(ListeParties parties, Camera2D camera);
void dessiner_listeparties(ListeParties parties);

#endif
