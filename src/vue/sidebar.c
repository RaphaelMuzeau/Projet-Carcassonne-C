#include <stdio.h>
#include "raylib.h"
#include "libca.h"
#include "sidebar.h"
#include "render.h"

/* Controles */

Controles creer_controles(int nb_tuiles)
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

    ctrl.nb_tuiles.contenu = ca_alloc(10, sizeof(char));
    ctrl.nb_tuiles = creer_texte(0, ctrl.apercu.y + ctrl.apercu.height + 10.0f, ctrl.nb_tuiles.contenu);
    ctrl.nb_tuiles.couleur = BLUE;
    rafraichir_controles(&ctrl, nb_tuiles);

    return ctrl;
}

void detruire_controles(Controles ctrl)
{
    free(ctrl.nb_tuiles.contenu);
}

void update_controles(Controles *ctrl)
{
    int largeur_ecran = GetScreenWidth();

    float dec = largeur_ecran - ctrl->champ.width - ctrl->champ.x;
    ctrl->champ.x              += dec;
    ctrl->apercu.x             += dec;
    ctrl->rotation.champ.x     += dec;
    ctrl->detruire.champ.x     += dec;
    ctrl->nb_tuiles.position.x += dec;
}

void rafraichir_controles(Controles *ctrl, int nb_tuiles)
{
    snprintf(ctrl->nb_tuiles.contenu, 10, "%d", nb_tuiles);
    ctrl->nb_tuiles.position.x = ctrl->apercu.x + ctrl->apercu.width/2.0f - mesurer_texte(ctrl->nb_tuiles).x/2.0f;
}

void dessiner_controles(Controles ctrl, RenderTexture2D render_tuile, float rotation)
{
    DrawRectangleRec(ctrl.champ, DARKGRAY);
    dessiner_bouton(ctrl.rotation);
    dessiner_bouton(ctrl.detruire);
    dessiner_texte(ctrl.nb_tuiles);

    Rectangle source = { .width = TEXTURE_SIZE, .height = -TEXTURE_SIZE };
    Vector2 origin = { CONTROLES_APERCU_SIZE / 2.0f, CONTROLES_APERCU_SIZE / 2.0f };
    ctrl.apercu.x += CONTROLES_APERCU_SIZE/2.0f;
    ctrl.apercu.y += CONTROLES_APERCU_SIZE/2.0f;

    DrawTexturePro(render_tuile.texture, source, ctrl.apercu, origin, rotation, WHITE);
}

/* CarteJoueur */

CarteJoueur creer_cartejoueur(float x, float y, Joueur *joueur)
{
    CarteJoueur carte = { 0 };

    carte.champ.height = CARTEJOUEUR_HEIGHT;
    carte.champ.width  = CARTEJOUEUR_WIDTH;
    carte.champ.x = x;
    carte.champ.y = y;

    carte.joueur  = joueur;

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
    DrawRectangleRoundedLinesEx(carte.champ, 0.30f, 1, 4, tour ? carte.joueur->couleur : BLACK);
    DrawLineEx((Vector2) {carte.champ.x, carte.champ.y + 30},
               (Vector2) {carte.champ.x + carte.champ.width, carte.champ.y + 30},
               4, tour ? carte.joueur->couleur : BLACK);
    dessiner_texte(carte.texte_nom);
    dessiner_texte(carte.texte_pts);
    dessiner_texte(carte.texte_meeple);
}

/* BarreJoueur */

BarreJoueurs creer_barrejoueurs(ListeJoueurs joueurs)
{
    BarreJoueurs barre = { 0 };
    barre.joueurs = joueurs;

    Camera2D vue = { 0 };
    vue.zoom = 1.0f;
    barre.scrollbar = creer_scrollbar(vue);

    barre.champ.x = GetScreenWidth() - SIDEBAR_WIDTH;
    barre.champ.y = CONTROLES_HEIGHT;
    barre.champ.width = SIDEBAR_WIDTH;
    barre.champ.height = GetScreenHeight() - CONTROLES_HEIGHT;

    float y = barre.champ.y + 10.0f;
    barre.cartes = ca_alloc(joueurs.nb_joueurs, sizeof(CarteJoueur));
    for (int i = 0; i < joueurs.nb_joueurs; i++) {
        barre.cartes[i] = creer_cartejoueur(0, y, &barre.joueurs.tableau[i]);
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

void dessiner_barrejoueurs(BarreJoueurs barre, int id_tour)
{
    DrawRectangleRec(barre.champ, LIGHTGRAY);
    BeginMode2D(barre.scrollbar.vue);
        for (int i = 0; i < barre.joueurs.nb_joueurs; i++)
            dessiner_cartejoueur(barre.cartes[i], i == id_tour);
    EndMode2D();
    dessiner_scrollbar(barre.scrollbar, barre.champ);
}

void update_barrejoueurs(BarreJoueurs *barre)
{
    update_scrollbar(&barre->scrollbar, barre->champ, barre->fin_liste);

    barre->champ.x = GetScreenWidth() - SIDEBAR_WIDTH;
    barre->champ.height = GetScreenHeight() - CONTROLES_HEIGHT;

    for (int i = 0; i < barre->joueurs.nb_joueurs; i++) {
        float dec = (barre->champ.x + (barre->champ.width - SCROLL_DEFAULT_WIDTH)/2) - CARTEJOUEUR_WIDTH/2 - barre->cartes[i].champ.x;
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
