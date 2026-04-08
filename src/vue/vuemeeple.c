#include <stdlib.h>
#include "libca.h"
#include "raylib.h"
#include "plateau.h"
#include "render.h"
#include "vuemeeple.h"
#include "joueur.h"

PlacementMeeple creer_placement(Jeu *jeu, int x, int y)
{
    PlacementMeeple p = { 0 };

    Tuile t = get(jeu->grille, x, y);
    if (t->id_meeple == -1) {
        ca_warn("creation de placement pour un meeple absent");
        return p;
    }

    p.couleur = jeu->joueurs.tableau[t->id_meeple].couleur;

    switch (t->position_meeple) {
    case D_NORD:
        p.position.x = (x + 0.33f) * TEXTURE_SIZE;
        p.position.y = (y + 0.10f) * TEXTURE_SIZE;
        break;
    case D_SUD:
        p.position.x = (x + 0.33f) * TEXTURE_SIZE;
        p.position.y = (y + 0.60f) * TEXTURE_SIZE;
        break;
    case D_EST:
        p.position.x = (x + 0.60f) * TEXTURE_SIZE;
        p.position.y = (y + 0.33f) * TEXTURE_SIZE;
        break;
    case D_OUEST:
        p.position.x = (x + 0.10f) * TEXTURE_SIZE;
        p.position.y = (y + 0.33f) * TEXTURE_SIZE;
        break;
    case D_MILIEU:
        p.position.x = (x + 0.33f) * TEXTURE_SIZE;
        p.position.y = (y + 0.33f) * TEXTURE_SIZE;
    }

    return p;
}

ListePlacements creer_listeplacements(void)
{
    return (ListePlacements) { 0 };
}

void detruire_listeplacements(ListePlacements placements)
{
    free(placements.tab);
}

void rafraichir_listeplacements(ListePlacements *placements, Jeu *jeu)
{
    // ignorer les placements actuels
    placements->len = 0;

    // iterer sur les localisations meeple de chaque joueur
    // et creer un placement pour chaque.
    for (int i = 0; i < jeu->joueurs.nb_joueurs; i++) {
        for (L_meeple meeple = jeu->joueurs.tableau[i].localisation_meeple; meeple != NULL; meeple = meeple->next) {
            // reallouer de la place si il le faut
            if (placements->len == placements->capacite) {
                placements->tab = ca_realloc(placements->tab,
                                             placements->capacite + PLACEMENT_REALLOC_SIZE,
                                             sizeof(PlacementMeeple));
                placements->capacite += PLACEMENT_REALLOC_SIZE;
            }

            placements->tab[placements->len++] = creer_placement(jeu, meeple->x, meeple->y);
        }
    }
}

// fonction privée
void dessiner_meeple(Vector2 position, Color couleur)
{
    Rectangle champ = { .width = TEXTURE_SIZE * 0.3f, .height = TEXTURE_SIZE * 0.3f };
    Vector2 origin = { champ.width / 2.0f, champ.height / 2.0f};
    champ.x = position.x + origin.x;
    champ.y = position.y + origin.y;

    DrawRectanglePro(champ, origin, 45.0f, couleur);
}


void dessiner_listeplacements(ListePlacements placements, Plateau plateau)
{
    BeginMode2D(plateau.camera);
        for (int i = 0; i < placements.len; i++)
            dessiner_meeple(placements.tab[i].position, placements.tab[i].couleur);
    EndMode2D();
}
