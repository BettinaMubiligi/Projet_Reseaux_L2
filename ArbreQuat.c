// Beeverly Gourdette & Bettina Mubiligi

#include "ArbreQuat.h"
#include "Chaine.h"
#include "Reseau.h"
#include <stdio.h>
#include <stdlib.h>



/*================ Outils pour créer un ArbreQuat ==================*/

/*Fonctions permettant de recupérer les coordonnées minimaux et maximaux x et y d'une chaine*/
void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax) {

  if (C == NULL) {
    printf("Liste de Chaines vide.\n");
    return;
  }

  if (C->chaines == NULL) {
    printf("Aucune chaines présentes dans la Liste de Chaine.\n");
    return;
  }

  /*Demarrage au premier point existant d'une chaine*/
  CellChaine *c = C->chaines;
  while (c->points == NULL) {
    c = c->suiv;
  }

  if (c == NULL) {
    printf("Il n'y a aucun point existant.\n");
    return;
  }

  CellPoint *p = c->points;
  *xmin = *xmax = p->x;
  *ymin = *ymax = p->y;

  double x, y;

  /*Comparaison avec chaque point de chaque chaine de la liste afin d'extraire les coordonnées x et y minimaux et maximaux*/
  while (c != NULL) {
    p = c->points;
    while (p != NULL) {
      x = p->x;
      y = p->y;
      if (x < (*xmin)) (*xmin) = x;
      if (x > (*xmax)) (*xmax) = x;
      if (y < (*ymin)) (*ymin) = y;
      if (y > (*ymax)) (*ymax) = y;
      p = p->suiv;
    }
    c = c->suiv;
  }
}

/*--------------------------------------------------------------*/ 


/*Fonction de création d'un ArbreQuat */
ArbreQuat *creerArbreQuat(double xc, double yc, double coteX, double coteY) {
  ArbreQuat *abr = (ArbreQuat *)malloc(sizeof(ArbreQuat));
  if (abr == NULL) {
    printf("Erreur lors de l'allocation mémoire de l'ArbreQuat \n");
    return NULL;
  }
  abr->coteX = coteX;
  abr->coteY = coteY;
  abr->xc = xc;
  abr->yc = yc;
  abr->noeud = NULL;
  abr->ne = NULL;
  abr->no = NULL;
  abr->se = NULL;
  abr->so = NULL;
  return abr;
}

/*=====================================================================*/

/*========== Libération de la mémoire d'un ArbreQuat ==================*/

/*Libération de la mémoire d'un ArbreQuat*/
void libererArbreQuat(ArbreQuat *abr) {
  if (abr == NULL) return;

  abr->noeud=NULL; //On ne libère pas le noeud ici, car noeud partagé avec Réseau.

  libererArbreQuat(abr->so);
  libererArbreQuat(abr->no);
  libererArbreQuat(abr->se);
  libererArbreQuat(abr->ne);

  free(abr);
}

/*=====================================================================*/

/*======== Outils pour la mise en relation Chaines et ArbreQuat =========*/

/*Mise a jour des centre xc et yc*/
void updateCentre(Noeud *N, double *yc, double *xc, ArbreQuat *parent) {
  if (N->x >= (parent->xc))
    *xc = (parent->xc) + (parent->coteX) / 4;
  else
    *xc = (parent->xc) - (parent->coteX) / 4;

  if (N->y >= (parent->yc))
    *yc = (parent->yc) + (parent->coteY) / 4;
  else
    *yc = (parent->yc) - (parent->coteY) / 4;
}


/*--------------------------------------------------------------*/

/*Retour l'arbre correspondant à la bonne direction à poursuivre selon les coordonnées x et y */
ArbreQuat *updateSens(double x, double y, ArbreQuat *abr) {
  if (y >= abr->yc && x >= abr->xc)
    return abr->ne;
  if (y >= abr->yc && x <= abr->xc)
    return abr->no;
  if (y <= abr->yc && x >= abr->yc)
    return abr->se;
  return abr->so;
}
/*--------------------------------------------------------------*/


