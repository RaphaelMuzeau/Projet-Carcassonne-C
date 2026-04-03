#include <stdio.h>
#include <stdlib.h>
#include "joueur.h"
#include "meeple.h"
#include "vec.h"
#include "tuile.h"
#include "pile.h"
#include "gentest.h"
#include "fichier.h"

Vec2D generer_test1(void)
{
    // Résultat = -1
    // nb_meeple[0] = 1
    Vec2D grille = creer_vec2D();

    Tuile tuile1 = creer_tuile();
    tuile1->est = Z_VILLE;
    tuile1->sud = Z_VILLE;
    tuile1->milieu = Z_VILLE;
    set(&grille, tuile1, 0, 0);

    Tuile tuile2 = creer_tuile();
    tuile2->est = Z_VILLE;
    tuile2->sud = Z_VILLE;
    tuile2->milieu = Z_VILLE;
    tuile2->nord = Z_VILLE;
    tuile2->ouest = Z_VILLE;
    set(&grille, tuile2, 1, 0);

    Tuile tuile3 = creer_tuile();
    tuile3->est = Z_VILLE;
    tuile3->sud = Z_VILLE;
    tuile3->milieu = Z_VILLE;
    tuile3->nord = Z_VILLE;
    tuile3->ouest = Z_VILLE;
    set(&grille, tuile3, 2, 0);

    Tuile tuile4 = creer_tuile();
    tuile4->est = Z_VILLE;
    tuile4->sud = Z_VILLE;
    tuile4->milieu = Z_VILLE;
    tuile4->nord = Z_VILLE;
    tuile4->ouest = Z_VILLE;
    set(&grille, tuile4, 3, 0);

    Tuile tuile5 = creer_tuile();
    tuile5->sud = Z_VILLE;
    tuile5->milieu = Z_VILLE;
    tuile5->nord = Z_VILLE;
    tuile5->ouest = Z_VILLE;
    set(&grille, tuile5, 4, 0);

    Tuile tuile6 = creer_tuile();
    tuile6->est = Z_VILLE;
    tuile6->sud = Z_VILLE;
    tuile6->milieu = Z_VILLE;
    tuile6->nord = Z_VILLE;
    set(&grille, tuile6, 0, 1);

    Tuile tuile7 = creer_tuile();
    tuile7->est = Z_VILLE;
    tuile7->sud = Z_VILLE;
    tuile7->milieu = Z_VILLE;
    tuile7->nord = Z_VILLE;
    tuile7->ouest = Z_VILLE;
    set(&grille, tuile7, 1, 1);

    Tuile tuile8 = creer_tuile();
    tuile8->est = Z_VILLE;
    tuile8->sud = Z_VILLE;
    tuile8->milieu = Z_VILLE;
    tuile8->nord = Z_VILLE;
    tuile8->ouest = Z_VILLE;
    tuile8->id_meeple = 0;
    tuile8->position_meeple = D_MILIEU;
    set(&grille, tuile8, 2, 1);

    Tuile tuile9 = creer_tuile();
    tuile9->est = Z_VILLE;
    tuile9->sud = Z_VILLE;
    tuile9->milieu = Z_VILLE;
    tuile9->nord = Z_VILLE;
    tuile9->ouest = Z_VILLE;
    set(&grille, tuile9, 3, 1);

    Tuile tuile10 = creer_tuile();
    tuile10->milieu = Z_VILLE;
    tuile10->nord = Z_VILLE;
    tuile10->ouest = Z_VILLE;
    set(&grille, tuile10, 4, 1);

    Tuile tuile11 = creer_tuile();
    tuile11->est = Z_VILLE;
    tuile11->milieu = Z_VILLE;
    tuile11->nord = Z_VILLE;
    tuile11->ouest = Z_VILLE;
    set(&grille, tuile11, 1, 2);

    Tuile tuile12 = creer_tuile();
    tuile12->est = Z_VILLE;
    tuile12->milieu = Z_VILLE;
    tuile12->nord = Z_VILLE;
    tuile12->ouest = Z_VILLE;
    set(&grille, tuile12, 2, 2);

    Tuile tuile13 = creer_tuile();
    tuile13->milieu = Z_VILLE;
    tuile13->nord = Z_VILLE;
    tuile13->ouest = Z_VILLE;
    set(&grille, tuile13, 3, 2);

    return grille;
}

