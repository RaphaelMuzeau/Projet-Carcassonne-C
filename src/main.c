#include <stdlib.h>
#include "test.h"
#include "raylib.h"
#include "page.h"

int main(void)
{
#ifdef RUN_UNIT_TESTS
    lancer_tests(); // le reste du code ne sera pas executé
#endif

    // Configuration global
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    // SetTraceLogLevel();
    int largeur_ecran = 640;
    int hauteur_ecran = 480;

    // Etat initial
    InitWindow(largeur_ecran, hauteur_ecran, "Carcassonne");
    SetTargetFPS(60);
    enum Page prochaine_page = P_TITRE;

    while (prochaine_page != P_QUITTER) {
        switch (prochaine_page) {
        case P_TITRE:
            prochaine_page = page_titre();
            break;
        case P_JEUX:
            // prochaine_page = page_jeux();
            prochaine_page = P_TITRE;
            break;
        case P_CUSTOM:
            // prochaine_page = page_options();
            prochaine_page = P_TITRE;
            break;
        case P_CHARGER:
            prochaine_page = page_charger();
            break;
        case P_REGLES:
            system("xdg-open rapport/analyse.pdf &"); // specifique à linux
            prochaine_page = P_TITRE;
            break;
        default:
            prochaine_page = P_QUITTER;
        }
    }
    CloseWindow();

    return 0;
}
