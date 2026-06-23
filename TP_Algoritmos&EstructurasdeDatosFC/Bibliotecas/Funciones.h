#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED



#include "TDA_Arbol.h"
#include "Indice.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define RESET   "\033[0m"
#define CYAN    "\033[1;36m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"

#define TAM_LINEA 256
///ESTRUCTURAS///
typedef struct{
    int dia,mes,anio;
}t_fecha;

typedef struct{
    unsigned DNI;
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
int CargarSocios(const char* path);

///FUNCIONE DEL MENU
int AltaSocio(t_indice* ind,FILE* pf, int(*cmp)(const void*, const void*));
int BajaSocio(t_indice* ind, FILE* pf);
int listarSociosOrdenados(const t_indice* ind, FILE* pf);

void pedirDNI(const tArbol* p,unsigned* dni,FILE* pf, int(*cmp)(const void*, const void*));
void pedirNombreoApellido(const char* mensaje, char* destino, int tam_max);
void pedirFecha(const char* mensaje, t_fecha* fecha, t_fecha* nacimiento);
void pedirSexo(char* sexo);
void pedirCategoria(char* categoria, const t_fecha* nacimiento, const t_fecha* hoy);
void obtenerFechaActual(t_fecha* hoy);
int calcularEdad(const t_fecha* nacimiento, const t_fecha* hoy);
///VALIDACIONES

int validarRango(int lim1, int lim2);
int esFechaMenor(const t_fecha* f1, const t_fecha* f2);
int CmpDNI(const void* a, const void* b);
unsigned validarPositivo(unsigned lim1, unsigned lim2);
int esBisiesto(int anio);
int diasEnMes(int mes, int anio);
t_fecha validarFecha();

void mostrarSocioOrdenado(void* info, unsigned tam, unsigned n, void* param);
void mostrarSocio(const t_socio* socio);

#endif // FUNCIONES_H_INCLUDED
