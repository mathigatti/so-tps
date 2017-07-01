#ifndef _NODO_H
#define _NODO_H

#define BUFFER_SIZE 1024

#define TAG_QUIT 0
#define TAG_ADDANDINC 1
#define TAG_MEMBER 2
#define TAG_MAXIMUM 3

#define CONSOLE_RANK 0

/* Función que maneja un nodo.
 * Recibe el rank del nodo.
 */
void nodo(unsigned int rank);

/* Simula un tiempo de procesamiento no determinístico.
 */
void trabajarArduamente();

bool nodeQuit();

#endif  /* _NODO_H */
