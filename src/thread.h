#ifndef _THREAD_LAB

#define _THREAD_LAB

#include "labyrinthe.h"

typedef struct argsLab{
    Labyrinthe *lab;
    Case *current;
}argsLab;

typedef enum { CAS_BLOQUE, CAS_SORTIE, CAS_INTERSECTION } TypeRetour;

typedef struct RetourParcours {
    TypeRetour type;
    void *donnee;
    int lenDonnee;
} RetourParcours;

void resoudLab_thread(Labyrinthe *lab);
void *parcours(void *lab);

#endif