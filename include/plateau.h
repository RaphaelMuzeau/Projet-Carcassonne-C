#ifndef plateau_h
#define plateau_h

#include "render.h"
#include "bouton.h"
#include "jeu.h"

/* Le Plateau est une vue scrollable et zoomable sur toute la grille,
 * il gere le placement graphique de tuiles, l'affichage de la tuile courante
 * sous le curseur, et le placement graphique de meeple
 */
typedef struct _Plateau {
    Rectangle vue;
    Camera2D camera;
    Chunk *chunks;
    int nb_chunks;
    int max_chunks;
    Vector2 pos_tuile; // position de la tuile courante
    bool placement;    // un placement est-il en cours ?
    Bouton nord, sud, est, ouest, milieu, aucun; // boutons de placement de meeple
} Plateau;

Plateau creer_plateau(Jeu *jeu, Texture spritesheet);
void detruire_plateau(Plateau plateau);

void dessiner_plateau(Plateau plateau, RenderTexture2D render_tuile, float rotation);
void placer_render_tuile(Plateau *plateau, RenderTexture2D render, int x, int y, float rotation);

/* informations necessaires au placement d'une tuile */
typedef struct _Placement {
    int x;
    int y;
    enum Direction position_meeple;
    bool placer_meeple;
} Placement;

// Met à jour le plateau et renvoit le placement décidé par le joueur.
// renvoit un placement avec (x, y) = (0, 0) si le joueur
// n'a pas encore décidé de placer une tuile.
Placement update_plateau(Plateau *plateau);

#endif
