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
int CargarArchivoBinOrdenadoArbolBinBusq(tArbol *p, const char* path, unsigned tamInfo)
{
    int cantReg,r;
    FILE* pf;
    if(*p)
        return TODO_MAL;
    if(!(pf = fopen(path,"rb")))
        return TODO_MAL;
    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf)/tamInfo;
    r = CargarDesdeDatosOrdenadosRec(p,pf,leerDesdeArchivoBin,0,cantReg-1,&tamInfo);
    fclose(pf);
    return r;
}
//****************************************************************************************************//
int CargarDesdeDatosOrdenadosRec(tArbol* p, void* ds, unsigned(*leer)(void**, void*, unsigned, void* params), int li, int ls, void* params)
{
   int m = (li+ls)/2,r;
   if(li>ls)
        return TODO_OK;
    (*p) = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if(!*p || !((*p)->tamInfo = leer(&(*p)->info,ds,m,params)))
    {
        free(*p);
        return TODO_MAL;
    }
    (*p)->izq = (*p)->der = NULL;
    if((r = CargarDesdeDatosOrdenadosRec(&(*p)->izq,ds,leer,li,m-1,params)))
        return r;
    return CargarDesdeDatosOrdenadosRec(&(*p)->der,ds,leer,m+1,ls,params);
}

//****************************************************************************************************//
unsigned leerDesdeArchivoBin(void** d, void* pf, unsigned pos, void* params)
{
    unsigned tam = *((int*)params);
    *d = malloc(tam);
    if(!*d)
        return 0;
    fseek((FILE*)pf, pos*tam, SEEK_SET);
    return fread(*d, tam, 1, (FILE*)pf);
}
//****************************************************************************************************//
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
        if(rc<0)
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

    free((*p)->info);

    if(!(*p)->izq && !(*p)->der)
    {
        free(*p);
        *p = NULL;
        return TODO_OK;
    }

    tNodoArbol **remp, *elim;

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
