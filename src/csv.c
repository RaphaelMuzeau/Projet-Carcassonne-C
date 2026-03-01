#include <stdbool.h>
#include <stdio.h>
#include "csv.h"
#include "pile.h"
#include "tuile.h"

void lire_tuile(enum Zone *p, FILE *f)
{
    unsigned char valeur;
    fread(&valeur, sizeof(unsigned char), 1, f);

    switch(valeur) {

        case 'r':
            *p = Z_ROUTE;
            fseek(f, sizeof(unsigned char)*5, SEEK_CUR);
            break;
        case 'p':
            fseek(f, sizeof(unsigned char)*3, SEEK_CUR);
            break;
        case 'a':
            *p = Z_ABBAYE;
            fseek(f, sizeof(unsigned char)*6, SEEK_CUR);
            break;
        case 'b':
            *p = Z_BLASON;
            fseek(f, sizeof(unsigned char)*6, SEEK_CUR);
            break;
        case 'v':
            fseek(f, sizeof(unsigned char)*3, SEEK_CUR);
            fread(&valeur, sizeof(unsigned char), 1, f);
            if (valeur == 'e') {
                *p = Z_VILLE;
                fseek(f, sizeof(unsigned char), SEEK_CUR);
            } else {
                *p = Z_VILLAGE;
                fseek(f, sizeof(unsigned char)*3, SEEK_CUR);
            }
            break;
    }
}

Pile lire_tuiles_csv(char* nom_fichier, int max_element)
{
    int i = 0;
    Pile p = creer_pile(max_element);
    FILE *fichier = fopen(nom_fichier, "r");

    while (i < p.nb_element_max) {
       p.tab[i] = init_tuile();
       lire_tuile(&p.tab[i]->nord, fichier);
       lire_tuile(&p.tab[i]->sud, fichier);
       lire_tuile(&p.tab[i]->est, fichier);
       lire_tuile(&p.tab[i]->ouest, fichier);
       lire_tuile(&p.tab[i]->milieu, fichier);
       i++;
    }

    fclose(fichier);
    return p;
}
