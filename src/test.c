/* framework de test simple:
 * les fonctions de test sont ecrites dans ce fichier avec une signature de
 * type `bool f(void)` et ajouté à la liste "unit_tests".
 * à l'appel de lancer_tests(), tous les tests sont executés en montrant le nombre
 * d'echecs et de reussites à l'utilisateur. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "grille.h"
#include "pile.h"
#include "tuile.h"
#include "varstring.h"
#include "csv.h"

typedef struct _Test {
    bool (*run)(void);
    const char *name;
} Test;
#define TEST(FUNCTION) { FUNCTION, #FUNCTION } // creer un Test à partir du nom d'une fonction

// ===========================
// ==== fonctions de test ====
// ===========================

bool test_init_pile(void)
{
    Pile p = creer_pile(10);

    if (!pile_vide(p)) {
        return false;
    }

    if (p.nb_element_max != 10)
        return false;

    return true;
}

bool test_tuile_creer(void)
{
    Tuile t = init_tuile();

    if (t->milieu != Z_PRE) return false;
    if (t->nord != Z_PRE)   return false;
    if (t->sud != Z_PRE)    return false;
    if (t->est != Z_PRE)    return false;
    if (t->ouest != Z_PRE)  return false;
    if (t->meeple) return false;

    free(t);
    return true;
}

bool test_tuile_compatibilite(void)
{
    Tuile depart  = init_tuile();
    Tuile arrivee = init_tuile();

    depart->nord  = Z_ROUTE;
    depart->sud   = Z_PRE;
    depart->est   = Z_VILLE;
    depart->ouest = Z_BLASON;

    arrivee->sud   = Z_ROUTE;
    arrivee->nord  = Z_PRE;
    arrivee->ouest = Z_VILLE;
    arrivee->est   = Z_VILLE;

    if (!compatibilite_tuile(depart, arrivee, D_NORD))  return false;
    if (!compatibilite_tuile(depart, arrivee, D_SUD))   return false;
    if (!compatibilite_tuile(depart, arrivee, D_EST))   return false;
    if (!compatibilite_tuile(depart, arrivee, D_OUEST)) return false;
    if (!compatibilite_tuile(arrivee, depart, D_NORD))  return false;
    if (!compatibilite_tuile(arrivee, depart, D_SUD))   return false;
    if (!compatibilite_tuile(arrivee, depart, D_EST))   return false;
    if (!compatibilite_tuile(arrivee, depart, D_OUEST)) return false;

    pivot_90(arrivee);

    if (compatibilite_tuile(depart, arrivee, D_NORD))  return false;
    if (compatibilite_tuile(depart, arrivee, D_SUD))   return false;
    if (compatibilite_tuile(depart, arrivee, D_EST))   return false;
    if (compatibilite_tuile(depart, arrivee, D_OUEST)) return false;
    if (compatibilite_tuile(arrivee, depart, D_NORD))  return false;
    if (compatibilite_tuile(arrivee, depart, D_SUD))   return false;
    if (compatibilite_tuile(arrivee, depart, D_EST))   return false;
    if (compatibilite_tuile(arrivee, depart, D_OUEST)) return false;

    free(depart);
    free(arrivee);
    return true;
}

bool test_grille_creer(void)
{
    Grille grille = init_grille(10);

    if (grille.taille != 10) return false;

    for(int i = 0; i < grille.taille; i++)
        for(int j = 0; j < grille.taille; j++)
            if (grille.tableau[i][j] != NULL)
                return false;


    destruction_grille(grille);
    return true;
}

bool test_grille_est_vide(void)
{
    Grille grille = init_grille(10);

    // cases allouées sont vides
    for(int i = 0; i < grille.taille; i++)
        for(int j = 0; j < grille.taille; j++)
            if (!est_vide(grille, i, j))
                return false;

    // cases hors champs
    if (!est_vide(grille, -11, 0)) return false;
    if (!est_vide(grille, 11, 0)) return false;
    if (!est_vide(grille, 0, -11)) return false;
    if (!est_vide(grille, 0, 11)) return false;

    // case presente
    grille.tableau[1][2] = init_tuile();
    if (est_vide(grille, 1, 2)) return false;

    destruction_grille(grille);
    return true;
}

bool test_grille_placer_tuile(void)
{
    Grille grille = init_grille(10);
    Tuile t = init_tuile();

    // placement hors champs
    if (placer_tuile(grille, -11, 0, t)) return false;
    if (placer_tuile(grille, 11, 0, t)) return false;
    if (placer_tuile(grille, 0, -11, t)) return false;
    if (placer_tuile(grille, 0, 11, t)) return false;

    // placement sans connexion
    if (placer_tuile(grille, 1, 1, t)) return false;
    if (placer_tuile(grille, 1, 0, t)) return false;
    if (placer_tuile(grille, 0, 1, t)) return false;
    if (placer_tuile(grille, 1, 9, t)) return false;
    if (placer_tuile(grille, 9, 1, t)) return false;
    if (grille.tableau[1][1] == t) return false;
    if (grille.tableau[1][0] == t) return false;
    if (grille.tableau[0][1] == t) return false;
    if (grille.tableau[1][9] == t) return false;
    if (grille.tableau[9][1] == t) return false;

    grille.tableau[1][1] = t;

    // tuile occupée
    if (placer_tuile(grille, 1, 1, t)) return false;

    // placement compatible
    t = init_tuile();
    if (!placer_tuile(grille, 0, 1, t)) return false;
    if (grille.tableau[0][1] != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 1, 0, t)) return false;
    if (grille.tableau[1][0] != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 2, 1, t)) return false;
    if (grille.tableau[2][1] != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 1, 2, t)) return false;
    if (grille.tableau[1][2] != t) return false;

    // placement incompatible
    t = init_tuile();
    t->nord = Z_VILLE; t->sud = Z_VILLE; t->est = Z_VILLE; t->ouest = Z_VILLE;
    grille.tableau[5][5] = t;

    t = init_tuile();
    if (placer_tuile(grille, 0, 1, t)) return false;
    if (grille.tableau[0][1] == t) return false;
    if (placer_tuile(grille, 1, 0, t)) return false;
    if (grille.tableau[1][0] == t) return false;
    if (placer_tuile(grille, 2, 1, t)) return false;
    if (grille.tableau[2][1] == t) return false;
    if (placer_tuile(grille, 1, 2, t)) return false;
    if (grille.tableau[1][2] == t) return false;
    free(t);

    destruction_grille(grille);
    return true;
}

bool test_inserer_tuile(void)
{
    int i;
    bool result_test = true;
    Pile p = creer_pile(1);

    Tuile t = malloc(sizeof(struct _Tuile));
    t->sud = Z_ABBAYE;
    t->est = Z_ABBAYE;
    t->nord = Z_ABBAYE;
    t->milieu = Z_ABBAYE;
    t->ouest = Z_ABBAYE;

    for (i = 0; i < p.nb_element_max ; i++) {
        if (!inserer_tuile(&p, t)) {
            result_test = false;
            break;
        }
    }

    if (inserer_tuile(&p, t))
        result_test = false;

    detruire_pile(&p);

    return result_test;
}

bool test_recup_tuile(void)
{
    Pile p = creer_pile(5);
    Tuile t = malloc(sizeof(struct _Tuile));
    t->sud = Z_ABBAYE;
    t->est = Z_ABBAYE;
    t->nord = Z_ABBAYE;
    t->milieu = Z_ABBAYE;
    t->ouest = Z_ABBAYE;

    inserer_tuile(&p, t);
    if (recup_tuile(&p) != t || p.nb_element != 0)
        return false;

    if (recup_tuile(&p) != NULL)
        return false;

    return true;
}

bool test_varstring_ajout_char(void)
{
    VarString s = creer_varstring();

    ajouter_char(&s, 'h');
    ajouter_char(&s, 'e');
    ajouter_char(&s, 'l');
    ajouter_char(&s, 'l');
    ajouter_char(&s, 'o');

    if (strcmp(s.texte, "hello")) return false;
    if (s.len != strlen(s.texte)) return false;
    if (s.capacite < strlen(s.texte) + 1) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_ajout_chaine(void)
{
    VarString s = creer_varstring();

    ajouter_chaine(&s, "hello");

    if (strcmp(s.texte, "hello")) return false;
    if (s.len != strlen(s.texte)) return false;
    if (s.capacite < strlen(s.texte) + 1) return false;

    ajouter_chaine(&s, " world");

    if (strcmp(s.texte, "hello world")) return false;
    if (s.len != strlen(s.texte)) return false;
    if (s.capacite < strlen(s.texte) + 1) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_vider(void)
{
    VarString s = creer_varstring();

    ajouter_chaine(&s, "hello");
    vider_varstring(&s);

    if (*s.texte != '\0') return false;
    if (s.len != 0) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_ajouter_null(void)
{
    VarString s = creer_varstring();

    ajouter_chaine(&s, "hello");
    size_t expected_len = s.len;
    size_t expected_cap = s.capacite;

    ajouter_chaine(&s, NULL);
    ajouter_char(&s, '\0');

    if (strcmp(s.texte, "hello")) return false;
    if (s.len != expected_len) return false;
    if (s.capacite != expected_cap) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_null(void)
{
    // pourrait declencher un crash
    ajouter_char(NULL, '\0');
    ajouter_chaine(NULL, NULL);
    vider_varstring(NULL);
    return true;
}

bool test_csv_compter_lignes(void)
{

    FILE *f0, *f1, *f2;
    f0 = fopen("data/test/0_test.csv", "r");
    if (compter_lignes(f0) != 1) return false;
    f1 = fopen("data/test/1_test.csv", "r");
    if (compter_lignes(f1) != 5) return false;
    f2 = fopen("data/test/test_non_existant.csv", "r");
    if (compter_lignes(f2) != 0) return false;
    return true;
}

bool test_csv_lecture_zone(void)
{
    FILE *f = fopen("data/test/0_test.csv", "r");
    Tuile t = init_tuile();

    lire_zone(&t->est, f);
    lire_zone(&t->nord, f);
    lire_zone(&t->ouest, f);
    lire_zone(&t->sud, f);
    lire_zone(&t->milieu, f);

    if (t->est != Z_ROUTE) return false;
    if (t->nord != Z_VILLE) return false;
    if (t->ouest!= Z_BLASON) return false;
    if (t->sud != Z_PRE) return false;
    if (t->milieu != Z_ABBAYE) return false;

    free(t);
    return true;
}

bool test_csv_lecture_fichier(void)
{
    int i;
    Pile p;
    p = lire_tuiles_csv("data/test/1_test.csv");

    if (p.nb_element < p.nb_element_max) return false;

    for (i = 0; i < p.nb_element_max; i++)
        if (p.tab[i] == NULL) return false;

    if (p.tab[0]->nord != Z_VILLE) return false;
    if (p.tab[0]->sud != Z_VILLE) return false;
    if (p.tab[0]->est != Z_ROUTE) return false;
    if (p.tab[0]->ouest != Z_ROUTE) return false;
    if (p.tab[0]->milieu != Z_ROUTE) return false;

    if (p.tab[1]->nord != Z_ROUTE) return false;
    if (p.tab[1]->sud != Z_VILLE) return false;
    if (p.tab[1]->est != Z_VILLE) return false;
    if (p.tab[1]->ouest != Z_PRE) return false;
    if (p.tab[1]->milieu != Z_ABBAYE) return false;

    if (p.tab[2]->nord != Z_BLASON) return false;
    if (p.tab[2]->sud != Z_BLASON) return false;
    if (p.tab[2]->est != Z_BLASON) return false;
    if (p.tab[2]->ouest != Z_BLASON) return false;
    if (p.tab[2]->milieu != Z_BLASON) return false;

    if (p.tab[3]->nord != Z_ROUTE) return false;
    if (p.tab[3]->sud != Z_ROUTE) return false;
    if (p.tab[3]->est != Z_PRE) return false;
    if (p.tab[3]->ouest != Z_ROUTE) return false;
    if (p.tab[3]->milieu != Z_VILLAGE) return false;

    if (p.tab[4]->nord != Z_BLASON) return false;
    if (p.tab[4]->sud != Z_BLASON) return false;
    if (p.tab[4]->est != Z_BLASON) return false;
    if (p.tab[4]->ouest != Z_ROUTE) return false;
    if (p.tab[4]->milieu != Z_BLASON) return false;

    detruire_pile(&p);
    return true;
}

bool test_csv_fichier_vide(void)
{
    Pile p;
    p = lire_tuiles_csv("data/test/2_test.csv");

    if (p.nb_element_max != 0) return false;
    detruire_pile(&p);

    return true;
}

bool test_csv_fichier_introuvable(void)
{
    Pile p;
    p = lire_tuiles_csv("data/test/test_non_existant.csv");

    if (p.nb_element_max != 0) return false;
    detruire_pile(&p);

    return true;
}

bool test_csv_fichier_invalide(void)
{
    Pile p;
    p = lire_tuiles_csv("data/test/3_test.csv");

    if (p.nb_element_max != 0) return false;

    detruire_pile(&p);
    return true;
}

// ajout à la liste de tests à executer
Test unit_tests[] = {
    TEST(test_tuile_creer),
    TEST(test_tuile_compatibilite),
    TEST(test_grille_creer),
    TEST(test_grille_est_vide),
    TEST(test_grille_placer_tuile),
    TEST(test_init_pile),
    TEST(test_inserer_tuile),
    TEST(test_recup_tuile),
    TEST(test_varstring_ajout_char),
    TEST(test_varstring_ajout_chaine),
    TEST(test_varstring_vider),
    TEST(test_varstring_ajouter_null),
    TEST(test_varstring_null),
    TEST(test_csv_compter_lignes),
    TEST(test_csv_lecture_zone),
    TEST(test_csv_lecture_fichier),
    TEST(test_csv_fichier_vide),
    TEST(test_csv_fichier_introuvable),
    TEST(test_csv_fichier_invalide),
};

// ===========================
// ==== fin des fonctions ====
// ===========================

#define len(ARRAY) sizeof(ARRAY)/sizeof(ARRAY[0])
_Noreturn void lancer_tests(void)
{
    int passed = 0;
    int failed = 0;

    Test t;
    bool result;

    fprintf(stderr, "lancement de la suite de tests:\n");
    for (size_t i = 0; i < len(unit_tests); i++) {
        t = unit_tests[i];
        fprintf(stderr, "  \x1b[0;1m%s\x1b[0;0m:", t.name);
        result = t.run();

        if (result) {
            fprintf(stderr, "  ok\n");
            ++passed;
        } else {
            fprintf(stderr, "  \x1b[91mechec\x1b[0m\n");
            ++failed;
        }
    }

    fprintf(stderr, "resultats:\n");

    if (failed > 0) {
        fprintf(stderr, "  %d ok, \x1b[91m%d echec(s)\x1b[0m.. exiting\n", passed, failed);
        exit(EXIT_FAILURE);
    } else {
        fprintf(stderr, "  %d ok, %d echecs.. exiting\n", passed, failed);
        exit(EXIT_SUCCESS);
    }
}
