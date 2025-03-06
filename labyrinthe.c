#include "_LABYRINTHE.h"
#include <time.h>

void clearScreen ()
{
    printf("\e[1;1H\e[2J");
    system("cls");
}

void afficheCouleur (ColorCode fg, ColorCode bg, char c)
{
    printf("\033[%d;%dm %c\033[m", FG_SHIFT + fg, BG_SHIFT + bg, c);
}

Labyrinthe createLabyrinthe(int hauteur, int largeur){
    Labyrinthe lab;
    lab.hauteur = hauteur + 1;
    lab.largeur = largeur + 1;
    lab.carte = malloc(sizeof(Case*) * lab.hauteur);
    for(int ligne = 0; ligne < lab.hauteur; ligne++){
        lab.carte[ligne] = malloc(sizeof(Case) * lab.largeur);
    }

    genereLabyrinthe(&lab, 1, 1, lab.largeur - 1, lab.hauteur - 1);

    init(&lab);

    int posXdebut = 1;
    int posXfin = lab.largeur - 2;
    int posYdebut = rand()%(lab.hauteur - 1);
    int posYfin = rand()%(lab.hauteur - 1);

    lab.carte[posYdebut][posXdebut] = DEBUT;
    lab.carte[posYfin][posXfin] = FIN;

    return lab;
}

void init(Labyrinthe *lab){
    for (int y = 0; y < lab->hauteur; y++){
        for (int x = 0; x < lab->largeur; x++){
            if (lab->largeur - 1 == x || 0 == x || 0 == y || lab->hauteur - 1 == y)
                lab->carte[y][x] = MUR;
        }
    }
}

void genereLabyrinthe(Labyrinthe* lab, int x1, int y1, int x2, int y2) {
    if (x2 - x1 < 2 || y2 - y1 < 2) return;

    int murX = x1 + (rand() % ((x2 - x1) / 2)) * 2 + 1;
    int murY = y1 + (rand() % ((y2 - y1) / 2)) * 2 + 1;

    for (int y = y1; y <= y2; y++) {
        lab->carte[y][murX] = MUR;
    }
    for (int x = x1; x <= x2; x++) {
        lab->carte[murY][x] = MUR;
    }

    int passage = rand() % 4;
    for (int i = 0; i < 4; i++) {
        if (i == passage) continue;
        switch (i) {
            case 0: lab->carte[murY][murX - 1] = VIDE; break;
            case 1: lab->carte[murY][murX + 1] = VIDE; break;
            case 2: lab->carte[murY - 1][murX] = VIDE; break;
            case 3: lab->carte[murY + 1][murX] = VIDE; break;
        }
    }

    genereLabyrinthe(lab, x1, y1, murX - 1, murY - 1);
    genereLabyrinthe(lab, murX + 1, y1, x2, murY - 1);
    genereLabyrinthe(lab, x1, murY + 1, murX - 1, y2);
    genereLabyrinthe(lab, murX + 1, murY + 1, x2, y2);
}

// Verifie si l'on est pas en dehors du labyrinthe (Out Of Bound)
bool isOOB(Labyrinthe lab, int posX, int posY){
    return lab.largeur <= posX || 0 > posX || lab.hauteur <= posY || 0 > posY; 
}

void libereLabyrinthe(Labyrinthe *m){
    for (int i = 0; i < m->largeur; i++){
        free(m->carte[i]);
    }
    free(m->carte);
}

void afficheLabyrinthe(Labyrinthe l){
    for(int y = 0; y < l.hauteur; y++){
        for(int x = 0; x < l.largeur; x++){
            switch (l.carte[y][x]){
            case MUR:
                afficheCouleur(BLANC, MAGENTA, ' ');
                break;
            case FIN:
                afficheCouleur(BLANC, ROUGE, ' ');
                break;
            case DEBUT:
                afficheCouleur(BLANC, VERT, ' ');
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
int main(){
    srand(time(NULL));
    Labyrinthe lab = createLabyrinthe(10, 10);
    afficheLabyrinthe(lab);
    libereLabyrinthe(&lab);
}
#endif
//gcc labyrinthe.c -o labyrinte -DDEBUG
