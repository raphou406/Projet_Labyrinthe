#ifndef labyrinthe
#define labyrinthe

/* utilisation de la couleur */
#define FG_SHIFT 30
#define BG_SHIFT 40

typedef enum {NOIR = 0, ROUGE, VERT, JAUNE, BLEU, MAGENTA, CYAN, BLANC} ColorCode;
typedef enum {START, FINISH, EMPTY} CellType;
typedef struct Cell{
    bool upWall;
    bool downWall;
    bool leftWall;
    bool rightWall;
    CellType type;
}Cel;
typedef struct Maze{
    //forcement rectangle
    /*

    hauteur[1] = 1010101010
    hauteur[1] + largeur[1] = 3
        0101010101
        1310101010
        0101010101
    
    */
    Case** carte;
    int hauteur;
    int largeur;
}Maze;

//Prototype des fonctions
Maze createMaze(int, int);


#endif

