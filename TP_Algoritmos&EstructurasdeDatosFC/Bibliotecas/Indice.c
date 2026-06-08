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
    return TODO_OK;
}
//****************************************************************************************************//
int ind_grabar(const t_indice* ind, const char* path)
{
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
