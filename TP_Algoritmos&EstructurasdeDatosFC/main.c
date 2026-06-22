#include "Bibliotecas/Funciones.h"//todas las bibliotecas estan en Funciones
int main()
{
    char opcion;
    t_indice indice;
    FILE *pf;
    if(CargarSociosenArchivoBinario("Archivos/socios.csv") == 0)
    {
        printf("\nError! Problema con Cargar el Archivo 'Socios.csv'");
        return 0;
    }
    else
    {
        printf("\nAviso! Archivo 'Socios.csv' Cargado Correctamente");
        pf = fopen("Archivos/Socios.dat","r+b");
        if(!pf)
            return 0;
    }

    ind_crear(&indice,sizeof(unsigned),CmpDNI);
    if(ind_cargar(&indice,"Archivos/socios.dat"))
    opcion = SeleccionarMenu();

    while(opcion != 'S')
    {
        switch(opcion)
        {
        case 'A':
            if(!AltaSocio(&indice,pf,CmpDNI))
                printf("\nError! Hubo un Problema con la Alta del Socio.");

            break;
        case 'B':
            break;
        case 'M':
            break;
        case 'L':
            break;
        case 'C':
            break;
        case 'S':
            ind_vaciar(&indice);
        }
        opcion = SeleccionarMenu();
    }


    return 0;
}
