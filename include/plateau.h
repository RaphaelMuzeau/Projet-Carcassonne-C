#ifndef plateau_h
#define plateau_h

#include "render.h"
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
} Plateau;

Plateau creer_plateau(Jeu *jeu, Texture spritesheet);
void detruire_plateau(Plateau plateau);

void update_plateau(Plateau *plateau);
void dessiner_plateau(Plateau plateau, RenderTexture2D render_tuile, float rotation);

void placer_render_tuile(Plateau *plateau, RenderTexture2D render, Vector2 position, float rotation);

#endif
