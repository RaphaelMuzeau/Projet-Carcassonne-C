#include <string.h>
#include "libca.h"
#include "fichier.h"
#include "raylib.h"
#include "page.h"
#include "sidebar.h"
#include "popup.h"

enum Page page_jeu(Jeu *jeu)
{
    // Etat initial
    enum Page prochaine_page = P_JEU;
    bool afficher_popup = false;

    // Elements de la page
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Controles ctrl = creer_controles();
    BarreJoueurs barrejoueurs = creer_barrejoueurs(jeu->joueurs);
    Popup popup = creer_popup();

    while (prochaine_page == P_JEU) {
        if (update_bouton(&retour) || WindowShouldClose())
            afficher_popup = true;

        if (afficher_popup) {
            centrer_popup(&popup);
            update_champsaisie(&popup.champ_partie);

            if (update_bouton_centre(&popup.annuler))
                afficher_popup = false;

            if (update_bouton_adapte(&popup.quitter))
                prochaine_page = P_TITRE;

            if (update_bouton_adapte(&popup.sauvegarder) && popup.champ_partie.saisie.len != 0) {
                char *fname = ca_alloc(sizeof(CHEMIN_PARTIES) + popup.champ_partie.saisie.len, sizeof(char));
                memcpy(fname, CHEMIN_PARTIES, sizeof(CHEMIN_PARTIES));
                strcat(fname, popup.champ_partie.saisie.texte);

                if (sauvegarder_partie(*jeu, fname))
                    prochaine_page = P_TITRE;

                free(fname);
            }
        }

        update_controles(&ctrl);
        update_barrejoueurs(&barrejoueurs);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            dessiner_bouton(retour);
            if (afficher_popup) dessiner_popup(popup);

            dessiner_barrejoueurs(barrejoueurs, 3);
            dessiner_controles(ctrl);
        EndDrawing();
    }

    detruire_barrejoueurs(barrejoueurs);
    detruire_jeu(*jeu);
    *jeu = (Jeu) { 0 };
    return prochaine_page;
}
