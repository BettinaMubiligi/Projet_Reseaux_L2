// Beeverly Gourdette & Bettina Mubiligi

#ifndef __RESEAU_H__
#define __RESEAU_H__
#include "Chaine.h"

typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des
 * voisins de chaque noeud) */
typedef struct cellnoeud {
  Noeud *nd;              /* Pointeur vers le noeud stock\'e */
  struct cellnoeud *suiv; /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud {
  int num;            /* Numero du noeud */
  double x, y;        /* Coordonnees du noeud*/
  CellNoeud *voisins; /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
  Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
  struct cellCommodite *suiv; /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
  int nbNoeuds;              /* Nombre de noeuds du reseau */
  int gamma;                 /* Nombre maximal de fibres par cable */
  CellNoeud *noeuds;         /* Liste des noeuds du reseau */
  CellCommodite *commodites; /* Liste des commodites a relier */
} Reseau;

Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y);
Reseau *reconstitueReseauListe(Chaines *C);
void ecrireReseau(Reseau *R, FILE *f);
int nbLiaisons(Reseau *R);
int nbCommodites(Reseau *R);
void afficheReseauSVG(Reseau *R, char *nomInstance);

// fonctions ajoutées
int InVoisins(Noeud *N1, Noeud *N2);
void fusionVoisins(Noeud *N1, Noeud *N2);
void addCommodite(Reseau *R, Chaines *C);
void printCommodite(Reseau *R);
void afficheReseau(Reseau *R);
Reseau *creerReseau(int gamma);
CellNoeud *creerCellNoeud(Noeud *N);
Noeud *creeNoeud(int num, double x, double y);
int compteVoisins(CellNoeud *voisins);
void libererReseau(Reseau *R);
void libererCellNoeud(CellNoeud *c);
void libererNoeud(Noeud *N);
void libererCellCommodite(CellCommodite *co);

#endif
