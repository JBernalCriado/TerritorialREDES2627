#include "server.h"
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

/*Inicializa los ids de las partidas a -1
*/
void iniciarPartidas(){
    for(int i=0 ; i<MAX_PARTIDAS ; i++){
        partidas[i].id = -1;
    }
}

/*Establece las posiciones iniciales en el tablero
@param tablero Matriz donde se guarda el tablero de la partida
*/
void crearTablero(struct Casilla tablero[5][5]){
/*Se encarga de inicializar el tablero*/

    for(int i=0 ; i<5 ; i++){ //Inicializa el tablero con casillas vacias
        for(int j=0 ; j<5 ; j++){
            tablero[i][j].jugador = 0;
            tablero[i][j].soldados = 0;
        }
    }

    //Inicializacion de la partida
        //Casilla inicial del jugador 1
        int x = numeroAleatorio();
        int y = numeroAleatorio();
        tablero[x][y].jugador = 1;
        tablero[x][y].soldados = 30;
        printf("x: %d, y: %d\n", x, y);
        
        //Casilla inicial del jugador 2
        while(1){
            x = numeroAleatorio();
            y = numeroAleatorio();
            if(tablero[x][y].jugador == 0){ //Para no solapar las casillas iniciales
                break;
            }
        }
        tablero[x][y].jugador = 2;
        tablero[x][y].soldados = 30;
        printf("x: %d, y: %d\n", x, y);
}

