#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "TDA_Arbol.h"
#include "Indice.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


///ESTRUCTURAS///
typedef struct{
    int dia,mes,anio;
}t_fecha;
typedef struct{
    int DNI;
    char apellidos[60];
    char nombres[60];
    t_fecha fecha_nacimiento;
    char sexo;
    t_fecha fecha_afiliacion;
    char categoria[10];
    t_fecha fecha_ultima_cuota;
    char estado;
    t_fecha fecha_baja;
}t_socio;

char SeleccionarMenu();
#endif // FUNCIONES_H_INCLUDED
