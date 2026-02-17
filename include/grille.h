#ifndef grille_h
#define grille_h

struct _Tuile {
    char milieu,gauche,droite,haut,bas;
    char meeple;
};
typedef struct _Tuile *Tuile;
typedef struct _Grille{
    int taille;
    Tuile **tableau;
} Grille;

Grille init_tableau(int taille);
void destruction_grille(Grille grille);
void pivot_90_droit_piece(Tuile piece);

#endif