#ifndef TDA_ARBOL_H_INCLUDED
#define TDA_ARBOL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MINIMO(X,Y)((X)<(Y)?(X):(Y))

typedef struct sNodoArbol{
    void* info;
    unsigned tamInfo;
    struct sNodoArbol *izq,
                      *der;
}tNodoArbol;
typedef tNodoArbol* tArbol;

void CrearArbol(tArbol *p);
void EliminarArbol(tArbol *p);

#endif // TDA_ARBOL_H_INCLUDED
