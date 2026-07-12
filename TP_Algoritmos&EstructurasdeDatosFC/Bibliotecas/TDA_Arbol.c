#include "TDA_Arbol.h"
//****************************************************************************************************//
void CrearArbol(tArbol *p)
{
    *p = NULL;
}
//****************************************************************************************************//
void EliminarArbol(tArbol *p)
{
    if(*p)
    {
        EliminarArbol(&(*p)->izq);
        EliminarArbol(&(*p)->der);

        free((*p)->info);
        free(*p);
        *p = NULL;
    }
}
//****************************************************************************************************//
int insertarArbolBinBusq(tArbol *pa,const void *d,unsigned tam, int (*cmp)(const void *, const void *))
{
    tNodoArbol *nue;
    int rc;

    if(*pa)
    {
        if((rc = cmp(d, (*pa)->info)) < 0)
            return insertarArbolBinBusq(&(*pa)->izq, d, tam, cmp);

        if (rc > 0)
            return insertarArbolBinBusq(&(*pa)->der, d, tam, cmp);
        return DUPLICADO;
    }

    if(!reservarMemoriaNodo(nue, sizeof(tNodoArbol), nue->info, tam))
        return SIN_MEM;

    nue->tamInfo = tam;
    memcpy(nue->info, d, tam);

    nue->der = NULL;
    nue->izq = NULL;

    *pa = nue;

    return TODO_OK;
}

//****************************************************************************************************//
int eliminarElementoArbol(tArbol *p, void *pd, unsigned tam, int(*cmp)(const void*, const void*))
{
    if(!(p = buscarNodoArbol(p,pd,cmp)))
        return TODO_MAL;
    memcpy(pd, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return eliminarRaizArbol(p);
}
//****************************************************************************************************/
int eliminarRaizArbol(tArbol *p)
{
    if(!*p)
        return TODO_MAL;

    tNodoArbol **remp, *elim;

    free((*p)->info);

    if(!(*p)->izq && !(*p)->der)
    {
        free(*p);
        *p = NULL;
        return TODO_OK;
    }

    if (alturaArbol(&(*p)->izq) > alturaArbol(&(*p)->der))
        remp = mayorNodoArbol(&(*p)->izq);
    else
        remp = menorNodoArbol(&(*p)->der);

    elim = *remp;


    (*p)->info = elim->info;
    (*p)->tamInfo = elim->tamInfo;

    *remp = elim->izq ? elim->izq : elim->der;

    free(elim);
    return TODO_OK;
}
//****************************************************************************************************/
void recorrerEnOrdenArbol(const tArbol* p,unsigned n, void* params,void(*Accion)(void*, unsigned, unsigned,void*))
{
    if(!*p)
        return;
    recorrerEnOrdenArbol(&(*p)->izq, n+1, params, Accion);
    Accion((*p)->info, (*p)->tamInfo, n, params);
    recorrerEnOrdenArbol(&(*p)->der, n+1, params, Accion);
}
//****************************************************************************************************//
int CargarDesdeDatosOrdenados(tArbol* p, void* ds, unsigned (*leer)(void**, void*, unsigned), int li, int ls)
{
    int m = (li + ls)/2,
         r;

    if(li > ls)
        return TODO_OK;
    (*p) = (tNodoArbol*)malloc(sizeof(tNodoArbol));

    if(!*p || !((*p)->tamInfo = leer(&(*p)->info, ds, m)))
    {
        free(*p);
        return SIN_MEM;
    }

    (*p)->izq = (*p)->der = NULL;

    if((r = CargarDesdeDatosOrdenados(&(*p)->izq, ds, leer, li, m-1)) != TODO_OK)
        return r;
    return CargarDesdeDatosOrdenados(&(*p)->der, ds, leer, m+1, ls);
}
//****************************************************************************************************//
int buscarElementoArbol(const tArbol* p, void* pd, unsigned tam, int(*cmp)(const void*, const void*))
{
    if(!( p = buscarNodoArbol(p, pd, cmp)))
        return 0;
    memcpy(pd, (*p)->info, MINIMO((*p)->tamInfo, tam));
    return 1;
}
//****************************************************************************************************//
tNodoArbol **buscarNodoArbol(const tArbol *p, const void *pd,int(*cmp)(const void*,const void*))
{
    int rc;
    if(!*p)
        return NULL;
    if(*p && (rc = cmp(pd, (*p)->info)))
    {
        if(rc < 0)
            return buscarNodoArbol(&(*p)->izq,pd,cmp);
        return buscarNodoArbol(&(*p)->der,pd,cmp);
    }
    return (tNodoArbol**)p;
}
//****************************************************************************************************//
unsigned alturaArbol(const tArbol *p)
{
    if(!*p)
        return 0;
    unsigned altI = alturaArbol(&(*p)->izq);
    unsigned altD = alturaArbol(&(*p)->der);
    return (altI > altD ? altI : altD) + 1;
}
//****************************************************************************************************//
tNodoArbol **mayorNodoArbol(const tArbol* p)
{
    if(!*p)
        return NULL;
    while((*p)->der)
        p = &(*p)->der;
    return (tNodoArbol**)p;
}
//****************************************************************************************************//
tNodoArbol **menorNodoArbol(const tArbol* p)
{
    if(!*p)
        return NULL;
    while((*p)->izq)
        p = &(*p)->izq;
    return (tNodoArbol**)p;
}
//****************************************************************************************************//
