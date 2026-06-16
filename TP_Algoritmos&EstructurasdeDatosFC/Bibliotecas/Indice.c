#include "Indice.h"
//****************************************************************************************************//
void ind_crear(t_indice* ind, size_t tam_clave, int (*cmp)(const void*, const void*))
{
}
//****************************************************************************************************//
int ind_insertar(t_indice* ind, void *clave, unsigned nro_reg)
{
    return TODO_OK;
}
//****************************************************************************************************//
int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg)
{
    return TODO_OK;
}
//****************************************************************************************************//
int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg)
{
    return TODO_OK;
}
//****************************************************************************************************//
int ind_cargar(t_indice* ind, const char* path)
{
    int resultado;

    resultado = CargarArchivoBinOrdenadoArbolBinBusq(&ind->arbol, path, ind->tamClave);
    if(resultado == 1)
        return 1;
    return 0;

}
//****************************************************************************************************//
int ind_grabar(const t_indice* ind, const char* path)
{
    FILE* pf;
    pf = fopen(path, "wb");
    if (pf == NULL)
        return 0;
    recorrerEnOrdenArbol(&ind->arbol,0,pf,AccionGrabar);
    fclose(pf);
    return TODO_OK;


}
//****************************************************************************************************//
void ind_vaciar(t_indice* ind)
{

}
//****************************************************************************************************//
int ind_recorrer(const t_indice* ind, void (*accion)(const void *, unsigned, void *), void* param)
{
    return TODO_OK;
}

/// FUNCIONES VARIAS -> Tal vez las podriamos poner en otro lado si Tai 9/12 quiere
void AccionGrabar(void* info, unsigned tam, unsigned n, void* params)
{
    t_entrada_indice entrada;
    memcpy(&entrada.clave,info,tam);
    entrada.nro_reg = n;
    fseek(params, 0 , SEEK_END);
    fwrite(&entrada,sizeof(t_entrada_indice),1,params);
}
