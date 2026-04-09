#include <stdbool.h>
#include "libca.h"
#include "grille.h"
#include "tuile.h"
#include "meeple.h"
#include "joueur.h"

bool est_vide(Vec2D grille, int x, int y)
{
    return get(grille, x, y) == NULL;
}

bool placer_tuile(Vec2D *grille, int x, int y, Tuile t)
{
    // Tuile occupé
    if (!est_vide(*grille, x, y))
        return false;

    // Tuile sans connexion
    if (est_vide(*grille, x - 1, y) && est_vide(*grille, x + 1, y) && est_vide(*grille, x, y - 1) && est_vide(*grille, x, y + 1))
        return false;

    // compatibilité
    if (!compatibilite_tuile(t, get(*grille, x - 1, y), D_OUEST)
     || !compatibilite_tuile(t, get(*grille, x + 1, y), D_EST)
     || !compatibilite_tuile(t, get(*grille, x, y - 1), D_NORD)
     || !compatibilite_tuile(t, get(*grille, x, y + 1), D_SUD))
        return false;

    set(grille, t, x, y);
    return true;
}

/* recherche */

int zone_pts(enum Zone zone, bool fin)
{
    if (zone == Z_ROUTE)  return 1;
    if (zone == Z_VILLE)  return fin ? 1 : 2;
    if (zone == Z_BLASON) return fin ? 2 : 4;
    return 0;
}

// invariant: t->d_arrive & zone
int recherche(Vec2D grille, int *nb_meeple, L_meeple *loc_meeple, int x, int y, enum Zone z, enum Direction d_arrive, bool fin)
{
    Tuile t = get(grille, x, y);

    if (t == NULL) return fin ? 0 : -1;

    int pts = zone_pts(zone_tuile(t, d_arrive), fin);
    if (pts == 0) return 0;

    bool complete = true; // la zone est-elle complete ?

    /* traiter le cas du coté isolé (fin de zone)
     * cette zone n'a pas encore été verifié car
     * la recherche ne revient pas sur ses pas. */
    if (!(t->milieu & z)) {
        if (t->est_verifie)
            pts = 0; // pas besoin d'ajouter des points dupliqués
        t->est_verifie = true;

        // ajouter un meeple aux liste si il est sur le coté d'arrivée
        if (t->id_meeple != -1 && t->position_meeple == d_arrive) {
            nb_meeple[t->id_meeple] += 1;
            L_meeple maillon = creer_maillon_meeple(x, y);
            ajouter_maillon_meeple(loc_meeple, maillon);
        }
    }
    /* on peut acceder aux autres cotes par le milieu et la tuile
     * n'a pas encore été verifiée */
    else if (!t->est_verifie) {
        t->est_verifie = true;

        // effectuer la recherche sur tous les cote de la tuile
        for (enum Direction d_depart = 0; d_depart < D_MILIEU; ++d_depart) {
            if (d_depart == d_arrive)
                continue; // on ne revient pas sur nos pas

            if (!(zone_tuile(t, d_depart) & z))
                continue; // ce n'est pas la zone cherché

            int tmp = 0;
            if      (d_depart == D_NORD)  tmp = recherche(grille, nb_meeple, loc_meeple, x,     y - 1, z, D_SUD,   fin);
            else if (d_depart == D_SUD)   tmp = recherche(grille, nb_meeple, loc_meeple, x,     y + 1, z, D_NORD,  fin);
            else if (d_depart == D_EST)   tmp = recherche(grille, nb_meeple, loc_meeple, x + 1,     y, z, D_OUEST, fin);
            else if (d_depart == D_OUEST) tmp = recherche(grille, nb_meeple, loc_meeple, x - 1,     y, z, D_EST,   fin);
            else ca_error("valeur d'enumeration invalide");

            if (tmp == -1) complete = false;

            pts += tmp;
        }

        // ajouter un meeple aux listes s'il est present et accessible par le milieu
        if (t->id_meeple != -1 && zone_tuile(t, t->position_meeple) & z) {
            nb_meeple[t->id_meeple] += 1;
            L_meeple maillon = creer_maillon_meeple(x, y);
            ajouter_maillon_meeple(loc_meeple, maillon);
        }
    } else
        return 0;

    return complete ? pts : -1;
}

