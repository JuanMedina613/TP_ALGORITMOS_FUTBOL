#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TDA_Arbol.h"


typedef struct{
    void* clave;
    unsigned nro_reg;
}t_entrada_indice;

typedef struct{
    tArbol arbol;
    unsigned tamClave;
    int(*cmp)(const void*,const void*);
}t_indice;


void ind_crear(t_indice* ind, size_t tam_clave, int (*cmp)(const void*, const void*));
int ind_insertar(t_indice* ind, void *clave, unsigned nro_reg);
int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg);
int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg);
int ind_cargar(t_indice* ind, const char* path);
int ind_grabar(const t_indice* ind, const char* path);
void ind_vaciar(t_indice* ind);
int ind_recorrer(const t_indice* ind, void (*accion)(void *, unsigned, unsigned, void *), void* param);

///FUNCIONES VARIAS -> SE PODRIA CREAR OTRO LUGAR PARA PONERLAS
int cargarIndiceDesdeArchivo(tArbol* p, const char* path, size_t tamClave, int(*cmp)(const void*,const void*));
void AccionGrabar(void* info, unsigned tam, unsigned n, void* params);
#endif // INDICE_H_INCLUDED
