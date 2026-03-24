#include "raylib.h"
#include "page.h"
#include "bouton.h"
#include "champsaisie.h"

enum Page page_configuration(bool custom)
{
    // Etat initial
    enum Page prochaine_page = P_TITRE;
    bool doit_quitter = false;
    int largeur_ecran = GetScreenWidth();

    SetExitKey(KEY_NULL); // echape retourne à l'ecran titre au lieu de fermer la fenetre

    // Elements de la page
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Bouton confirmer = creer_bouton_adapte(0, 10, "confirmer ->");

    Texte titre_nb_joueur  = creer_texte(0, 100, "Nombre de joueurs");
    float largeur_titre_nb_joueur = mesurer_texte(titre_nb_joueur).x;
    ChampSaisie nb_joueur = creer_champsaisie(0, 130, 150, 40, true);

    Texte titre_nb_meeple  = creer_texte(0, 230, "Nombre de meeple");
    float largeur_titre_nb_meeple = mesurer_texte(titre_nb_meeple).x;
    ChampSaisie nb_meeple = creer_champsaisie(0, 260, 150, 40, true);

    Texte titre_nb_tuile  = creer_texte(0, 360, "Nombre de tuiles");
    float largeur_titre_nb_tuile = mesurer_texte(titre_nb_tuile).x;
    ChampSaisie nb_tuile = creer_champsaisie(0, 390, 150, 40, true);

    while (!doit_quitter) {
        largeur_ecran = GetScreenWidth();

        if (WindowShouldClose()) {
                prochaine_page = P_QUITTER;
                doit_quitter = true;
        }

        if (update_bouton(&retour) || IsKeyPressed(KEY_ESCAPE)) {
                prochaine_page = P_TITRE;
                doit_quitter = true;
        }

        if (update_bouton(&confirmer)) {
            // TODO creer une partie avec config
            prochaine_page = P_JEUX;
            doit_quitter = true;
        }

        /* realigner le bouton de confirmation à droite */
        confirmer.champ.x = largeur_ecran - confirmer.champ.width - 10;
        adapter_bouton(&confirmer); // recentre le texte après le decalage

        /* centrer les champs et leurs titres */
        nb_joueur.champ.x = (float) largeur_ecran/2 - nb_joueur.champ.width/2;
        titre_nb_joueur.position.x = (float) largeur_ecran/2 - largeur_titre_nb_joueur/2;
        update_champsaisie(&nb_joueur);

        if (custom) {
            nb_meeple.champ.x = (float) largeur_ecran/2 - nb_meeple.champ.width/2;
            titre_nb_meeple.position.x = (float) largeur_ecran/2 - largeur_titre_nb_meeple/2;

            nb_tuile.champ.x = (float) largeur_ecran/2 - nb_tuile.champ.width/2;
            titre_nb_tuile.position.x = (float) largeur_ecran/2 - largeur_titre_nb_tuile/2;

            update_champsaisie(&nb_meeple);
            update_champsaisie(&nb_tuile);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            dessiner_bouton(retour);
            dessiner_bouton(confirmer);

            dessiner_texte(titre_nb_joueur);
            dessiner_champsaisie(nb_joueur);
            if (custom) {
                dessiner_texte(titre_nb_meeple);
                dessiner_champsaisie(nb_meeple);
                dessiner_texte(titre_nb_tuile);
                dessiner_champsaisie(nb_tuile);
            }
        EndDrawing();
    }
    detruire_champsaisie(nb_joueur);
    detruire_champsaisie(nb_meeple);
    detruire_champsaisie(nb_tuile);
    SetExitKey(KEY_ESCAPE);

    return prochaine_page;
}