Vec2D generer_test2(void)
{
    // Résultat = 4
    // nb_meeple[0] = 1
    Vec2D grille = creer_vec2D();

    Tuile tuile1 = creer_tuile();
    tuile1->est = Z_VILLE;
    tuile1->sud = Z_VILLE;
    tuile1->milieu = Z_VILLE;
    tuile1->id_meeple = 0;
    tuile1->position_meeple = D_SUD;
    set(&grille, tuile1, 0, 0);

    Tuile tuile2 = creer_tuile();
    tuile2->sud = Z_VILLE;
    tuile2->milieu = Z_VILLE;
    tuile2->ouest = Z_VILLE;
    set(&grille, tuile2, 1, 0);

    Tuile tuile3 = creer_tuile();
    tuile3->est = Z_VILLE;
    tuile3->milieu = Z_VILLE;
    tuile3->nord = Z_VILLE;
    set(&grille, tuile3, 0, 1);

    Tuile tuile4 = creer_tuile();
    tuile4->milieu = Z_VILLE;
    tuile4->ouest = Z_VILLE;
    tuile4->nord = Z_VILLE;
    set(&grille, tuile4, 1, 1);

    return grille;
}

Vec2D generer_test3(void)
{
    // Résultat = 5
    // nb_meeple[0] = 1
    Vec2D grille = creer_vec2D();

    Tuile tuile1 = creer_tuile();
    tuile1->sud = Z_ROUTE;
    tuile1->milieu = Z_VILLAGE;
    set(&grille, tuile1, 0, 0);

    Tuile tuile2 = creer_tuile();
    tuile2->est = Z_ROUTE;
    tuile2->milieu = Z_ROUTE;
    tuile2->nord = Z_ROUTE;
    set(&grille, tuile2, 0, 1);

    Tuile tuile3 = creer_tuile();
    tuile3->est = Z_ROUTE;
    tuile3->milieu = Z_ROUTE;
    tuile3->ouest = Z_ROUTE;
    tuile3->id_meeple = 0;
    tuile3->position_meeple = D_EST;
    set(&grille, tuile3, 1, 1);

    Tuile tuile4 = creer_tuile();
    tuile4->sud = Z_ROUTE;
    tuile4->milieu = Z_ROUTE;
    tuile4->ouest = Z_ROUTE;
    set(&grille, tuile4, 2, 1);

    Tuile tuile5 = creer_tuile();
    tuile5->milieu = Z_VILLAGE;
    tuile5->nord = Z_ROUTE;
    set(&grille, tuile5, 2, 2);

    return grille;
}

Vec2D generer_test4(void)
{
    // Résultat = 4
    // nb_meeple[0] = 1
    // nb_meeple[1] = 1
    Vec2D grille = creer_vec2D();

    Tuile tuile1 = creer_tuile();
    tuile1->milieu = Z_VILLAGE;
    tuile1->sud = Z_ROUTE;
    tuile1->id_meeple = 0;
    tuile1->position_meeple = D_SUD;
    set(&grille, tuile1, 0, 0);

    Tuile tuile2 = creer_tuile();
    tuile2->est = Z_ROUTE;
    tuile2->milieu = Z_ROUTE;
    tuile2->nord = Z_ROUTE;
    set(&grille, tuile2, 0, 1);

    Tuile tuile3 = creer_tuile();
    tuile3->est = Z_ROUTE;
    tuile3->milieu = Z_ROUTE;
    tuile3->ouest = Z_ROUTE;
    tuile3->id_meeple = 1;
    tuile3->position_meeple = D_MILIEU;
    set(&grille, tuile3, 1, 1);

    Tuile tuile4 = creer_tuile();
    tuile4->est = Z_VILLE;
    tuile4->sud = Z_VILLE;
    tuile4->milieu = Z_VILLE;
    tuile4->nord = Z_VILLE;
    tuile4->ouest = Z_ROUTE;
    set(&grille, tuile4, 2, 1);

    return grille;
}

