#ifndef csv_h
#define csv_h

#include <stdbool.h>
#include <stdio.h>
#include "pile.h"
#include "tuile.h"

Pile lire_tuiles_csv(char* nom_fichier, int max_element);
void lire_tuile(enum Zone *p, FILE *f);

#endif