/*Registra un usuario en la lista de clientes
@param socket id del socket del cliente que se va a añadir
*/
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
        if((clients[i].socket==socket && strlen(clients[i].usuario)!=0) && (clients[i].registered==1)){return 1;}
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
int comprobarClave(char parametros[50], int socket){
    int puntero_usuario;
    int puntero_clave;
    for (puntero_usuario = 0; puntero_usuario < MAX_CLIENTS; puntero_usuario++){
        if (clients[puntero_usuario].socket == socket && strlen(clients[puntero_usuario].usuario)!=0){
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
    clients[puntero_usuario].usuario[0]='\0';
    clients[puntero_usuario].registered=0;
    return 0;   
}

/*Lista por terminal los clientes*/
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
 
/*Registra un usuario en la lista de usuarios
@param username nombre del nuevo usuario
@param password contraseña del nuevo usuario
@return 1 si se añade correctamente
        0 si no hay partidas disponibles
*/
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

/*Mete a un jugador en la lista de partidas
@param socketjugador id del socket del jugador que va a entrar en partida
@return -1 si el jugador ya esta en una partida
        0 si no hay partidas disponibles
        1 si se ha podido añadir
*/
int agregarUsuarioPartida(int socketjugador){
    int i=0;
    for(i ; i<MAX_PARTIDAS ; i++){
        if(partidas[i].jugadores[0] == socketjugador || partidas[i].jugadores[1]==socketjugador){
            return -1;
        }
    }
    i=0;
    while(partidas[i].id != -1 && i<MAX_PARTIDAS){
        i++;
    }
    if(partidas[i].estado==0){
        partidas[i].jugadores[0]=socketjugador;
        partidas[i].estado=1;
        return 1;
    }
    else if(partidas[i].estado==1){
        partidas[i].jugadores[1]=socketjugador;
        partidas[i].id = i;
        partidas[i].estado = 2;
        partidas[i].turno = 0;

        printf("[PARTIDA] Indice:%i, ID:%i Jugador 1: %i, Jugador 2: %d\n",i, partidas[i].id, partidas[i].jugadores[0], partidas[i].jugadores[1]);
        crearTablero(partidas[i].tablero);

        char webos[45] = "+Ok. Empieza la partida. (Pulsa enter)";
        send(partidas[i].jugadores[0], webos, sizeof(webos), 0);
        send(partidas[i].jugadores[1], webos, sizeof(webos), 0);

        enviarTableroAJugadores(partidas[i].tablero, i);
        return 1;
    }

    return 0;
    
}

/*Manda el codigo del tablero a los dos jugadores de una partida
@param tablero Matriz del tablero de juego
@param indicePartida indice de la partida del tablero que se quiere usar
*/
void enviarTableroAJugadores(struct Casilla tablero[5][5], int indicePartida){
    char msg[200]="\0";
    char msg1[200];
    
    codificarTablero(tablero, msg);
    
    strcpy(msg1, msg);
    strcat(msg1, "1");
    send(partidas[indicePartida].jugadores[0], msg1, sizeof(msg1), 0);
    
    strcpy(msg1, msg);
    strcat(msg1, "2");
    send(partidas[indicePartida].jugadores[1], msg1, sizeof(msg1), 0);
}

/*Codifica la matriz tablero en una cadena de caracteres
@param tablero Matriz del tablero de juego
@param mensaje String que guardara la codificacion
*/
void codificarTablero(struct Casilla tablero[5][5], char mensaje[200]){
    mostrarTablero(tablero);
    int cursor = 0;
    char zirtam[200];
    char sold[3];
    for(int fila = 0; fila<5 ; fila++){
        for(int col = 0 ; col<5 ; col++){
            if(tablero[fila][col].jugador == 0){
              zirtam[cursor] = '0';
              cursor++;
            }
            if(tablero[fila][col].jugador == 1){
                zirtam[cursor] = '1';
                
                if(tablero[fila][col].soldados < 10){
                    snprintf(sold, sizeof(sold), "%i", tablero[fila][col].soldados);
                    cursor++;
                    zirtam[cursor] = '0';
                    zirtam[cursor+1] = sold[0];

                }
                else{   
                    snprintf(sold, sizeof(sold), "%i", tablero[fila][col].soldados);
                    cursor++;
                    zirtam[cursor] = sold[0];
                    zirtam[cursor+1] = sold[1];
                }
                cursor+=2;                
            }
            if(tablero[fila][col].jugador == 2){
                zirtam[cursor] = '2';
                if(tablero[fila][col].soldados < 10){
                    snprintf(sold, sizeof(sold), "%i", tablero[fila][col].soldados);
                    cursor++;
                    zirtam[cursor] = '0';
                    zirtam[cursor+1] = sold[0];

                }
                else{

                    snprintf(sold, sizeof(sold), "%i", tablero[fila][col].soldados);
                    cursor++;
                    zirtam[cursor] = sold[0];
                    zirtam[cursor+1] = sold[1];
                }
                cursor+=2; 
            }   
        }
    }
    zirtam[strlen(zirtam)-1] = ']';
    int index=0;
    for(int i=0 ; i<strlen(zirtam)-1 ; i++){
        if(isdigit(zirtam[i]) != 0 || zirtam[i]==']'){
            mensaje[index]=zirtam[i];
            index++;
        }
    }
    strcat(mensaje, "]");
    printf("%s\n", mensaje);
}

/*Transforma la codificacion de "codificarTablero" de vuelta en una matriz
@param tablero Matriz del tablero de juego ya descodificada
@param mensaje String con la codificacion
*/
void reconstruirTablero(struct Casilla tablero[5][5], char mensaje[200]){
    int fila=0, col=0;
    int cursor=0;
    int jugador;

    if(mensaje[strlen(mensaje)-1] == '1'){
        jugador=1;
    }
    else{
        jugador=2;
    }
    while(mensaje[cursor] != ']'){
        if(strlen(mensaje)==0){
            printf("Cadena vacia\n");
            break;
        }
        if(mensaje[cursor] == '0'){
            tablero[fila][col].jugador = 0;
            tablero[fila][col].soldados = 0;
            col++;
            cursor++;
            continue;
        }
        if(mensaje[cursor] == '1' || mensaje[cursor] == '2'){
            tablero[fila][col].jugador = mensaje[cursor] - '0';
            tablero[fila][col].soldados = (mensaje[cursor+1] - '0') *10 + (mensaje[cursor+2] - '0');
            cursor+=3;
            col++;
            continue;
        }
        if(mensaje[cursor] == ' '){
            cursor++;
            continue;
        }
        if(col > 4){
            col=0;
            fila++;
            cursor++;
            char a[10];
            scanf("%s", a);
            continue;
        }
        if(mensaje[cursor] == ']'){
            break;
        }

    }
    mostrarTableroClientes(tablero,jugador);
}

/*Elimina el caracter del salto de linea al final de la cadena si existe
  @param cadena Cadena de caracteres a modificar
  @return Puntero a la cadena modificada
*/
char* quitarn(char* cadena){

    size_t longitud = strlen(cadena);
    if (longitud > 0 && cadena[longitud - 1] == '\n') {
        cadena[longitud - 1] = '\0';
    }
    return cadena;
}

/*Genera el numero aleatorio para las casillas iniciales
  @return Numero aleatorio entre 0 y 4
*/
int numeroAleatorio(){
   return rand() % 5;
}


/*Muestra el tablero de juego con los jugadores y sus soldados
  @param tablero Matriz del mapa de juego
*/
void mostrarTablero(struct Casilla tablero[5][5]){
    printf("\n");
    for(int i=-1 ; i<5 ; i++){
        for(int j=-1 ; j<5 ; j++){
            if(i == -1 && j == -1){
                printf("   ");
            }
            else if(i == -1){
                printf("  %i ", i + j + 2);
            }
            else if(j == -1){
                printf(" %c ", 'A' + i);
            }
            else{
            switch (tablero[i][j].jugador){
            case 0:
                if(i%2==0){
                    if(j%2!=0){printf("%s%s(30)%s",WHITE, BG_WHITE,DEFAULT);}
                    else{printf("%s%s(30)%s",GRAY, BG_GRAY,DEFAULT);}
                }
                if(i%2!=0){
                    if(j%2!=0){printf("%s%s(30)%s",GRAY, BG_GRAY,DEFAULT);}
                    else{printf("%s%s(30)%s",WHITE, BG_WHITE,DEFAULT);}
                }
                break;
            case 1:
                    if(tablero[i][j].soldados<10){printf("%s  %i %s",BG_RED, tablero[i][j].soldados,DEFAULT);}
                    else{printf("%s %i %s",BG_RED, tablero[i][j].soldados,DEFAULT);}
            break;
            case 2:
                    if(tablero[i][j].soldados<10){printf("%s  %i %s",BG_BLUE, tablero[i][j].soldados,DEFAULT);}
                    else{printf("%s %i %s",BG_BLUE, tablero[i][j].soldados,DEFAULT);}
                break;
            }  }
        }
        printf("\n");
    }
    printf("\n");
}

/*Modificacion de "mostrarTablero" para los clientes
@param tablero Matriz con el tablero de juego
@param jugador id del jugador al que se le va a mostrar
*/
void mostrarTableroClientes(struct Casilla tablero[5][5], int jugador){
    printf("\n");
    for(int i=-1 ; i<5 ; i++){
        for(int j=-1 ; j<5 ; j++){
            if(i == -1 && j == -1){
                printf("   ");
            }
            else if(i == -1){printf("  %i ", i + j + 2);}
            else if(j == -1){printf(" %c ", 'A' + i);}
            else{
            switch (tablero[i][j].jugador){
            case 0:
                if(i%2==0){
                    if(j%2!=0){printf("%s%s(30)%s",WHITE, BG_WHITE,DEFAULT);}
                    else{printf("%s%s(30)%s",GRAY, BG_GRAY,DEFAULT);}
                }
                if(i%2!=0){
                    if(j%2!=0){printf("%s%s(30)%s",GRAY, BG_GRAY,DEFAULT);}
                    else{printf("%s%s(30)%s",WHITE, BG_WHITE,DEFAULT);}
                }
                break;
            case 1:
                if(jugador==1){
                    if(tablero[i][j].soldados<10){printf("%s  %i %s",BG_RED, tablero[i][j].soldados,DEFAULT);}
                    else{printf("%s %i %s",BG_RED, tablero[i][j].soldados,DEFAULT);}
                }else{printf("%s%s 30 %s",BG_RED, RED, DEFAULT);}
            break;
            case 2:
                if(jugador==2){
                    if(tablero[i][j].soldados<10){printf("%s  %i %s",BG_BLUE, tablero[i][j].soldados,DEFAULT);}
                    else{printf("%s %i %s",BG_BLUE, tablero[i][j].soldados,DEFAULT);}
                }else{printf("%s%s 30 %s",BG_BLUE,BLUE,DEFAULT);}
                break;
            }            
        }
            
 
        }
        printf("\n");
    }
    printf("\n");
}
/*
Función encargada de comprobar que la acción solicitada por el jugador sea realizable por el mismo
@param socket ya se sabe a estas alturas
@param accion indicada por el jugador se selecciona como un valor arbitrario
@return -1 no es su turno // 0 es posible // 1 es su turno pero no es posible
*/
int comprobarAccion(int socket, int accion){
    int indicePartida = getIndicePartida(socket);
    int jugador = getIdJugador(socket, indicePartida);
    //Ya que el turno al inicar partida es 0 si es par le toca al jugador 1 y si es impar al 2
    printf("[PARTIDA] Indice:%i, ID:%i Jugador 1: %i, Jugador 2: %d\n",indicePartida, partidas[indicePartida].id, partidas[indicePartida].jugadores[0], partidas[indicePartida].jugadores[1]);
    printf("Socket: %i, Jugador: %d\n", socket, jugador);
    if(indicePartida == -1){return 2;}
    if(partidas[indicePartida].turno%2 == 0){
        if(jugador==2){return -1;}
    }
    else{if(jugador==1){return -1;}}

    int contrario = jugador == 1 ? 2:1;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(partidas[indicePartida].tablero[i][j].jugador != jugador)
                continue;

            if((i > 0 && ((accion == 1 && partidas[indicePartida].tablero[i - 1][j].jugador == contrario) ||
                          (accion == 2 && partidas[indicePartida].tablero[i - 1][j].jugador == jugador) ||
                          (accion == 3 && partidas[indicePartida].tablero[i - 1][j].jugador == 0))) ||
               (i < 4 && ((accion == 1 && partidas[indicePartida].tablero[i + 1][j].jugador == contrario) ||
                          (accion == 2 && partidas[indicePartida].tablero[i + 1][j].jugador == jugador) ||
                          (accion == 3 && partidas[indicePartida].tablero[i + 1][j].jugador == 0))) ||
               (j > 0 && ((accion == 1 && partidas[indicePartida].tablero[i][j - 1].jugador == contrario) ||
                          (accion == 2 && partidas[indicePartida].tablero[i][j - 1].jugador == jugador) ||
                          (accion == 3 && partidas[indicePartida].tablero[i][j - 1].jugador == 0))) ||
               (j < 4 && ((accion == 1 && partidas[indicePartida].tablero[i][j + 1].jugador == contrario) ||
                          (accion == 2 && partidas[indicePartida].tablero[i][j + 1].jugador == jugador) ||
                          (accion == 3 && partidas[indicePartida].tablero[i][j + 1].jugador == 0))))
                return 0;
        }
    }
    return 1;
}


