// Beeverly Gourdette & Bettina Mubiligi

#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include<stdio.h>
#include "SVGwriter.h"
#include <math.h>


/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;


Chaines* lectureChaines(FILE *f);
void ecrireChaines(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);
void affichage(Chaines *C);
double longueurChaine(CellChaine *c);
CellPoint * insertCellPoint(CellChaine * c, double x, double y);
void libererCellChaine(CellChaine *c);
void libererChaines(Chaines *C);
CellChaine * creerCellChaine(int numero);
Chaines * creerChaines(int gamma, int nbChaines);
Chaines *generationAleatoire(int nbChaines, int nbPointsChaine, int xmax,
                             int ymax);


#endif	
