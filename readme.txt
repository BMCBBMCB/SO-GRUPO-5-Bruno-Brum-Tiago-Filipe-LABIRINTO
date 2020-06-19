//Explicação geral do programa
É percorrido um labirinto que é carregado a partir de um ficheiro txt usando comandos de movimento das teclas "w" "a" "s" "d" (maiúscula ou minúscula).
É colocado o rato no início do labirinto e, através dos comandos, feita a tentativa de chegar ao fim.
O labirinto é carregado num terminal onde será carregado o servidor e é a parte visual.
O movimento é feito a partir do terminal onde será carregado o cliente e é a parte onde serão apresentadas as mensagens informativas que iremos mencionar mais à frente.
O cursor é inicializado na posição inicial marcado por um "S" e uando o cursor chegar a parte final, marcada num labirinto com "G", é mostrada uma mensagem que o cliente ganhou no servidor.
O cliente pode escolher 3 modos:
-Normal, onde "W" fará o cursor andar para cima no labirinto, "A" para a esquerda, "D" para a direita e "S" para trás.
-3 teclas, onde "w" fará o cursor andar na direção que está apontado. "a" que fará o cursor virar 90º para a esquerda e "d" que fará o mesmo que "a" mas para a direita.
-automático, onde o cursor irá seguir um caminho estabelecido especifico de forma autonoma


//Inicialização
Para correr o programa é necessário fazer a compilação do seguinte modo:
gcc -pthread  -o server_datagram server_datagram.c moving_in_screen.c reading_a_file.c mythreads.c 
gcc -pthread  -o client_datagram client_datagram.c moving_in_screen.c reading_a_file.c mythreads.c


//Correr o programa
Depois de feita a compilação, abre-se 2 terminais no local do programa e:
Corre-se num o servidor em full-screen com -> ./server_datagram
Corre-se no outro o cliente com -> ./client_datagram 127.0.0.1 5050 //sendo o 127.0.0.1 o endereço que conecta com o servidor e 5050 a porta


//Durante programa
Após ter escolhido o modo de jogo, qualquer movimento irá mostrar informação no lado do cliente sobre as paredes do labirinto.
Isto é, se houver uma parede a rodear, irá aparecer uma mensagem no lado do cliente a informar sobre isto ou uma a informar que não existem paredes no redor do cursor na posição atual no labirinto.
Também será informada a posição, se está no início ou fim do labirinto.
No caso de estar no modo de 3 teclas, também é informada a direção que o cursor aponta(norte,sul,este,oste).


//Extras
Ao acabar o jogo, pode recomeçar em outro modo ou o mesmo se desejar
Clicar no "4" durante a execução do programa serve para fechar ambos o servidor e cliente.