Vec2D generer_test5(void)
{
    // Résultat = 4
    // nb_meeple[0] = 1
    Vec2D grille = creer_vec2D();

    Tuile tuile1 = creer_tuile();
    tuile1->est = Z_VILLE;
    tuile1->sud = Z_VILLE;
    tuile1->milieu = Z_VILLE;
    tuile1->id_meeple = 0;
    tuile1->position_meeple = D_SUD;
    set(&grille, tuile1, 0, 0);

    Tuile tuile2 = creer_tuile();
    tuile2->sud = Z_VILLE;
    tuile2->milieu = Z_VILLE;
    tuile2->ouest = Z_VILLE;
    set(&grille, tuile2, 1, 0);

    Tuile tuile3 = creer_tuile();
    tuile3->est = Z_VILLE;
    tuile3->milieu = Z_VILLE;
    tuile3->nord = Z_VILLE;
    set(&grille, tuile3, 0, 1);

    Tuile tuile4 = creer_tuile();
    tuile4->milieu = Z_VILLE;
    tuile4->nord = Z_VILLE;
    tuile4->ouest = Z_VILLE;
    set(&grille, tuile4, 1, 1);

    return grille;
}

void generer_fichier_grille(void)
{
    Vec2D grille = creer_vec2D();

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            Tuile tmp = creer_tuile();
            set(&grille, tmp, i, j);
        }
    }

    FILE *f = fopen("data/test/fichier_test_grille.bin", "w");
    sauvegarder_grille(&grille, f);
    detruire_vec2D(grille);
    fclose(f);
}

void generer_fichier_pile(void)
{
    int nb_tuiles = 56;

    Pile p = creer_pile(nb_tuiles);

    for (int i = 0; i < p.nb_element_max; i++) {
        Tuile tmp = creer_tuile();
        inserer_tuile(&p, tmp);
    }

    FILE *f = fopen("data/test/fichier_test_pile.bin", "w");

    sauvegarder_pile(p, f);

    fclose(f);
    detruire_pile(&p);
}

void generer_fichier_joueur(void)
{
    Joueur test = creer_joueur(1, 7);
    test.nom = "Damien";
    test.pts = 300;

    L_meeple meeple1 = creer_maillon_meeple(0, 0, D_SUD);
    ajouter_maillon_meeple(&test.localisation_meeples, meeple1);

    L_meeple meeple2 = creer_maillon_meeple(0, 12, D_NORD);
    ajouter_maillon_meeple(&test.localisation_meeples, meeple2);

    L_meeple meeple3 = creer_maillon_meeple(-4, -8, D_EST);
    ajouter_maillon_meeple(&test.localisation_meeples, meeple3);

    FILE *f = fopen("data/test/fichier_test_joueur.bin", "w");
    sauvegarder_joueur(test, f);

    fclose(f);
    detruire_joueur(test);
}

void generer_fichier_joueur_vide(void)
{
    Joueur test = creer_joueur(1, 7);
    test.nom = "Damien";
    test.pts = 300;

    FILE *f = fopen("data/test/fichier_test_joueur_vide.bin", "w");
    sauvegarder_joueur(test, f);
    detruire_joueur(test); // ne marche pas ?
    fclose(f);
}

void generer_fichier_liste_joueurs(void)
{
    ListeJoueurs joueurs = creer_listejoueurs(3, 4);
    joueurs.tableau[0].nom = "Damien";
    joueurs.tableau[0].pts = 300;
    joueurs.tableau[0].nb_meeple_restant = 1;
    L_meeple meeple1 = creer_maillon_meeple(0, 0, D_SUD);
    ajouter_maillon_meeple(&joueurs.tableau[0].localisation_meeples, meeple1);

    L_meeple meeple2 = creer_maillon_meeple(0, 12, D_NORD);
    ajouter_maillon_meeple(&joueurs.tableau[0].localisation_meeples, meeple2);

    L_meeple meeple3 = creer_maillon_meeple(-4, -8, D_EST);
    ajouter_maillon_meeple(&joueurs.tableau[0].localisation_meeples, meeple3);

    joueurs.tableau[1].nom = "Léo";
    joueurs.tableau[1].pts = 22;
    // Léo n'a pas de meeple+

    joueurs.tableau[2].nom  = "Seth";
    joueurs.tableau[2].pts = 777;
    joueurs.tableau[2].nb_meeple_restant = 3;
    L_meeple meeple4 = creer_maillon_meeple(7, 6, D_OUEST);
    ajouter_maillon_meeple(&joueurs.tableau[2].localisation_meeples, meeple4);

    FILE *f = fopen("data/test/fichier_test_liste_joueurs.bin", "w");

    sauvegarder_liste_joueurs(joueurs, f);
    fclose(f);
    detruire_listejoueurs(joueurs);
}
