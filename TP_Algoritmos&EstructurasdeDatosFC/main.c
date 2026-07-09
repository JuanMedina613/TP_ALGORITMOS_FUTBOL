#include "Bibliotecas/Funciones.h"
int main()
{
    char opcion, path_socios[TAM_LINEA];
    t_indice indice;
    FILE *pf;

    pf = fopen(PATH_SOCIOS_DAT, "r+b");
    if(!pf)
    {
        if (!pedirPath(path_socios, TAM_LINEA))
        {
            fprintf(stderr, "\nError! Problema al validar la ruta '%s'", path_socios);
            return 1;
        }

        if(CargarSociosenArchivoBinario(path_socios) == TODO_MAL)
        {
            fprintf(stderr, "\nError! Problema con Cargar el Archivo '%s'\n", path_socios);
            return 1;
        }
        printf("\nAviso! Archivo '%s' Cargado Correctamente\n", path_socios);
        system("pause");

        pf = fopen(PATH_SOCIOS_DAT, "r+b");
        if(!pf)
        {
            fprintf(stderr, "\nError! No se pudo abrir '%s'\n", PATH_SOCIOS_DAT);
            return 1;
        }
    }

    ind_crear(&indice, sizeof(long), CmpDNI);
    if(ind_cargar(&indice, PATH_SOCIOS_IDX) != TODO_OK)
    {
        if(CargarArchivoBinenArbolBinBusq(&indice, PATH_SOCIOS_DAT) != TODO_OK)
        {
            fprintf(stderr, "\nError! No se pudo construir el indice.\n");
            fclose(pf);
            return 1;
        }
    }
    opcion = SeleccionarMenu();

    while(opcion != 'S')
    {
        switch(opcion)
        {
        case 'A':
            AltaSocio(&indice, pf);
            break;

        case 'B':
            BajaSocio(&indice, pf);
            break;

        case 'M':
            modificarSocio(&indice, pf);
            break;

        case 'L':
            listarSociosOrdenados(&indice, pf);
            break;

        case 'C':
            CompactarYReindexar(&indice, pf, PATH_SOCIOS_DAT);
            break;
        }

        opcion = SeleccionarMenu();
    }
    if(ind_grabar(&indice, PATH_SOCIOS_IDX)!= TODO_OK)
        fprintf(stderr, "\nError! No se pudo grabar el archivo de indice.\n");

    ind_vaciar(&indice);
    fclose(pf);

    return 0;
}
