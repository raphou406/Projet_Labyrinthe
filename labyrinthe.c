#include "labyrinthe.h"
#include <stdio.h>
#include <stdlib.h>

void clearScreen ()
{
    printf("\e[1;1H\e[2J");
    system("cls");
}

void afficheCouleur (ColorCode fg, ColorCode bg, char c)
{
    printf("\033[%d;%dm %c\033[m", FG_SHIFT + fg, BG_SHIFT + bg, c);
}

Maze createMaze(int height, int width){
    Maze maze;
    maze.carte = malloc(sizeof(Case*) * height);
    for (int i = 0; i < width; i++){
        maze.carte[i] = malloc(sizeof(Case) * width);
    }
    for(int )
}

void initMaze(Maze m){
    
}

void freeMaze(Maze m){
    for (int i = 0; i < m.largeur; i++){
        free(m.carte[i]);
    }
    free(m.carte);
    
}

void displayMaze(Maze lab){

}
#ifdef DEBUG
int main(){

}
#endif
//gcc labyrinthe.c -o labyrinte -DDEBUG
