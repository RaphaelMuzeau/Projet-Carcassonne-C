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

/* Un Chunk est une texture assez large pour contenir les textures de plusieurs
 * tuiles dans un quadrillage, elle permet de dessiner une portion de la grille
 * une seule fois et garder le resultat en memoire pour les prochaines frames.
 */
typedef struct _Chunk {
    RenderTexture2D render;
    int x;
    int y;
} Chunk;

void dessiner_tuile(Chunk *chunks, int *nb_chunks, RenderTexture2D render_tuile, Vector2 position);
RenderTexture2D generer_texture(Tuile t);

/* arrondit x au plus petit multiple de p
 * avec p un puissance de 2 */
int multiple_inf(float x, int p);
/* renvoit x modulo z sur l'ensemble {0..z-1}.
 * cette fonction est necessaire car l'operateur C '%'
 * renvoit une valeur negative si x < 0. */
int mod(int x, int z);

#endif
