#ifndef csv_h
#define csv_h

#include <stdio.h>
#include <stdbool.h>
#include "pile.h"
#include "tuile.h"

/* Ces fonctions acceptent uniquement
 * des .csv valident pour le jeu Carcasonne */
Pile lire_tuiles_csv(char* nom_fichier);
bool lire_zone(enum Zone *p, FILE *f);
int compter_lignes(FILE *f);

#endif
