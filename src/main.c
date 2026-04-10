#include <stdlib.h>
#include <time.h>
#include "test.h"
#include "libca.h"
#include "raylib.h"
#include "jeu.h"
#include "page.h"

int main(void)
{
#ifdef RUN_UNIT_TESTS
    lancer_tests(); // le reste du code ne sera pas executé
#endif

    // Configuration global
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    srand(time(NULL));
    // SetTraceLogLevel();
    int largeur_ecran = 640;
    int hauteur_ecran = 480;
    Jeu jeu = { 0 };

    // Etat initial
    InitWindow(largeur_ecran, hauteur_ecran, "Carcassonne");
    SetTargetFPS(60);
    enum Page prochaine_page = P_TITRE;

    while (prochaine_page != P_QUITTER) {
        switch (prochaine_page) {
        case P_TITRE:
            prochaine_page = page_titre();
            break;
        case P_JEU:
            prochaine_page = page_jeu(&jeu);
            break;
        case P_CONF_NORMAL:
            prochaine_page = page_configuration(&jeu, C_NORMAL);
            break;
        case P_CONF_CUSTOM:
            prochaine_page = page_configuration(&jeu, C_CUSTOM);
            break;
        case P_CONF_CSV:
            prochaine_page = page_configuration(&jeu, C_CSV);
            break;
        case P_CHARGER:
            prochaine_page = page_charger(&jeu);
            break;
        case P_AIDE:
            if (system("xdg-open rapport/aide.pdf &") == 127) // specifique à linux
                ca_warn("l'ouverture des règles nécessite xdg-open");
            prochaine_page = P_TITRE;
            break;
        default:
            prochaine_page = P_QUITTER;
        }
    }
    CloseWindow();

    return 0;
}
