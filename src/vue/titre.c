#include "libca.h"
#include "raylib.h"
#include "page.h"
#include "bouton.h"

enum Page page_titre(void)
{
    // Etat initial
    enum Page prochaine_page = P_TITRE;

    int largeur_ecran = 0;
    int hauteur_ecran = 0;

    // Elements de la page
    Texture2D fond = LoadTexture("data/pictures/fond.png");
    float echelle = 1.0f;

    Texte titre = creer_texte(50, 50, "Carcassonne");
    titre.taille = 60;

    Bouton btn_nouvelle = creer_bouton_adapte(50, 150, "Nouvelle Partie");
    Bouton btn_custom   = creer_bouton_adapte(50, 220, "Nouvelle Partie Custom");
    Bouton btn_charger  = creer_bouton_adapte(50, 290, "Charger Partie");
    Bouton btn_regles   = creer_bouton_adapte(50, 360, "Règles du Jeu");
    Bouton btn_quitter  = creer_bouton_adapte(50, 430, "Quitter Le Jeu");

    while (prochaine_page == P_TITRE) {
        largeur_ecran = GetScreenWidth();
        hauteur_ecran = GetScreenHeight();

        if (update_bouton(&btn_nouvelle)) prochaine_page = P_JEU; // FIXME
        if (update_bouton(&btn_custom))   prochaine_page = P_CUSTOM;
        if (update_bouton(&btn_charger))  prochaine_page = P_CHARGER;
        if (update_bouton(&btn_regles))   prochaine_page = P_REGLES;
        if (update_bouton(&btn_quitter) || WindowShouldClose()) prochaine_page = P_QUITTER;

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

    return prochaine_page;
}
