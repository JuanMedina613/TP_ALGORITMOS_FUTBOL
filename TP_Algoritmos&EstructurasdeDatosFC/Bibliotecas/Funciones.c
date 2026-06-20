#include "Funciones.h"
///***********************************************************************************************//
char SeleccionarMenu()
{
    char decision;
    char opciones[] = "ABMLCS";

    do
    {
        system("cls");

        printf(YELLOW " ================ MENU ================\n" RESET);
        printf(GREEN " [A] " RESET "Alta a un Socio.\n");
        printf(GREEN " [B] " RESET "Baja a un Socio.\n");
        printf(GREEN " [M] " RESET "Modificar Socio.\n");
        printf(GREEN " [L] " RESET "Listar todos los Socios Ordenados.\n");
        printf(GREEN " [C] " RESET "Compactar y Reindexar.\n");
        printf(RED   " [S] " RESET "Salir.\n");
        printf(YELLOW " =======================================\n" RESET);
        printf(" Seleccione una Opcion: ");

        scanf(" %c", &decision);
        decision = toupper(decision);

        if(strchr(opciones, decision) == NULL)
        {
            printf(RED "\n Error! Opcion Invalida...\n" RESET);
            printf(" Presione una tecla para continuar...");
            getchar();
            getchar();
        }
    }
    while(strchr(opciones, decision) == NULL);
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
int AltaSocio(t_indice* ind, FILE* pf, int(*cmp)(const void*, const void*))
{
    t_socio socio;
    unsigned registros;

    system("cls");
    printf(CYAN "=========================================\n");
    printf("         CARGA DE UN NUEVO SOCIO\n");
    printf("=========================================\n" RESET);

    rewind(pf);
    pedirDNI(&ind->arbol, &socio.DNI, pf, cmp);
    pedirNombreoApellido("Nombres", socio.nombres, sizeof(socio.nombres));
    pedirNombreoApellido("Apellidos", socio.apellidos, sizeof(socio.apellidos));
    pedirFecha("Nacimiento", &socio.fecha_nacimiento, NULL);
    pedirSexo(&socio.sexo);

    obtenerFechaActual(&socio.fecha_afiliacion);
    socio.fecha_ultima_cuota = socio.fecha_afiliacion;

    pedirCategoria(socio.categoria, &socio.fecha_nacimiento, &socio.fecha_afiliacion);

    socio.estado = 'A';
    socio.fecha_baja.dia = 0;
    socio.fecha_baja.mes = 0;
    socio.fecha_baja.anio = 0;

    registros = (ftell(pf) / sizeof(t_socio)) + 1;

    ind_insertar(ind, &socio.DNI, registros);
    fseek(pf, 0, SEEK_END);
    fwrite(&socio, sizeof(t_socio), 1, pf);

    printf(GREEN "\n[!] Socio cargado exitosamente.\n" RESET);
    system("pause");

    return TODO_OK;
}

void pedirDNI(const tArbol* p, unsigned* dni, FILE* pf, int(*cmp)(const void*, const void*))
{
    tNodoArbol** aux;
    t_entrada_indice indice;
    t_socio socio;
    int es_valido;

    do
    {
        es_valido = 1;
        printf(YELLOW "\nIngrese el DNI del Socio: " RESET);
        *dni = validarRango(10000, 100000000);

        aux = buscarNodoArbol(p, dni, cmp);
        if (aux != NULL && *aux != NULL)
        {
            memcpy(&indice, (*aux)->info, (*aux)->tamInfo);
            fseek(pf, indice.nro_reg * sizeof(t_socio), SEEK_SET);
            fread(&socio, sizeof(t_socio), 1, pf);

            if (socio.estado != 'B')
            {
                printf(RED "Error! El DNI ingresado ya pertenece a un socio activo.\n" RESET);
                es_valido = 0;
            }
        }
    } while (es_valido == 0);
}

void pedirNombreoApellido(const char* mensaje, char* destino, int tam_max)
{
    int es_valido, i;
    size_t largo;
    do
    {
        es_valido = 1; i = 0;
        printf(YELLOW "Ingrese %s: " RESET, mensaje);
        fgets(destino, tam_max, stdin);

        largo = strlen(destino);
        if (largo > 0 && destino[largo - 1] == '\n') { destino[largo - 1] = '\0'; largo--; }

        if (largo == 0) { printf(RED "Error! El campo no puede estar vacio.\n" RESET); es_valido = 0; }

        while(destino[i] != '\0' && es_valido)
        {
            if (!isalpha((unsigned char)destino[i]) && !isspace((unsigned char)destino[i]))
            {
                printf(RED "Error: Solo letras y espacios.\n" RESET);
                es_valido = 0;
            }
            i++;
        }
    } while (!es_valido);
}
///***********************************************************************************************//
void pedirFecha(const char* mensaje, t_fecha* fecha, t_fecha* minima)
{
    int es_valida;
    int diasLimite;
    do
    {
        es_valida = 1;
        printf("\nCarga de la Fecha de %s", mensaje);
        printf("\nIngrese el Anio: ");
        fecha->mes = validarRango(1900,2026);
        printf("\nIngrese el Mes: ");
        fecha->mes = validarRango(1,12);
        printf("\nIngrese el Dia: ");
        scanf("%d",&fecha->dia);

        switch (fecha->mes)
        {
        case 4:
        case 6:
        case 9:
        case 11:
            diasLimite = 30;
            break;
        case 2:
            if ((fecha->anio % 4 == 0 && fecha->anio % 100 != 0) || (fecha->anio % 400 == 0))
                diasLimite = 29;
            else
                diasLimite = 28;
            break;
        default:
            diasLimite = 31;
        }
        if (fecha->dia < 1 || fecha->dia > diasLimite)
        {
            printf("Error! Dia Invalido. El Mes %d del anio %d tiene %d dias.", fecha->mes, fecha->anio, diasLimite);
            es_valida = 0;
        }
        if (minima != NULL)
        {
            if (esFechaMenor(fecha, minima))
            {
                printf("\nError! La fecha de %s no puede ser anterior a la fecha de Nacimiento.", mensaje);
                es_valida = 0;
            }
        }
    }
    while (!es_valida);
}
///***********************************************************************************************//
int esFechaMenor(const t_fecha* f1, const t_fecha* f2)
{
    if (f1->anio != f2->anio)
        return f1->anio < f2->anio;
    if (f1->mes != f2->mes)
        return f1->mes < f2->mes;
    return f1->dia < f2->dia;
}
///***********************************************************************************************//
void pedirSexo(char* sexo)
{
    do
    {
        printf(YELLOW "Ingrese el Sexo (F/M/O): " RESET);
        scanf(" %c", sexo);
        *sexo = toupper(*sexo);
        if(*sexo != 'F' && *sexo != 'M' && *sexo != 'O')
            printf(RED "Error! Sexo Invalido.\n" RESET);
    } while(*sexo != 'F' && *sexo != 'M' && *sexo != 'O');
}
///***********************************************************************************************//
void obtenerFechaActual(t_fecha* hoy)
{
    time_t tiempo_crudo = time(NULL);

    struct tm* tiempo_local = localtime(&tiempo_crudo);

    hoy->dia = tiempo_local->tm_mday;
    hoy->mes = tiempo_local->tm_mon + 1;       // Hay que sumarle 1 al mes
    hoy->anio = tiempo_local->tm_year + 1900;  // Hay que sumarle 1900 al año
}
///***********************************************************************************************//
int validarRango(int lim1, int lim2)
{
    int dato;
    do
    {
        scanf("%d",&dato);
        if(dato < lim1 || dato > lim2)
            printf("\nError! Numero Ingresado Fuera de Rango... Reingrese:");
    }
    while(dato < lim1 || dato > lim2);
    return dato;
}
///***********************************************************************************************//
unsigned validarPositivo(unsigned lim1, unsigned lim2)
{
    unsigned dato;
    do
    {
        scanf("%u",&dato);
        if(dato < lim1 || dato > lim2)
            printf("\nError! Numero Ingresado Fuera de Rango... Reingrese:");
    }
    while(dato < lim1 || dato > lim2);
    return dato;
}
///***********************************************************************************************//
void pedirCategoria(char* categoria, const t_fecha* nacimiento, const t_fecha* hoy)
{
    int opcion;
    int es_valido;

    int edad = calcularEdad(nacimiento, hoy);

    do
    {
        es_valido = 1;

        printf("\n--- MENU DE CATEGORIAS ---");
        printf("\nEdad calculada del socio: %d anios\n", edad);
        printf("[1] MENOR     (0 a 13 anios)\n");
        printf("[2] CADETE    (14 a 17 anios)\n");
        printf("[3] ADULTO    (18+ anios)\n");
        printf("[4] VITALICIO (Mayores de 50 anios)\n");
        printf("[5] HONORARIO (Cualquier edad)\n");
        printf("[6] JUBILADO  (Mayores de 60 anios)\n");
        printf("\nSeleccione la Categoria (1-6): ");

        opcion = validarRango(1, 6);

        switch(opcion)
        {
        case 1: // MENOR
            if (edad > 13)
            {
                printf("\nError: El socio es demasiado grande para ser MENOR.\n");
                es_valido = 0;
            }
            else
                strcpy(categoria, "MENOR");
            break;

        case 2: // CADETE
            if (edad < 14 || edad > 17)
            {
                printf("\nError: La categoria CADETE es estricta para edades de 14 a 17 anios.\n");
                es_valido = 0;
            }
            else
                strcpy(categoria, "CADETE");
            break;

        case 3: // ADULTO
            if (edad < 18)
            {
                printf("\nError: El socio es menor de edad, no puede ser ADULTO.\n");
                es_valido = 0;
            }
            else
                strcpy(categoria, "ADULTO");
            break;

        case 4: // VITALICIO
            if (edad < 50)
            {
                printf("\nError: El socio es muy joven para ser VITALICIO.\n");
                es_valido = 0;
            }
            else
                strcpy(categoria, "VITALICIO");
            break;

        case 5: // HONORARIO
            strcpy(categoria, "HONORARIO");
            break;

        case 6: // JUBILADO
            if (edad < 60)
            {
                printf("\nError: El socio no tiene edad de JUBILADO.\n");
                es_valido = 0;
            }
            else
                strcpy(categoria, "JUBILADO");
        }

    }
    while (es_valido == 0);
}
///***********************************************************************************************//
int calcularEdad(const t_fecha* nacimiento, const t_fecha* hoy)
{
    int edad = hoy->anio - nacimiento->anio;
    if (hoy->mes < nacimiento->mes || (hoy->mes == nacimiento->mes && hoy->dia < nacimiento->dia))
        edad--;
    return edad;
}
///***********************************************************************************************//
int CmpDNI(const void* a, const void* b)
{
    const t_entrada_indice* indice_a = (const t_entrada_indice*)a;
    const t_entrada_indice* indice_b = (const t_entrada_indice*)b;

    unsigned* dni_a = (unsigned*)(indice_a->clave);
    unsigned* dni_b = (unsigned*)(indice_b->clave);
    if (*dni_a > *dni_b)
        return 1;
    if (*dni_a < *dni_b)
        return -1;

    return 0;
}
///***********************************************************************************************//
int modificarSocio(t_indice* ind, FILE* pf, int(*cmp)(const void*, const void*))
{
    t_socio socio;
    t_fecha hoy;
    char opcion;
    tNodoArbol** aux;
    t_entrada_indice indice;
    t_entrada_indice buscado;

    system("cls");
    printf(CYAN "=========================================\n");
    printf("         MODIFICAR SOCIO\n");
    printf("=========================================\n" RESET);

    puts("Ingrese el DNI del socio a modificar: ");
    socio.DNI = validarPositivo(10000, 100000000);
    buscado.clave = &socio.DNI;
    aux = buscarNodoArbol(&(ind->arbol), &buscado, cmp);
    if (aux != NULL && *aux != NULL)
    {
        memcpy(&indice, (*aux)->info, (*aux)->tamInfo);
        fseek(pf, indice.nro_reg * sizeof(t_socio), SEEK_SET);
        fread(&socio, sizeof(t_socio), 1, pf);

        if (socio.estado == 'B')
        {
            printf(RED "Error! El DNI ingresado no pertenece a un socio activo.\n" RESET);
            return TODO_MAL;
        }
        }
        else
        {
            printf(RED "Error! El DNI ingresado no pertenece a un socio activo.\n" RESET);
            return TODO_MAL;
        }

    do
    {
        system("cls");

        printf(YELLOW " ================ MENU ================\n" RESET);
        printf(GREEN " [A] " RESET "Apellido.\n");
        printf(GREEN " [B] " RESET "Nombre.\n");
        printf(GREEN " [C] " RESET "Categoria.\n");
        printf(GREEN " [D] " RESET "Sexo.\n");
        printf(GREEN " [E] " RESET "Fecha de ultima cuota paga.\n");
        printf(RED   " [S] " RESET "Salir.\n");
        printf(YELLOW " =======================================\n" RESET);
        printf(" Seleccione un campo a modificar: ");

        scanf("%c", &opcion);
        opcion = toupper(opcion);

        switch (opcion)
        {
            case 'A':
            pedirNombreoApellido("Apellidos", socio.apellidos, sizeof(socio.apellidos));
            break;

            case 'B':
            pedirNombreoApellido("Nombres", socio.nombres, sizeof(socio.nombres));
            break;

            case 'C':
            obtenerFechaActual(&hoy);
            pedirCategoria(socio.categoria, &socio.fecha_nacimiento, &hoy);
            break;

            case 'D':
            pedirSexo(&socio.sexo);
            break;

            case 'E':
            puts("\nIngrese la nueva fecha de ultima cuota paga: ");
            socio.fecha_ultima_cuota = validarFecha();
            break;

            case 'S': break;

            default:
                printf(RED "\n Error! Opcion Invalida...\n" RESET);
                printf(" Presione una tecla para continuar...");
                getchar();
                getchar();;
        }

    }while(opcion != 'S');


    fseek(pf, (long int)(-sizeof(t_socio)), SEEK_CUR);
    fwrite(&socio, sizeof(t_socio), 1, pf);

    printf(GREEN "\n[!] Socio modificado exitosamente.\n" RESET);
    system("pause");

    return TODO_OK;
}
///***********************************************************************************************//
int esBisiesto(int anio)
{
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

int diasEnMes(int mes, int anio)
{
    switch(mes)
    {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return esBisiesto(anio) ? 29 : 28;
        default:
            return 0;
    }
}

t_fecha validarFecha()
{
    t_fecha f;
    printf("Ingrese anio: ");
    f.anio = validarRango(1900, 2100);

    printf("Ingrese mes: ");
    f.mes = validarRango(1, 12);

    printf("Ingrese dia: ");
    f.dia = validarRango(1, diasEnMes(f.mes, f.anio));

    return f;
}
///***********************************************************************************************//
///***********************************************************************************************//
///***********************************************************************************************//
