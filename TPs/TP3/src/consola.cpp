#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include "consola.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include "nodo.hpp"

using namespace std;

#define CMD_LOAD    "load"
#define CMD_ADD     "addAndInc"
#define CMD_MEMBER  "member"
#define CMD_MAXIMUM "maximum"
#define CMD_QUIT    "quit"
#define CMD_SQUIT   "q"

static unsigned int np;

// Crea un ConcurrentHashMap distribuido
static void load(list<string> params) {

    char msg[BUFFER_SIZE];
    for (unsigned int i = 1; i<np; i++){
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,i,TAG_LOAD,MPI_COMM_WORLD);
    }

    MPI_Status status;   // required variable for receive routines
    for (list<string>::iterator it=params.begin(); it != params.end(); ++it) {
        MPI_Recv(NULL,0,MPI_CHAR,MPI_ANY_SOURCE,TAG_LOAD,MPI_COMM_WORLD,&status);

        strcpy(msg, (*it).c_str());
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,status.MPI_SOURCE,TAG_LOAD,MPI_COMM_WORLD);
    }
    
    //cout << "La lista esta procesada" << endl;

    unsigned int acks = 0;
    while(acks < np-1){
        //Espero a cada nodo
        MPI_Recv(NULL,0,MPI_CHAR,MPI_ANY_SOURCE,TAG_LOAD,MPI_COMM_WORLD,&status);
        //Le aviso que ya terminamos
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,status.MPI_SOURCE,TAG_LOAD_FIN,MPI_COMM_WORLD);
        acks++;
    }

}

// Esta función debe avisar a todos los nodos que deben terminar
static void quit() {
    char msg[BUFFER_SIZE];
    for (unsigned int i = 1; i<np ; i++){
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,i,TAG_QUIT,MPI_COMM_WORLD);
    }
}

// Esta función calcula el máximo con todos los nodos
static void maximum() {

    HashMap h;

    char msg[BUFFER_SIZE];
    for (unsigned int i = 1; i<np; i++){
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,i,TAG_MAXIMUM,MPI_COMM_WORLD);
    }

    MPI_Status status;   // required variable for receive routines
    unsigned int acks = 0;
    while(acks < np-1){ // Ya todos los que no son consola confirmaron que terminaron
        MPI_Recv(&msg,BUFFER_SIZE,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        if(status.MPI_TAG == TAG_MAXIMUM_END) {
            acks++;
        } if(status.MPI_TAG == TAG_MAXIMUM_WORD){
            string word(msg);
            h.addAndInc(word);
        }
    }

    pair<string, unsigned int> result = h.maximum();

    cout << "El máximo es <" << result.first <<"," << result.second << ">" << endl;

}


// Esta función busca la existencia de *key* en algún nodo
static void member(string key) {
    char msg[BUFFER_SIZE];
    strcpy(msg, (key).c_str());

    for (unsigned int i = 1; i<np; i++){
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,i,TAG_MEMBER,MPI_COMM_WORLD);
    }

    unsigned int acks = 0;
    MPI_Status status;   // required variable for receive routines

    bool esta = false;
    while(!esta && acks < np-1){
        MPI_Recv(&esta,1,MPI_C_BOOL,MPI_ANY_SOURCE,TAG_MEMBER,MPI_COMM_WORLD,&status);
        if(esta)
            cout << "El string <" << key << "> está." << endl;
        acks++;
    }

    if(!esta)
        cout << "El string <" << key << "> no está." << endl;

    // Espero que contesten los que faltan para que no queden mensajes colgados
    while(acks < np-1){
        MPI_Recv(&esta,1,MPI_C_BOOL,MPI_ANY_SOURCE,TAG_MEMBER,MPI_COMM_WORLD,&status);
        acks++;
    }

    
}


// Esta función suma uno a *key* en algún nodo
static void addAndInc(string key) {
    char msg[BUFFER_SIZE];
    strcpy(msg, (key).c_str());

    for (unsigned int i = 1; i<np; i++){
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,i,TAG_ADDANDINC,MPI_COMM_WORLD);
    }

    MPI_Status status;   // required variable for receive routines
    MPI_Recv(NULL,0,MPI_C_BOOL,MPI_ANY_SOURCE,TAG_ADDANDINC,MPI_COMM_WORLD,&status);

    bool ack = true;
    MPI_Send(&ack,1,MPI_C_BOOL,status.MPI_SOURCE,TAG_ADDANDINC_ACK,MPI_COMM_WORLD);

    cout << "Agregado: " << key << endl;

    ack = false;
    for (unsigned int i = 1; i<np; i++){
        if((int) i != status.MPI_SOURCE){
            MPI_Send(&ack,1,MPI_C_BOOL,i,TAG_ADDANDINC_ACK,MPI_COMM_WORLD);
        }
    }

    unsigned int acks = 0;
    while(acks < np-2){
        MPI_Recv(NULL,0,MPI_C_BOOL,MPI_ANY_SOURCE,TAG_ADDANDINC,MPI_COMM_WORLD,&status);
        acks++;
    }

}


/* static int procesar_comandos()
La función toma comandos por consola e invoca a las funciones correspondientes
Si devuelve true, significa que el proceso consola debe terminar
Si devuelve false, significa que debe seguir recibiendo un nuevo comando
*/

static bool procesar_comandos() {

    char buffer[BUFFER_SIZE];
    size_t buffer_length;
    char *res, *first_param, *second_param;

    // Mi mamá no me deja usar gets :(
    res = fgets(buffer, sizeof(buffer), stdin);

    // Permitimos salir con EOF
    if (res==NULL)
        return true;

    buffer_length = strlen(buffer);
    // Si es un ENTER, continuamos
    if (buffer_length<=1)
        return false;

    // Sacamos último carácter
    buffer[buffer_length-1] = '\0';

    // Obtenemos el primer parámetro
    first_param = strtok(buffer, " ");

    if (strncmp(first_param, CMD_QUIT, sizeof(CMD_QUIT))==0 ||
        strncmp(first_param, CMD_SQUIT, sizeof(CMD_SQUIT))==0) {

        quit();
        //printf("Adios!\n");
        return true;
    }

    if (strncmp(first_param, CMD_MAXIMUM, sizeof(CMD_MAXIMUM))==0) {
        maximum();
        return false;
    }

    // Obtenemos el segundo parámetro
    second_param = strtok(NULL, " ");
    if (strncmp(first_param, CMD_MEMBER, sizeof(CMD_MEMBER))==0) {
        if (second_param != NULL) {
            string s(second_param);
            member(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_ADD, sizeof(CMD_ADD))==0) {
        if (second_param != NULL) {
            string s(second_param);
            addAndInc(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_LOAD, sizeof(CMD_LOAD))==0) {
        list<string> params;
        while (second_param != NULL)
        {
            string s(second_param);
            params.push_back(s);
            second_param = strtok(NULL, " ");
        }

        load(params);
        return false;
    }

    printf("Comando no reconocido");
    return false;
}

void consola(unsigned int np_param) {
    np = np_param;
    // printf("Comandos disponibles:\n");
    // printf("  "CMD_LOAD" <arch_1> <arch_2> ... <arch_n>\n");
    // printf("  "CMD_ADD" <string>\n");
    // printf("  "CMD_MEMBER" <string>\n");
    // printf("  "CMD_MAXIMUM"\n");
    // printf("  "CMD_SQUIT"|"CMD_QUIT"\n");

    bool fin = false;
    while (!fin) {
        //printf("> ");
        fflush(stdout);
        fin = procesar_comandos();
    }
}
