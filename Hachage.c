// Beeverly Gourdette & Bettina Mubiligi

#include "Hachage.h"
#include "Reseau.h"
#include "stdio.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/*================ Outils pour créer la TableHachage ==================*/

/*Fonction de création de la clé*/
int key(double x, double y) {
  return floor(y + (x + y) * (x + y + 1) * 1.0 / 2.0);
}

/*--------------------------------------------------------------*/

/*Fonction de hachage*/
int hachage(int k, int M) {
  double A = (sqrt(5) - 1) / 2;
  return floor(M * (k * A - floor(k * A)));
}

/*=====================================================================*/

/*====== Outils de mise en relation Chaines et TableHachage  =========*/


TableHachage *creerTableHachage(int M) {
  TableHachage *H = (TableHachage *)malloc(sizeof(TableHachage));
  if (H == NULL) {
    printf("Erreur lors de l'allocation  mémoire de la TableHachage\n");
    return NULL;
  }
  H->tailleMax = M;
  H->T = (CellNoeud **)malloc(sizeof(CellNoeud *) * M);
  if (H->T == NULL) {
    printf("Erreur lors de l'allocation mémoire du tableau T de la TableHachage\n");
    return NULL;
  }
  for (int i = 0; i < M; i++) {
    H->T[i] = NULL;
  }
  return H;
}

/*--------------------------------------------------------------*/


/*Recherche un Noeud de coordonnées x et y dans R. Création et insertion du Noeud si non trouvé*/
Noeud *rechercheCreeNoeudHachage(Reseau *R, TableHachage *H, double x, double y) {

  if (R == NULL) {
    printf("Reseau non alloué\n");
    return NULL;
  }
  if (H == NULL) {
    printf("Table de Hachage non allouée\n");
    return NULL;
  }

  int k = hachage(key(x, y), H->tailleMax);
  CellNoeud *c = H->T[k];

  /*Parcours du CellNoeud jusqu'a trouver les coordonnées correspondantes.*/
  while (c) {
    if (c->nd->x == x && c->nd->y == y) {
      return c->nd;
    }
    c = c->suiv;
  }

  /*Création du Noeud dans le Réseau R s'il n'existe pas et crée un CellNoeud pour la table de Hachage*/
  Noeud *N = creeNoeud(R->nbNoeuds+1,x,y);
  CellNoeud *cellH = creerCellNoeud(N);
  if (N == NULL || cellH == NULL) {
    libererNoeud(N);
    printf("Erreur : rechercheCreeNoeudHachage : \n");
    return NULL;
  }
  c = creerCellNoeud(N);
  
    if (c == NULL) {
    libererNoeud(N);
    libererCellNoeud(cellH);
    printf("Erreur : rechercherCreeNoeudHachage\n");
    return NULL;
  }

  c->suiv=R->noeuds;
  R->noeuds=c;
  (R->nbNoeuds)++;

  /*Ajout du CellNoeud dans la table de Hachage avec la clé k*/
  cellH->suiv = H->T[k];
  H->T[k] = cellH;

  return N;
}

/*--------------------------------------------------------------*/


/*Fonction de création d'un Reseau R et d'une TableHachage H à partir d'une liste de chaines C*/
Reseau *reconstitueReseauHachage(Chaines *C, int M) {
  if (C == NULL) {
    printf("Liste de Chaines non alloué\n");
    return NULL;
  }

  /*Création du Réseau R et de la table de hachage H avec chaque champs
   * initialisé à vide*/
  Reseau *R = creerReseau(C->gamma);
  if (R == NULL) {
    printf("Erreur : ReconstitueReseauHachage\n");
    return NULL;
  }

  TableHachage *H = creerTableHachage(M);
  if (H == NULL || H->T == NULL) {
    printf("Erreur : ReconstitueReseauHachage\n");
    return NULL;
  }
  /*Parcours de la liste de chaines C et ajout des Noeuds dans le Réseau. */
  CellChaine *current = C->chaines;
  while (current != NULL) {
    CellPoint *p = current->points;
    Noeud *prec = NULL, *extrA, *extrB, *n;
    while (p != NULL) {
      //Création des noeuds et mise à jour de la liste des voisins
      n = rechercheCreeNoeudHachage(R, H, p->x, p->y);
      if (prec != NULL) {
        fusionVoisins(n, prec);
      } else {
        extrA = n; // Si aucun précédent alors extrémité A
      }
      prec = n;
      p = p->suiv;
    }
    extrB = n;

    CellCommodite* co = malloc(sizeof(CellCommodite));
    co->extrA = extrA;
    co->extrB = extrB;
    co->suiv = R->commodites;
    R->commodites = co;
    current = current->suiv;
  }

 /*Ajout des CellCommodités dans le Réseau*/
  libererTableHachage(H);

  return R;
}
/*=====================================================================*/


/*============ Fonction de Libération TableHachage ====================*/


void libererTableHachage(TableHachage *H){
    if(H==NULL) return;

    CellNoeud *del, *c;
    for(int i=0; i<(H->tailleMax); i++){
        c = H->T[i];
        del = NULL;
        while(c){
            del = c;
            c = c->suiv;
            libererCellNoeud(del);
        }
    }

    free(H->T);
    free(H);
}
