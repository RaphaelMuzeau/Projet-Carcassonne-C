#include <stdio.h>
#include "raylib.h"
#include "libca.h"
#include "sidebar.h"

/* Controles */

Controles creer_controles(void)
{
    int largeur_ecran = GetScreenWidth();
    Controles ctrl = { 0 };

    ctrl.champ.x = largeur_ecran - SIDEBAR_WIDTH;
    ctrl.champ.y = 0;
    ctrl.champ.width  = SIDEBAR_WIDTH;
    ctrl.champ.height = CONTROLES_HEIGHT;

    ctrl.apercu.x = largeur_ecran - CONTROLES_APERCU_SIZE - CONTROLES_PADDING;
    ctrl.apercu.y = CONTROLES_PADDING;
    ctrl.apercu.width  = CONTROLES_APERCU_SIZE;
    ctrl.apercu.height = CONTROLES_APERCU_SIZE;

    ctrl.rotation = creer_bouton_adapte(0, 10 + CONTROLES_PADDING, "90°");
    ctrl.rotation.champ.x = (ctrl.champ.x + ctrl.apercu.x) / 2 - ctrl.rotation.champ.width / 2;

    ctrl.detruire = creer_bouton_adapte(0, ctrl.rotation.champ.y + 60, " X ");
    ctrl.detruire.champ.x = (ctrl.champ.x + ctrl.apercu.x) / 2 - ctrl.detruire.champ.width / 2;

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

/* CarteJoueur */

CarteJoueur creer_cartejoueur(float x, float y, Joueur *joueur, Color couleur)
{
    CarteJoueur carte = { 0 };

    carte.champ.height = CARTEJOUEUR_HEIGHT;
    carte.champ.width  = CARTEJOUEUR_WIDTH;
    carte.champ.x = x;
    carte.champ.y = y;

    carte.joueur  = joueur;
    carte.couleur = couleur;

    carte.texte_nom = creer_texte(0, y + 3, joueur->nom);
    carte.texte_nom.position.x = x + carte.champ.width/2 - mesurer_texte(carte.texte_nom).x/2;

    carte.texte_meeple = creer_texte(carte.champ.x + 10, y + 40, ca_alloc(16, sizeof(char)));
    snprintf(carte.texte_meeple.contenu, 16, "meeple : %d", joueur->nb_meeple_restant);

    carte.texte_pts = creer_texte(carte.champ.x + 10, y + 70, ca_alloc(16, sizeof(char)));
    snprintf(carte.texte_pts.contenu, 16, "points : %d", joueur->pts);

    return carte;
}

void detruire_cartejoueur(CarteJoueur carte)
{
    free(carte.texte_meeple.contenu);
    free(carte.texte_pts.contenu);
}

void rafraichir_cartejoueur(CarteJoueur *carte)
{
    snprintf(carte->texte_meeple.contenu, 16, "meeple : %d", carte->joueur->nb_meeple_restant);
    snprintf(carte->texte_pts.contenu, 16, "points : %d", carte->joueur->pts);
}

void dessiner_cartejoueur(CarteJoueur carte, bool tour)
{
    DrawRectangleRounded(carte.champ, 0.30f, 1, RAYWHITE);
    DrawRectangleRoundedLinesEx(carte.champ, 0.30f, 1, 4, tour ? carte.couleur : BLACK);
    DrawLineEx((Vector2) {carte.champ.x, carte.champ.y + 30},
               (Vector2) {carte.champ.x + carte.champ.width, carte.champ.y + 30},
               4, tour ? carte.couleur : BLACK);
    dessiner_texte(carte.texte_nom);
    dessiner_texte(carte.texte_pts);
    dessiner_texte(carte.texte_meeple);
}

/* BarreJoueur */

BarreJoueurs creer_barrejoueurs(ListeJoueurs joueurs, Rectangle ecran)
{
    BarreJoueurs barre = { 0 };
    barre.joueurs = joueurs;

    Camera2D vue = { 0 };
    vue.zoom = 1.0f;
    barre.scrollbar = creer_scrollbar(vue);

    float y = ecran.y + 10.0f;
    barre.cartes = ca_alloc(joueurs.nb_joueurs, sizeof(CarteJoueur));
    for (int i = 0; i < joueurs.nb_joueurs; i++) {
        barre.cartes[i] = creer_cartejoueur(0, y, &barre.joueurs.tableau[i], RED);
        y += CARTEJOUEUR_HEIGHT + 20.0f;
    }
    barre.fin_liste = y - CARTEJOUEUR_HEIGHT;

    return barre;
}

void detruire_barrejoueurs(BarreJoueurs barre)
{
    for (int i = 0; i < barre.joueurs.nb_joueurs; i++) {
        detruire_cartejoueur(barre.cartes[i]);
        free(barre.cartes[i].joueur->nom);
    }
    free(barre.cartes);
}

void dessiner_barrejoueurs(BarreJoueurs barre, Rectangle ecran, int id_tour)
{
    DrawRectangleRec(ecran, LIGHTGRAY);
    BeginMode2D(barre.scrollbar.vue);
        for (int i = 0; i < barre.joueurs.nb_joueurs; i++)
            dessiner_cartejoueur(barre.cartes[i], i == id_tour);
    EndMode2D();
    dessiner_scrollbar(barre.scrollbar, ecran);
}

void update_barrejoueurs(BarreJoueurs *barre, Rectangle ecran)
{
    update_scrollbar(&barre->scrollbar, ecran, barre->fin_liste);

    for (int i = 0; i < barre->joueurs.nb_joueurs; i++) {
        float dec = (ecran.x + (ecran.width - SCROLL_DEFAULT_WIDTH)/2) - CARTEJOUEUR_WIDTH/2 - barre->cartes[i].champ.x;
        barre->cartes[i].champ.x += dec;
        barre->cartes[i].texte_nom.position.x += dec;
        barre->cartes[i].texte_pts.position.x += dec;
        barre->cartes[i].texte_meeple.position.x += dec;
    }
}

// met à jour le compteur de points et de meeple de chaque joueur
void rafraichir_barrejoueurs(BarreJoueurs *barre)
{
    for (int i = 0; i < barre->joueurs.nb_joueurs; i++)
        rafraichir_cartejoueur(&barre->cartes[i]);
}
