#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include "moving.h"
#include "reading.h"
#include "mythreads.h"

pthread_t prod, cons;
pthread_cond_t empty, fill;
pthread_mutex_t mutex;
int sockr, length, len, escolha;
bool cima, dir, esq, baixo; //para saber onde o cursor aponta
struct sockaddr_in server;
char buf[1024];
char nbuf[1024];
char buff[1];                           //fila que é partilhada com producer e consumer com tamanho "1", porque as mensagens que o servidor vai receber, será apenas de um caracter.
int count = 0;
bool x;
char **fich;
int **maze;
int *p;
int info[6]; //onde se guardará os valores sobre o que rodeia o rato, para onde está virado e onde está(fim/inicio/nenhum)
	//info[0] Posição se 0->inicio se 1->sítio dentro do labirinto se 2->final
	//info[1] Direção se 0->Norte se 1->Este se 2->Oeste se 3->Sul
	//info[2] Parede em cima -> 1 sim   0 não
	//info[3] Parede à direita -> 1 sim   0 não
	//info[4] Parede à esquerda -> 1 sim   0 não
	//info[5] Parede em baixo -> 1 sim   0 não

void *producer(void *arg){
  pthread_mutex_lock(&mutex);
  while(count == 1){
    pthread_cond_wait(&empty, &mutex);
  }
  recvfrom(sockr, buf, 1024, 0, (struct sockaddr*)&server, &len);    //producer recebe mensagem a partir de socket.
  strcpy(buff,buf);                                                  //producer guarda a informação recebida por socket numa buffer que é partilhada com consumer -> buff.
  count++;                                                     //adiciona 1 ao "count" para informar no consumer que o "buff" está cheio
  pthread_cond_signal(&fill);                                   //envia sinal que buff está cheio
  pthread_mutex_unlock(&mutex);
}

