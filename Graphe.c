// Beeverly Gourdette & Bettina Mubiligi

#include "Graphe.h"
#include "ArbreQuat.h"
#include "Chaine.h"
#include "Struct_File.h"

#include "Hachage.h"
#include "Reseau.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*=================== Outils de création des Graphes =============*/

Sommet *creerSommet(int num, int x, int y) {

  Sommet *s = (Sommet *)malloc(sizeof(Sommet));
  if (s == NULL) {
    printf("Erreur lors de l'allocation mémoire du Sommet\n");
    return NULL;
  }
  s->L_voisin = NULL;
  s->num = num;
  s->x = x;
  s->y = y;
  return s;
}

/*--------------------------------------------------------------*/

Arete *creerArete(int u, int v) {

  Arete *a = (Arete *)malloc(sizeof(Arete));
  if (a == NULL) {
    printf("Erreur lors de l'allocation mémoire de l'Arete\n");
    return NULL;
  }
  a->u = u;
  a->v = v;
  return a;
}

/*--------------------------------------------------------------*/

Cellule_arete *creerCelluleArete(Arete *a) {

  Cellule_arete *ca = (Cellule_arete *)malloc(sizeof(Cellule_arete));
  if (ca == NULL) {
    printf("Erreur lors de l'allocation mémoire de la CelluleArete\n");
    return NULL;
  }
  ca->a = a;
  ca->suiv = NULL;
  return ca;
}

/*--------------------------------------------------------------*/

Graphe *creerGraphe(Reseau *r) {
  Graphe *g = (Graphe *) malloc(sizeof(Graphe));
  if (g == NULL) {
    printf("Erreur lors de l'allocation mémoire du graphe\n");
    return NULL;
  }

  g->nbsom = r->nbNoeuds;
  g->gamma = r->gamma;
  g->nbcommod = nbCommodites(r);

  // Allocation mémoire pour les tableaux des commodités et des sommets
  g->T_commod = (Commod *)malloc(g->nbcommod*sizeof(Commod));
  if (g->T_commod == NULL) {
    printf("Erreur lors de l'allocation mémoire de T_commod du graphe\n");
    free(g);
    return NULL;
  }

  g->T_som = (Sommet **)malloc(g->nbsom*sizeof(Sommet *));
  if (g->T_som == NULL) {
    printf("Erreur lors de l'allocation mémoire de T_som du graphe\n");
    free(g->T_commod);
    free(g);
    return NULL;
  }

  // Initialisation des pointeurs de T_som à NULL pour éviter les comportements indéterminés
  for (int i = 0; i < g->nbsom; i++) {
    g->T_som[i] = NULL;
  }

  CellCommodite *co = r->commodites;  
  // Remplissage du tableau des commodités du graphe
  for (int i = 0; i < g->nbcommod; i++) {
    g->T_commod[i].e1 = co->extrA->num - 1;
    g->T_commod[i].e2 = co->extrB->num - 1;
    co = co->suiv;
  }

  // Remplissage du tableau de pointeurs sur sommet
  CellNoeud *n = r->noeuds;
  while (n != NULL) {
    Sommet *s = creerSommet(n->nd->num - 1, n->nd->x, n->nd->y);
    // Création de la liste de voisins
    CellNoeud *v = n->nd->voisins;
    while (v != NULL) {
      Arete *a = creerArete(n->nd->num - 1, v->nd->num - 1);
      Cellule_arete *ca = creerCelluleArete(a);

      ca->suiv = s->L_voisin;
      s->L_voisin = ca;
      v = v->suiv;
    }
    g->T_som[n->nd->num - 1] = s;
    n = n->suiv;
  }
  return g;
}

/*=================== Outils de manipulation des Graphes =============*/

// Exercice 7.2
int nbAretes(Graphe *g, int u, int v) {
  S_file *file = (S_file *)malloc(sizeof(S_file));
  Init_file(file);
  /*tableau d'entiers pour savoir quels sommets sont visités (0 si non ou 1)*/
  int visité[g->nbsom];
  /*On initialise tous les sommets comme non visités avec une distance nulle
  Les sommets vont de 1 à nbsom donc on n'initialise pas la case 0*/
  for (int i = 1; i < g->nbsom + 1; i++) {
    visité[i] = 0;
  }
  visité[u] = 1;
  enfile(file, u); // debut du parcours

  while (!estFileVide(file)) {
    int s = defile(file);
    if (s == v) {
      /* printf(" Plus petit nombre d'arretes entre %d et %d : %d\n", u, v,
             visité[s] - 1);*/
      return visité[s] - 1;
    } else { // sinon il faut parcourir les voisins
      Cellule_arete *n = g->T_som[s]->L_voisin;
      while (n != NULL) {
        if (visité[n->a->v] == 0) {
          visité[n->a->v] = visité[n->a->u] + 1;
          enfile(file, n->a->v);
        }
        n = n->suiv;
      }
    }
  }

  return -1; // pas de chemin trouvé
}

/*--------------------------------------------------------------*/

/* Exercice 7.3 : on pourrait stocker les chemins issus de u dans une liste
 * chainée ou dans un arbre avec plusieurs fils*/

