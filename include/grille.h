#ifndef grille_h
#define grille_h

enum Zone {
    Z_ROUTE,
    Z_VILLE,
    Z_BLASON,
    Z_PRE,
    Z_VILLAGE,
    Z_ABBAYE,
};

struct _Tuile {
    enum Zone milieu, gauche, droite, haut, bas;
    char meeple;
};
typedef struct _Tuile *Tuile;

typedef struct _Grille {
    int taille;
    Tuile **tableau;
} Grille;

Grille init_tableau(int taille);
void destruction_grille(Grille grille);
void pivot_90_droit_piece(Tuile piece);

#endif
