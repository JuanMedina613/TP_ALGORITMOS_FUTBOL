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
unsigned alturaArbol(const tArbol *p)
{
    if(!*p)
        return 0;
    unsigned altI = alturaArbol(&(*p)->izq);
    unsigned altD = alturaArbol(&(*p)->der);
    return (altI > altD ? altI : altD) + 1;
}
tNodoArbol **mayorNodoArbol(const tArbol* p)
{
    if(!*p)
        return NULL;
    while((*p)->der)
        p = &(*p)->der;
    return (tNodoArbol**)p;
}

tNodoArbol **menorNodoArbol(const tArbol* p)
{
    if(!*p)
        return NULL;
    while((*p)->izq)
        p = &(*p)->izq;
    return (tNodoArbol**)p;
}
