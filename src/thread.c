#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

#define MAX_THREADS 2

// Variables global pour les threads
static bool found = false;
static int nbrThreads = 0;
int maxThread = 0;

Case **queue ;
int tailleQueue;
int tete;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// permet de modifier la valeur de found en safe thread
void set_found(bool value) {
    pthread_mutex_lock(&mutex);
    found = value;
    pthread_mutex_unlock(&mutex);
}

// permet d'avoir la valeur de found en safe thread
bool get_found() {
    pthread_mutex_lock(&mutex);
    bool value = found;
    pthread_mutex_unlock(&mutex);
    return value;
}

void resoudLab_thread(Labyrinthe *lab) {
    found = false;

    argsLab args;
    args.lab = lab;
    args.current = lab->debut;

    pthread_t start;

    nbrThreads++;
    pthread_create(&start, NULL, parcours, &args);

    lab->debut->valeur = 0;

    while(nbrThreads > 0){} // tant qu'il y a des threads je continue - puisque tout est thread pas besoin de verifié si la sortie est trouvé

    if (get_found()) { // trace le chemin
        Case current = *lab->fin;
        while (current.valeur) {
            Case *voisins[] = {
                &lab->carte[current.y - 1][current.x],
                &lab->carte[current.y + 1][current.x],
                &lab->carte[current.y][current.x - 1],
                &lab->carte[current.y][current.x + 1],
            };

            Case min = current;
            for (int voisin = 0; voisin < 4; voisin++) {
                if (voisins[voisin]->type != MUR && voisins[voisin]->valeur < min.valeur && voisins[voisin]->isVisite) {
                    min = *voisins[voisin];
                }
            }
            current = min;
            lab->carte[current.y][current.x].type = CHEMIN;
        }
    }

    lab->carte[lab->debut->y][lab->debut->x].type = DEBUT;
}

void *parcours(void *parametres) {
    argsLab *args = (argsLab *)parametres;
    Case *current = args->current;

    while (!get_found()) {
        int nbrVoisin = 0;
        Case *voisins[] = {
            &args->lab->carte[current->y - 1][current->x],
            &args->lab->carte[current->y + 1][current->x],
            &args->lab->carte[current->y][current->x - 1],
            &args->lab->carte[current->y][current->x + 1],
        };

        for (int voisin = 0; voisin < 4; voisin++) {
            if (voisins[voisin]->type == FIN) {// si je trouve la sortie
                voisins[voisin]->valeur = current->valeur + 1;
                args->lab->fin = voisins[voisin];
                set_found(true);

                pthread_mutex_lock(&mutex);
                nbrThreads--;
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }

            if (voisins[voisin]->type != MUR && !voisins[voisin]->isVisite) {// relance un nouveau thread avec les nouveaux chemin 
                nbrVoisin++;

                voisins[voisin]->isVisite = true;
                voisins[voisin]->valeur = current->valeur + 1;
                
                argsLab *newArgs = malloc(sizeof(argsLab));
                newArgs->lab = args->lab;
                newArgs->current = voisins[voisin];
                pthread_t newChemin;
                pthread_mutex_lock(&mutex);
                nbrThreads++;
                pthread_mutex_unlock(&mutex);
                pthread_create(&newChemin, NULL, parcours, newArgs);
                break; // c'est tres barbare on crée un thread meme si c'est une ligne droite mais c'est pas grave ca fonctionne
            }
        }

        if (nbrVoisin == 0){ // si c'est un cul-de-sac on tue le thread
            break;
        }
    }

    pthread_mutex_lock(&mutex);
    nbrThreads--;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


void resoudLab_threadAvecLimite(Labyrinthe *lab) {
    found = false;
    queue = malloc(sizeof(Case*) * 10);
    tailleQueue = 0;
    tete = 0;

    argsLab *args = malloc(sizeof(argsLab));
    args->lab = lab;
    args->current = lab->debut;

    lab->debut->valeur = 0;
    lab->debut->isVisite = true;

    pthread_t start;
    nbrThreads = 1;
    pthread_create(&start, NULL, parcours_avecLimiteThreads, args);

    while (!get_found()) { // laisse les threads chercher la sortie tant qu'elle n'est pas trouvé     
    }
    if (get_found()) { // trace le chemin de l'entrée vers la sortie
        Case current = *lab->fin;
        while (current.valeur) {
            Case *voisins[] = {
                &lab->carte[current.y - 1][current.x],
                &lab->carte[current.y + 1][current.x],
                &lab->carte[current.y][current.x - 1],
                &lab->carte[current.y][current.x + 1],
            };

            Case min = current;
            for (int i = 0; i < 4; i++) {
                if (voisins[i]->type != MUR && voisins[i]->valeur < min.valeur && voisins[i]->isVisite) {
                    min = *voisins[i];
                }
            }
            current = min;
            lab->carte[current.y][current.x].type = CHEMIN;
        }
    }

    lab->carte[lab->debut->y][lab->debut->x].type = DEBUT;

    free(queue);
}

void *parcours_avecLimiteThreads(void *parametres) {
    argsLab *args = (argsLab *)parametres;
    Case *current = args->current;

    while (!get_found()) {

        Case *voisins[] = {
            &args->lab->carte[current->y - 1][current->x],
            &args->lab->carte[current->y + 1][current->x],
            &args->lab->carte[current->y][current->x - 1],
            &args->lab->carte[current->y][current->x + 1],
        };

        int nbrVoisin = 0;
        for (int i = 0; i < 4; i++) {
            Case *voisin = voisins[i];

            if (voisin->type == FIN) {
                set_found(true);
                pthread_mutex_lock(&mutex);
                nbrThreads--;
                pthread_mutex_unlock(&mutex);

                voisin->valeur = current->valeur + 1;
                args->lab->fin = voisin;
                free(args);
                pthread_exit(NULL);
            }

            if (voisin->type != MUR && !voisin->isVisite) { // la case est accessible et non exploré
                pthread_mutex_lock(&mutex);
                voisin->isVisite = true;
                pthread_mutex_unlock(&mutex);

                voisin->valeur = current->valeur + 1;
                nbrVoisin++;

                pthread_mutex_lock(&mutex);
                if (nbrThreads < MAX_THREADS) { // si j'ai encore des threads disponible j'en recrée un
                    nbrThreads++;
                    if (nbrThreads > maxThread) maxThread = nbrThreads;

                    argsLab *newArgs = malloc(sizeof(argsLab));
                    newArgs->lab = args->lab;
                    newArgs->current = voisin;
                    pthread_t t;
                    pthread_create(&t, NULL, parcours_avecLimiteThreads, newArgs);
                } else { // sinon je fais du backtracking
                    queue = realloc(queue, sizeof(Case*) * (tailleQueue + 1));
                    queue[tailleQueue++] = voisin;
                }
                pthread_mutex_unlock(&mutex);
            }
        }

        if (nbrVoisin == 0 && tete >= tailleQueue) { // si c'est un cul-de-sac on décrémente le nombre de thread et on le tue
            pthread_mutex_lock(&mutex);
            nbrThreads--;
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        pthread_mutex_lock(&mutex);
        if (tete < tailleQueue) { // on verfie qu'il y a un suivant
            current = queue[tete++];
        }
        pthread_mutex_unlock(&mutex);
    }
    free(args);
    pthread_mutex_lock(&mutex);
    nbrThreads--;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}