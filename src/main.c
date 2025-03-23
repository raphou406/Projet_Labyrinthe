#include <time.h>
#include <string.h>
#include "labyrinthe.h"

void resoudLab(Labyrinthe *lab){
    Case current;
    bool found = false;

    int tailleQueue = 1, tete = 0, distance = 0;
    Case** queue = malloc(sizeof(Case*)); 
    queue[0] = lab->debut;

    while (!found && tete != tailleQueue){
        current = *queue[tete++];
        lab->carte[current.y][current.x].valeur = distance++;

        Case *voisins[] = 
        {
            &lab->carte[current.y - 1][current.x],
            &lab->carte[current.y + 1][current.x],
            &lab->carte[current.y][current.x - 1],
            &lab->carte[current.y][current.x + 1],
        };

        for (int voisin = 0; voisin < 4; voisin++){
            if (voisins[voisin]->type != MUR && !voisins[voisin]->isVisite){
                voisins[voisin]->isVisite = true;
                queue = realloc(queue, sizeof(Case*) * (tailleQueue + 1));
                queue[tailleQueue++] = voisins[voisin];
            }
            if (voisins[voisin]->type == FIN){
                found = true;
                voisins[voisin]->valeur = distance;
                lab->fin = voisins[voisin];
                break;
            }
        }
        
    }
    if (found){
        current = *lab->fin;
        while(current.valeur){
            Case *voisins[] = 
            {
                &lab->carte[current.y - 1][current.x],
                &lab->carte[current.y + 1][current.x],
                &lab->carte[current.y][current.x - 1],
                &lab->carte[current.y][current.x + 1],
            };

            for (int voisin = 0; voisin < 4; voisin++){
                if (voisins[voisin]->type != MUR && voisins[voisin]->valeur < current.valeur && voisins[voisin]->isVisite){
                    current = *voisins[voisin];
                }
            }
            lab->carte[current.y][current.x].type = CHEMIN;
        }
    }
    lab->carte[lab->debut->y][lab->debut->x].type = DEBUT;
    free(queue);
}

#ifndef DEBUG
int main(){
    srand(time(NULL));
    int taille = 25;

    Labyrinthe lab = createLabyrinthe(taille);
    afficheLabyrinthe(lab);
    resoudLab(&lab);
    afficheLabyrinthe(lab);
    libereLabyrinthe(&lab);
    return 0;
}
#endif