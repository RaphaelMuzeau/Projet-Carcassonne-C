#include "raylib.h"
#include "page.h"
#include "sidebar.h"

enum Page page_jeu(void)
{
    // Etat initial
    enum Page prochaine_page = P_JEU;

    SetExitKey(KEY_NULL); // echape propose de sauvegarder la partie

    Controles ctrl = creer_controles();

    Joueur damien  = { 0 };
    damien.nom = "damien";
    damien.nb_meeple_restant = 5;

    CarteJoueur carte = creer_cartejoueur(GetScreenWidth() - SIDEBAR_WIDTH + 10, 200, &damien, RED);

    while (prochaine_page == P_JEU) {
        if (IsKeyPressed(KEY_ESCAPE))
            prochaine_page = P_TITRE;

        update_controles(&ctrl);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(GetScreenWidth() - SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, GetScreenHeight(), MAROON);
            dessiner_controles(ctrl);
            dessiner_cartejoueur(carte, false);
        EndDrawing();
    }
    SetExitKey(KEY_ESCAPE);

    return prochaine_page;
}
