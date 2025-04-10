#include <time.h>
#include <string.h>
#include "labyrinthe.h"
#include "thread.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void wait( int seconds )
{   // Pretty crossplatform, both ALL POSIX compliant systems AND Windows
    #ifdef _WIN32
        Sleep( 1000 * seconds );
    #else
        sleep( seconds );
    #endif
}

void resoudLab(Labyrinthe *lab){
    Case current;
    bool found = false;

    lab->debut->valeur = 0;
    int tailleQueue = 1, tete = 0, distance = 0;
    Case** queue = malloc(sizeof(Case*)); 
    queue[0] = lab->debut;


    while (!found && tete != tailleQueue){
        current = *queue[tete++];

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
                voisins[voisin]->valeur = distance++;
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

            Case min = current;
            for (int voisin = 0; voisin < 4; voisin++){
                if (voisins[voisin]->type != MUR && voisins[voisin]->valeur < min.valeur && voisins[voisin]->isVisite){
                    min = *voisins[voisin];
                }
            }
            current = min;
            lab->carte[current.y][current.x].type = CHEMIN;
        }
    }
    lab->carte[lab->debut->y][lab->debut->x].type = DEBUT;
    free(queue);
}

#ifndef DEBUG
int main(){
    int taille = 10;
    srand(time(NULL));

    Labyrinthe lab = createLabyrinthe(taille);
    afficheLabyrinthe(lab);
    //resoudLab(&lab);

    resoudLab_thread(&lab);

    afficheLabyrinthe(lab);
    libereLabyrinthe(&lab);
    wait(1);
    return 0;
}
#endif