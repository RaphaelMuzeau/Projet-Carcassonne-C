#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fichier.h"
#include "tuile.h"
#include "vec.h"
#include "pile.h"
#include "meeple.h"
#include "libca.h"

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

void sauvegarder_joueur(Joueur *tab_joueurs, int nb_joueurs, FILE *f)
{

    fwrite(&nb_joueurs, sizeof(int), 1, f);

    for (int i = 0; i < nb_joueurs; i++) {
        Joueur joueur_tmp = tab_joueurs[i];
        joueur_tmp.localisation_meeples = NULL;
        fwrite(&joueur_tmp, sizeof(Joueur), 1, f);


        /* Sauvegarde la position pour nb_mepple_pose et prépare de l'espace*/
        fpos_t pos_meeple_pose;
        fgetpos(f, &pos_meeple_pose);
        fseek(f, sizeof(int), SEEK_CUR);
        int nb_meeple_pose = 0;

        struct _Maillon tmp = *tab_joueurs[i].localisation_meeples;
        do {
            struct _Maillon tmp2 = tmp;
            tmp2.next = NULL;
            fwrite(&tmp2, sizeof(struct _Maillon), 1, f);
            tmp = *tmp.next;
            nb_meeple_pose++;
        } while (tmp.next != NULL);
        fwrite(&tmp, sizeof(struct _Maillon), 1, f);
        nb_meeple_pose++;

        /* Écrit au bon endroit nb_meeple_pose et revient à la fin */
        fsetpos(f, &pos_meeple_pose);
        fwrite(&nb_meeple_pose, sizeof(int), 1, f);
        fseek(f, 0, SEEK_END);
    }
}

Joueur *charger_joueur(FILE *f)
{

    int nb_joueurs = 0;
    if (fread(&nb_joueurs, sizeof(int), 1, f) != 1)
        goto erreur_joueur;

    Joueur *tab_joueurs = ca_alloc(nb_joueurs, sizeof(Joueur));

    for (int i = 0; i < nb_joueurs; i++) {
        if (fread(&tab_joueurs[i], sizeof(Joueur), 1, f) != 1)
            goto erreur_joueur;

        int nb_meeple_pose = 0;
        if (fread(&nb_meeple_pose, sizeof(int), 1, f) != 1)
            goto erreur_joueur;
        int cmpt = 0;
        do {
            L_meeple tmp = NULL;
            if (fread(tmp, sizeof(struct _Maillon), 1, f) != 1)
                goto erreur_joueur;

            ajout_meeple_chaine(&tab_joueurs[i], tmp);
            cmpt++;
        } while (cmpt <nb_meeple_pose);
    }

erreur_joueur:
    fprintf(stderr, "carcassonne: fichier invalide (joueurs)\n");
    exit(EXIT_FAILURE);
}
