#include "Bibliotecas/Funciones.h"//todas las bibliotecas estan en Funciones
int main()
{
    char opcion;
    t_indice indice;
    FILE *pf;
    if(CargarSocios("Archivos/socios.csv") == 0)
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

    ind_crear(&indice, sizeof(unsigned), CmpDNI);
    ///ACA DEBERIA ESTAR LA CREACION DEL INDICE "SUPONGO"
    opcion = SeleccionarMenu();

    while(opcion != 'S')
    {
        switch(opcion)
        {
        case 'A':
            AltaSocio(&indice,pf,CmpDNI);
            break;
        case 'B':
            BajaSocio(&indice, pf);
            break;
        case 'M':
            break;
        case 'L':
            listarSociosOrdenados(&indice, pf);
            break;
        case 'C':
            break;
        case 'S':
            break;
        }

        opcion = SeleccionarMenu();
    }
    fclose(pf);

    return 0;
}
