#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fichier.h"
#include "tuile.h"
#include "vec.h"
#include "pile.h"
// TODO: Décider d'une version de format à écrire en haut du fichier.

int ecrire_grille(Vec2D *g, int x, int y, FILE *f)
{
    Tuile cur = get(*g, x, y);

    if (!cur)
        return 0;

    if (cur->is_verified)
        return 0;

    fwrite(cur, sizeof(struct _Tuile), 1, f);
    fwrite(&x, sizeof(int), 1, f);
    fwrite(&y, sizeof(int), 1, f);

    cur->is_verified = 1;

    return ecrire_grille(g, x+1, y, f) + ecrire_grille(g, x-1, y, f) + // EST & OUEST
           ecrire_grille(g, x, y+1, f) + ecrire_grille(g, x, y-1, f) + 1; // NORD & SUD

}

void sauvegarder_grille(Vec2D *g, FILE *f)
{
    fpos_t pos_nb_tuiles;     // Sauvegarde l'endroit où le nb_tuiles sera écrit
    fgetpos(f, &pos_nb_tuiles);

    fseek(f, sizeof(int), SEEK_CUR); // Prépare de la place pour nb_tuiles

    int nb_tuiles = ecrire_grille(g, 0, 0, f);

    fsetpos(f, &pos_nb_tuiles);  // Retour arrière pour écrire nb_tuiles
    fwrite(&nb_tuiles, sizeof(int), 1, f);
    fseek(f, 1, SEEK_END);      // Retourne à la fin du fichier
}

Vec2D charger_grille(FILE *f)
{
    int x = 0, y = 0, nb_tuiles;
    Vec2D g = creer_vec2D();

    if (fread(&nb_tuiles, sizeof(int), 1, f) != 1)
        goto erreur_grille;

    for (int i = 0; i < nb_tuiles; i++) {
        Tuile tmp = creer_tuile();
        if (fread(tmp, sizeof(struct _Tuile), 1, f) != 1)
            goto erreur_grille;

        if (fread(&x, sizeof(int), 1, f) != 1)
            goto erreur_grille;

        if (fread(&y, sizeof(int), 1, f) != 1)
            goto erreur_grille;

        set(&g, tmp, x, y);
    }

    return g;

erreur_grille:
    fprintf(stderr, "carcassonne: fichier invalide (grille)\n");
    exit(EXIT_FAILURE);
}

void sauvegarder_pile(Pile p, FILE *f)
{
    fwrite(&p.nb_element, sizeof(int), 1, f);

    for (int i = 0; i < p.nb_element; i++) {
        fwrite(p.tab[i], sizeof(struct _Tuile), 1, f);
    }
}

Pile charger_pile(FILE *f)
{
    int nb_element;
    if (fread(&nb_element, sizeof(int), 1, f) != 1)
        goto erreur_pile;

    Pile p = creer_pile(nb_element);

    for (int i = 0; i < p.nb_element_max; i++) {
        Tuile tmp = creer_tuile();
        if (fread(tmp, sizeof(struct _Tuile), 1, f) != 1)
            goto erreur_pile;
        inserer_tuile(&p, tmp);
    }

    return p;

erreur_pile:
    fprintf(stderr, "carcassonne: fichier invalide (pile)\n");
    exit(EXIT_FAILURE);
}
