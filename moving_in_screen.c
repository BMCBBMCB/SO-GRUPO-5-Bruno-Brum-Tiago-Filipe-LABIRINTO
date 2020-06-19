#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#define COLUMNS 67
#define ROWS 32

struct termios oldtc;
struct termios newtc;
/*Cria estruturas termios para armazenar os parametros e
informações do terminal*/

void clearscreen(){
  /*Esta função faz uso das ANSI escape sequences,
  sequências de bytes que ao serem impressas,
  são interpretadas pelo terminal como comandos
  e não como caratéres.*/
  //printf("\033[2J");
  system("clear");  // foi utilizado este com o objetivo de corrigir um bug

}

void cbreak() {
  /*
  Esta função coloca o terminal no modo cbreak ou rare,
  no qual o terminal interpreta um carater de cada vez,
  mas continua a reconhecer combinações de teclas de controlo,
  como Ctrl-C.
  */
  tcgetattr(STDIN_FILENO, &oldtc);
  /*Obtẽm os parâmetros/atributos associados ao
  terminal e coloca-os na struct termios oldtc*/
  newtc = oldtc; //Atribui o valor de oldtc a newtc.
  newtc.c_lflag &= ~(ICANON | ECHO);
  /*Modifica a flag c_lflag que controla o modo do terminal,
  e efetua um bitwise-and com o bitwise-not do bitwise-or das constantes ICANON
  e ECHO, efetivamente definindo o modo não-canónico e a não-ecoação dos carateres
  introduzidos.
  Com o modo canónico desativado, a input do utilizador é dada caratér a carater,
  sem necessidade de delimitadores como newline, entre outras coisas.
  Com ECHO desativado, os carateres introduzidos não são ecoados, ou escritos,
  no ecrã.*/
  tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
  /*Define os atributos do terminal tal como definidos em newtc,
  ou seja, desativa o modo canónico e o eco*/
}

void nocbreak() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
  /*Repõe os atributos do terminal para aqueles obtidos no início do programa e
  guardados em oldtc.*/
  fflush(stdout);
}

int *up(int **maze, int *pos){
  pos[1]++;                            //soma 1 à posição anterior
  if (maze[pos[1]-1][pos[0]-1] != 1){
    printf("\033[%dA",1);              // movimenta para cima
  }
  else{
    pos[1]--;                          // caso a posição seguinte for uma parede é subtraido 1
  }                                    // à posição atual
  return pos;                          // retorna a posição atual
}

int *down(int **maze, int *pos){
  pos[1]--;
  if (maze[pos[1]-1][pos[0]-1] != 1){
    printf("\033[%dB",1);              // movimenta para baixo
  }
  else{
    pos[1]++;
  }
  return pos;
}

int *right(int **maze, int *pos){
  pos[0]++;
  //printf("\033[%dC",1);
  if (maze[pos[1]-1][pos[0]-1] != 1){
    printf("\033[%dC",1);              // movimenta para a direita
  }
  else{
    pos[0]--;
  }
  return pos;
}

int *left(int **maze, int *pos){
  pos[0]--;
  if (maze[pos[1]-1][pos[0]-1] != 1){
    printf("\033[%dD",1);             // movimenta para a esquerda
  }
  else{
    pos[0]++;
  }
  return pos;
}

int *mapa(int **maze){
  int *pos;
  pos = malloc(sizeof(int*) * 2);
  for(int i=ROWS;i>=0;i--){           // calcula a posição do S (pisição inicial)
    for(int j = 0; j < COLUMNS; j++){
      if (maze[i][j] == 2){
        pos[0] = i+2;
        pos[1] = j;
      }
    }
  }
  printf("\033[%d;%dH",ROWS+2-pos[1],pos[0]);   //coloca o cursor na posição do S
  return pos;
}

