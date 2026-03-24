#ifndef page_h
#define page_h

#include <stdbool.h>

/* Denomme les differentes pages constituant l'interface graphique.
 * Chaque page a une fonction associé qui modifie l'etat du jeux et renvoit
 * la prochaine page à afficher ou P_QUITTER pour arreter le jeux.
 */
enum Page {
    P_TITRE,
    P_JEUX,
    P_NORMAL,
    P_CUSTOM,
    P_JOUEURS,
    P_CHARGER,
    P_REGLES,
    P_QUITTER,
};

enum Page page_titre(void);
// enum Page page_jeux(void);
enum Page page_configuration(bool custom);
enum Page page_joueurs(int nb_joueurs);
enum Page page_charger(void);

//TODO utiliser P_QUITTER pour casser les boucles

#endif
