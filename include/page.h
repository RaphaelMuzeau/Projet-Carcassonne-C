#ifndef page_h
#define page_h

#include <stdbool.h>
#include "jeu.h"

/* Denomme les differentes pages constituant l'interface graphique.
 * Chaque page a une fonction associé qui modifie l'etat du jeux et renvoit
 * la prochaine page à afficher ou P_QUITTER pour arreter le jeux.
 */
enum Page {
    P_TITRE,
    P_JEU,
    P_CONF_NORMAL,
    P_CONF_CUSTOM,
    P_CONF_CSV,
    P_JOUEURS,
    P_CHARGER,
    P_AIDE,
    P_QUITTER,
};

enum Configuration {
    C_NORMAL,
    C_CUSTOM,
    C_CSV,
};

enum Page page_titre(void);
enum Page page_jeu(Jeu *jeu);
enum Page page_configuration(Jeu *jeu, enum Configuration config);
enum Page page_joueurs(ListeJoueurs joueurs);
enum Page page_charger(Jeu *jeu);

#endif
