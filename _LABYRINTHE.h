#ifndef _LABYRINTHE
#define _LABYRINTHE

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ENTER() printf("entering function %s\n", __func__);
#define LEAVE() printf("leaving function %s\n", __func__);

/* utilisation de la couleur */
#define FG_SHIFT 30
#define BG_SHIFT 40
typedef enum {NOIR = 0, ROUGE, VERT, JAUNE, BLEU, MAGENTA, CYAN, BLANC} ColorCode;

/* Structure du labyrinthe */
typedef enum {MUR = 0, DEBUT, FIN, VIDE} Case;

typedef struct Labyrinthe{
    Case** carte;
    int hauteur;
    int largeur;
    Case depart;
    Case Fin;
}Labyrinthe;


/* Prototype des fonctions */

/* fonctions du laybrinthe */
Labyrinthe createLabyrinthe(int, int);
void libereLabyrinthe(Labyrinthe*);
void afficheLabyrinthe(Labyrinthe);
void init(Labyrinthe*);
void genereLabyrinthe(Labyrinthe*, int x1, int y1, int x2, int y2);
bool isOOB(Labyrinthe, int, int);

/* fonction terminal */
void clearScreen();
void afficheCouleur(ColorCode, ColorCode, char);

#endif