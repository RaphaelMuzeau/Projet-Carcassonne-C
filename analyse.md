# Rapport d'analyse

### Analyse des besoins

Développement d'un jeu "Carcassonne" interactif avec interface graphique pour GNU/Linux en utilisant le langage C ainsi qu'une librairie graphique.
La phase de développement est comprise entre le 22 janvier et le 9 avril avec une équipe composée de trois personne : 

[MUZEAU Raphaël](https://github.com/RaphaelMuzeau) contact: raphael.muzeau@etudiant.univ-perp.fr<br>
[DANNA--VASSEUR Lucas](https://github.com/DannVass-Lucas) contact: lucas.danna--vasse@etudiant.univ-perp.fr <br>
[CORTALE--BECKER Tom](https://github.com/CrtlTom) contact: tom.cortale-beck@etudiant.univ-perp.fr<br>

### Faisabilité :


---

## Spécifications

- Règles principales (***sans extensions***) du jeux de plateau Carcassonne.
- Un nombre fini de tuiles placées au cours d'une partie.
- Import d'un "jeu" de tuiles au format .csv fourni.
- Géneration aléatoire de tuiles possible.
- Sauvegarde et recharge d'une partie en cours.
- Une interface graphique.

### *Règle du jeu :*

Le Carcasonne est un jeu pouvant être joué de 2 à 5 joueurs. Celui-ci est composé de 72 tuiles (dans sa version d'origine) dont trois types dinstincts : <br>

1. Route
2. Ville
3. Abbaye.

Chaque joueur possède 7 meeple d'une couleur différentes (rouge, jaune, noir, vert, bleu..). Ces meeple servent au joueur à s'atrribuer une "zone".  <br> 
Il existe autant de type de zone que de type de tuile. Celles-ci seront étudiées une à une. Chaque zone sera étudié après le placement de la première tuile de ladite zone jusqu'à la validation de la zone, lorsque la dernière tuile sera posée pour la compléter.


 - ### La route :

	Une fois une tuile "route" posée, le joueur peut décider d'y poser un meeple pour s'attribuer la route. Un joueur peut s'attribuer une route si, et seulement si, aucun autre joueur ne s'y trouve.
	
	\[IMAGE EXMPLE 1]
	
	Cependant, il est possible que deux joueurs possèdent une même route, comme dans l'exemple ci-contre : 
	
	\[IMAGE EXEMPLE 2]
	
	Ce cas ne peut arriver que lorsque deux routes séparées à l'origine se joignent. Dans ce cas, les deux joueurs se sont bien **attribués** la route quand ils étaient seuls, ce qui respecte la règle d'origine. Cette règle vaut pour toutes les zones.
	
	Une *route* n'est considérée comme terminée seulement lorsque le point de départ d'une route (ville, abbaye ou décoration) rencontre un autre point de départ (ici, considéré comme la "fin" de la route).
	
	> Durant la partie en cours, les points sont comptés de la manière suivante : <br> Chaque tuile constituant la route rapport **1 point** au joueur ayant le plus de meeple sur la route.
	<br>

- ### La ville :

	Une fois une tuile "ville" posée, le joueur peut décider d'y poser un meeple pour s'attribuer la ville. <br>
	La règle reste la même que pour la route, un **meeple** d'un autre joueur ne peut être posée sur une zone appartenant déjà à un autre joueur.
	
	\[IMAGE EXMPLE 3]
	
	***Mais,*** deux mepple peuvent se trouver sur une même zone. <br>
	Une zone n'est considére comme terminée que lorsque l'intérieur de la ville est entourée de "muraille" (ville aux abord d'un pré) sans aucun "trou ". 
	
	> Ici, durant la partie en cours, chaque tuile vaut **2 points** mais, si celle-ci possède un ***blason***, alors vous gagnez **2 points** en plus sur la tuile constituant la ville.
	<br>
	
- ### L'abbaye :

	Les tuiles "abbaye" sont toujours représentées par une "abbaye" à leur centre. Ici, pour compléter une zone, il vous faut l'entourer de huit autres tuiles avec l'abbaye en son centre, comme sous l'image ci-dessous : 
	
	\[IMAGE EXEMPLE 4]
	
	> Durant la partie en cours, une zone rapporte un point par tuile, l'abbaye incluse. Une zone complète vous rapporte alors **9 points**.
	
	<br>
	Une fois n'importe quelle zone complétée, le joueur peut alors récupérer son meeple, sinon, celui-ci reste à sa position jusqu'à la fin de la partie.
	Les points sont attribués au joueur ayant le plus de meeple sur ladite zone.
	
### En fin de partie :

Lors d'une fin de partie, certaines zones ne sont pas complétées. La délibération des points sera ainsi faite : 

- Chaque route incomplète rapporte **1 point**
- Chaque ville incomplète rapporte **1 point** et **1 point** de plus si la tuile possède un blason.
- Chaque abbaye rapporte **1 point** et **1 point** par tuile adjacente.
 
 
---
## Conception Architecturale

### Modèle MVC :

- **Modèle** génère et contient la grille, représente l'ensemble des structures manipulées par l'utilisateur via le contrôleur, ainsi que l'automatisation des points.
- **Vue** affiche l'état du modèle en temps réel selon les entrées utilisateur.
- **Contrôleur** recoit les entrées de la vue et modifie le modèle en conséquence. (ex, positionnement d'une tuile certaines coordonées).

### Arborescence :

```
.
├── bin
├── data
├── include
├── lib
├── obj
└── src
│   └── model
│   └── view
│   └── controller
```

#### Dépendances :

Utilisation de la librairie [Raylib](https://github.com/raysan5/raylib) pour l'interface utilisateur.
