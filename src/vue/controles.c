#include "raylib.h"
#include "bouton.h"
#include "controles.h"

Controles creer_controles(void)
{
    int largeur_ecran = GetScreenWidth();
    Controles ctrl = { 0 };

    ctrl.apercu.x = largeur_ecran - 128 - CONTROLES_DEFAULT_PADDING;
    ctrl.apercu.y = CONTROLES_DEFAULT_PADDING;
    ctrl.apercu.width = 128;
    ctrl.apercu.height = 128;

    ctrl.rotation = creer_bouton_adapte(ctrl.apercu.x - 80, 10 + CONTROLES_DEFAULT_PADDING, "90°");
    ctrl.detruire = creer_bouton_adapte(ctrl.apercu.x - 80, ctrl.rotation.champ.y + 60, " X ");

    ctrl.champ.x = ctrl.rotation.champ.x - CONTROLES_DEFAULT_PADDING;
    ctrl.champ.y = 0;
    ctrl.champ.width = largeur_ecran - ctrl.champ.x;
    ctrl.champ.height = 128 + 2*CONTROLES_DEFAULT_PADDING;

    return ctrl;
}

void update_controles(Controles *ctrl)
{
    int largeur_ecran = GetScreenWidth();

    float dec = largeur_ecran - ctrl->champ.width - ctrl->champ.x;
    ctrl->champ.x          += dec;
    ctrl->apercu.x         += dec;
    ctrl->rotation.champ.x += dec;
    ctrl->detruire.champ.x += dec;

    update_bouton_adapte(&ctrl->rotation);
    update_bouton_adapte(&ctrl->detruire);
}

void dessiner_controles(Controles ctrl)
{
    DrawRectangleRec(ctrl.champ, DARKGRAY);
    DrawRectangleRec(ctrl.apercu, DARKGREEN);
    dessiner_bouton(ctrl.rotation);
    dessiner_bouton(ctrl.detruire);
}
