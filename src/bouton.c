#include <stdbool.h>
#include "raylib.h"
#include "bouton.h"

Bouton creer_bouton(float x, float y, char *texte)
{
    return (Bouton) {
        .champ = (Rectangle) { .x = x, .y = y },
        .couleur = BOUTON_COLOR_NEUTRAL,
        .texte = creer_texte(texte, 0, 0),
    };
}

bool update_bouton(Bouton *b)
{
    // mettre à jour la taille du bouton et centrer le texte
    // en laissant une marge de 10px sur chaque bord
    Vector2 taille_texte = mesurer_texte(b->texte);
    b->champ.width = taille_texte.x + 20.0f;
    b->champ.height = taille_texte.y + 20.0f;
    b->texte.position.x = b->champ.x + 10.0f;
    b->texte.position.y = b->champ.y + 10.0f;

    // verifier si le bouton est survolé, enfoncé ou neutre
    bool est_clique = false;
    enum { E_NEUTRE, E_SURVOLE, E_ENFONCE } etat = E_NEUTRE;

    if (CheckCollisionPointRec(GetMousePosition(), b->champ)) {
        etat = E_SURVOLE;
        // unique click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            est_clique = true;
        // click maintenu
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            etat = E_ENFONCE;
    }

    // changer la couleur selon l'etat
    switch (etat) {
    case E_NEUTRE:
        b->couleur = BOUTON_COLOR_NEUTRAL;
        break;
    case E_SURVOLE:
        b->couleur = BOUTON_COLOR_HOVERED;
        break;
    case E_ENFONCE:
        b->couleur = BOUTON_COLOR_PUSHED;
        break;
    }

    return est_clique;
}

void dessiner_bouton(Bouton b)
{
    DrawRectangleRec(b.champ, b.couleur);
    DrawRectangleLinesEx(b.champ, 4, BLACK);
    dessiner_texte(b.texte);
}
