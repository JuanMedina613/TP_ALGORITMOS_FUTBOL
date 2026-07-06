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
#define ORANGE "\x1b[38;5;208m"

#define TAM_LINEA 256
#define TAM_NYA 60
#define TAM_CATEGORIA 10

///PATHS DE ARCHIVOS///
#define PATH_SOCIOS_IDX "Archivos/socios.idx"
#define PATH_SOCIOS_DAT "Archivos/socios.dat"

///RANGOS VALIDOS ///
#define DNI_MIN 10000u
#define DNI_MAX 100000000u
#define DIA_MIN 1u
#define MES_MIN 1u
#define MES_MAX 12u
#define ANIO_MIN 1900u
#define ANIO_MAX 2026u

///EDADES LIMITE POR CATEGORIA///
#define EDAD_MENOR_MAX      13
#define EDAD_CADETE_MIN     14
#define EDAD_CADETE_MAX     17
#define EDAD_ADULTO_MIN     18
#define EDAD_VITALICIO_MIN  50
#define EDAD_JUBILADO_MIN   60

#define DNI_EXISTE   1
#define DNI_NUEVO    2

///ESTRUCTURAS///
typedef struct{
    int dia,mes,anio;
}t_fecha;

typedef struct{
    long DNI;
    char apellidos[TAM_NYA + 1];
    char nombres[TAM_NYA + 1];
    t_fecha fecha_nacimiento;
    char sexo;
    t_fecha fecha_afiliacion;
    char categoria[TAM_CATEGORIA + 1];
    t_fecha fecha_ultima_cuota;
    char estado;
    t_fecha fecha_baja;
}t_socio;

char SeleccionarMenu();
int CargarSociosenArchivoBinario(const char* path);
int CargarArchivoBinenArbolBinBusq(tArbol* p, const char* path, unsigned tam,int(*cmp)(const void*, const void*));
int pedirPath(char *dest, size_t tam);
int crearArchivoSocios(const char *path);
int LeerArchivoDatos(const char* path);

///FUNCIONE DEL MENU
int AltaSocio(t_indice* ind, FILE* pf);
int BajaSocio(t_indice* ind, FILE* pf);
int listarSociosOrdenados(const t_indice* ind, FILE* pf);
int modificarSocio(t_indice* ind, FILE* pf);
int ListarSociosOrdenados(const t_indice* ind, FILE* pf);
int CompactarYReindexar(t_indice* ind, FILE* ppf, const char* path);


///FUNCIONES NECESARIAS PARA LAS FUNCIONES DEL MENU
void mostrarSocioOrdenado(void* info, unsigned tam, unsigned n, void* param);
int pedirDNI(const t_indice* ind, long* dni, FILE* pf);
void pedirNombreoApellido(const char* mensaje, char* destino, int tam_max);
void pedirFecha(const char* mensaje, t_fecha* fecha, t_fecha* nacimiento);
void pedirSexo(char* sexo);
void pedirCategoria(char* categoria, const t_fecha* nacimiento, const t_fecha* hoy);
void obtenerFechaActual(t_fecha* hoy);
int calcularEdad(const t_fecha* nacimiento, const t_fecha* hoy);
long validarRango(long lim1, long lim2);
t_fecha validarFecha();
int esFechaMenor(const t_fecha* f1, const t_fecha* f2);
int esBisiesto(int anio);
int diasEnMes(int mes, int anio);
void mostrarSocio(const t_socio* socio);


///CMP DEL INDICE
int CmpDNI(const void* a, const void* b);

void mostrarSocioOrdenado(void* info, unsigned tam, unsigned n, void* param);
void mostrarSocio(const t_socio* socio);

#endif // FUNCIONES_H_INCLUDED
