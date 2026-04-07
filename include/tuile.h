#ifndef tuile_h
#define tuile_h

#include <stdbool.h>

// Z_BLASON et Z_VILLE ont un bit en commun pour etre considérés
// comme compatible avec un & binaire.
enum Zone {
    Z_PRE     = 0x01, // 0b000001
    Z_ROUTE   = 0x02, // 0b000010
    Z_VILLE   = 0x04, // 0b000100
    Z_BLASON  = 0x0c, // 0b001100
    Z_VILLAGE = 0x10, // 0b010000
    Z_ABBAYE  = 0x20, // 0b100000
};

enum Direction {
    D_SUD = 0,
    D_NORD,
    D_EST,
    D_OUEST,
    D_MILIEU,
};

struct _Tuile {
    enum Zone milieu, nord, sud, est, ouest;
    int id_meeple;
    enum Direction position_meeple;
    bool est_verifie;
};
typedef struct _Tuile *Tuile;

Tuile creer_tuile(void);
/* pivot à droite */
void pivot_90(Tuile piece);
enum Zone zone_tuile(Tuile t, enum Direction d);
bool compatibilite_tuile(Tuile depart, Tuile arrivee, enum Direction d);
enum Zone generer_milieu(void);
enum Zone generer_cote(int aleatoire);
Tuile generer_tuile(void);
#endif
