#ifndef _THREAD_LAB

#define _THREAD_LAB

#include "labyrinthe.h"

typedef struct argsLab{
    Labyrinthe *lab;
    Case *current;
}argsLab;

void resoudLab_thread(Labyrinthe *lab);
void resoudLab_threadAvecLimite(Labyrinthe *lab);
void *parcours(void *lab);
void *parcours_avecLimiteThreads(void *lab);

#endif