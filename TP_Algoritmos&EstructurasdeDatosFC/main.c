#include "Bibliotecas/Funciones.h"
int main()
{
    char opcion, PATH_SOCIOS_DAT[TAM_LINEA]="Archivos/socios.dat";
    t_indice indice;
    FILE *pf;

    /*
    if (!pedirPath(PATH_SOCIOS_DAT, TAM_LINEA))
    {
            fprintf(stderr, "\nError! Problema al validar la ruta '%s'\n", PATH_SOCIOS_CSV);
            return 1;
    }
    */

    pf = fopen(PATH_SOCIOS_DAT, "r+b");
    if(!pf)
    {
        // hice un cambio aca, socios.dat no existe todavia. paso el csv una sola vez. Sino, en cada ejecucion  pisariamos todas las altas/bajas ya guardadas y el indice quedaria apuntando a posiciones que ya no existen

        if(CargarSociosenArchivoBinario(PATH_SOCIOS_CSV) == TODO_MAL)
        {
            fprintf(stderr, "\nError! Problema con Cargar el Archivo '%s'\n", PATH_SOCIOS_CSV);
            return 1;
        }
        printf("\nAviso! Archivo '%s' Cargado Correctamente\n", PATH_SOCIOS_CSV);

        pf = fopen(PATH_SOCIOS_DAT, "r+b");
    }

    if(!pf)
    {
        fprintf(stderr, "\nError! No se pudo abrir '%s'\n", PATH_SOCIOS_DAT);
        return 1;
    }

    ind_crear(&indice, sizeof(unsigned), CmpDNI);

    if(ind_cargar(&indice, PATH_SOCIOS_IDX)!= TODO_OK)
    {
        if(CargarArchivoBinenArbolBinBusq(&indice.arbol, PATH_SOCIOS_DAT, sizeof(unsigned), CmpDNI) != TODO_OK)
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
            if(!AltaSocio(&indice, pf))
                fprintf(stderr, "\nError! Hubo un Problema con la Alta del Socio.\n");
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
            CompactarYReindexar(&indice, &pf, PATH_SOCIOS_DAT);
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
