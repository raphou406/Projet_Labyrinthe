#ifndef LABYRINTHE_H
#define LABYRINTHE_H

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
typedef enum {MUR = 0, DEBUT, FIN, VIDE, CHEMIN} Type;

typedef struct Case{
    int valeur;
    Type type;
    bool isVisite;
    int x, y;
}Case;


typedef struct Labyrinthe{
    Case** carte;
    int taille;
    Case *debut, *fin;
}Labyrinthe;


/* Prototype des fonctions */

/* fonctions du laybrinthe */
Labyrinthe createLabyrinthe(int);
void libereLabyrinthe(Labyrinthe*);
void afficheLabyrinthe(Labyrinthe);
void init(Labyrinthe*);
void genereLabyrinthe(Labyrinthe*, int taille);
bool is_finished(Labyrinthe*);

/* fonction terminal */
void clearScreen();
void afficheCouleur(ColorCode, ColorCode, char);

#endif