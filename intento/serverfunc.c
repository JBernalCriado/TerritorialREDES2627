#include "server.h"
#include "funciones_juego.h"
// Nombres y contrasenas de usuarios
struct User users[100] = {
    {"alvaro", "alvaro"},
    {"juan", "juan"},
    {"pedro", "pedro"},
    {"maria", "maria"},
    {"lucia", "lucia"},
    {"carlos", "carlos"},
    {"ana", "ana"},
    {"luis", "luis"},
    {"sofia", "sofia"},
    {"javier", "javier"}};

// Struct de clientes conectados
struct Cliente clients[MAX_CLIENTS];

struct Partida partidas[MAX_PARTIDAS];

int registrarCliente(int socket){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].socket == 0){
            clients[i].socket = socket;
            clients[i].registered = 0;
            strcpy(clients[i].usuario, "");
            strcpy(clients[i].contrasena, "");
            printf("Cliente registrado en la posición %d (socket: %d)\n", i, socket);
            return 1;
        }
    }
    return 0;
}
/*
Comprueba que el cliente este ya en linea.
@paranm parametros, es el nombre introducido por el usuario
@return 1 si esta en linea, 0 si no esta en linea
*/
int esCliente(char parametros[50]){
    for(int i=0 ; i<MAX_CLIENTS ; i++){
        if(strcmp(clients[i].usuario,parametros)==0){return 1;}
    }
    return 0;
}
/*
Comprueba que el nombre introducido exista en la estructura de datos
@param parametros nombre introducido por usuario
@return 0 si no está en la estructura por lo que ha de registrarse
@return 1 está la ed, puede proceder 
*/
int esUsuario(char parametros[50]){
    for(int i=0 ; i<100 ; i++){
        if(strcmp(users[i].usuario,parametros)==0){return 1;}
    }
    return 0;
}

/*
Asigna un socket al usuario y lo guarda en el vector de clientes
@param socket id del socket recibido
@param paramtros cadena con el nombre del usuario
@return 1 si se asigno correctamente, 0 si algo salio mal
*/
int asignarUsuario(int socket, char parametros[50]){
    int i=0;
    while(clients[i].socket!=NULL){
        i++;
    }
    if(i<MAX_CLIENTS){
        clients[i].socket=socket;
        strcpy(clients[i].usuario, parametros);
        return 1;
    }
    return 0;
}
/*
Comprueba que el usuario está en la matriz de clientes para estar en ella
debe haber iniciado sesión
@param socket es el descriptor generado por los mensajes recibidos
@return 1 si el socket se encuentra en la matriz de clientes y está registrado, está autorizado a otras acciones
        0 si no está en la matriz de registrados y no podrá realizar otras acciones
*/
int comprobarCliente(int socket){
    for(int i=0 ; i<MAX_CLIENTS ; i++){
        if((clients[i].socket==socket ) && (clients[i].registered==1)){return 1;}
    }
    return 0;
}

/*
COmprueba que el usuario que hay en la ed de clientes haya escrito su verdadera contraseña
@param parametros  contraseña introducida
@param socket es la clave primari por la que vamos a buscar
@return 1 es que la contraseña introducida es correcta
 0 Si ha escrito una gilipollez que no coincide.
*/
int comprobarClave(char parametros[50], char usuario_actual[50]){
    int puntero_usuario;
    int puntero_clave;
    printf("Usuario actual: %s\n", usuario_actual);
    for (puntero_usuario = 0; puntero_usuario < MAX_CLIENTS; puntero_usuario++){
        if (strcmp(clients[puntero_usuario].usuario,usuario_actual)==0){
            printf("Usuario encontrado: %s\n", clients[puntero_usuario].usuario);
            break;
        }
    }
    for(puntero_clave=0 ; puntero_clave<100 ; puntero_clave++){
        if(strcmp(clients[puntero_usuario].usuario, users[puntero_clave].usuario)==0){
            if(strcmp(users[puntero_clave].contrasena,parametros)==0){
                strcpy(clients[puntero_usuario].contrasena, parametros);
                clients[puntero_usuario].registered=1;
                return 1;
            }  
        }
    }
        
    return 0;   
}