int Conquista(int socket, char coordenadas[50]){
    int indicePartida = getIndicePartida(socket);
    int jugador = getIdJugador(socket, indicePartida);

    int origen[2], destino[2], soldados;
    char* color = jugador == 1 ? "\033[1;31m" : "\033[1;34m";

    origen[0] = origen[1] = destino[0] = destino[1] = -1;
    soldados = -1;
    traductorCoordenadasC(coordenadas, origen, destino, &soldados);

    if (origen[0] < 0 || origen[0] >= 5 || origen[1] < 0 || origen[1] >= 5 ||
        destino[0] < 0 || destino[0] >= 5 || destino[1] < 0 || destino[1] >= 5 ||
        soldados <= 0 ||
        partidas[indicePartida].tablero[origen[0]][origen[1]].jugador != jugador ||
        partidas[indicePartida].tablero[destino[0]][destino[1]].jugador != 0 ||
        partidas[indicePartida].tablero[origen[0]][origen[1]].soldados < soldados ||
        abs(origen[0] - destino[0]) + abs(origen[1] - destino[1]) != 1) {
        printf("Conquista no permitida: debes mandar soldados de una casilla tuya a una vacia adyacente.\n");
        return 0;
    }

    partidas[indicePartida].tablero[destino[0]][destino[1]].jugador = jugador;
    partidas[indicePartida].tablero[destino[0]][destino[1]].soldados = soldados;
    partidas[indicePartida].tablero[origen[0]][origen[1]].soldados -= soldados;
    if (partidas[indicePartida].tablero[origen[0]][origen[1]].soldados == 0) {
        partidas[indicePartida].tablero[origen[0]][origen[1]].jugador = 0;
    }
    printf("Conquistaste la casilla y mandaste %i soldados\n", soldados);
    return 1;
}
/*
Funcion encargada de obterner el id de la partida del jugadorr
@paranm socket
*/
int getIndicePartida(int socket){
    int indicePartida;
    for(indicePartida=0 ; indicePartida<MAX_PARTIDAS ; indicePartida++){
    printf("[PARTIDA] Indice:%i, ID:%i Jugador 1: %i, Jugador 2: %d\n",indicePartida, partidas[indicePartida].id, partidas[indicePartida].jugadores[0], partidas[indicePartida].jugadores[1]);
        if((partidas[indicePartida].id!=-1) && (partidas[indicePartida].jugadores[0]==socket) || (partidas[indicePartida].jugadores[1]==socket)){
            return indicePartida;
        }
    }
    return -1;
}
/*

*/
int getIdJugador(int socket, int indicePartida){
    return partidas[indicePartida].jugadores[0] == socket ? 1:2;
}

