#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "moving.h"
#include "reading.h"

void main(int argc, char* argv[])
{
  int sock;
  char mensagem[50];
  char mens[1024]; //mensagem que o cliente irá receber com informações sobre cursor no labirinto

  struct sockaddr_in server;
  struct addrinfo *hp,hints;

  //struct hostent *hp, *gethostbyname();
  /* Create socket on which to send. */
  sock = socket(AF_INET,SOCK_DGRAM, 0);
  if (sock == -1) {
    perror("opening datagram socket");
    exit(1);
  }

  /*
  * Construct name, with no wildcards, of the socket to ‘‘send’’
  * to. gethostbyname returns a structure including the network
  * address of the specified host. The port number is taken from
  * the command line.
  */
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  int res = getaddrinfo(argv[1],argv[2],&hints,&hp);
  if (res != 0) {
    fprintf(stderr, "%s: getting address error\n", argv[1]);
    exit(2);
  }

  int len = sizeof(server);
  int esc;
  bool y = false;
  while(y == false){
    printf("Escolha quais os modos que pretende jogar:\n1-Modo Normal\n2-Modo 3 Teclas\n3-Rato Autônomo\n4-Sair\n\n");
    cbreak(); //Colocar o terminal no modo cbreak ou rare
    char t = getchar(); 
    while (!(53>t && t>48)) { //se char for dirente das opções irá ser feito um loop até que seja inserida uma opção válida
        printf("\n\nLamento mas nao pode\nEscolha quais os modos que pretende jogar:\n1-Modo Normal\n2-Modo 3 Teclas\n3-Rato Autônomo\n4-Sair\n\n");
        t = getchar();
    }

    nocbreak(); //Por o terminal de volta no modo cooked
	//é posto o valor de esc igual à opção selecionada
    if(t==49){ //se igual a 1
        esc = 1; 
        sendto(sock,"1", sizeof "1" ,0, hp->ai_addr,hp->ai_addrlen);
    }
    else if(t==50){ //se igual a 2
        esc = 2;
        sendto(sock,"2", sizeof "2" ,0, hp->ai_addr,hp->ai_addrlen);
    }
    else if(t==51){ //se igual a 3
        esc = 3;
        sendto(sock,"3", sizeof "3" ,0, hp->ai_addr,hp->ai_addrlen);
    }
    else if(t==52){ //se igual a 4
        sendto(sock,"4", sizeof "4" ,0, hp->ai_addr,hp->ai_addrlen);
        y = true;  //coloca a repetiçaõ em cima inválida, acabando o jogo
    }

    bool x = false;
    if(esc == 1){ //se for escolhido modo 1 de 4 teclas
      while(x == false){
        cbreak(); //Colocar o terminal no modo cbreak ou rare
        char t = getchar();

	//mandadas sockets com a tecla que se tocou através dos ascii codes
        if(t==97){ 
            sendto(sock,"a", sizeof "a" ,0, hp->ai_addr,hp->ai_addrlen);
        }
        else if(t==115){
            sendto(sock,"s", sizeof "s" ,0, hp->ai_addr,hp->ai_addrlen);
        }
        else if(t==100){
            sendto(sock,"d", sizeof "d" ,0, hp->ai_addr,hp->ai_addrlen);
        }
        else if(t==119){
            sendto(sock,"w", sizeof "w" ,0, hp->ai_addr,hp->ai_addrlen);

        }

        recvfrom(sock, mens, 1024, 0, (struct sockaddr*)&server, &len); //recebe mensagem do servidor com informações do labirinto
        if(strstr(mens,"O rato está no fim do labirinto")){
          x = true;
        }
        nocbreak(); //Por o terminal de volta no modo cooked
        printf("%s", mens); //mostrar mensagem com informações do labirinto na posição atual


      }
    }
    else if(esc == 2){ //se for escolhido modo 2 de 3 teclas
      while(x == false){
        cbreak(); //Colocar o terminal no modo cbreak ou rare
        char t = getchar();
        if(t==97){
            sendto(sock,"a", sizeof "a" ,0, hp->ai_addr,hp->ai_addrlen);
        }
        else if(t==115){
            sendto(sock,"s", sizeof "s" ,0, hp->ai_addr,hp->ai_addrlen);
        }
        else if(t==100){
            sendto(sock,"d", sizeof "d" ,0, hp->ai_addr,hp->ai_addrlen);
        }
        else if(t==119){
            sendto(sock,"w", sizeof "w" ,0, hp->ai_addr,hp->ai_addrlen);
        }

        nocbreak(); //Por o terminal de volta no modo cooked
        recvfrom(sock, mens, 1024, 0, (struct sockaddr*)&server, &len);//recebe mensagem do servidor com informações do labirinto
        if(strstr(mens,"O rato está no fim do labirinto")){
          x = true;
        }
        printf("%s", mens); //mostrar mensagem com informações do labirinto na posição atual

      }
    }
    else if(esc == 3){ //se for escolhido modo 3 automatico
      //a string com os comandos todos para se chegar ao fim do labirinto
      char * input = "wwwwwwwwwwwwwwwwwwwwwwwwddddddddddddddddddddwwddddssddddddddssddddwwddddddddssddddddddssaaaassaaaawwaaaassaaaawwaaaassaaaawwaaaaaaaaaaaaaaaaaaaaaaaassdddddddddddddddddddssaaaaaaaaaaaaaaaaaassddddddddddddddddddddssddddddddddwwaaa";
      
      size_t length = strlen(input);
      int index = 0;
      for(index; index < length; index++){ //percorrer a string char por char
        cbreak(); //Colocar o terminal no modo cbreak ou rare
        sendto(sock, &input[index], sizeof input[index], 0, hp->ai_addr,hp->ai_addrlen);
        nocbreak(); //Por o terminal de volta no modo cooked
        sleep(1);
      }
    }
  }

  close(sock);
  exit(0);
}
