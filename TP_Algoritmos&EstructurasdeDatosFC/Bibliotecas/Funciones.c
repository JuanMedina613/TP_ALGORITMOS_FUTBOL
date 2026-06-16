#include "Funciones.h"
///***********************************************************************************************//
char SeleccionarMenu()
{
    char decision;
    char opciones[]= "ABMLCS";
    do
    {
        printf("\n ================ MENU ================");
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
int CargarSocios(const char* path)
{
    FILE* pb,*pf;
    pf = fopen(path, "rt");
    if (!pf)
        return TODO_MAL;

    char nuevo[TAM_LINEA];
    strcpy(nuevo, path);
    char* punto = strrchr(nuevo, '.');
    if (!punto)
    {
        fclose(pf);
        return TODO_MAL;
    }
    strcpy(punto, ".dat");
    pb = fopen(nuevo, "wb");
    if(!pb)
    {
        fclose(pf);
        return TODO_MAL;
    }

    t_socio socio;
    char linea[TAM_LINEA];
    char fecha_auxiliar[11] = "";// PUEDE QUE VENGA VACIO Y HAY QUE CONTROLARLO
    while (fgets(linea, sizeof(linea), pf))
    {

        sscanf(linea,
            "%d,%59[^,],%59[^,],"       // DNI, apellidos, nombres
            "%d/%d/%d,"                  // fecha_nacimiento
            "%c,"                        // sexo
            "%d/%d/%d,"                  // fecha_afiliacion
            "%9[^,],"                    // categoria
            "%d/%d/%d,"                  // fecha_ultima_cuota
            "%c,"                        // estado
            "%10[^\n]",                  // fecha_baja (puede ser vacío)
            &socio.DNI,
            socio.apellidos,
            socio.nombres,
            &socio.fecha_nacimiento.dia, &socio.fecha_nacimiento.mes, &socio.fecha_nacimiento.anio,
            &socio.sexo,
            &socio.fecha_afiliacion.dia, &socio.fecha_afiliacion.mes, &socio.fecha_afiliacion.anio,
            socio.categoria,
            &socio.fecha_ultima_cuota.dia, &socio.fecha_ultima_cuota.mes, &socio.fecha_ultima_cuota.anio,
            &socio.estado,
            fecha_auxiliar);

        // fecha_baja: si no es A la fecha existe.
        if (socio.estado != 'A')
            sscanf(fecha_auxiliar,"%d/%d/%d", &socio.fecha_baja.dia, &socio.fecha_baja.mes,&socio.fecha_baja.anio);
        else
            socio.fecha_baja.dia = socio.fecha_baja.mes = socio.fecha_baja.anio = 0;
        fwrite(&socio,sizeof(t_socio),1,pb);
    }

    fclose(pf);
    fclose(pb);
    return TODO_OK;
}
///***********************************************************************************************//
