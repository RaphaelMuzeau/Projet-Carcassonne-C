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

Pile lire_tuiles_csv(char* nom_fichier)
{
    int max_element = compter_lignes(nom_fichier);
    Pile p = creer_pile(max_element);
    FILE *fichier = fopen(nom_fichier, "r");

    while (p.nb_element < p.nb_element_max) {
       p.tab[p.nb_element] = init_tuile();
       lire_tuile(&p.tab[p.nb_element]->nord, fichier);
       lire_tuile(&p.tab[p.nb_element]->sud, fichier);
       lire_tuile(&p.tab[p.nb_element]->est, fichier);
       lire_tuile(&p.tab[p.nb_element]->ouest, fichier);
       lire_tuile(&p.tab[p.nb_element]->milieu, fichier);
       p.nb_element++;
    }

    fclose(fichier);
    return p;
}

int compter_lignes(char *fichier)
{
    FILE *f = fopen(fichier, "r");
    unsigned char valeur;
    int nb_lignes = 0;
    while (fread(&valeur, sizeof(unsigned char), 1, f) != 0) {
        if (valeur == '\n')
            nb_lignes++;
    }

    return nb_lignes;
}
