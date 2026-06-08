#ifndef vec_h
#define vec_h

#include "tuile.h"

#define VEC_REALLOC_SIZE 64                             // nombre d'octets ajoutés pour agrandir un vecteur
#define VEC_REALLOC_NB (VEC_REALLOC_SIZE/sizeof(Tuile)) // nombre de tuiles ajoutés pour agrandir un vecteur
_Static_assert(VEC_REALLOC_SIZE > 0, "VEC_REALLOC_SIZE doit etre supérieur à 0");
_Static_assert(VEC_REALLOC_SIZE % sizeof(Tuile) == 0 , "VEC_REALLOC_SIZE doit etre un multiple de sizeof(Tuile)");

/* Tableau variable de Tuiles indexable de INT_MIN à INT_MAX,
 * toutes les cases sont à NULL par défaut.
 * Un Vec ne doit être manipulé qu'à l'aide de vget() et vset().
 */
typedef struct _Vec {
    Tuile *_tableau;
    int _capacite; // nombre de tuiles pouvant etre contenues
    int _decy;
} Vec;

Vec creer_vec(void);
void detruire_vec(Vec v);
Tuile vget(Vec v, int y);
void vset(Vec *v, Tuile t, int y);

#define VEC2D_REALLOC_NB (VEC_REALLOC_SIZE/sizeof(Vec))
_Static_assert(VEC_REALLOC_SIZE % sizeof(Vec) == 0 , "VEC_REALLOC_SIZE doit etre un multiple de sizeof(Vec)");

/* Se comporte exactement comme un Vec mais
 * est indexable par x et y.
 */
typedef struct _Vec2D {
    Vec *_tableau;
    int _capacite;
    int _decx;
} Vec2D;

Vec2D creer_vec2D(void);
void detruire_vec2D(Vec2D g);
Tuile get(Vec2D g, int x, int y);
void set(Vec2D *g, Tuile t, int x, int y);

void print_row(Vec2D g);
#endif
