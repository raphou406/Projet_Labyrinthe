#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

// Flag global pour indiquer si la sortie a été trouvée
static bool found = false;

// Mutex pour synchroniser l'accès à `found`
pthread_mutex_t found_mutex = PTHREAD_MUTEX_INITIALIZER;

// Fonction pour définir la valeur de `found` de manière thread-safe
void set_found(bool value) {
    pthread_mutex_lock(&found_mutex);
    found = value;
    pthread_mutex_unlock(&found_mutex);
}

// Fonction pour obtenir la valeur de `found` de manière thread-safe
bool get_found() {
    pthread_mutex_lock(&found_mutex);
    bool value = found;
    pthread_mutex_unlock(&found_mutex);
    return value;
}

// Fonction principale pour résoudre le labyrinthe avec des threads
void resoudLab_thread(Labyrinthe *lab) {
    found = false;

    argsLab args;
    args.lab = lab;
    args.current = lab->debut;

    pthread_t *threads = malloc(sizeof(pthread_t));
    int lenThreads = 1;
    pthread_create(&threads[lenThreads - 1], NULL, parcours, &args);

    lab->debut->valeur = 0;

    while (!get_found()) {
        void *res;

        // Attendre le dernier thread créé
        pthread_join(threads[lenThreads - 1], &res);

        RetourParcours *sortie = (RetourParcours *)res;

        switch (sortie->type) {
            case CAS_BLOQUE:
                printf("Cul-de-sac trouvé.\n");
                free(sortie);
                break;

            case CAS_SORTIE:
                set_found(true);
                free(sortie);
                break;

            case CAS_INTERSECTION: {
                Case **cases = (Case **)sortie->donnee;
                for (int i = 0; i < sortie->lenDonnee; i++) {
                    Case *nextCase = cases[i];

                    if (!get_found()) {
                        argsLab *newArgs = malloc(sizeof(argsLab)); // Copie indépendante
                        newArgs->lab = args.lab;
                        newArgs->current = nextCase;

                        threads = realloc(threads, sizeof(pthread_t) * (++lenThreads));
                        pthread_create(&threads[lenThreads - 1], NULL, parcours, newArgs);

                        printf("Thread créé pour la case (%d, %d): %d\n", nextCase->x, nextCase->y, lenThreads);
                    }
                }
                free(cases);
                free(sortie);
                break;
            }
        }

        afficheLabyrinthe(*lab);
    }

    // Attendre tous les threads restants
    for (int i = 0; i < lenThreads - 1; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);

    if (get_found()) {
        // Marquer le chemin trouvé
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

// Fonction pour explorer le labyrinthe avec un thread
void *parcours(void *parametres) {
    argsLab *args = (argsLab *)parametres;
    Case *current = args->current;

    RetourParcours *retour = malloc(sizeof(RetourParcours));
    Case **toThread = malloc(sizeof(Case*));
    int tailleVisite = 0;

    while (!get_found()) {
        Case *voisins[] = {
            &args->lab->carte[current->y - 1][current->x],
            &args->lab->carte[current->y + 1][current->x],
            &args->lab->carte[current->y][current->x - 1],
            &args->lab->carte[current->y][current->x + 1],
        };

        for (int voisin = 0; voisin < 4; voisin++) {
            if (voisins[voisin]->type == FIN) {
                voisins[voisin]->valeur = current->valeur + 1;
                args->lab->fin = voisins[voisin];
                set_found(true);

                free(toThread);
                retour->donnee = &current;
                retour->type = CAS_SORTIE;
                pthread_exit(retour);
            }

            if (voisins[voisin]->type != MUR && !voisins[voisin]->isVisite) {
                toThread = realloc(toThread, sizeof(Case*) * (++tailleVisite));
                toThread[tailleVisite - 1] = voisins[voisin];

                voisins[voisin]->isVisite = true;
                voisins[voisin]->valeur = current->valeur + 1;
                voisins[voisin]->type = CHEMIN;
                printf("x:%d ,y:%d\n", voisins[voisin]->x, voisins[voisin]->y);
            }
        }

        if (tailleVisite > 1) {
            retour->donnee = toThread;
            retour->lenDonnee = tailleVisite;
            retour->type = CAS_INTERSECTION;
            pthread_exit(retour);
        } else if (tailleVisite == 1) {
            current = toThread[0];
        } else {
            printf("Thread (%d, %d) détecte un cul-de-sac.\n", current->x, current->y);
            free(toThread);
            retour->donnee = &current;
            retour->type = CAS_BLOQUE;
            pthread_exit(retour);
        }
    }

    free(toThread);
    free(retour);
    pthread_exit(NULL);
}