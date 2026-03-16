#include <assert.h>

#include "raylib.h"
#include "raymath.h"
#include "scrollbar.h"

ScrollBar creer_scrollbar(Camera2D camera, float largeur)
{
    ScrollBar barre = { 0 };
    barre.camera = camera;
    barre.bouton = creer_bouton(GetScreenWidth() - largeur, 0, "");
    barre.bouton.champ.width = largeur;

    return barre;
}

void update_scrollbar(ScrollBar *barre, float fin_espace)
{
    int hauteur_ecran = GetScreenHeight();

    // evite la division par zero et se comporte comme attendu.
    if (fin_espace == 0)
        fin_espace = 1;

    // permet de scroller avec la molette ou en tirant le bouton
    barre->camera.target.y += - GetMouseWheelMoveV().y * 50.0f;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), barre->bouton.champ))
            barre->est_tire = true;
    } else {
        barre->est_tire = false;
    }

    if (barre->est_tire)
        barre->camera.target.y = (GetMouseY() - barre->bouton.champ.height/2) * (fin_espace / hauteur_ecran);

    // garde la camera entre le debut et la fin de la page sans depasser
    if (fin_espace - hauteur_ecran < 0.0f)
        barre->camera.target.y = 0.0f;
    else
        barre->camera.target.y = Clamp(barre->camera.target.y, 0.0f, fin_espace - hauteur_ecran);

    // adapte la hauteur du bouton à la hauteur scrollable
    barre->bouton.champ.height = hauteur_ecran * (hauteur_ecran / fin_espace);

    // place le bouton au niveau actuel dans la page proportionellement à l'ecran
    barre->bouton.champ.y = hauteur_ecran * (barre->camera.target.y / fin_espace);

    // replace la barre à droite
    barre->bouton.champ.x = GetScreenWidth() - barre->bouton.champ.width;

    update_bouton(&barre->bouton); // cliquer ne fait rien
}

void dessiner_scrollbar(ScrollBar barre)
{
    // dessine le fond et le bouton tirable
    DrawRectangle(barre.bouton.champ.x, 0, barre.bouton.champ.width, GetScreenHeight(), SCROLL_BACKGROUND_COLOR);
    dessiner_bouton(barre.bouton);
}
