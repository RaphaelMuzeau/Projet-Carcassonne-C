#include <string.h>
#include "libca.h"
#include "fichier.h"
#include "raylib.h"
#include "page.h"
#include "sidebar.h"
#include "champsaisie.h"

#define CHEMIN_PARTIES "data/parties/"

enum Page page_jeu(Jeu *jeu)
{
    // Etat initial
    enum Page prochaine_page = P_JEU;
    int largeur_ecran = GetScreenWidth();
    int hauteur_ecran = GetScreenHeight();

    bool afficher_popup = false;

    // Elements de la page
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");

    Controles ctrl = creer_controles();
    BarreJoueurs barrejoueurs = creer_barrejoueurs(jeu->joueurs);

    Rectangle popup = { .width = 700, .height = 200 };
    ChampSaisie champ_partie = creer_champsaisie(0, 0, popup.width * 0.8f, 50, false);
    Bouton annuler = creer_bouton_adapte(0, 0, "annuler");
    Bouton sauvegarder = creer_bouton_adapte(0, 0, "sauvegarder\net quitter");
    Bouton quitter = creer_bouton_adapte(0, 0, "quitter sans\nsauvegarder");
    annuler.texte.taille = 20.0f;
    quitter.texte.taille = 15.0f;
    sauvegarder.texte.taille = 15.0f;

    while (prochaine_page == P_JEU) {
        largeur_ecran = GetScreenWidth();
        hauteur_ecran = GetScreenHeight();

        if (update_bouton(&retour) || WindowShouldClose())
            afficher_popup = true;

        // centrer le popup
        if (afficher_popup) {
            popup.x = largeur_ecran/2.0f - popup.width/2.0f;
            popup.y = hauteur_ecran/2.0f - popup.height/2.0f;

            champ_partie.champ.x = popup.x + popup.width * 0.1f;
            champ_partie.champ.y = popup.y + popup.height * 0.25f;

            quitter.champ.x = popup.x + popup.width * 0.1f;
            quitter.champ.y = champ_partie.champ.y + champ_partie.champ.height + 20.0f;

            annuler.champ.x = popup.x + popup.width/2.0f - annuler.champ.width/2.0f;
            annuler.champ.y = champ_partie.champ.y + champ_partie.champ.height + 20.0f;
            annuler.champ.height = quitter.champ.height;

            sauvegarder.champ.x = popup.x + popup.width * 0.9f - sauvegarder.champ.width;
            sauvegarder.champ.y = champ_partie.champ.y + champ_partie.champ.height + 20.0f;

            // gerer l'entrée utilisateur
            update_champsaisie(&champ_partie);

            if (update_bouton_centre(&annuler))
                afficher_popup = false;

            if (update_bouton_adapte(&quitter))
                prochaine_page = P_TITRE;

            if (update_bouton_adapte(&sauvegarder) && champ_partie.saisie.len != 0) {
                char *fname = ca_alloc(sizeof(CHEMIN_PARTIES) + champ_partie.saisie.len, sizeof(char));
                memcpy(fname, CHEMIN_PARTIES, sizeof(CHEMIN_PARTIES));
                strcat(fname, champ_partie.saisie.texte);

                if (sauvegarder_partie(*jeu, fname))
                    prochaine_page = P_TITRE;

                free(fname);
            }
        }

        update_controles(&ctrl);
        update_barrejoueurs(&barrejoueurs);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            dessiner_barrejoueurs(barrejoueurs, 3);
            dessiner_controles(ctrl);

            dessiner_bouton(retour);

            if (afficher_popup) {
                DrawRectangleRec(popup, DARKGRAY);
                dessiner_champsaisie(champ_partie);
                dessiner_bouton(annuler);
                dessiner_bouton(sauvegarder);
                dessiner_bouton(quitter);
            }
        EndDrawing();
    }

    detruire_barrejoueurs(barrejoueurs);
    detruire_jeu(*jeu);
    *jeu = (Jeu) { 0 };
    return prochaine_page;
}
