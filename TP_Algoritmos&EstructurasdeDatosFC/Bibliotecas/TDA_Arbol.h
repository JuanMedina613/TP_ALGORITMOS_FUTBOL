#ifndef TDA_ARBOL_H_INCLUDED
#define TDA_ARBOL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TODO_OK 1
#define TODO_MAL 0
#define DUPLICADO 2
#define SIN_MEM 3

#define MINIMO(X,Y)((X)<(Y)?(X):(Y))
#define reservarMemoriaNodo( X , Y , Z , W ) ( \
    ( ( X ) = (typeof( X ))malloc( Y ) ) == NULL || \
    ( ( Z ) = malloc( W ) ) == NULL ? \
    free( X ), 0 : 1 )

typedef struct sNodoArbol{
    void* info;
    unsigned tamInfo;
    struct sNodoArbol *izq,
                      *der;
}tNodoArbol;
typedef tNodoArbol* tArbol;

void CrearArbol(tArbol *p);
void EliminarArbol(tArbol *p);
int insertarArbolBinBusq(tArbol *p, const void *pd, unsigned tam, int(*cmp)(const void*,const void*));
int eliminarElementoArbol(tArbol *p, void *pd, unsigned tam, int(*cmp)(const void*, const void*));
int eliminarRaizArbol(tArbol *p);
void recorrerEnOrdenArbol(const tArbol* p,unsigned n, void* params,void(*Accion)(void*, unsigned, unsigned,void*));
int CargarDesdeDatosOrdenados(tArbol* p, void* ds, unsigned (*leer)(void**, void*, unsigned), int li, int ls);
tNodoArbol **buscarNodoArbol(const tArbol *p, const void *pd,int(*cmp)(const void*,const void*));
unsigned alturaArbol(const tArbol *p);
tNodoArbol **mayorNodoArbol(const tArbol* p);
tNodoArbol **menorNodoArbol(const tArbol* p);

#endif // TDA_ARBOL_H_INCLUDED
