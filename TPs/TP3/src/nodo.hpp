#ifndef _NODO_H
#define _NODO_H

#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <cstring>
#include <stdlib.h>


#define BUFFER_SIZE 1024


#define TAG_QUIT 0
#define TAG_LOAD 1
#define TAG_ADDANDINC 2
#define TAG_MEMBER 3
#define TAG_MAXIMUM 4
#define TAG_MAXIMUM_NUMBER_RESPONSE 5
#define TAG_MAXIMUM_WORD_RESPONSE 6




#define CONSOLE_RANK 0

/* Función que maneja un nodo.
 * Recibe el rank del nodo.
 */
void nodo(unsigned int rank);

/* Simula un tiempo de procesamiento no determinístico.
 */
void trabajarArduamente();

bool nodeQuit();
bool nodeLoad(char msg[], HashMap &h);
bool nodeMaximum(HashMap &h);

#endif  /* _NODO_H */
