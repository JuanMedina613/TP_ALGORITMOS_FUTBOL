#include "Funciones.h"
///***********************************************************************************************//
char SeleccionarMenu()
{
    char decision;
    char opciones[]= "ABMLCS";
    do
    {
        printf("\n [A] Alta a un Socio.");
        printf("\n [B] Baja a un Socio.");
        printf("\n [M] Modificar Socio.");
        printf("\n [L] Listar todos los Socios Ordenados.");
        printf("\n [C] Compactar y Reindexar.");
        printf("\n [S] Salir.");
        printf("\n Seleccione una Opcion: ");

        scanf(" %c",&decision);//-> el espacion entes del % limpia el buffer no quitar
                               //-> tampoco usar el fflush(stdin) ya que no anda bien
        decision = toupper(decision);
        if(strchr(opciones,decision) == NULL)
        {
            printf("\nError! Opcion Seleccion Invalida...\n");
            system("pause");
            system("cls");
        }
    }while(strchr(opciones,decision) == NULL);
    return decision;
}
///***********************************************************************************************//
