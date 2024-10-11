// Beeverly Gourdette & Bettina Mubiligi

#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include "Chaine.h"
#include "Reseau.h"
#include "Struct_File.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int u, v; /* Numeros des sommets extremite */
} Arete;

typedef struct cellule_arete {
  Arete *a; /* pointeur sur l’arete */
  struct cellule_arete *suiv;
} Cellule_arete;

typedef struct {
  int num; /* Numero du sommet (le meme que dans T_som) */
  double x, y;
  Cellule_arete *L_voisin; /* Liste chainee des voisins */
} Sommet;

typedef struct {
  int e1, e2; /* Les deux extremites de la commodite */
} Commod;

typedef struct {
  int nbsom;      /* Nombre de sommets */
  Sommet **T_som; /* Tableau de pointeurs sur sommets */
  int gamma;
  int nbcommod;     /* Nombre de commodites */
  Commod *T_commod; /* Tableau des commodites */
} Graphe;

Graphe *creerGraphe(Reseau *r);
S_file *Liste_chaine(Graphe *g, int u, int v); // ex 7.3
int reorganiseReseau(Reseau *r);
// Fonctions ajoutées
void rechercheArete(int **matrice, S_file *chaine, int i, int j);
void afficherGraphe(Graphe *g);
Arete *creerArete(int u, int v);
Sommet *creerSommet(int num, int x, int y);
Cellule_arete *creerCelluleArete(Arete *a);
void libererGraphe(Graphe *g);
int nbAretes(Graphe *g, int u, int v); // ex.7.2
void libererSommet(Sommet *s);
void libererCelluleArete(Cellule_arete *ca);

#endif
// Beeverly Gourdette & Bettina Mubiligi

#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include "Chaine.h"
#include "Reseau.h"
#include "Struct_File.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int u, v; /* Numeros des sommets extremite */
} Arete;

typedef struct cellule_arete {
  Arete *a; /* pointeur sur l’arete */
  struct cellule_arete *suiv;
} Cellule_arete;

typedef struct {
  int num; /* Numero du sommet (le meme que dans T_som) */
  double x, y;
  Cellule_arete *L_voisin; /* Liste chainee des voisins */
} Sommet;

typedef struct {
  int e1, e2; /* Les deux extremites de la commodite */
} Commod;

typedef struct {
  int nbsom;      /* Nombre de sommets */
  Sommet **T_som; /* Tableau de pointeurs sur sommets */
  int gamma;
  int nbcommod;     /* Nombre de commodites */
  Commod *T_commod; /* Tableau des commodites */
} Graphe;

Graphe *creerGraphe(Reseau *r);
S_file *Liste_chaine(Graphe *g, int u, int v); // ex 7.3
int reorganiseReseau(Reseau *r);
// Fonctions ajoutées
void rechercheArete(int **matrice, S_file *chaine, int i, int j);
void afficherGraphe(Graphe *g);
Arete *creerArete(int u, int v);
Sommet *creerSommet(int num, int x, int y);
Cellule_arete *creerCelluleArete(Arete *a);
void libererGraphe(Graphe *g);
int nbAretes(Graphe *g, int u, int v); // ex.7.2
void libererSommet(Sommet *s);
void libererCelluleArete(Cellule_arete *ca);

#endif
// Beeverly Gourdette & Bettina Mubiligi

#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include "Chaine.h"
#include "Reseau.h"
#include "Struct_File.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int u, v; /* Numeros des sommets extremite */
} Arete;

typedef struct cellule_arete {
  Arete *a; /* pointeur sur l’arete */
  struct cellule_arete *suiv;
} Cellule_arete;

typedef struct {
  int num; /* Numero du sommet (le meme que dans T_som) */
  double x, y;
  Cellule_arete *L_voisin; /* Liste chainee des voisins */
} Sommet;

typedef struct {
  int e1, e2; /* Les deux extremites de la commodite */
} Commod;

typedef struct {
  int nbsom;      /* Nombre de sommets */
  Sommet **T_som; /* Tableau de pointeurs sur sommets */
  int gamma;
  int nbcommod;     /* Nombre de commodites */
  Commod *T_commod; /* Tableau des commodites */
} Graphe;

Graphe *creerGraphe(Reseau *r);
S_file *Liste_chaine(Graphe *g, int u, int v); // ex 7.3
int reorganiseReseau(Reseau *r);
// Fonctions ajoutées
void rechercheArete(int **matrice, S_file *chaine, int i, int j);
void afficherGraphe(Graphe *g);
Arete *creerArete(int u, int v);
Sommet *creerSommet(int num, int x, int y);
Cellule_arete *creerCelluleArete(Arete *a);
void libererGraphe(Graphe *g);
int nbAretes(Graphe *g, int u, int v); // ex.7.2
void libererSommet(Sommet *s);
void libererCelluleArete(Cellule_arete *ca);

#endif