S_file *Liste_chaine(Graphe *g, int u, int v) {
  S_file *file = (S_file *)malloc(sizeof(S_file));
  Init_file(file);
  int *visité = (int *)malloc((g->nbsom) * sizeof(int));
  // tableau pour stocker les prédecesseurs de chaque sommet dans la chaine
  int *predecesseurs = (int *)malloc((g->nbsom) * sizeof(int));

  // on initialise les tableau à 0
  for (int i = 0; i < g->nbsom; i++) {
    visité[i] = 0;
    predecesseurs[i] = 0;
  }

  visité[u] = 1;
  enfile(file, u);
  predecesseurs[u] = 0;

  while (!estFileVide(file)) {
    int s = defile(file);
    if (s == v) {
      // on a trouvé le sommet, on reconstitue la chaine
      S_file *chaine = (S_file *)malloc(sizeof(S_file));
      Init_file(chaine);
      int x = v;
      // on ajoute les prédecesseurs dans la chaine uns par uns
      while (x != 0) {
        enfile(chaine, x);
        // affichage de la chaine obtenue dans chaine (ordre inverse)
        printf(" %d ", x);
        //   x prend maintenant la valeur de son predecesseur
        x = predecesseurs[x];
      }
      return chaine;
    }

    Cellule_arete *n = g->T_som[s]->L_voisin;
    while (n != NULL) {
      if (visité[n->a->v] == 0) {
        visité[n->a->v] = 1;
        enfile(file, n->a->v);
        // s sera le prédecesseuur de n->a->v dans la chaine
        predecesseurs[n->a->v] = s;
      }
      n = n->suiv;
    }
  }
  return NULL; // Pas de chaine trouvée
}

/*--------------------------------------------------------------*/
/*Ne fontionne pas correctement */
int reorganiseReseau(Reseau *r) {
  Graphe *g = creerGraphe(r);
  int **matrice = (int **)malloc((g->nbsom) * sizeof(int *));
  for (int i = 0; i < g->nbsom; i++) {
    matrice[i] = (int *)malloc((g->nbsom) * sizeof(int));
    for (int j = 0; j < g->nbsom; j++) {
      matrice[i][j] = 0;
    }
  }
  /*Tableau qui stocke la taille de la chaine la plus courte pour toutes les
   * commodité*/
  int *T_taille = (int *)malloc(g->nbcommod * sizeof(int));
  /*Tableau qui stocke la liste des sommets (ou arêtes si on prend les sommets 2
   * à 2) de la chaine la plus courte pour chaque commodité*/
  S_file **T_chaine = (S_file **)malloc(g->nbcommod * sizeof(S_file *));

  /*On remplit les deux tableaux*/
  for (int i = 0; i < g->nbcommod; i++) {
    T_taille[i] = nbAretes(g, g->T_commod[i].e1, g->T_commod[i].e2);
    T_chaine[i] = Liste_chaine(g, g->T_commod[i].e1, g->T_commod[i].e2);
    printf("\n");
  }

  /*Ensuite, pour chaque arêtes (contenues dans la matrice), pour chaque chaine
  la plus courte obtenue par commodité, on compte le nombre de fois où
   * l'arête est présente avec la fonction rechercheArete. Si c'est supérieur
  à gamma, on renvoie 0, 1 sinon*/

  for (int i = 0; i < g->nbsom; i++) {
    for (int j = 0; j < g->nbsom; j++) {
      for (int k = 0; k < g->nbcommod; k++) {
        S_file *t = T_chaine[k];
        rechercheArete(matrice, t, i, j);
        if (matrice[i][j] >= g->gamma) {
          printf("gamma : %d, arête(i,j): %d, %d \n", g->gamma, i, j);
          return 0; /*Si le nombre de chaînes qui passent par cette arête est
                       supérieur ou égal à gamma*/
        }
      }
    }
  }
    return 1;
}


/*--------------------------------------------------------------*/

void rechercheArete(int **matrice, S_file *chaine, int i, int j) {
  Cellule_file *t = chaine->tete;
  while (t != NULL && t->suiv != NULL) { // Vérifier t->suiv != NULL
    if (t->val == i && t->suiv->val == j) {
      matrice[i][j]++;
      matrice[j][i]++;
    }
    t = t->suiv;
  }
}

/*--------------------------------------------------------------*/

void afficherGraphe(Graphe *g) {
  printf("\nAffichage du Graphe \n");
  printf("Nombre de sommets : %d\n", g->nbsom);
  printf("Nombre de commodités : %d\n", g->nbcommod);

  printf("Sommets :\n");
  for (int i = 0; i < g->nbsom; i++) {
    printf("Sommet %d : (%.2f, %.2f)\n", g->T_som[i]->num, g->T_som[i]->x,
           g->T_som[i]->y);
    printf("\tVoisins :");
    Cellule_arete *ca = g->T_som[i]->L_voisin;
    while (ca != NULL) {
      printf("%d ", ca->a->v);
      ca = ca->suiv;
    }
    printf("\n");
  }
  printf(" Commodités :\n");
  for (int i = 0; i < g->nbcommod; i++) {
    printf("Commodité %d : %d - %d\n", i + 1, g->T_commod[i].e1,
           g->T_commod[i].e2);
  }
}

/*=================== Outils de libération des graphes =============*/

void libererSommet(Sommet *s) {
  if (s == NULL)
    return;
  s->L_voisin = NULL;
  free(s);
}
/*--------------------------------------------------------------*/

void libererCelluleArete(Cellule_arete *ca) {
  if (ca == NULL)
    return;

  ca->a = NULL;
  ca->suiv = NULL;
  free(ca);
}

/*--------------------------------------------------------------*/
void libererGraphe(Graphe *g) {
  if (g == NULL)return;

  for (int i = 0; i < g->nbsom; i++) {
    Cellule_arete *ca = g->T_som[i]->L_voisin;
    while (ca != NULL) {
      Cellule_arete *del= ca->suiv;
      free(ca->a);  
      free(ca);     
      ca = del;
      }
      free(g->T_som[i]);  
  }
  free(g->T_som);       
  free(g->T_commod);    
  free(g);              
}