void *consumer(void *arg){
  pthread_mutex_lock(&mutex);
  while(count == 0){
    pthread_cond_wait(&fill, &mutex);
  }
    //inicializar informacao sobre info tudo a 0 menos onde está em termos de posição
    info[1] = 0;
    info[2] = 0;
    info[3] = 0;
    info[4] = 0;
    info[5] = 0;

  if(escolha==0){
    cbreak();
    char *mens  = mover(maze, buf, p, info); //recebe mensagem com informações sobre labirinto dependendo da posição do cursor
    nocbreak();
    strcpy(buff,"");
  }
  else if(escolha==1){
    cbreak();
    char *mens  = mover(maze, buf, p, info); //recebe mensagem com informações sobre labirinto dependendo da posição do cursor
    nocbreak();
    strcpy(buff,"");
    sendto(sockr, mens, 1024 ,0, (struct sockaddr*)&server, len); //manda-as para o cliente
    if(strstr(mens,"O rato está no fim do labirinto")){ //se na mensagem estiver presente esta frase o cursor chegou ao fim, tendo que se mudar o valor de x 
	    x = true;
    }
  }
  else if(escolha==3){
    cbreak();
    char *mens  = mover(maze, buf, p, info);
    nocbreak();
    if(strstr(mens,"O rato está no fim do labirinto")){
	    x = true;
    }
    strcpy(buff,"");
  }
  else if(escolha==2){ //se escolha é o modo 3 teclas, é mais complicado
      if(strcmp("w",buff) == 0){  //só se for "w" existe o movimento neste modo
	      char *mess = (char*) malloc(1024*sizeof(char));
        if(cima==true){
          cbreak();
          char *mens = mover(maze, "w", p, info);
	        char *mess1 = "\nDireção : Norte\n"; //se cima==true então adiciona informação que rato aponta para norte à mensagem
	        strncat(mess,mess1,1000);
          strncat(mess,mens,1000); //adicionar a menss a informação sobre direcao e todas as informacoes da mens
          sendto(sockr, mess, 1024 ,0, (struct sockaddr*)&server, len);
          nocbreak();
        }
	//outros casos
        else if(dir==true){
          cbreak();
          char *mens = mover(maze, "d", p, info);
          char *mess1 = "\nDireção : Este\n";
	        strncat(mess,mess1,1000);
          strncat(mess,mens,1000);
          sendto(sockr, mess, 1024 ,0, (struct sockaddr*)&server, len);
          nocbreak();
        }
        else if(baixo==true){
          cbreak();
          char *mens = mover(maze, "s", p, info);
          char *mess1 = "\nDireção : Sul\n";
	        strncat(mess,mess1,1000);
          strncat(mess,mens,1000);
          sendto(sockr, mess, 1024 ,0, (struct sockaddr*)&server, len);
          nocbreak();
        }
        else if(esq==true){
          cbreak();
          char *mens = mover(maze, "a", p, info);
          char *mess1 = "\nDireção : Oeste\n";
	        strncat(mess,mess1,1000);
          strncat(mess,mens,1000);
          sendto(sockr, mess, 1024 ,0, (struct sockaddr*)&server, len);
          nocbreak();
        }
        if(strstr(mess,"O rato está no fim do labirinto")){
          x = true;
        }
      }

      //abaixo estão todas as condições para verificar a parte de virar a direção
      else if((strcmp(buff,"d")==0)&&(cima == true)){
        cima = false;
        dir = true;
	      sendto(sockr, "\nDireção : Este\n", 1024 ,0, (struct sockaddr*)&server, len);
      }
      else if((strcmp(buff,"d")==0)&&(dir == true)){
        dir = false;
        baixo = true;
	      sendto(sockr, "\nDireção : Sul\n", 1024 ,0, (struct sockaddr*)&server, len);
      }
      else if((strcmp(buff,"d")==0)&&(baixo == true)){
        baixo = false;
        esq = true;
	      sendto(sockr, "\nDireção : Oeste\n", 1024 ,0, (struct sockaddr*)&server, len);
      }
      else if((strcmp(buff,"d")==0)&&(esq == true)){
        esq = false;
        cima = true;
	      sendto(sockr, "\nDireção : Norte\n", 1024 ,0, (struct sockaddr*)&server, len);
      }
      else if((strcmp(buff,"a")==0)&&(cima == true)){
        cima = false;
        esq = true;
	      sendto(sockr, "\nDireção : Oeste\n", 1024 ,0, (struct sockaddr*)&server, len);
      }
      else if((strcmp(buff,"a")==0)&&(dir == true)){
        dir = false;
        cima = true;
	      sendto(sockr, "\nDireção : Norte\n", 1024 ,0, (struct sockaddr*)&server, len);
      }
      else if((strcmp(buff,"a")==0)&&(baixo == true)){
        baixo = false;
        dir = true;
	      sendto(sockr, "\nDireção : Este\n", 1024 ,0, (struct sockaddr*)&server, len);
      }
      else if((strcmp(buff,"a")==0)&&(esq == true)){
        esq = false;
        baixo = true;
      	sendto(sockr, "\nDireção : Sul\n", 1024 ,0, (struct sockaddr*)&server, len);
      }
  }
  count--;                                            //subtrai 1 ao "count" para informar no producer que o "buff" está vazio
  pthread_cond_signal(&empty);                          //envia sinal que buff está vazio
  pthread_mutex_unlock(&mutex);
}


void main(){
  /* Create socket from which to read. */
  sockr = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockr == -1) {
    perror("opening datagram socket");
    exit(1);
  }


  /* Create name with wildcards. */
  server.sin_family = AF_INET;
  //name.sin_addr.s_addr = INADDR_ANY;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  //name.sin_port = 0;
  server.sin_port = htons(5050);


  if (bind(sockr,(struct sockaddr *)&server, sizeof server) == -1) {
    perror("binding datagram socket");
    exit(1);
  }

  /* Find assigned port value and print it out. */
  length = sizeof(server);
  len = sizeof(server);
  printf("Socket port #%d\n", ntohs( server.sin_port));
  bool y = false;
  while(y == false){
	  escolha = 0;
    	  x = false;
	  printf("\n\nEscolha um modo de jogo \n");
	  recvfrom(sockr, buff, 1024, 0, (struct sockaddr*)&server, &len);
	  if(strcmp("1",buff)==0){
	    escolha = 1;
	  }
	  else if(strcmp("2",buff)==0){
	    escolha = 2;
	    cima = true;
	    baixo, esq, dir = false;
	  }
	  else if(strcmp("3",buff)==0){
	    escolha = 3;
	  }
           else if(strcmp("4",buff)==0){ //caso de acabar jogo
	    y=true;
            x=true;
            clearscreen();
	  }

    if(x==false){ //caso de estar no fim
          clearscreen();
  	  maze = ler(0, fich);
  	  p = mapa(maze);
    }


	  while(!x){
	      pthread_create(&prod, NULL, producer, NULL);
	      pthread_create(&cons, NULL, consumer, NULL);
	      pthread_join(prod, NULL);
	      pthread_join(cons, NULL);

	  }
  }


  exit(127); /* only if execv fails */
  close(sockr);
  exit(0);
}