/*Extrae las coordenadas de para que las entienda el programa y extrae la cantidad de soldados usados.
  @param coordenadas Cadena de caracteres con las coordenadas y soldados
  @param origen Array de dos enteros para almacenar las coordenadas (x, y) de origen
  @param destino Array de dos enteros para almacenar las coordenadas (x, y) de destino
  @param soldados Puntero a un entero para almacenar la cantidad de soldados
*/
void traductorCoordenadasC(const char coordenadas[], int origen[2], int destino[2], int *soldados){
    char columnaOrigen, columnaDestino;
    int filaOrigen, filaDestino;
    int n_soldados;
    printf("%s\n", coordenadas);
    if(sscanf(coordenadas, "%c%d %c%d %d", &columnaOrigen, &filaOrigen,
        &columnaDestino, &filaDestino, &n_soldados) == 5 &&
        isalpha((unsigned char)columnaOrigen) &&
        isalpha((unsigned char)columnaDestino))
    {
        columnaOrigen = (char)toupper((unsigned char)columnaOrigen);
        columnaDestino = (char)toupper((unsigned char)columnaDestino);
        origen[0] = columnaOrigen - 'A';
        origen[1] = filaOrigen-1;
        destino[0] = columnaDestino - 'A';
        destino[1] = filaDestino-1;
        *soldados = n_soldados;
    }
    else {
        printf("Formato invalido. Use: [Casilla origen] [Casilla destino] soldados\n");
        return;
    }
}