int amorce_recherche(Vec2D grille, int *nb_meeple, L_meeple *loc_meeple, int x, int y, enum Direction d, bool fin)
{
    Tuile t = get(grille, x, y);
    enum Zone z = zone_tuile(t, d);

    // si on peut commencer par le milieu, pas besoin d'amorce
    if (t->milieu & z)
        return recherche(grille, nb_meeple, loc_meeple, x, y, z, D_MILIEU, fin);

    t->est_verifie = true;

    // sinon, on ajoute le meeple present si il y en a un
    if (t->id_meeple != -1 && t->position_meeple == d) {
        nb_meeple[t->id_meeple] += 1;
        L_meeple maillon = creer_maillon_meeple(x, y);
        ajouter_maillon_meeple(loc_meeple, maillon);
    }

    // et on commence par la prochaine tuile
    int pts = 0;
    if      (d == D_NORD)  pts = recherche(grille, nb_meeple, loc_meeple, x,     y - 1, z, D_SUD,   fin);
    else if (d == D_SUD)   pts = recherche(grille, nb_meeple, loc_meeple, x,     y + 1, z, D_NORD,  fin);
    else if (d == D_EST)   pts = recherche(grille, nb_meeple, loc_meeple, x + 1,     y, z, D_OUEST, fin);
    else if (d == D_OUEST) pts = recherche(grille, nb_meeple, loc_meeple, x - 1,     y, z, D_EST,   fin);
    else ca_error("valeur d'enumeration invalide");

    if (pts == -1)
        return -1;
    return pts + zone_pts(z, fin);
}

void recherche_est_verifie(Vec2D grille, int x, int y)
{
    Tuile t = get(grille, x , y);

    if (t == NULL || !t->est_verifie)
        return;

    t->est_verifie = false;
    recherche_est_verifie(grille, x+1, y);
    recherche_est_verifie(grille, x-1, y);
    recherche_est_verifie(grille, x, y+1);
    recherche_est_verifie(grille, x, y-1);
}

/* recherche abbaye */

void recherche_abbaye(Vec2D grille, ListeJoueurs joueurs, int x, int y, bool fin)
{
    Tuile t = get(grille, x, y);
    if (t == NULL) return;

    int id_joueur = t->id_meeple;
    int pts = 0;

    if (id_joueur == -1) return;

    for(int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            Tuile tmp = get(grille, x + i, y + j);

            if (tmp != NULL)
                pts += 1;
            else if (!fin)
                return;

        }
    }

    joueurs.tableau[id_joueur].pts += pts;
    if (!fin) retirer_meeple(grille, joueurs, x, y);
    else t->id_meeple = -1;
}

void verification_abbaye(Vec2D grille, ListeJoueurs joueurs, int x, int y)
{
    for(int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
            Tuile t = get(grille, x + i, y + j);
            if (t != NULL && t->milieu == Z_ABBAYE)
                recherche_abbaye(grille, joueurs, x + i, y + j, false);
        }
}

/* placement de meeple */

bool placer_meeple(Vec2D grille, Joueur *joueur, int x, int y, enum Direction d)
{
    Tuile t = get(grille, x, y);

    if (t == NULL) return false;
    if (t->id_meeple != -1) return false;
    if (zone_tuile(t, d) & (Z_PRE | Z_VILLAGE)) return false;
    if (joueur->nb_meeple_restant <= 0) return false;

    L_meeple new = creer_maillon_meeple(x, y);
    ajouter_maillon_meeple(&joueur->localisation_meeple, new);
    t->id_meeple = joueur->id;
    t->position_meeple = d;

    joueur->nb_meeple_restant -= 1;

    return true;
}

void retirer_meeple(Vec2D grille, ListeJoueurs listejoueurs, int x, int y)
{
    Tuile t = get(grille, x, y);

    if (t == NULL) return;
    if (t->id_meeple == -1) return;

    listejoueurs.tableau[t->id_meeple].nb_meeple_restant += 1;
    retirer_maillon_meeple(&listejoueurs.tableau[t->id_meeple].localisation_meeple, x, y);
    t->id_meeple = -1;
}
