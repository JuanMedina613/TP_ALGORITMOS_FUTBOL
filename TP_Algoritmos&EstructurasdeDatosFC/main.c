#include "Bibliotecas/Funciones.h"//todas las bibliotecas estan en Funciones
int main()
{
    char opcion;

    if(CargarSocios("Archivos/socios.csv") == 0)
    {
        printf("\nError! Problema con Cargar el Archivo 'Socios.csv'");
        return 0;
    }
    else
        printf("\nAviso! Archivo 'Socios.csv' Cargado Correctamente");


    opcion = SeleccionarMenu();

    while(opcion != 'S')
    {
        switch(opcion)
        {
        case 'A':
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
        }
    }

    return 0;
}
