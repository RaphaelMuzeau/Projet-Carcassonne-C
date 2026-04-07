#ifndef render_h
#define render_h

#include "raylib.h"
#include "tuile.h"

#define TEXTURE_SIZE 256
_Static_assert(TEXTURE_SIZE > 0, "TEXTURE_SIZE doit etre supérieur à 0");
_Static_assert((TEXTURE_SIZE & (TEXTURE_SIZE - 1)) == 0, "TEXTURE_SIZE doit etre une puissance de 2");

#define CHUNK_SIZE (TEXTURE_SIZE * 32)
_Static_assert(CHUNK_SIZE > 0, "CHUNK_SIZE doit etre supérieur à 0");
_Static_assert(CHUNK_SIZE % TEXTURE_SIZE == 0, "CHUNK_SIZE doit etre un multiple de TEXTURE_SIZE");
_Static_assert((CHUNK_SIZE & (CHUNK_SIZE - 1)) == 0, "CHUNK_SIZE doit etre une puissance de 2");

#define PRE_COLOR (Color) { 118, 151, 61, 255 }

/* Un Chunk est une texture assez large pour contenir les textures de plusieurs
 * tuiles dans un quadrillage, elle permet de dessiner une portion de la grille
 * une seule fois et garder le resultat en memoire pour les prochaines frames.
 */
typedef struct _Chunk {
    RenderTexture2D render;
    int x;
    int y;
} Chunk;

enum Sprite {
    S_COTE = 0, // cote simple
    S_OUEST_NORD,
    S_SUD_NORD,
    S_OUEST_NORD_EST,
    S_PLEIN,
};

RenderTexture2D generer_render_tuile(Tuile t, Texture spritesheet);

void dessiner_chunk(Chunk chunk);
void dessiner_tuile(RenderTexture2D render_tuile, Vector2 position, float rotation, int alpha);

/* arrondit x au plus petit multiple de p
 * avec p un puissance de 2 */
int multiple_inf(float x, int p);
/* renvoit x modulo z sur l'ensemble {0..z-1}.
 * cette fonction est necessaire car l'operateur C '%'
 * renvoit une valeur negative si x < 0. */
int mod(int x, int z);

#endif
