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
#define PATH_SOCIOS_CSV "Archivos/socios.csv"
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

//NECESARIA PARA PODER GRABAR
typedef struct {
    long clave;
    unsigned nro_reg;
} registro_archivo;

///------------FUNCIONES------------///
char SeleccionarMenu();

int CargarSociosenArchivoBinario(const char* path);
int CargarArchivoBinenArbolBinBusq(t_indice* indice, const char* path);
int pedirPath(char *dest, size_t tam);
int crearArchivoSocios(const char *path);

///FUNCIONE DEL MENU
int AltaSocio(t_indice* ind, FILE* pf);
int BajaSocio(t_indice* ind, FILE* pf);
int listarSociosOrdenados(const t_indice* ind, FILE* pf);
int modificarSocio(t_indice* ind, FILE* pf);
int CompactarYReindexar(t_indice* ind, FILE* ppf, const char* path);


///FUNCIONES NECESARIAS PARA LAS FUNCIONES DEL MENU
void mostrarSocioOrdenado(void* info, unsigned tam, unsigned n, void* param);

int pedirDNI(const t_indice* ind, long* dni, FILE* pf);
void pedirNombreoApellido(const char* mensaje, char* destino, int tam_max);
void pedirFecha(const char* mensaje, t_fecha* fecha, t_fecha* nacimiento);
void pedirSexo(char* sexo);
void pedirCategoria(char* categoria, const t_fecha* nacimiento, const t_fecha* hoy);

t_fecha validarFecha();

int calcularEdad(const t_fecha* nacimiento, const t_fecha* hoy);
int esFechaMenor(const t_fecha* f1, const t_fecha* f2);
int esBisiesto(int anio);
int diasEnMes(int mes, int anio);
long validarRango(long lim1, long lim2);

void obtenerFechaActual(t_fecha* hoy);


/////////->->->Funciones Necesarias
int LeerArchivoDatos(const char* path);
int CmpDNI(const void* a, const void* b);
void mostrarSocio(const t_socio* socio);
void mostrarSocioCompleto(const t_socio* socio);
void AccionGrabar(void* info, unsigned tam, unsigned n, void* params);
unsigned leerParaIndice(void** d, void* pf, unsigned pos);
#endif // FUNCIONES_H_INCLUDED
