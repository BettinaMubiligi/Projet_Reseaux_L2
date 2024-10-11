**Rapport du Projet**
**Beeverly Gourdette & Bettina Mubiligi**

**Une reformulation courte du sujet.**
**Description des structures manipulées et la description globale du code**
fichiers .h, .c, fonctions principales etc.

**Description des jeux d’essais utilisés (ChaineMain.c, ReconstitueReseau.c et ReconstitueReseauComp.c)**
**L’analyse comment´ee (et statistique) des performances de vos programmes**



**Réponses aux Questions** : 

**Q4.2** - Pour l'implémentation d'une table de Hachage utilisant comme clé l'utilisation de la fonction keyF sur les coordonnées x et y d'un point P est approprié. En effet, si nous prenons par exemple des points x et y entre 1 et 10 aléatoirement, nous remarquons que les clés sont toutes différentes. En conséquence les risques de collisions sont faibles ce qui signifie un gain de performance lors de la recherche d'un point (Moins de recherches dans les sous-tableaux de T). Les clés obtenus à partir des points provenant de la base de données burma_cha ont un comportement similaire. Ainsi nous prenons quelques points avec les coordonnées arrondis à l'entier inferieur, les clés sont toutes différentes.

**Q6.1** - On observe que les temps de reconstitution du réseau par arbre et par listes sont souvent proches, même si la méthode par arbre semble prendre un peu moins de temps en moyenne. Par hachage, on remarque que les temps sont similaires aux deux autres méthodes, mais qu'en fonction de la taille de la table de hachage, le temps varie : plus la table est grande, moins la reconstitution du réseau prends de temps. Cela pourrait s'expliquer par un nombre de collisions faible avec les tables plus grandes.

**Q6.4** - En étudiant les graphiques, on observe que plus le nombre de points total des chaînes augmente, plus le temps pour reconstituer le 
réseau augmente en conséquence. Il semblerait également que nos observtions passées soit confirmées : la reconstitution d'un réseau avec un arbre soit en moyenne plus rapide que celle avec une table de hachage, qui est influencée par la taille de la table. En effet, en observant le 2e graphique, on voit que plus la table de hachage est grande, moins le temps cpu pour la reconstitution d'un réseau sera élevé. Il faudrait que la taille de la table de Hachage soit la plus grande possible pour avoir un temps de reconstitution optimal.

Plus d'information sur le PDF