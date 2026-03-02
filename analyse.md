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

Le Carcasonne est un jeu pouvant être joué de 2 à 5 joueurs. Celui-ci est composé de 72 tuiles (dans sa version d'origine) constituées de differentes zones :

- Route
- Ville
- Abbaye

Chaque joueur possède 7 meeple d'une couleur différentes (rouge, jaune, noir, vert, bleu..). Ces meeple servent au joueur à s'attribuer une zone.  <br>
Lorsqu'une la zone est complété, ce joueur recevra des points en conséquence et pourra récuperer son meeple.


#### Déroulement :
À chaque tour,
1. Un joueur pioche une tuile sur la pile et la pose de manière à prolonger le 		paysage formé par les tuiles deja presentes.
	> Si la tuile ne peut pas etre placée, le joueur peut passer son tour, cette tuile ne sera pas réutilisée.
2. Ce joueur peut alors decider ou non de poser un __unique__ meeple sur une des zones de cette tuile pour se l'attribuer.
3. Les points gagnés sont evalués.

Le jeu se termine lorsque la pile de tuiles est vide.

Etudions chaque zone une à une, du placement de la première tuile de ladite zone à la validation de la zone, lorsque la dernière tuile sera posée pour la compléter.

 - ### La route :

	Une fois une tuile avec une zone "route" posée, le joueur peut décider d'y poser un meeple pour s'attribuer la route. Un joueur peut poser un meeple sur une route si, et seulement si, aucun autre meeple ne s'y trouve.
	
	![](data/analyse/route1.png)
	
	Cependant, il est possible que deux joueurs possèdent une même route, comme dans l'exemple ci-contre : 
	
	![](data/analyse/route2.png)
	
	Ce cas ne peut arriver que lorsque deux routes séparées à l'origine se joignent. Dans ce cas, les deux joueurs se sont bien **attribués** la route quand ils étaient seuls, ce qui respecte la règle d'origine. Cette règle vaut pour toutes les *zones*.
		
	Une *route* est considérée comme terminée lorsque le point de départ d'une route (ville, abbaye ou décoration) rencontre un autre point de départ (ici, considéré comme la "fin" de la route). Autrement dit, qu'aucun chemin de la route ne mène à une tuile vide.
	
	Une fois une route terminée, le joueur ayant placé le plus de meeple dessus gagne **1 point** par tuile traversé par la route. Si plusieurs joueurs ont posé le meme nombre de meeples, il gagnent tous l'integralité des points. Si aucun meeple n'a été placé sur la route, aucun point n'est accordé.
	
	![](data/analyse/route3.png)
	
	Tous les meeples reviennent alors à leur joueur.
	
- ### La ville :

	Une fois une tuile avec une zone "ville" posée, le joueur peut décider d'y poser un meeple pour s'attribuer la ville. La règle reste la même que pour la route, aucun autre meeple ne doit y etre deja present.
	
	Une *ville* n'est considére comme terminée que lorsque l'intérieur de la ville est entourée de "muraille" (ville aux abord d'un pré) sans aucun "trou". 
	
	![](data/analyse/ville1.png)
	![](data/analyse/ville2.png)
	
	Ici, durant la partie en cours, chaque tuile vaut **2 points** mais, si celle-ci possède un ***blason***, alors vous gagnez **2 points** en plus sur la tuile constituant la ville.
	
	![](data/analyse/ville3.png)

- ### L'abbaye :

	Les tuiles *abbaye* sont toujours représentées par une abbaye à leur centre. Ici, pour compléter une zone, il vous faut l'entourer de huit autres tuiles avec l'abbaye en son centre, comme sous l'image ci-dessous : 
	
	![](data/analyse/abbaye1.png)
	
	Durant la partie en cours, une zone rapporte un point par tuile, l'abbaye incluse. Une zone complète vous rapporte alors **9 points**.
	
Une fois n'importe quelle zone complétée, le joueur peut alors récupérer son meeple, sinon, celui-ci reste à sa position jusqu'à la fin de la partie.
	
### En fin de partie :

Lors d'une fin de partie, certaines zones ne sont pas complétées. La délibération des points sera ainsi faite : 

- Chaque tuile traversé par une route incomplète rapporte **1 point**
- Chaque tuile traversé par une ville incomplète rapporte **1 point** et **1 point** de plus si la tuile possède un blason.
- Chaque abbaye rapporte **1 point** et **1 point** par tuile adjacente.
 
Le joueur avec le plus de points remporte la partie.

### *Interface Utilisateur :*

Toutes les interactions avec le jeu se feront par le biais d'une interface graphique, en fenetre flottante ou en plein écran.

Le joueur sera d'abord acceuilli par un écran titre simple:

![](data/analyse/ui1.png)

Une partie standard lance le jeu avec les regles standards de carcassonnes, 72 tuiles, 7 meeples par joueur.

L'interface principale du jeu se compose d'un unique écran décrit ci-dessous:

![](data/analyse/ui2.png)

Le joueur peut librement deplacer la camera sur le terrain de tuiles.

Si le joueur decide de quitter la partie, un pop-up lui propose de sauvegarder sa progression dans un fichier.

![](data/analyse/ui3.png)

Au moment de lancer une partie custom, un écran de configuration permet de gerer les parametres de la partie. Pour une partie standard, cet écran n'indique que le nombre de joueurs.

![](data/analyse/ui4.png)

Pour charger une partie, le joueur ne peut que selectionner parmis les fichiers disponibles.

![](data/analyse/ui5.png)

### *Format csv*

Les tuiles d'une partie peuvent êtres importées à l'aide d'un fichier un csv:
```csv
# cote 1, cote 2, cote 3, cote 4, centre
route,ville,route,pre,route
route,ville,pre,route,route
blason,blason,blason,route,blason
route,ville,pre,route,route
route,ville,ville,route,route
```
chaque entrées specifie les zones présentes à chaque coté de la tuile et en son centre, écrite en toute lettre *(route, ville, blason, pre, village, abbaye)*. Une abbaye ou un village ne peuvent apparaitre qu'au centre d'une tuile.

Ces fichiers csv seront listé avec les parties chargeable comme une partie standard dont la pile de tuile sera prédefini.

Ces fichiers ne précisent pas l'ordre de *tirage* des tuiles, qui reste aléatoire, la pile est "mélangée" même si elle est chargée à partir d'une fichier.

---

## Conception Architecturale

### Modèle MVC :

- **Modèle**  -- génère et contient le terrain du jeu, la pile de tuile, les meeples placés, les joueurs et leurs données.
- **Vue** -- affiche l'état du modèle en temps réel et reçoit les entrées utilisateur.
- **Contrôleur** -- recoit les entrées de la vue et modifie le modèle en conséquence. (ex, positionnement d'une tuile à certaines coordonées, calcul des points, placement et retrait des meeples..).

### Arborescence :

```
.
├── bin
├── data
│   ├── parties
│   └── assets  // données requisent par l'interface graphique
├── include
├── lib
├── obj
└── src
    ├── model
    ├── view
    └── controller
```

Seuls les fichiers présents dans `data/parties` seront présentés au joueur.

#### Dépendances :

Utilisation de la librairie [Raylib](https://github.com/raysan5/raylib) pour l'interface utilisateur.
