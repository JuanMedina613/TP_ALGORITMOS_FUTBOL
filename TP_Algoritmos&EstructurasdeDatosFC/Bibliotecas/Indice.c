#include "Indice.h"
#include "Funciones.h"
//****************************************************************************************************//
void ind_crear(t_indice* ind, size_t tam_clave, int (*cmp)(const void*, const void*))
{
    CrearArbol(&ind->arbol);
    ind->tamClave = tam_clave;
    ind->cmp = cmp;
}
//****************************************************************************************************//
int ind_insertar (t_indice* ind, void *clave, unsigned nro_reg)
{
    t_entrada_indice nue;
    nue.clave = malloc(ind->tamClave);
    if(!nue.clave)
        return SIN_MEM;

    memcpy(nue.clave, clave, ind->tamClave);
    nue.nro_reg = nro_reg;

    if(!insertarArbolBinBusq(&(ind->arbol), &nue, sizeof(t_entrada_indice), ind->cmp))
    {
        free(nue.clave);
        return TODO_MAL;
    }
    return TODO_OK;
}
//****************************************************************************************************//
int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg)
{
    t_entrada_indice elem;
    elem.clave = clave;

    if(eliminarElementoArbol(&(ind->arbol), &elem, sizeof(t_entrada_indice), ind->cmp) == TODO_OK)
    {
        *nro_reg = elem.nro_reg;
        return TODO_OK;
    }
    return TODO_MAL;
}
//****************************************************************************************************//
int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg)
{
    t_entrada_indice Buscado;
    Buscado.clave = clave;

    if(buscarElementoArbol(&(ind)->arbol, &Buscado, sizeof(t_entrada_indice), ind->cmp) == TODO_MAL)
        return TODO_MAL;

    *nro_reg = Buscado.nro_reg;

    return TODO_OK;
}
//****************************************************************************************************//
int ind_cargar(t_indice* ind, const char* path)
{
    int cantReg, r;

    FILE* pf = fopen(path, "rb");
    if(!pf)
        return TODO_MAL;

    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf) / (sizeof(unsigned) + ind->tamClave);
    r = CargarDesdeDatosOrdenados(&ind->arbol, pf, leerParaIndice, 0, cantReg - 1);

    fclose(pf);
    return r;
}
//****************************************************************************************************//
int ind_grabar(const t_indice* ind, const char* path)
{
    FILE* pf = fopen(path, "wb");
    if(!pf)
        return TODO_MAL;

    if(ind->arbol)
       recorrerEnOrdenArbol(&(ind->arbol), 0, pf, AccionGrabar);

    fclose(pf);
    return TODO_OK;
}
//****************************************************************************************************//
void ind_vaciar(t_indice* ind)
{
    EliminarArbol(&ind->arbol);
}
//****************************************************************************************************//
int ind_recorrer (const t_indice* ind, void (*accion)(void *, unsigned, unsigned, void *), void* param)
{
    if(!(ind->arbol))
        return TODO_MAL;

    recorrerEnOrdenArbol(&(ind->arbol),0,param,accion);
    return TODO_OK;
}
//****************************************************************************************************//
