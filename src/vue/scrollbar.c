#include "raylib.h"
#include "raymath.h"
#include "scrollbar.h"

ScrollBar creer_scrollbar(Camera2D vue)
{
    ScrollBar barre = { 0 };
    barre.vue = vue;
    barre.bouton.champ.width = SCROLL_DEFAULT_WIDTH;

    return barre;
}

void update_scrollbar(ScrollBar *barre, Rectangle ecran, float hauteur_espace)
{
    // evite la division par zero et se comporte comme attendu.
    if (hauteur_espace == 0)
        hauteur_espace = 1;
    if (ecran.height == 0)
        ecran.height = 1;

    // permet de scroller avec la molette ou en tirant le bouton
    if (CheckCollisionPointRec(GetMousePosition(), ecran))
        barre->vue.target.y -= GetMouseWheelMoveV().y * 50.0f;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), barre->bouton.champ))
            barre->est_tire = true;
    } else {
        barre->est_tire = false;
    }

    if (barre->est_tire)
        barre->vue.target.y = (GetMouseY() - barre->bouton.champ.height/2) * (hauteur_espace / ecran.height);

    // garde la camera entre le debut et la fin de la page sans depasser
    if (hauteur_espace < ecran.height)
        barre->vue.target.y = ecran.x; // Clamp privilegie la borne superieure
    else
        barre->vue.target.y = Clamp(barre->vue.target.y, ecran.x, hauteur_espace - ecran.height);

    // adapte la hauteur du bouton à la hauteur scrollable
    barre->bouton.champ.height = ecran.height * (ecran.height / hauteur_espace);

    // place le bouton au niveau actuel dans la page proportionellement à l'ecran
    barre->bouton.champ.y = ecran.height * (barre->vue.target.y / hauteur_espace);

    // replace la barre à droite
    barre->bouton.champ.x = ecran.width - barre->bouton.champ.width;

    update_bouton(&barre->bouton); // cliquer ne fait rien
}

void dessiner_scrollbar(ScrollBar barre, Rectangle ecran)
{
    // dessine le fond et le bouton tirable
    DrawRectangle(barre.bouton.champ.x, ecran.y, barre.bouton.champ.width, ecran.height, SCROLL_BACKGROUND_COLOR);
    dessiner_bouton(barre.bouton);
}
