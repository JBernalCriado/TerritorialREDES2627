#include "cliente.h"

struct Casilla tablero[5][5];

int main(){
    int sock;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    /*
    Función para abrir el socket
    afintet indica el grupo de ips a usar
    sockstream indica el tipo de socket a usar
    0 el protocolo con 0 es automático
    */
    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0){
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        printf("%s%sError al conectar con el servidor%s \n", BG_RED, BLACK, DEFAULT);
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("%s%sConectado al servidor %s:%d%s\n", BG_GREEN, WHITE, SERVER_IP, SERVER_PORT, DEFAULT);

    while (1)
    {
        buffer[0]='\0';
        printf("Opciones: \n USUARIO [usuario] \n PASSWORD [contraseña] \n REGISTRO  [usuario] [contraseña] \n INCIAR-PARTIDA \n ATACAR \n REFORZAR \n CONQUISTAR \n PASAR \n SALIR \n");
        fgets(message, sizeof(message), stdin);

        if (strncmp(message, "SALIR", 5) == 0){
        send(sock, message, strlen(message), 0);
        break;}
        send(sock, message, strlen(message), 0);

        int bytes_received = read(sock, buffer, sizeof(buffer) - 1) ;

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';


            if(isdigit(buffer[0])==0){
            printf("[USUARIO]: %s\n", buffer);
            }
            else{
                printf("Mostrando tablero\n");
                strcat(buffer, "]");
                reconstruirTablero(tablero ,buffer); //codigo a la carbonara
            }

            if (strncmp(buffer, "Esperando", 9) == 0)
            {
                while (1)
                {
                    bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
                    if (bytes_received > 0)
                    {
                        buffer[bytes_received] = '\0';
                        printf("[SERVER]: %s\n", buffer);
                        strcpy(buffer, quitarn(buffer));
                        if (strcmp(buffer, "+Ok.Empieza la partida.") == 0)
                            break;
                    }
                }
            }
        }
        
    }

    close(sock);
    printf("Desconectado del servidor.\n");


    return EXIT_SUCCESS;
}