int comprobarNombre(char nombre[50]){
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (strcmp(clients[i].usuario, nombre) == 0 && clients[i].registered == 1)
        {
            return 1;
        }
    }
    return 0;
}

void listarClientes(){
    printf("%s%s", BG_YELLOW, BLACK);
    printf("\n=== Lista de clientes conectados ===\n");
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].socket != 0) // Solo muestra los que están en uso
        {

            printf("Posición %d:\n", i);
            printf("  Socket: %d\n", clients[i].socket);
            printf("  Introducido clave: %s\n", clients[i].registered ? "Sí" : "No");
            printf("  Usuario: %s\n", strlen(clients[i].usuario) > 0 ? clients[i].usuario : "(sin usuario)");
            printf("-----------------------------------\n");
            
        }
    }
    printf("=== Fin de la lista ===\n");
    printf("%s\n", DEFAULT);
}
/*Comprueba que haya un usuario con tal nombre*/
int buscarUsuario(char parameter[50]){
    
    for (int i = 0; i < 100; i++){
        if (strcmp(users[i].usuario, parameter) == 0){
            printf("Usuario %s encontrado.\n", parameter);
            return 1;
        }
    }
    printf("Usuario %s no encontrado.\n", parameter);
    return 0;
}



//introduce la contraseña del cliente y cambia el estado
int introducirContra(int socket, char *password){/*
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].socket == socket && buscarUsuario(clients[i].usuario, password) && 0 == strcmp(password, clients[i].contrasena)){
            printf("contraseña\n");
            strcpy(clients[i].contrasena, password);
            clients[i].registered = 1;
            return 1;
        }
    }*/
    return 0;
}   

int buscarEntreUsuario(char *usuario, char *password){
    if (usuario == NULL || password == NULL || strlen(password) == 0){return 0;}
    for (int i = 0; i < 100; i++){
        if ((strcmp(users[i].usuario, usuario) == 0) && (strcmp(users[i].contrasena, password) == 0)){return 1;}
    }
    return 0;   
}

int agregarUsuario(char *username, char *password){
    for (int i = 0; i < 100; i++){
        if (strcmp(users[i].usuario, "") == 0){
            strcpy(users[i].usuario, username);
            strcpy(users[i].contrasena, password);
            printf("Usuario %s añadido.\n", username);
            return 1;
        }
    }
    return 0;
}
/*Recorre todas las partidas para ver donde está X jugador*/
int getIdPartidaJugador(int socket){
    for(int i = 0; i < MAX_PARTIDAS; i++){
        if(partidas[i].estado!=2){continue;}
        if(partidas[i].jugadores[0]==socket || partidas[i].jugadores[1]==socket){
            printf("hola\n");
            return i;
        }
    }
    return -1;
}

