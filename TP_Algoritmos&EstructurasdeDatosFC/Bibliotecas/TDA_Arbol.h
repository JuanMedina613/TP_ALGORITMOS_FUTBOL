#ifndef TDA_ARBOL_H_INCLUDED
#define TDA_ARBOL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TODO_OK 1
#define TODO_MAL 0

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

int CargarArchivoBinOrdenadoArbolBinBusq(tArbol *p, const char* path, unsigned tamInfo);
int CargarDesdeDatosOrdenadosRec(tArbol* p, void* ds, unsigned(*leer)(void**, void*, unsigned, void* params), int li, int ls, void* params);
unsigned leerDesdeArchivoBin(void** d, void* pf, unsigned pos, void* params);
void recorrerEnOrdenArbol(const tArbol* p,unsigned n, void* params,void(*Accion)(void*, unsigned, unsigned,void*));


///FUNCIONES QUE CREO NECESARIAS
int insertarArbol(tArbol *p, const void *pd, unsigned tam, int(*cmp)(const void*,const void*));
tNodoArbol **buscarNodoArbol(const tArbol *p, const void *pd,int(*cmp)(const void*,const void*));
int eliminarElementoArbol(tArbol *p, const void *pd, unsigned tam, int(*cmp)(const void*, const void*));
int eliminarRaizArbol(tArbol *p);
unsigned alturaArbol(const tArbol *p);
tNodoArbol **mayorNodoArbol(const tArbol* p);
tNodoArbol **menorNodoArbol(const tArbol* p);

#endif // TDA_ARBOL_H_INCLUDED
