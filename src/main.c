#include <stdbool.h>
#include "libca.h"
#include "test.h"
#include "raylib.h"
#include "bouton.h"

int main(void)
{
#ifdef RUN_UNIT_TESTS
    lancer_tests(); // le reste du code ne sera pas executé
#endif

    // Configuration global
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    int largeur_ecran = 640;
    int hauteur_ecran = 480;

    // Etat initial
    InitWindow(largeur_ecran, hauteur_ecran, "Carcassonne");
    SetTargetFPS(60);
    bool doit_quitter = false;

    // Elements de la page
    Texture2D fond = LoadTexture("data/pictures/fond.png");
    float echelle = 1.0f;

    Texte titre = creer_texte("Carcassonne", 50, 50);
    titre.taille = 60;

    Bouton btn_nouvelle = creer_bouton(50, 150, "Nouvelle Partie");
    Bouton btn_custom   = creer_bouton(50, 220, "Nouvelle Partie Custom");
    Bouton btn_charger  = creer_bouton(50, 290, "Charger Partie");
    Bouton btn_regles   = creer_bouton(50, 360, "Règles du Jeu");
    Bouton btn_quitter  = creer_bouton(50, 430, "Quitter Le Jeu");

    while (!WindowShouldClose() && !doit_quitter) {
        largeur_ecran = GetScreenWidth();
        hauteur_ecran = GetScreenHeight();

        update_bouton(&btn_nouvelle);
        update_bouton(&btn_custom);
        update_bouton(&btn_charger);
        update_bouton(&btn_regles);

        if (update_bouton(&btn_quitter))
            doit_quitter = true;

        echelle = max((float) largeur_ecran / fond.width, (float) hauteur_ecran / fond.height);

        if (echelle < 1)
            echelle = 1;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureEx(fond, (Vector2) { 0.0f, 0.0f }, 0.0f, echelle, WHITE);
            dessiner_texte(titre);

            dessiner_bouton(btn_nouvelle);
            dessiner_bouton(btn_custom);
            dessiner_bouton(btn_charger);
            dessiner_bouton(btn_regles);
            dessiner_bouton(btn_quitter);
        EndDrawing();
    }
    UnloadTexture(fond);

    CloseWindow();


    return 0;
}
