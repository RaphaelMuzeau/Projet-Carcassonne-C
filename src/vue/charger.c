#include <stdlib.h>
#include "raylib.h"
#include "page.h"
#include "scrollbar.h"
#include "listeparties.h"

// devra prendre en argument des pointeurs vers les structures
// du modèle à charger.
enum Page page_charger(void)
{
    // Etat initial
    enum Page prochaine_page = P_CHARGER;

    SetExitKey(KEY_NULL); // echape retourne à l'ecran titre au lieu de fermer la fenetre

    Rectangle ecran = { 0 };
    Camera2D vue = { 0 };
    vue.zoom = 1.0f;

    // Elements de la page
    ScrollBar scrollbar = creer_scrollbar(vue);

    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Bouton rafraichir = creer_bouton_adapte(retour.champ.width + 20, 10, "rafraichir");

    Texte titre = creer_texte(50, 70, "Charger une partie :");

    ListeParties parties = creer_listeparties(120);
    char *partie_selectionne = NULL;

    while (prochaine_page == P_CHARGER) {
        ecran.width = GetScreenWidth();
        ecran.height = GetScreenHeight();
        update_scrollbar(&scrollbar, ecran, parties.fin_liste);

        if (update_bouton_camera(&retour, scrollbar.vue) || IsKeyPressed(KEY_ESCAPE))
            prochaine_page = P_TITRE;

        if (WindowShouldClose())
            prochaine_page = P_QUITTER;

        if (update_bouton_camera(&rafraichir, scrollbar.vue)) {
            detruire_listeparties(parties);
            parties = creer_listeparties(120);
        }

        if ((partie_selectionne = update_listeparties(parties, scrollbar.vue)) != NULL)
            // TODO charger la partie
            prochaine_page = P_JEU;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(scrollbar.vue);
                dessiner_bouton(retour);
                dessiner_bouton(rafraichir);
                dessiner_texte(titre);
                dessiner_listeparties(parties);
            EndMode2D();

            dessiner_scrollbar(scrollbar, ecran);
        EndDrawing();
    }
    detruire_listeparties(parties);
    SetExitKey(KEY_ESCAPE);

    return prochaine_page;
}
