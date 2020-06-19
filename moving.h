void clearscreen();

void cbreak();

void nocbreak();

int *up(int **maze, int *pos);
int *down(int **maze, int *pos);
int *right(int **maze, int *pos);
int *left(int **maze, int *pos);

int *mapa(int **maze);
char *mover(int **maze, char *m, int *pos, int *info); //mover irá fazer return da mensagem