/*Insertion d'un Noeud dans un ArbreQuat.*/
void insererNoeudArbre(Noeud *N, ArbreQuat **a, ArbreQuat *parent) {
  if (N == NULL) {
    printf("Noeud non alloué\n");
    return;
  }
  if (parent == NULL) {
    printf("Parent non alloué\n");
    return;
  }

  /*Cas d'arrêt : L'arbre a est vide : Création de l'ArbreQuat avec comme noeud N et toutes les directions initialisées à NULL. */
  double yc, xc;
  if ((*a) == NULL) {
    double coteX = parent->coteX / 2;
    double coteY = parent->coteY / 2;
    updateCentre(N, &yc, &xc, parent);
    *a = creerArbreQuat(xc, yc, coteX, coteY);
    (*a)->noeud = N;
    return;
  }
  /*Cas Cellule interne : On continue de déplacer dans la bonne direction jusqu'à trouver une Feuille*/
  if ((*a != NULL) && ((*a)->noeud == NULL)) {
    ArbreQuat *abrN = updateSens(N->x, N->y, *a);
    insererNoeudArbre(N, &abrN, *a);
    return;
  }

  /*Cas Feuille : Il faut transformer la Feuille en Cellule interne et deplacer les deux noeuds :  noeud d'origine et N. */
  else {
    Noeud *old = (*a)->noeud;
    (*a)->noeud = NULL;

    ArbreQuat *abrOld = updateSens(old->x, old->y, *a);
    ArbreQuat *abrN = updateSens(N->x, N->y, *a);

    insererNoeudArbre(old, &abrOld, *a);
    insererNoeudArbre(N, &abrN, *a);
    return;
  }
}

/*--------------------------------------------------------------*/

/*Recherche un Noeud avec les coordonnées x et y dans un ArbreQuat, le crée s(il n'existe pas)*/
Noeud *rechercheCreeNoeudArbre(Reseau *R, ArbreQuat **a, ArbreQuat *parent, double x, double y) {
  if (R == NULL) {
    printf("Réseau non alloué\n");
    return NULL;
  }
  if (parent == NULL) {
    printf("Parent non alloué\n");
    return NULL;
  }

  /*Cas Arbre Vide: Création de l'arbre et création du Noeud dans Réseau si pas dedans*/
  if ((*a) == NULL) {
    Noeud *N = creeNoeud(R->nbNoeuds+1,x,y);
    CellNoeud *c = creerCellNoeud(N);
    c->suiv = R->noeuds;
    R->noeuds = c;
    R->nbNoeuds++;
    insererNoeudArbre(N, a, parent);
    return N;
  }

  /*Cas Cellule interne  : Recherche dans la bonne direction.*/
  if ((*a != NULL) && ((*a)->noeud == NULL)) {
    ArbreQuat *abrN = updateSens(x, y, *a);
    return rechercheCreeNoeudArbre(R, &abrN, *a, x, y);
  }

  /*Cas Feuille  : Mise à jour du réseau, ajout et création de l'arbre si Noeud pas dedans.*/
  else {
    if ((*a)->noeud->x == x && (*a)->noeud->y == y)
      return (*a)->noeud;
  }
  return NULL;
}

/*=====================================================================*/

/*======== Mise en relation Chaines, ArbreQuat et Réseau =========*/

/*Reconstitue un Réseau à parir d'un chaine */
Reseau *reconstitueReseauArbre(Chaines *C) {
  if (C == NULL) {
    printf("Liste de Chaines non alloué\n");
    return NULL;
  }

  /* Création du Réseau R */
  Reseau *R = creerReseau(C->gamma);
  if (R == NULL) {
    printf("Erreur : ReconstitueReseauArbre\n");
    return NULL;
  }

  /* Création de l'arbre initial */

  double xmin, ymin, xmax, ymax;
  chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);

  double xc = (xmin + xmax) / 2;
  double yc = (ymin + ymax) / 2;
  double coteX = xmax - xmin;
  double coteY = ymax - ymin;
  ArbreQuat * abrN;
  Noeud* N, *extrA, *extrB;
  ArbreQuat *a = creerArbreQuat(xc,yc,coteX,coteY); 

  /* Traitement des chaines */
  CellChaine *c = C->chaines;
  CellPoint *p;
  Noeud *prec;
  while (c != NULL) {
    p = c->points;
    prec = NULL;
    while (p != NULL) {
      
      abrN = updateSens(p->x,p->y,a);
      N = rechercheCreeNoeudArbre(R, &abrN, a, p->x, p->y);
      if (prec != NULL) {
        fusionVoisins(N, prec);
      }
      else {
        extrA = N; // Si pas de précédent alors le Noeud est l'extremité A
      }
      prec = N;
      p = p->suiv;
      libererArbreQuat(abrN);
    }

    extrB = N; //Le dernier Noeud est l'extremité B

    /*Création du CellCommodite et insertion dans le Réseau*/
    CellCommodite *co = malloc(sizeof(CellCommodite));
    co->extrA = extrA;
    co->extrB = extrB;
    co->suiv=R->commodites;
    R->commodites = co;
    
    c = c->suiv;
  }


  libererArbreQuat(a);

  return R;
}

/*=====================================================================*/