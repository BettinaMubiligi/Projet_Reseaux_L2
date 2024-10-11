// Beeverly Gourdette & Bettina Mubiligi

#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include "Reseau.h"

typedef struct{
  int tailleMax;
  CellNoeud** T;
} TableHachage ;

int key(double x, double y);
int hachage (int k,int M);
TableHachage * creerTableHachage(int M);
Reseau * reconstitueReseauHachage(Chaines *C, int M);
Noeud * rechercheCreeNoeudHachage(Reseau * R, TableHachage * H, double x, double y);
void libererTableHachage(TableHachage *H);

#endif
