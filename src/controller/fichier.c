#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libca.h"
#include "fichier.h"
#include "joueur.h"
#include "meeple.h"
#include "tuile.h"
#include "grille.h"
#include "pile.h"

#define VERSION "SV0.1"
#define LEN_VER sizeof(VERSION)

bool sauvegarder_partie(Jeu partie, char *fname)
{
    FILE *f = fopen(fname, "w");
    if (f == NULL) {
        perror("carcassonne");
        return false;
    }

    fwrite(VERSION, sizeof(char), LEN_VER, f);
    sauvegarder_grille(partie.grille, f);
    sauvegarder_pile(partie.pile, f);
    sauvegarder_liste_joueurs(partie.joueurs, f);

    fclose(f);
    return true;
}

bool charger_partie(Jeu *partie, char *fname)
{
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        perror("carcassonne");
        return false;
    }

    char version[LEN_VER] = { 0 };
    if (fread(version, sizeof(char), LEN_VER, f) != LEN_VER || strcmp(version, "SV0.1")) {
        perror("carcassonne");
        fclose(f);
        return false;
    }

    partie->grille = charger_grille(f);
    partie->pile = charger_pile(f);
    partie->joueurs = charger_liste_joueurs(f);

    fclose(f);
    return true;
}

int ecrire_grille(Vec2D g, int x, int y, FILE *f)
{
    Tuile cur = get(g, x, y);

    if (!cur)
        return 0;

    if (cur->is_verified)
        return 0;

    fwrite(cur, sizeof(struct _Tuile), 1, f);
    fwrite(&x, sizeof(int), 1, f);
    fwrite(&y, sizeof(int), 1, f);

    cur->is_verified = true;

    return ecrire_grille(g, x+1, y, f) + ecrire_grille(g, x-1, y, f) + // EST & OUEST
           ecrire_grille(g, x, y+1, f) + ecrire_grille(g, x, y-1, f) + 1; // NORD & SUD
}

void sauvegarder_grille(Vec2D g, FILE *f)
{
    fpos_t pos_nb_tuiles;     // Sauvegarde l'endroit où le nb_tuiles sera écrit
    fgetpos(f, &pos_nb_tuiles);

    fseek(f, sizeof(int), SEEK_CUR); // Prépare de la place pour nb_tuiles

    int nb_tuiles = ecrire_grille(g, 0, 0, f);
    recherche_is_verified(g, 0, 0);

    fsetpos(f, &pos_nb_tuiles);  // Retour arrière pour écrire nb_tuiles
    fwrite(&nb_tuiles, sizeof(int), 1, f);
    fseek(f, 0, SEEK_END);      // Retourne à la fin du fichier
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
    ca_error("fichier invalide (grille)");
}

void sauvegarder_pile(Pile p, FILE *f)
{
    fwrite(&p.nb_element, sizeof(int), 1, f);
    fwrite(&p.gen_aleatoire, sizeof(bool), 1, f);

    if (!p.gen_aleatoire) {
        for (int i = 0; i < p.nb_element; i++) {
            fwrite(p.tab[i], sizeof(struct _Tuile), 1, f);
        }
    }
}

Pile charger_pile(FILE *f)
{

    int nb_element;
    bool aleatoire;
    if (fread(&nb_element, sizeof(int), 1, f) != 1)
        goto erreur_pile;

    if (fread(&aleatoire, sizeof(bool), 1, f) != 1)
        goto erreur_pile;

    Pile p = creer_pile(nb_element, aleatoire);

    if (!p.gen_aleatoire) {
        for (int i = 0; i < p.nb_element_max; i++) {
            Tuile tmp = creer_tuile();
            if (fread(tmp, sizeof(struct _Tuile), 1, f) != 1)
                goto erreur_pile;
            inserer_tuile(&p, tmp);
        }
    }
    return p;

erreur_pile:
    ca_error("fichier invalide (pile)");
}

void sauvegarder_liste_joueurs(ListeJoueurs tab, FILE *f)
{
    fwrite(&tab.nb_joueurs, sizeof(int), 1, f);
    for (int i = 0; i < tab.nb_joueurs; i++) {
        sauvegarder_joueur(tab.tableau[i], f);
    }
}

ListeJoueurs charger_liste_joueurs(FILE *f)
{
    int nb_joueurs = 0;
    if (fread(&nb_joueurs, sizeof(int), 1, f) != 1)
        goto erreur_liste_joueurs;

    ListeJoueurs joueurs = creer_listejoueurs(nb_joueurs, 0);

    for (int i = 0; i < joueurs.nb_joueurs; i++)
        joueurs.tableau[i] = charger_joueur(f);

    return joueurs;

erreur_liste_joueurs:
    ca_error("fichier invalide (lise_joueurs)");
}

void sauvegarder_joueur(Joueur joueur, FILE *f)
{
    Joueur joueur_tmp = joueur;
    joueur_tmp.localisation_meeples = NULL;
    joueur_tmp.nom= NULL;
    fwrite(&joueur_tmp, sizeof(Joueur), 1, f);

    size_t taille_nom = strlen(joueur.nom) + 1; // +1 pour le \0
    fwrite(&taille_nom, sizeof(size_t), 1, f);
    fwrite(joueur.nom, sizeof(char), taille_nom, f);

    // Sauvegarde la position pour nb_meeple_pose et prépare de l'espace
    fpos_t pos_meeple_pose = { 0 };
    fgetpos(f, &pos_meeple_pose);
    fseek(f, sizeof(int), SEEK_CUR);
    int nb_meeple_pose = 0;

    /*
     * Se fait via des structures privés pour changer
     * la variable tmp2 sans modifier tmp */
    if (joueur.localisation_meeples != NULL) {
        struct _Maillon tmp = *joueur.localisation_meeples;
        do {
            struct _Maillon tmp2 = tmp;
            tmp2.next = NULL;
            fwrite(&tmp2, sizeof(struct _Maillon), 1, f);
            if (tmp.next != NULL)
                tmp = *tmp.next;
            nb_meeple_pose++;
        } while (tmp.next != NULL);
        fwrite(&tmp, sizeof(struct _Maillon), 1, f);
        nb_meeple_pose++;
    }

    // Écrit au bon endroit nb_meeple_pose et revient à la fin
    fsetpos(f, &pos_meeple_pose);
    fwrite(&nb_meeple_pose, sizeof(int), 1, f);
    fseek(f, 0, SEEK_END);
}

Joueur charger_joueur(FILE *f)
{
    Joueur j;
    if (fread(&j, sizeof(Joueur), 1, f) != 1)
        goto erreur_joueur;

    size_t taille_nom = 0;
    if (fread(&taille_nom, sizeof(size_t), 1, f) != 1)
        goto erreur_joueur;

    j.nom = ca_alloc(taille_nom, sizeof(char));
    size_t tmp = fread(j.nom, sizeof(char), taille_nom, f);

    if (tmp != taille_nom)
        goto erreur_joueur;

    int nb_meeple_pose = 0;
    if (fread(&nb_meeple_pose, sizeof(int), 1, f) != 1)
        goto erreur_joueur;

    if (nb_meeple_pose) {
        int cmpt = 0;
        do {
            L_meeple tmp = creer_maillon_meeple(0, 0, 0);
            if (fread(tmp, sizeof(struct _Maillon), 1, f) != 1)
                goto erreur_joueur;
            ajouter_maillon_meeple(&j.localisation_meeples, tmp);
            cmpt++;
        } while (cmpt <nb_meeple_pose);
    }
    return j;

erreur_joueur:
    ca_error("fichier invalide (joueur)");
}
