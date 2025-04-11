#include "labyrinthe.h"

void clearScreen ()
{
    printf("\e[1;1H\e[2J");
    system("cls");
}

void afficheCouleur (ColorCode fg, ColorCode bg, char c)
{
    printf("\033[%d;%dm %c\033[m", FG_SHIFT + fg, BG_SHIFT + bg, c);
}

Labyrinthe createLabyrinthe(int taille){
    Labyrinthe lab;
    (taille % 2 == 1) ? taille : ++taille; // taille toujours impair sinon c'est cassé
    lab.taille = taille;

    // Allocation de la place
    lab.carte = malloc(sizeof(Case*) * lab.taille);
    for(int ligne = 0; ligne < lab.taille; ligne++){
        lab.carte[ligne] = malloc(sizeof(Case) * lab.taille);
    }

    init(&lab);
    genereLabyrinthe(&lab, lab.taille);

    // Poisitionne l'entrée et la sortie
    int posXdebut = 1;
    int posXfin = lab.taille - 2;
    int posYdebut = rand()%(lab.taille - 1) + 1;
    int posYfin = rand()%(lab.taille - 1);
    
    while(lab.carte[posYdebut][posXdebut].type == MUR)
        posYdebut = rand()%(lab.taille - 1) + 1;
    while (lab.carte[posYfin][posXfin].type == MUR)
        posYfin = rand()%(lab.taille - 1);

    lab.carte[posYdebut][posXdebut].type = DEBUT;
    lab.carte[posYfin][posXfin].type = FIN;

    lab.debut = &lab.carte[posYdebut][posXdebut];
    lab.fin = &lab.carte[posYfin][posXfin];

    lab.debut->isVisite = true;

    return lab;
}

/// @brief genere le Labyrinthe en positionnant les mur et en attribuant des valeurs pour chaque case vide
/// @param lab 
void init(Labyrinthe *lab){
    int nb = 1;
    for (int y = 0; y < lab->taille; y++){
        for (int x = 0; x < lab->taille; x++){
            if (lab->taille - 1 == x || 0 == x || 0 == y || lab->taille - 1 == y ||
                    x % 2 == 0 || y % 2 == 0){
                lab->carte[y][x].type = MUR;
                lab->carte[y][x].valeur = -1;
            }else{
                lab->carte[y][x].type = VIDE;
                lab->carte[y][x].valeur = nb++;
            }
            lab->carte[y][x].isVisite = false;
            lab->carte[y][x].x = x;
            lab->carte[y][x].y = y;
        }
    }
}

bool is_finished(Labyrinthe *maze)
{
	for (int i = 1; i < maze->taille; i += 2)
	{
		for (int j = 1; j < maze->taille; j += 2)
		{
			if (maze->carte[i][j].valeur != maze->carte[1][1].valeur)
				return false;
		}
	}

	return true;
}

void genereLabyrinthe(Labyrinthe *maze, int maze_taille){
	while (is_finished(maze) == 0)
	{
		int x = rand() % (maze_taille - 2) + 1;
		int y;

		if (x % 2 == 0)
			y = ((rand() % ((maze_taille - 1) / 2))) * 2 + 1;

		else
			y = ((rand() % ((maze_taille - 2) / 2))) * 2 + 2;

		int cell_1;
		int cell_2;

		if (maze->carte[y - 1][x].type == MUR)
		{
			cell_1 = maze->carte[y][x - 1].valeur;
			cell_2 = maze->carte[y][x + 1].valeur;
		}

		else
		{
			cell_1 = maze->carte[y - 1][x].valeur;
			cell_2 = maze->carte[y + 1][x].valeur;
		}

		if (cell_1 != cell_2)
		{
			maze->carte[y][x].valeur = 0;
			maze->carte[y][x].type = VIDE;

			for (int i = 1; i < maze_taille; i += 2)
			{
				for (int j = 1; j < maze_taille; j += 2)
				{
					if (maze->carte[i][j].valeur == cell_2){
						maze->carte[i][j].valeur = cell_1;
						maze->carte[i][j].type = VIDE;
                    }
				}
			}
		}
	}
}

void libereLabyrinthe(Labyrinthe *m){
    for (int i = 0; i < m->taille; i++){
        free(m->carte[i]);
    }
    free(m->carte);
}

void afficheLabyrinthe(Labyrinthe l){
    for(int y = 0; y < l.taille; y++){
        for(int x = 0; x < l.taille; x++){
            switch (l.carte[y][x].type){
            case MUR:
                afficheCouleur(BLANC, MAGENTA, ' ');
                break;
            case FIN:
                afficheCouleur(BLANC, ROUGE, ' ');
                break;
            case DEBUT:
                afficheCouleur(BLANC, VERT, ' ');
                break;
            case CHEMIN:
                afficheCouleur(BLANC, JAUNE, ' ');
                break;
            
            default:
                afficheCouleur(BLANC, NOIR, ' ');
                break;
            }
        }
        printf("\n");
    }
}

#ifdef DEBUG
#include <time.h>

int main(int argc, char ** argv){
    int taille;
    printf("%d\n", argc);

    taille = 11;
    if (argc > 1){
        taille = atoi(argv[1]);
    }
    printf("%d\n", taille);
    srand(time(NULL));
    
        Labyrinthe lab = createLabyrinthe(taille);
        afficheLabyrinthe(lab);
        libereLabyrinthe(&lab);
}
#endif
//gcc labyrinthe.c -o labyrinte -DDEBUG
// source de la genération du code https://www.youtube.com/watch?v=K7vaT8bZRuk , YTB : DIMENSION CODE