void actualizarPartida(int socket){
    int indicePartida = getIndicePartida(socket);
    partidas[indicePartida].turno++;
    
    enviarTableroAJugadores(partidas[indicePartida].tablero, indicePartida);
}

int Ataque(int socket, char coordenadas[50]){
    int indicePartida = getIndicePartida(socket);
    int jugador = getIdJugador(socket, indicePartida);

    int enemigo = jugador == 1 ? 2 : 1;
    const char *color = jugador == 1 ? "31" : "34";
    int origen[2], destino[2], soldados;

    printf("\n\033[1;%smAqui tienes tus casillas marcadas\033[1;0m\n", color);

    origen[0] = origen[1] = destino[0] = destino[1] = -1;
    soldados = -1;
    traductorCoordenadasC(coordenadas, origen, destino, &soldados);

    if (origen[0] < 0 || origen[0] >= 5 || origen[1] < 0 || origen[1] >= 5 ||
        destino[0] < 0 || destino[0] >= 5 || destino[1] < 0 || destino[1] >= 5 ||
        soldados <= 0 ||
        partidas[indicePartida].tablero[origen[0]][origen[1]].jugador != jugador ||
        soldados > partidas[indicePartida].tablero[origen[0]][origen[1]].soldados ||
        partidas[indicePartida].tablero[destino[0]][destino[1]].jugador != enemigo ||
        (abs(origen[0] - destino[0]) + abs(origen[1] - destino[1]) != 1)) {
        printf("Ataque no permitido: debes atacar una casilla del enemigo adyacente con suficientes soldados.\n");
        return 0;
    }

    printf("Ataque permitido\n");
    partidas[indicePartida].tablero[destino[0]][destino[1]].soldados -= soldados;
    partidas[indicePartida].tablero[origen[0]][origen[1]].soldados -= soldados;
    if (partidas[indicePartida].tablero[origen[0]][origen[1]].soldados == 0) {
        partidas[indicePartida].tablero[origen[0]][origen[1]].jugador = 0;
    }
    if(partidas[indicePartida].tablero[destino[0]][destino[1]].soldados < 0) {
        partidas[indicePartida].tablero[destino[0]][destino[1]].soldados = soldados;
        printf("Conquistaste la casilla del jugador %i, mantuviste %i soldados\n", enemigo, partidas[indicePartida].tablero[destino[0]][destino[1]].soldados);
        partidas[indicePartida].tablero[destino[0]][destino[1]].jugador = jugador;
    } 
    else if(partidas[indicePartida].tablero[destino[0]][destino[1]].soldados == 0) {
        printf("Ambos os quedasteis sin soldados, casilla liberada\n");
        partidas[indicePartida].tablero[destino[0]][destino[1]].jugador = 0;
    } 
    else{
        printf("Perdiste la batalla por la casilla, perdiste %i soldados\n", soldados);
        partidas[indicePartida].tablero[destino[0]][destino[1]].soldados += soldados;
    }
    return 1;
}

