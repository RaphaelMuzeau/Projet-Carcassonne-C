#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "csv.h"
#include "pile.h"
#include "tuile.h"

bool lire_zone(enum Zone *z, FILE *f)
{
    unsigned char valeur;
    valeur = fgetc(f);

    switch(valeur) {

    // Chaque lettre représente une tuile, le saut réalisé ensuite permet
    // de passer à la prochaine valeur (suite du mot + virgule -> pour
    // les valeurs en fin de ligne, le "\n" sera aussi sauté)

    case 'r':
        *z = Z_ROUTE;
        fseek(f, sizeof(unsigned char)*5, SEEK_CUR);
        break;
    case 'p':
        fseek(f, sizeof(unsigned char)*3, SEEK_CUR);
        break;
    case 'a':
        *z = Z_ABBAYE;
        fseek(f, sizeof(unsigned char)*6, SEEK_CUR);
        break;
    case 'b':
        *z = Z_BLASON;
        fseek(f, sizeof(unsigned char)*6, SEEK_CUR);
        break;
    case 'v':
        // ville et village ayant la même 1er lettre.
        // On compare aussi le 4e caractère "e" ou "a".
        fseek(f, sizeof(unsigned char)*3, SEEK_CUR);
        valeur = fgetc(f);
        if (valeur == 'e') {
            *z = Z_VILLE;
            fseek(f, sizeof(unsigned char), SEEK_CUR);
        } else {
            *z = Z_VILLAGE;
            fseek(f, sizeof(unsigned char)*3, SEEK_CUR);
        }
        break;
    default:
        return false;
    }

    return true;
}

Pile lire_tuiles_csv(char* nom_fichier)
{
    FILE *fichier = fopen(nom_fichier, "r");
    int max_element = compter_lignes(fichier);
    Pile p = creer_pile(max_element);
    if (fichier == NULL) return p;

    Tuile t;
    fseek(fichier, 0, SEEK_SET);

    if (fichier == NULL) return p;

    while (p.nb_element < p.nb_element_max) {
        t = init_tuile();
        if (!lire_zone(&t->nord, fichier)) goto fichier_invalide;
        if (!lire_zone(&t->sud, fichier)) goto fichier_invalide;
        if (!lire_zone(&t->est, fichier)) goto fichier_invalide;
        if (!lire_zone(&t->ouest, fichier)) goto fichier_invalide;
        if (!lire_zone(&t->milieu, fichier)) goto fichier_invalide;
        inserer_tuile(&p, t);

    }

    fclose(fichier);
    return p;

    fichier_invalide:
        fclose(fichier);
        free(t);
        detruire_pile(&p);
        p = creer_pile(0);
        return p;
}

int compter_lignes(FILE *f)
{
    if (f == NULL) return 0;

    int valeur;
    int nb_lignes = 0;

    while ((valeur = fgetc(f)) != EOF) {
        if (valeur == '\n')
            nb_lignes++;
    }

    return nb_lignes;
}
