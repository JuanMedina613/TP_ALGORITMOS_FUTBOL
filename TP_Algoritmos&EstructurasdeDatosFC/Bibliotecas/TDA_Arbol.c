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
