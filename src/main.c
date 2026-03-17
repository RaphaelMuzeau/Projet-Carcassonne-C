#include "test.h"
#include "fichier.h"
#include "tuile.h"
#include "vec.h"

int main(void)
{

    Vec2D grille = creer_vec2D();
    FILE *f = fopen("plateau3.bin", "r");

    /* sauvegarder_grille test */
    // Tuile tuile1 = creer_tuile();
    // Tuile tuile2= creer_tuile();
    // tuile2->sud = Z_VILLE;
    //
    // Tuile tuile3 = creer_tuile();
    // Tuile tuile4 = creer_tuile();
    // Tuile tuile5= creer_tuile();
    //
    // tuile5->milieu = Z_VILLE;
    // set(&grille, tuile1, 0, 0);
    // set(&grille, tuile2, 0, 1);
    // set(&grille, tuile3, 1, 1);
    // set(&grille, tuile4, 1, 0);
    // set(&grille, tuile5, 2, 0);
    //
    // Tuile t = get(grille, 0, 0);
    // printf("%c %x", 51, t->sud);
    //
    // sauvegarder_grille(&grille, f);

    /* CHARGER GRILLE test */
    charger_grille(&grille, f);
    Tuile tuile1 = get(grille, 2, 0);
    printf("GET : %d %d %d %d %d\n", tuile1->sud, tuile1->nord, tuile1->est, tuile1->ouest, tuile1->milieu);


#ifdef RUN_UNIT_TEST
    lancer_tests(); // le reste du code ne sera pas executé
#endif
    return 0;
}
