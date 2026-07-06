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

    return insertarArbolBinBusq(&(ind->arbol), &nue, sizeof(t_entrada_indice), ind->cmp);

}
//****************************************************************************************************//
int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg)
{
    t_entrada_indice buscado;

    buscado.clave = clave;
    t_entrada_indice *Encontrado;

    tNodoArbol **NodoEliminar = buscarNodoArbol(&(ind->arbol), &buscado, ind->cmp);

    if(NodoEliminar == NULL || *NodoEliminar == NULL)
        return TODO_MAL;

    Encontrado = (t_entrada_indice*)(*NodoEliminar)->info;
    *nro_reg = Encontrado->nro_reg;

    free(Encontrado->clave);

    return eliminarRaizArbol(NodoEliminar);
}
//****************************************************************************************************//
int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg)
{
    t_entrada_indice Buscado;
    t_entrada_indice *Encontrado;
    tNodoArbol **NodoEncontrado;

    Buscado.clave = clave;

    NodoEncontrado = buscarNodoArbol(&(ind->arbol), &Buscado, ind->cmp);

    if(NodoEncontrado == NULL || *NodoEncontrado == NULL)
        return TODO_MAL;

    Encontrado = (t_entrada_indice*)(*NodoEncontrado)->info;
    *nro_reg = Encontrado->nro_reg;

    return TODO_OK;
}
//****************************************************************************************************//
int ind_cargar(t_indice* ind, const char* path)
{
    int resultado;

    resultado = cargarIndiceDesdeArchivo(&ind->arbol,path, ind->tamClave,ind->cmp);

    return resultado;
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
/// FUNCIONES VARIAS
//****************************************************************************************************/
int cargarIndiceDesdeArchivo(tArbol* p, const char* path, size_t tamClave, int(*cmp)(const void*,const void*))
{
    FILE* pf = fopen(path, "rb");
    if(!pf)
        return TODO_MAL;

    t_entrada_indice nueva_entrada;

    size_t tamRegistro = tamClave + sizeof(unsigned);
    void* bufferLectura = malloc(tamRegistro);

    if(!bufferLectura)
    {
        fclose(pf);
        return TODO_MAL;
    }

    while(fread(bufferLectura, tamRegistro, 1, pf) == 1)
    {
        nueva_entrada.clave = malloc(tamClave);
        if(!nueva_entrada.clave)
        {
            printf("\nError! Sin Memoria.\n");
            free(bufferLectura);
            fclose(pf);
            return TODO_MAL;
        }

        memcpy(nueva_entrada.clave, bufferLectura, tamClave);

        memcpy(&nueva_entrada.nro_reg, (char*)bufferLectura + tamClave, sizeof(unsigned));

        if(!(insertarArbolBinBusq(p, &nueva_entrada, sizeof(t_entrada_indice), cmp)))
        {
            printf("\nError! Sin Memoria.\n");
            free(nueva_entrada.clave);
            free(bufferLectura);
            fclose(pf);
            return TODO_MAL;
        }
    }

    free(bufferLectura);
    fclose(pf);
    return TODO_OK;
}
//****************************************************************************************************//
void AccionGrabar(void* info, unsigned tam, unsigned n, void* params)
{
    FILE* pf = (FILE*)params;
    t_entrada_indice* entrada = (t_entrada_indice*)info;

    fwrite(entrada->clave, sizeof(unsigned), 1, pf);
    fwrite(&entrada->nro_reg, sizeof(unsigned), 1, pf);
}
//****************************************************************************************************//