int Reforzar(int socket, char coordenadas[50]){
    int indicePartida = getIndicePartida(socket);
    int jugador = getIdJugador(socket, indicePartida);

    int origen[2], destino[2], soldados;
    char* color = jugador == 1 ? "\033[1;31m" : "\033[1;34m";

    origen[0] = origen[1] = destino[0] = destino[1] = -1;
    soldados = -1;
    traductorCoordenadasC(coordenadas, origen, destino, &soldados);

    if (origen[0] < 0 || origen[0] >= 5 || origen[1] < 0 || origen[1] >= 5 ||
        destino[0] < 0 || destino[0] >= 5 || destino[1] < 0 || destino[1] >= 5 ||
        soldados <= 0 ||
        partidas[indicePartida].tablero[origen[0]][origen[1]].jugador != jugador ||
        partidas[indicePartida].tablero[destino[0]][destino[1]].jugador != jugador ||
        soldados > partidas[indicePartida].tablero[origen[0]][origen[1]].soldados ||
        abs(origen[0] - destino[0]) + abs(origen[1] - destino[1]) != 1) {
        printf("Refuerzo no permitido: ambas casillas deben ser tuyas y adyacentes. Debes tener suficientes soldados en la casilla de origen.\n");
        return 0;
    }

    partidas[indicePartida].tablero[origen[0]][origen[1]].soldados -= soldados;
    partidas[indicePartida].tablero[destino[0]][destino[1]].soldados += soldados;
    if (partidas[indicePartida].tablero[origen[0]][origen[1]].soldados == 0) {
        partidas[indicePartida].tablero[origen[0]][origen[1]].jugador = 0;
    }
    printf("Refuerzo realizado: mandaste %i soldados\n", soldados);
    return 1;
}

