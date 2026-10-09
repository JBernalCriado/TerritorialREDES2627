#include "server.h"
int main(){

    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    //Pa que enchufe
    int server_socket, new_socket, client_sockets[MAX_CLIENTS] = {0};
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    srand(time(NULL));

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d\n", PORT);

    fd_set read_fds;
    int max_sd;
    //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

    // LEEER MENSAJES
    
    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        while (1)
    {
        int accionRealizada=0;
        FD_ZERO(&read_fds);

        FD_SET(server_socket, &read_fds);
        max_sd = server_socket;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_sockets[i];
            if (sd > 0)
                FD_SET(sd, &read_fds);
            if (sd > max_sd)
                max_sd = sd;
        }

        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR)
        {
            perror("select");
        }

        if (FD_ISSET(server_socket, &read_fds))
        {
            new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
            if (new_socket < 0)
            {
                perror("accept");
                continue;
            }

            registrarCliente(new_socket);

            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_sockets[i] == 0)
                {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_sockets[i];
            if (FD_ISSET(sd, &read_fds))
            {
                char entrada[100]; //Guarda todo el comando introducido por el jugador
                char comando[20]; //Guarda el comando
                char parametros[80]; //Guarda los parametros del comando
                int partes;
                int opcion = 0;
                int bytes_read = recv(sd, entrada, sizeof(entrada), 0);

                if (bytes_read <= 0)
                {
                    printf("Cliente desconectado.\n");
                    close(sd);
                    client_sockets[i] = 0;
                }
                else
                {
                    entrada[bytes_read] = '\0';
                    printf("Mensaje recibido: %s\n", entrada);
                    partes=sscanf(entrada, "%s %s", comando, parametros);
                }

            //Comando vacio


                    quitarn(entrada);
                    if (partes < 1) {
                        printf("Comando vacio.\n");
                        continue;
                    }

                    //Paso a mayusculas
                    for (char *letra = comando; *letra != '\0'; letra++)
                        *letra = (char) toupper((unsigned char) *letra);

                    int descriptor=client_sockets[i];

                    //Asignacion de acciones
                    if ((strcmp(comando, "USUARIO") == 0) && (comprobarCliente(descriptor) == 0))
                        opcion = 1;
                    else if ((strcmp(comando, "PASSWORD") == 0) && (comprobarCliente(descriptor) == 0))
                        opcion = 2;                    
                    else if ((strcmp(comando, "REGISTRO") == 0) && (comprobarCliente(descriptor) == 0))
                        opcion = 3;
                    else if ((strcmp(comando, "INICIAR-PARTIDA") == 0) && (comprobarCliente(descriptor) == 1))
                        opcion = 4;
                    else if ((strcmp(comando, "ATACAR") == 0) && (comprobarCliente(descriptor) == 1))
                        opcion = 5;
                    else if ((strcmp(comando, "REFORZAR") == 0) && (comprobarCliente(descriptor) == 1))
                        opcion = 6;
                    else if ((strcmp(comando, "CONQUISTAR") == 0) && (comprobarCliente(descriptor) == 1))
                        opcion = 7;
                    else if ((strcmp(comando, "PASAR") == 0) && (comprobarCliente(descriptor) == 1))
                        opcion = 8;
                    else if ((strcmp(comando, "SALIR") == 0) && (comprobarCliente(descriptor) == 0))
                        opcion = 9;
                    switch (opcion) {
                    case 1: //USUARIO
                        if(comprobarNombre(parametros)){
                            char msg []="-Err. Usuario ya conectado";
                            send(client_sockets[i], msg, strlen(msg),0);
                        listarClientes();
                            break;
                        }if (buscarUsuario(parametros, client_sockets[i])){
                            asignarUsuario(client_sockets[i], parametros);
                            char msg[] = "Usuario correcto, introduce la contraseña\n";
                            send(client_sockets[i], msg, strlen(msg), 0);
                        listarClientes();
                            break;
                        }else{
                            char msg[] = "Usuario incorrecto.\n";
                            send(client_sockets[i], msg, strlen(msg), 0);
                        listarClientes();
                            break;
                        }
                    case 2://CONTRASEÑA
                        if (strlen(parametros)==0 || parametros=="\0"){
                            char aviso[100];
                            sprintf(aviso,"%s Por favor, introduzca una contraseña%s \n", BG_RED, DEFAULT );
                            send(client_sockets[i], aviso, strlen(aviso), 0);
                        }
                        if (introducirContra(client_sockets[i], parametros)){
                            char msg[] = "Contraseña correcta, ha iniciado sesión\n\n\n";
                            send(client_sockets[i], msg, strlen(msg), 0);
                        }
                        else{
                            char msg[] = "Contraseña incorrecta y usuario no coinciden\n";
                            send(client_sockets[i], msg, strlen(msg), 0);
                        }
                        listarClientes();
                        break;

                    case 3://REGISTRAR UN NUEVO USUARIO
                        char nombre[35];
                        char clave[35];
                        char basura[20];  //Orden previa a los datos
                        //debugeo printf("%s", parametros);
                        sscanf(entrada, "%s %s %s", basura, nombre, clave);
                            if (buscarUsuario(nombre, client_sockets[i]) == 0){
                                agregarUsuario(nombre, clave);
                                char msg[] = "Usuario registrado correctamente, ya puede iniciar sesión\n";
                                send(client_sockets[i], msg, strlen(msg), 0);
                            }
                            else{
                                char msg[] = "El usuario ya existe.\n";
                                send(client_sockets[i], msg, strlen(msg), 0);
                            } 
                    
                        strcpy(entrada, "");
                        break;
                    case 4: //INIAR UNA PARTIDA
                        switch (agregarUsuarioPartida(client_sockets[i])){
                        case 0:{
                            char msg[] = "Ha ocurrido un error de emparejamiento\n";
                            send(client_sockets[i], msg, strlen(msg), 0);
                            break;
                        }
                        case 1:{
                            char msg[] = "Esperando a otro jugador...\n";
                            send(client_sockets[i], msg, strlen(msg), 0);
                            break;
                        }
                        // Si hay dos jugadores comienza la partida desde la función agregarUsuarioPartida
                        default:{break;}
                        }
                        break;
                    break;
                    case 5:
                        if(comprobarAccion(client_sockets[i], 1)&& partes == 2){
                            enviarAtaque(client_sockets[i], parametros);
                        } //ataque

                        break;
                    /*case 6:
                        comprobarAccion(client_sockets[i], 2);//refuerzo
                        break;
                    case 7:
                        comprobarAccion(client_sockets[i],3); //conquista
                        break;
                    case 8:
                        if (partes == 1)
                        accionRealizada = 1;
                        break;*/
                    case 9: 
                        procesarSalida(client_sockets[i]);
                    break;
                    default:
                        printf("Opción no reconocida: %s\n", comando);
                        char msg[] = "Comando no reconocido.\n";
                        send(client_sockets[i], msg, strlen(msg), 0);
                        break;
                    }




    

            }}

                
}

exit(EXIT_SUCCESS);
}