int agregarUsuarioPartida(int socketjugador){
int esPartida = getIdPartidaJugador(socketjugador);
if (esPartida!=-1){
    printf("%s[DEBUG] Jugador %d aún estaba en la partida %d. Limpiando... %s\n", BG_RED, socketjugador, esPartida, DEFAULT);
    resetearPartida(&partidas[esPartida]);

}
for (int i = 0; i < MAX_PARTIDAS; i++){
        if (partidas[i].estado == 0){
            partidas[i].jugadores[0] = socketjugador;
            partidas[i].estado = 1;
            return 1;
        }
        else if (partidas[i].estado == 1){
            partidas[i].jugadores[1] = socketjugador;
            partidas[i].estado = 2;
            partidas[i].turno=0;
            CrearTablero(partidas[i].tablero);
            /*
            int fila1j=0, fila2j=0, columna1j=0, columna2j=0;
            
            for (int fila = 0; fila < 5; fila++){
                for (int columna = 0; columna < 5; columna++){
                    printf("[%d] [%d]", fila, columna);
                    if (partidas[i].tablero[fila][columna].jugador==1){
                        fila1j=fila;
                        columna1j=columna;
                    }
                    if (partidas[i].tablero[fila][columna].jugador==2){
                        fila2j=fila;
                        columna2j=columna;
                    }
                    char test[75];
                    sprintf(test,"%s test %d %s \n", BG_RED, columna, DEFAULT );
                    send(partidas[i].jugadores[1], test, strlen(test), 0);
                }
                
            }
            */

            char msg1[100];
            char msg2[100];

            
            sprintf(msg1, "+Ok.Empieza la partida.\n");

            send(partidas[i].jugadores[1], msg1, strlen(msg1), 0);
            send(partidas[i].jugadores[0], msg1, strlen(msg1), 0);
            mostrarTablero(partidas[i].tablero);
            char tablero[200];
            codificarTablero(partidas[i].tablero, tablero);

            sleep(1);
            write(partidas[i].jugadores[0], tablero, strlen(tablero));
            sleep(1);
            write(partidas[i].jugadores[1], tablero, strlen(tablero));

            
            printf("%s %sPARTIDA INICIADA \n %s \n", BG_GREEN,WHITE, DEFAULT);
            return 2;
        }
    }
    return 0;
}

int comprobarAccion(int socket, int accion){
        int partidaIndex = getIdPartidaJugador(socket);
    if (partidaIndex == -1){
        send(socket, "-Err. No estás en una partida.\n", 40, 0);
        return -1;
    }
        struct Partida *p = &partidas[partidaIndex];
    int jugador = getIndiceJugador(p, socket);
    
        if (p->estado != 2){
            send(socket, "-Err. La partida no está activa.\n", 40, 0);
            return -1;
        }

        if (p->turno != jugador){
            send(socket, "-Err. No es tu turno.\n", 40, 0);
            return -1;
        }
    return accionPermitida(partidas[partidaIndex].tablero, jugador, accion);
}

int enviarAtaque(int socket, char *coords){
        int partidaIndex = getIdPartidaJugador(socket);
    if (partidaIndex == -1){
        send(socket, "-Err. No estás en una partida.\n", 40, 0);
        return -1;
    }
        struct Partida *p = &partidas[partidaIndex];
    int jugador = getIndiceJugador(p, socket);
    
        if (p->estado != 2){
            send(socket, "-Err. La partida no está activa.\n", 40, 0);
            return -1;
        }

        if (p->turno != jugador){
            send(socket, "-Err. No es tu turno.\n", 40, 0);
            return -1;
        }
    ataque(partidas[partidaIndex].tablero, jugador, coords);
    partidas[partidaIndex].turno++;
    char tablero[200];
            codificarTablero(partidas[partidaIndex].tablero, tablero);

            sleep(1);
            send(partidas[partidaIndex].jugadores[1], tablero, strlen(tablero), 0);
            send(partidas[partidaIndex].jugadores[0], tablero, strlen(tablero), 0);
}


int getIndiceJugador(struct Partida *p, int socket){
    return (p->jugadores[0] == socket) ? 0 : 1;
}


void resetearPartida(struct Partida *p){
    memset(p, 0, sizeof(struct Partida));
}
/*Se encarga de llevar los turnos de la partida*/
void sumarTurno(int socket){
    int idpartida=getIdPartidaJugador(socket);
    partidas[idpartida].turno++;
    printf("id:%d turno:%d, jugador1:%d jugador2: %d, estado:%d", partidas[idpartida].id, partidas[idpartida].turno,partidas[idpartida].jugadores[0],partidas[idpartida].jugadores[1], partidas[idpartida].estado );
}


void procesarSalida(int socket)
{
    /*pid_t pid = getpid();
    kill(pid, 9);*/
}