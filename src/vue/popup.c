#include "raylib.h"
#include "bouton.h"
#include "champsaisie.h"
#include "popup.h"

Popup creer_popup(void)
{
    Popup popup = { 0 };
    popup.champ = (Rectangle) { .width = 700, .height = 200 };
    popup.champ_partie = creer_champsaisie(0, 0, popup.champ.width * 0.8f, 50, false);
    popup.annuler = creer_bouton_adapte(0, 0, "annuler");
    popup.sauvegarder = creer_bouton_adapte(0, 0, "sauvegarder\net quitter");
    popup.quitter = creer_bouton_adapte(0, 0, "quitter sans\nsauvegarder");
    popup.annuler.texte.taille = 20.0f;
    popup.quitter.texte.taille = 15.0f;
    popup.sauvegarder.texte.taille = 15.0f;

    return popup;
}

void centrer_popup(Popup *p)
{
    p->champ.x = GetScreenWidth()/2.0f - p->champ.width/2.0f;
    p->champ.y = GetScreenHeight()/2.0f - p->champ.height/2.0f;

    p->champ_partie.champ.x = p->champ.x + p->champ.width * 0.1f;
    p->champ_partie.champ.y = p->champ.y + p->champ.height * 0.25f;

    p->quitter.champ.x = p->champ.x + p->champ.width * 0.1f;
    p->quitter.champ.y = p->champ_partie.champ.y + p->champ_partie.champ.height + 20.0f;

    p->annuler.champ.x = p->champ.x + p->champ.width/2.0f - p->annuler.champ.width/2.0f;
    p->annuler.champ.y = p->champ_partie.champ.y + p->champ_partie.champ.height + 20.0f;
    p->annuler.champ.height = p->quitter.champ.height;

    p->sauvegarder.champ.x = p->champ.x + p->champ.width * 0.9f - p->sauvegarder.champ.width;
    p->sauvegarder.champ.y = p->champ_partie.champ.y + p->champ_partie.champ.height + 20.0f;
}

void dessiner_popup(Popup p)
{
    DrawRectangleRec(p.champ, DARKGRAY);
    dessiner_champsaisie(p.champ_partie);
    dessiner_bouton(p.annuler);
    dessiner_bouton(p.sauvegarder);
    dessiner_bouton(p.quitter);
}