/*Comprobacion de la condicion de victoria o fin del juego
  @param tablero Matriz del mapa de juego
  @param turno Numero de turnos jugados
  @param abadono marca si el oponente ha abandonado la partida
  @return 1 si hay un ganador o empate (fin de la partida), 0 en caso contrario
*/
int comprobarVictoria(int socket){
    
    int indicePartida = getIndicePartida(socket);
    int indiceJugador = getIdJugador(socket, indicePartida);
    
    char miwebo2[100];

    int territoriosJugador1 = 0;
    int territoriosJugador2 = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(partidas[indicePartida].tablero[i][j].jugador == 1)
                territoriosJugador1++;
            else if(partidas[indicePartida].tablero[i][j].jugador == 2)
                territoriosJugador2++;
        }
    }

    printf("\n\033[1;32mCasillas conquistadas: \033[1;31mjugador 1: %i \033[0m| \033[1;34mjugador 2: %i\033[0m\n",territoriosJugador1, territoriosJugador2);
    
    if(territoriosJugador1 == 25){
        sprintf(miwebo2, "[FIN] Gana el jugador 1: ha conquistado todas las casillas.\n");
        send(partidas[indicePartida].jugadores[0], miwebo2, strlen(miwebo2), 0); 
        send(partidas[indicePartida].jugadores[1], miwebo2, strlen(miwebo2), 0); 
        return 1;
    }
    if(territoriosJugador2 == 25){
        sprintf(miwebo2,"[FIN] Gana el jugador 2: ha conquistado todas las casillas.\n");
        send(partidas[indicePartida].jugadores[0], miwebo2, strlen(miwebo2), 0); 
        send(partidas[indicePartida].jugadores[1], miwebo2, strlen(miwebo2), 0); 
        return 1;
    }
    if(territoriosJugador1 == 0 && territoriosJugador2 == 0){
        sprintf(miwebo2, "[FIN] Empate: ambos jugadores se han quedado sin casillas.\n");
        send(partidas[indicePartida].jugadores[0], miwebo2, strlen(miwebo2), 0); 
        send(partidas[indicePartida].jugadores[1], miwebo2, strlen(miwebo2), 0); 
        return 1;
    }
    if(territoriosJugador1 == 0){
        sprintf(miwebo2,"[FIN] Gana el jugador 2: el jugador 1 se ha quedado sin casillas.\n");
        send(partidas[indicePartida].jugadores[0], miwebo2, strlen(miwebo2), 0); 
        send(partidas[indicePartida].jugadores[1], miwebo2, strlen(miwebo2), 0); 
        return 1;
    }
    if(territoriosJugador2 == 0){
        sprintf(miwebo2,"[FIN] Gana el jugador 1: el jugador 1 se ha quedado sin casillas.\n");
        send(partidas[indicePartida].jugadores[0], miwebo2, strlen(miwebo2), 0); 
        send(partidas[indicePartida].jugadores[1], miwebo2, strlen(miwebo2), 0); 
        return 1;
    }
    if(partidas[indicePartida].turno >= MAX_TURNOS){
        if(territoriosJugador1 > territoriosJugador2){
        sprintf(miwebo2,"[FIN] Gana el jugador 1 por tener mas territorios.\n");
        send(partidas[indicePartida].jugadores[0], miwebo2, strlen(miwebo2), 0); 
        send(partidas[indicePartida].jugadores[1], miwebo2, strlen(miwebo2), 0); 
        }else if(territoriosJugador2 > territoriosJugador1){
        sprintf(miwebo2,"[FIN] Gana el jugador 2 por tener mas territorios.\n");
        send(partidas[indicePartida].jugadores[0], miwebo2, strlen(miwebo2), 0); 
        send(partidas[indicePartida].jugadores[1], miwebo2, strlen(miwebo2), 0); 
        }else{
        sprintf(miwebo2,"[FIN] Empate: ambos jugadores tienen el mismo numero de territorios.\n");
        send(partidas[indicePartida].jugadores[0], miwebo2, strlen(miwebo2), 0); 
        send(partidas[indicePartida].jugadores[1], miwebo2, strlen(miwebo2), 0); 
        return 1;
        }
    
    }

    return 0;
}
/*Son las 2:44 del 11 de octubre de 2026, sacadnos de aquí, estamos hasta la polla
@param socket sufrimiento
@return libertad
*/
int salir(int socket){
    int indicepartida=getIndicePartida(socket);
    char msg[100]="+Ok. Buena tirada de cable.\n";
    if(indicepartida!=-1){
        printf("%s\n",msg);
        if(partidas[indicepartida].jugadores[0]==socket){
            send(partidas[indicepartida].jugadores[0], msg, sizeof(msg), 0);
            sprintf(msg, "+Ok. El oponente ha terminado la partida.");
            send(partidas[indicepartida].jugadores[1], msg, sizeof(msg), 0);
        }else{
            send(partidas[indicepartida].jugadores[1], msg, sizeof(msg), 0);
            sprintf(msg, "+Ok. El oponente ha terminado la partida.");
            send(partidas[indicepartida].jugadores[0], msg, sizeof(msg), 0);
        }
        comprobarVictoria(socket);
        chaparPartida(socket);
    }
    else{
        strcpy(msg, "+Ok. Chau.");
        send(socket, msg, sizeof(msg), 0);
    }
    for(int i=0 ; i<MAX_CLIENTS ; i++){
        if(clients[i].socket==socket){
            clients[i].contrasena[0]='\0';
            clients[i].usuario[0]='\0';
            clients[i].registered=0;
            clients[i].socket=0;
        }
    }
    pid_t a = fork();
    switch (a)
    {
    case -1:
        printf("Hijo defectuoso\n");
        exit(EXIT_FAILURE);
        break;
    
    case 0:
        execlp("bash","bash", "./salir.sh", (char*) NULL);
        exit(EXIT_SUCCESS);
        break;
    default:
        printf("hola\n");
    break;
    }

}

void chaparPartida(int socket){
    printf("Chapando la partida\n");
    int indicePartida = getIndicePartida(socket);
    partidas[indicePartida].id=-1;
    partidas[indicePartida].estado=0;
    partidas[indicePartida].jugadores[0]=0;
    partidas[indicePartida].jugadores[1]=0;
    partidas[indicePartida].turno=0;

}

void establecerSemilla(){
    srand(time(NULL)*(ANYDESK*rand()+(rand()/ANYDESK*98675430)*(rand()%ANYDESK)*(ANYDESK%rand())));
}

/*Limpiar el buffer para evitar problemas en el fgetc
*/
void limpiarBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}