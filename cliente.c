#include "cliente.h"

struct Casilla tablero[5][5];


int main(){
    int sock;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int jugador;
    bool esfin=false;
	socklen_t len_sockname;
    	fd_set readfds, auxfds;
    	int salida;
    	int fin = 0;
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock==-1){
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    
	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del 
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        printf("%s%sError al conectar con el servidor%s \n", BG_RED, BLACK, DEFAULT);
        close(sock);
        exit(EXIT_FAILURE);
    }

        //Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0,&readfds);
    FD_SET(sock,&readfds);

    printf("%s%sConectado al servidor %s:%d%s\n", BG_GREEN, WHITE, SERVER_IP, SERVER_PORT, DEFAULT);
    
    do
	{
        auxfds = readfds;
        salida = select(sock+1,&auxfds,NULL,NULL,NULL);
        buffer[0]='\0';
        int bytes_received=0;

        //Tengo mensaje desde el servidor
        if(FD_ISSET(sock, &auxfds)){
            bzero(buffer,sizeof(buffer));
            recv(sock,buffer,sizeof(buffer),0);
            
            printf("\n%s\n",buffer);
            strcpy(buffer, quitarn(buffer));
            if(isdigit(buffer[0])!=0){
                reconstruirTablero(tablero, buffer);
            }
            if(strcmp(buffer, "+Ok. Esperando a otro jugador...")==0){
                while(1){
                    sleep(1);
                    bytes_received = recv(sock, buffer, sizeof(buffer)-1, 0);
                    if(bytes_received>0){
                        strcpy(message, quitarn(message));
                        printf("[SERVER]: %s\n", buffer);
                        if(strcmp(buffer, "+Ok. Empieza la partida. (Pulsa enter)") == 0){
                            break;
                        }
                    }
                }
            }
            if(strcmp(buffer, "+Ok. Buena tirada de cable.")==0){
                printf("10 segundos de ban por tirar del cable\n");
                sleep(10);
                printf("Desconectando del servidor\n");
                fin=true;
            }
            if(strcmp(buffer, "+Ok. El oponente ha terminado la partida.")==0 || strcmp(buffer, "+Ok. Chau.")==0){
                printf("Desconectando del servidor\n");
                fin=true;
            }
            else{
                continue;
            }
        }
        else
        {
            printf("%s%s\n Opciones:\n\tUSUARIO [usuario]\n\tPASSWORD [contraseña]\n\tREGISTRO [usuario] [contraseña]%s\n\tINICIAR-PARTIDA%s%s\n\tATACAR [coordenada origen] [coordenada destino] [numero de soldados]\n\tREFORZAR [coordenada origen] [coordenada destino] [numero de soldados]\n\tCONQUISTAR [coordenada origen] [coordenada destino] [numero de soldados]\n\tPASAR%s\n\tSALIR%s\n\t\tEscribe aqui: ", RESHIRAM,BLACK,BG_GREEN,WHITE,BG_CYAN, BG_RED, DEFAULT);
            limpiarBuffer();
            fgets(message, sizeof(message), stdin);
            strcpy(message, quitarn(message));
            if(strcmp(message,"\0")==0){
                printf("Comando vacio\n");
                continue;
            }
            send(sock, message, strlen(message), 0);
        }
        
				
    }while(fin==false);
		
    close(sock);

    exit(EXIT_SUCCESS);
		
}



