#include "raylib.h"
#include "page.h"
#include "sidebar.h"

enum Page page_jeu(Jeu jeu)
{
    // Etat initial
    enum Page prochaine_page = P_JEU;
    int largeur_ecran = GetScreenWidth();
    int hauteur_ecran = GetScreenHeight();

    Controles ctrl = creer_controles();

    Rectangle champ_barrejoueurs = { 0 };
    champ_barrejoueurs.x = largeur_ecran - SIDEBAR_WIDTH;
    champ_barrejoueurs.y = CONTROLES_HEIGHT;
    champ_barrejoueurs.width = SIDEBAR_WIDTH;
    champ_barrejoueurs.height = hauteur_ecran - CONTROLES_HEIGHT;

    BarreJoueurs barrejoueurs = creer_barrejoueurs(jeu.joueurs, champ_barrejoueurs);

    while (prochaine_page == P_JEU) {
        largeur_ecran = GetScreenWidth();
        hauteur_ecran = GetScreenHeight();

        if (WindowShouldClose())
            prochaine_page = P_TITRE;

        champ_barrejoueurs.x = largeur_ecran - SIDEBAR_WIDTH;
        champ_barrejoueurs.height = hauteur_ecran - CONTROLES_HEIGHT;

        update_controles(&ctrl);
        update_barrejoueurs(&barrejoueurs, champ_barrejoueurs);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            dessiner_barrejoueurs(barrejoueurs, champ_barrejoueurs, 3);
            dessiner_controles(ctrl);
        EndDrawing();
    }

    detruire_jeu(jeu);
    return prochaine_page;
}