char *mover(int **maze, char *caract, int *p, int *info){
  int *pos = p;
  char t = *caract;
  char *mens = (char*) malloc(1024*sizeof(char)); //mensagem informativa que irá ser retornada no fim
  strcpy(mens, ""); //inicializada vazia

  if (t == 87 || t == 119)	pos = up(maze, pos);  //se "W" ou "w" corre funcao para tentar movimentar o cursor para cima(up)
  if (t == 68 || t == 100) 	pos = right(maze, pos); //se "D" ou "d"
  if (t == 115 || t == 83) 	pos = down(maze, pos); //se "S" ou "s"
  if (t == 97 || t == 65)  	pos = left(maze, pos); //se "A" ou "a"



  if (maze[pos[1]-1][pos[0]-1] == 3){  //Se chegar à posição do G o jogo acaba
    printf("\033[34;0HYOU WIN!");
    info[0]=2; //info[0] Posição se 0->inicio se 1->sítio dentro do labirinto se 2->final
  }
  else if (maze[pos[1]-1][pos[0]-1] == 2){  //Se chegar à posição do S o jogo está no início
    info[0]=0; //info[0] Posição se 0->inicio se 1->sítio dentro do labirinto se 2->final
  }
  else {info[0]=1;} //info[0] Posição se 0->inicio se 1->sítio dentro do labirinto se 2->final


info[6]; //onde se guardará os valores sobre o que rodeia o rato, para onde está virado e onde está(fim/inicio/nenhum)
	//info[0] Posição se 0->inicio se 1->sítio dentro do labirinto se 2->final
	//info[1] Direção se 0->Norte se 1->Este se 2->Oeste se 3->Sul
	//info[2] Parede em cima -> 1 sim   0 não
	//info[3] Parede à direita -> 1 sim   0 não
	//info[4] Parede à esquerda -> 1 sim   0 não
	//info[5] Parede em baixo -> 1 sim   0 não

  if(maze[pos[1]-1][pos[0]-1+1] == 1 && maze[pos[1]-1+1][pos[0]-1] != 1 && maze[pos[1]-1-1][pos[0]-1] != 1 && maze[pos[1]-1][pos[0]-1-1] != 1){  //Direita
    	//caso só parede à direita
    	info[3] = 1;
  }
  else if(maze[pos[1]-1][pos[0]-1-1] == 1 && maze[pos[1]-1+1][pos[0]-1] != 1 && maze[pos[1]-1][pos[0]-1+1] != 1 && maze[pos[1]-1-1][pos[0]-1] != 1){  //Esquerda
    	//caso só parede à esquerda
    	info[4] = 1;
  }
  else if(maze[pos[1]-1+1][pos[0]-1] == 1 && maze[pos[1]-1][pos[0]-1-1] != 1 && maze[pos[1]-1-1][pos[0]-1] != 1 && maze[pos[1]-1][pos[0]-1+1] != 1){  //Cima
    	//caso só parede em cima
	info[2] = 1;
  }
  else if(maze[pos[1]-1-1][pos[0]-1] == 1 && maze[pos[1]-1][pos[0]-1-1] != 1 && maze[pos[1]-1+1][pos[0]-1] != 1 && maze[pos[1]-1][pos[0]-1+1] != 1){  //Baixo
    	//caso só parede em baixo
    	info[5] = 1;
  }
  else if(maze[pos[1]-1][pos[0]-1+1] == 1 && maze[pos[1]-1+1][pos[0]-1] == 1 && maze[pos[1]-1-1][pos[0]-1] != 1 && maze[pos[1]-1][pos[0]-1-1] != 1){  //Direita
    	//caso só parede à direita e em cima
	info[2] = 1;
	info[3] = 1;
  }
  else if(maze[pos[1]-1][pos[0]-1-1] == 1 && maze[pos[1]-1+1][pos[0]-1] == 1 && maze[pos[1]-1][pos[0]-1+1] != 1 && maze[pos[1]-1-1][pos[0]-1] != 1){  //Esquerda
   	//caso só parede à esquerda e em cima
	info[4] = 1;
	info[2] = 1;
  }
  else if(maze[pos[1]-1][pos[0]-1-1] == 1 && maze[pos[1]-1+1][pos[0]-1] != 1 && maze[pos[1]-1][pos[0]-1+1] != 1 && maze[pos[1]-1-1][pos[0]-1] == 1){  //Esquerda
    	//caso só parede à esquerda e em baixo
	info[4] = 1;
	info[5] = 1;
  }
  else if(maze[pos[1]-1][pos[0]-1+1] == 1 && maze[pos[1]-1+1][pos[0]-1] != 1 && maze[pos[1]-1-1][pos[0]-1] == 1 && maze[pos[1]-1][pos[0]-1-1] != 1){  //Direita
   	//caso só parede à direita e em baixo
	info[3] = 1;
	info[5] = 1;
  }
  else if(maze[pos[1]-1+1][pos[0]-1] == 1 && maze[pos[1]-1][pos[0]-1-1] != 1 && maze[pos[1]-1-1][pos[0]-1] == 1 && maze[pos[1]-1][pos[0]-1+1] != 1){  //Cima
   	//caso só parede em cima e em baixo
	info[2] = 1;
	info[5] = 1;
  }
  else{  
    	//strcpy(mens, " ");

  }


//prints sobre paredes
      if(info[2]==1){
        strncat(mens,"Tem uma parede acima de si!\n",1000);
      }
      if(info[5]==1){
        strncat(mens,"Tem uma parede abaixo de si!\n",1000);
      }
      if(info[4]==1){
        strncat(mens,"Tem uma parede à sua esquerda!\n",1000);
      }
      if(info[3]==1){
        strncat(mens,"Tem uma parede à sua direita!\n",1000);
      } 
      if(info[2] ==0 && info[3]==0 && info[4]== 0 && info[5]==0) {
        strncat(mens,"\nNão tem paredes ao seu redor de momento\n",1000);
      }	

//prints sobre onde está
      if(info[0]==0) {
	strncat(mens,"O rato está no início do labirinto!\n",1000);
	}
      if(info[0]==3) {
        strncat(mens,"O rato está dentro do labirinto!\n",1000);
      }
      if(info[0] == 2){
        strncat(mens,"O rato está no fim do labirinto!\n",1000);
      }	

strncat(mens, "\n\n", 10); //linhas para não confundir mensagens



  return mens;
}
