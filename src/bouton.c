#include "raylib.h"
#include "bouton.h"

/* relation au texte */

void adapter_bouton(Bouton *b)
{
    Vector2 taille_texte = mesurer_texte(b->texte);
    b->champ.width = taille_texte.x + 20.0f;
    b->champ.height = taille_texte.y + 20.0f;
    b->texte.position.x = b->champ.x + 10.0f;
    b->texte.position.y = b->champ.y + 10.0f;
}

void centrer_texte(Bouton *b)
{
    Vector2 taille_texte = mesurer_texte(b->texte);
    b->texte.position.x = b->champ.x + (b->champ.width  - taille_texte.x) / 2.0f;
    b->texte.position.y = b->champ.y + (b->champ.height - taille_texte.y) / 2.0f;
}

/* creation */

Bouton creer_bouton(float x, float y, char *texte)
{
    return (Bouton) {
        .champ = (Rectangle) { .x = x, .y = y },
        .couleur = BOUTON_COLOR_NEUTRAL,
        .texte = creer_texte(x, y, texte),
    };
}

Bouton creer_bouton_adapte(float x, float y, char *texte)
{
    Bouton b = creer_bouton(x, y, texte);
    adapter_bouton(&b);

    return b;
}

/* mise à jour */

bool update_bouton(Bouton *b)
{
    // verifier si le bouton est survolé, enfoncé ou neutre
    bool est_clique = false;
    enum { E_NEUTRE, E_SURVOLE, E_ENFONCE } etat = E_NEUTRE;

    if (CheckCollisionPointRec(GetMousePosition(), b->champ)) {
        etat = E_SURVOLE;
        // unique clique
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            est_clique = true;
        // clique maintenu
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

bool update_bouton_camera(Bouton *b, Camera2D camera)
{
    // verifier si le bouton est survolé, enfoncé ou neutre
    bool est_clique = false;
    enum { E_NEUTRE, E_SURVOLE, E_ENFONCE } etat = E_NEUTRE;

    if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), b->champ)) {
        etat = E_SURVOLE;
        // unique clique
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            est_clique = true;
        // clique maintenu
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

bool update_bouton_adapte(Bouton *b)
{
    adapter_bouton(b);
    return update_bouton(b);
}

bool update_bouton_centre(Bouton *b)
{
    centrer_texte(b);
    return update_bouton(b);
}

/* dessin */

void dessiner_bouton(Bouton b)
{
    DrawRectangleRec(b.champ, b.couleur);
    DrawRectangleLinesEx(b.champ, 4, BLACK);
    dessiner_texte(b.texte);
}
