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
///FUNCIONES QUE CREO NECESARIAS
int insertarArbol(tArbol *p, const void *pd, unsigned tam, int(*cmp)(const void*,const void*));
tNodoArbol **buscarNodoArbol(const tArbol *p, const void *pd,int(*cmp)(const void*,const void*));
int eliminarElementoArbol(tArbol *p, const void *pd, unsigned tam, int(*cmp)(const void*, const void*));
int eliminarRaizArbol(tArbol *p);
unsigned alturaArbol(const tArbol *p);
tNodoArbol **mayorNodoArbol(const tArbol* p);
tNodoArbol **menorNodoArbol(const tArbol* p);
void recorrerEnOrdenArbol(const tArbol *p, void(*accion)(void*));//->a definir los parametros...
#endif // TDA_ARBOL_H_INCLUDED
