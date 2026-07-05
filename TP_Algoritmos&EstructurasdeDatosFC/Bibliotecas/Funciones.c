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
int CargarSociosenArchivoBinario(const char* path)
{
    FILE* pb,*pf;
    pf = fopen(path, "rt");
    if (!pf)
        return TODO_MAL;

    char nuevo[TAM_LINEA];
    t_socio socio;
    char linea[TAM_LINEA];
    char fecha_auxiliar[11] = "";// PUEDE QUE VENGA VACIO Y HAY QUE CONTROLARLO
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
int CargarArchivoBinenArbolBinBusq(tArbol* p, const char* path, unsigned tam,int(*cmp)(const void*, const void*))
{
    FILE* pf = fopen(path,"rb");
    if(!pf)
        return TODO_MAL;

    unsigned contador = 0;
    t_socio socio;
    t_entrada_indice entrada;

    fread(&socio,sizeof(t_socio),1,pf);
    while(!feof(pf))
    {
        if(socio.estado != 'B')
        {
            entrada.clave = malloc(tam);
            if (!entrada.clave)
            {
                fclose(pf);
                return TODO_MAL;
            }
            memcpy(entrada.clave, &socio.DNI, tam);
            entrada.nro_reg = contador;

            if(insertarArbolBinBusq(p,&entrada,sizeof(t_entrada_indice),cmp) != TODO_OK)
                free(entrada.clave);
        }
        contador++;
        fread(&socio, sizeof(t_socio), 1, pf);
    }
    fclose(pf);
    return TODO_OK;
}
///***********************************************************************************************//
int AltaSocio(t_indice* ind, FILE* pf)
{
    t_socio socio;
    unsigned registros;
    int estado_condicion;
    system("cls");
    printf(CYAN "=========================================\n");
    printf("         CARGA DE UN NUEVO SOCIO\n");
    printf("=========================================\n" RESET);

    rewind(pf);

    estado_condicion = pedirDNI(ind, &socio.DNI, pf);
    if(estado_condicion == 2)
    {
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

        fseek(pf, 0, SEEK_END);

        registros = (ftell(pf) / sizeof(t_socio));

        ind_insertar(ind, &socio.DNI, registros);
        //fseek(pf, 0, SEEK_END);
        fwrite(&socio, sizeof(t_socio), 1, pf);

        printf(GREEN "\n[!] Socio cargado exitosamente.\n" RESET);
        system("pause");

    }


    return TODO_OK;
}

int pedirDNI(const t_indice* ind, unsigned* dni, FILE* pf)
{
    unsigned nro_reg;
    t_socio socio;
    int es_valido;

    printf(YELLOW "\nIngrese el DNI del Socio: " RESET);
    *dni = validarRango(DNI_MIN, DNI_MAX);
    while (getchar() != '\n');

    if (ind_buscar(ind, dni, &nro_reg) == TODO_OK)
    {
        fseek(pf, (nro_reg - 1)* sizeof(t_socio), SEEK_SET);
        fread(&socio, sizeof(t_socio), 1, pf);

        if(socio.estado == 'I')
        {
            socio.estado = 'A';
            fseek(pf, (long int)sizeof(t_socio)*(-1), SEEK_CUR);
            fwrite(&socio, sizeof(t_socio), 1, pf);
            printf(RED "\nAviso! Socio Inactivo (%s %s- %d) dado de Alta Correctamente." RESET "\n", socio.nombres, socio.apellidos, socio.DNI);
        }
        else
            printf( RED "\nError! El DNI ingresado ya pertenece a un Socio Activo." RESET);

        system("pause");
        es_valido = 1;
    }
    else
        es_valido = 2;

    return es_valido;
}

void pedirNombreoApellido(const char* mensaje, char* destino, int tam_max)
{
    int es_valido, i;
    size_t largo;
    do
    {
        es_valido = 1;
        i = 0;
        fflush(stdin);
        printf(YELLOW "Ingrese %s: " RESET, mensaje);
        fgets(destino, tam_max, stdin);

        largo = strlen(destino);
        if (largo > 0 && destino[largo - 1] == '\n')
        {
            destino[largo - 1] = '\0';
            largo--;
        }

        if (largo == 0)
        {
            fprintf(stderr, RED "Error! El campo no puede estar vacio.\n" RESET);
            es_valido = 0;
        }

        while(destino[i] != '\0' && es_valido)
        {
            if (!isalpha((unsigned char)destino[i]) && !isspace((unsigned char)destino[i]))
            {
                fprintf(stderr, RED "Error: Solo letras y espacios.\n" RESET);
                es_valido = 0;
            }
            i++;
        }
    }
    while (!es_valido);
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
        fecha->anio = validarRango(ANIO_MIN, ANIO_MAX);

        printf("\nIngrese el Mes: ");
        fecha->mes = validarRango(MES_MIN, MES_MAX);

        printf("\nIngrese el Dia: ");
        scanf("%d",&fecha->dia);

        diasLimite = diasEnMes(fecha->mes, fecha->anio);

        if (fecha->dia < 1 || fecha->dia > diasLimite)
        {
            printf("Error! Dia Invalido. El Mes %d del anio %d tiene %d dias.", fecha->mes, fecha->anio, diasLimite);
            es_valida = 0;
        }
        if (minima != NULL && es_valida)
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
    }
    while(*sexo != 'F' && *sexo != 'M' && *sexo != 'O');
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
unsigned validarRango(unsigned lim1, unsigned lim2)
{
    unsigned dato;
    do
    {
        scanf("%d",&dato);
        if(dato < lim1 || dato > lim2)
            printf("\nError! Fuera de Rango [%d - %d]... Reingrese:", lim1,lim2);
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
        printf("[1] MENOR     (0 a %d anios)\n", EDAD_MENOR_MAX);
        printf("[2] CADETE    (%d a %d anios)\n", EDAD_CADETE_MIN, EDAD_CADETE_MAX);
        printf("[3] ADULTO    (%d+ anios)\n", EDAD_ADULTO_MIN);
        printf("[4] VITALICIO (Mayores de %d anios)\n", EDAD_VITALICIO_MIN);
        printf("[5] HONORARIO (Cualquier edad)\n");
        printf("[6] JUBILADO  (Mayores de %d anios)\n", EDAD_JUBILADO_MIN);
        printf("\nSeleccione la Categoria (1-6): ");

        opcion = validarRango(1, 6);

        switch(opcion)
        {
        case 1: // MENOR
            if (edad > EDAD_MENOR_MAX)
            {
                printf("\nError: El socio es demasiado grande para ser MENOR.\n");
                es_valido = 0;
            }
            else
                strcpy(categoria, "MENOR");
            break;

        case 2: // CADETE
            if (edad < EDAD_CADETE_MIN || edad > EDAD_CADETE_MAX)
            {
                printf("\nError: La categoria CADETE es estricta para edades de 14 a 17 anios.\n");
                es_valido = 0;
            }
            else
                strcpy(categoria, "CADETE");
            break;

        case 3: // ADULTO
            if (edad < EDAD_ADULTO_MIN)
            {
                printf("\nError: El socio es menor de edad, no puede ser ADULTO.\n");
                es_valido = 0;
            }
            else
                strcpy(categoria, "ADULTO");
            break;

        case 4: // VITALICIO
            if (edad < EDAD_VITALICIO_MIN)
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
            if (edad < EDAD_JUBILADO_MIN)
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
    socio.DNI = validarRango(DNI_MIN, DNI_MAX);
    buscado.clave = &socio.DNI;
    aux = buscarNodoArbol(&(ind->arbol), &buscado, cmp);
    if (aux != NULL && *aux != NULL)
    {
        memcpy(&indice, (*aux)->info, (*aux)->tamInfo);
        fseek(pf, indice.nro_reg * sizeof(t_socio), SEEK_SET);
        fread(&socio, sizeof(t_socio), 1, pf);

        if (socio.estado == 'B')
        {
            fprintf(stderr, RED "Error! El DNI ingresado no pertenece a un socio activo.\n" RESET);
            system("pause");
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

        case 'S':
            break;

        default:
            printf(RED "\n Error! Opcion Invalida...\n" RESET);
            printf(" Presione una tecla para continuar...");
            getchar();
            getchar();;
        }

    }
    while(opcion != 'S');


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
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
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
    f.anio = (int)validarRango(ANIO_MIN, ANIO_MAX);

    printf("Ingrese mes: ");
    f.mes = validarRango(MES_MIN, MES_MAX);

    printf("Ingrese dia: ");
    f.dia = validarRango(DIA_MIN, diasEnMes(f.mes, f.anio));

    return f;
}

int listarSociosOrdenados(const t_indice* ind, FILE* pf)
{
    if (!ind || !pf || !ind->arbol)
        return TODO_MAL;

    printf(CYAN "\n========== LISTADO DE SOCIOS ORDENADOS POR DNI ==========\n" RESET);

    ind_recorrer(ind, mostrarSocioOrdenado, pf);

    printf(CYAN "=========================================================\n" RESET);
    system("pause");

    return TODO_OK;
}

void mostrarSocioOrdenado(void* info, unsigned tam, unsigned n, void* param)
{
    t_entrada_indice entrada;
    t_socio socio;
    FILE* pf = (FILE*)param;

    memcpy(&entrada, info, tam);

    fseek(pf, entrada.nro_reg * sizeof(t_socio), SEEK_SET);
    fread(&socio, sizeof(t_socio), 1, pf);

    if (socio.estado != 'B')
        mostrarSocio(&socio);
}

void mostrarSocio(const t_socio* socio)
{
    printf("\nDNI: %u", socio->DNI);
    printf("\nApellido: %s", socio->apellidos);
    printf("\nNombre: %s", socio->nombres);
    printf("\nFecha Nacimiento: %02d/%02d/%04d",
           socio->fecha_nacimiento.dia,
           socio->fecha_nacimiento.mes,
           socio->fecha_nacimiento.anio);
    printf("\nSexo: %c", socio->sexo);
    printf("\nFecha Afiliacion: %02d/%02d/%04d",
           socio->fecha_afiliacion.dia,
           socio->fecha_afiliacion.mes,
           socio->fecha_afiliacion.anio);
    printf("\nCategoria: %s", socio->categoria);
    printf("\nUltima cuota paga: %02d/%02d/%04d",
           socio->fecha_ultima_cuota.dia,
           socio->fecha_ultima_cuota.mes,
           socio->fecha_ultima_cuota.anio);
    printf("\nEstado: %c", socio->estado);
    printf("\n----------------------------------------\n");
}
///***********************************************************************************************//
int BajaSocio(t_indice* ind, FILE* pf)
{
    unsigned DniBuscado,
             nro_reg,
             RegEliminado;
    t_socio socio;

    system("cls");
    printf(CYAN "=========================================\n");
    printf("         BAJA DE UN SOCIO\n");
    printf("=========================================\n" RESET);

    printf(YELLOW "Ingrese el DNI del socio a dar de baja: " RESET);
    DniBuscado = validarRango(10000, 100000000);

    if (ind_buscar(ind, &DniBuscado, &nro_reg) == TODO_MAL)
    {
        fprintf(stderr, RED "\nError: El socio no existe o ya esta dado de baja.\n" RESET);
        system("pause");
        return TODO_MAL;
    }

    fseek(pf, nro_reg * sizeof(t_socio), SEEK_SET);
    fread(&socio, sizeof(t_socio), 1, pf);

    socio.estado = 'B';
    obtenerFechaActual(&socio.fecha_baja);

    fseek(pf, nro_reg * sizeof(t_socio), SEEK_SET);
    fwrite(&socio, sizeof(t_socio), 1, pf);

    ind_eliminar(ind, &DniBuscado, &RegEliminado);

    printf(GREEN "\n[!] El socio con DNI %u fue dado de baja exitosamente.\n" RESET, DniBuscado);
    system("pause");

    return TODO_OK;
}
///***********************************************************************************************//
int CompactarYReindexar(t_indice* ind, FILE** ppf, const char* path, int(*cmp)(const void*, const void*))
{
    FILE* pTemp;
    t_socio socio;
    char pathTemp[TAM_LINEA];

    if(!ind || !ppf || !*ppf)
        return TODO_MAL;

    system("cls");
    printf(CYAN "=========================================\n");
    printf("      COMPACTAR Y REINDEXAR\n");
    printf("=========================================\n" RESET);

    snprintf(pathTemp, sizeof(pathTemp), "%s.tmp", path);

    pTemp = fopen(pathTemp, "wb");
    if(!pTemp)
    {
        fprintf(stderr, RED "Error! No se pudo crear el archivo temporal.\n" RESET);
        return TODO_MAL;
    }

    rewind(*ppf);

    fread(&socio, sizeof(t_socio), 1, *ppf);
    while(!feof(*ppf))
    {
        if(socio.estado != 'B') // se descartan los socios dados de baja
            fwrite(&socio, sizeof(t_socio), 1, pTemp);

        fread(&socio, sizeof(t_socio), 1, *ppf);
    }

    fclose(pTemp);
    fclose(*ppf);
    *ppf = NULL;

    if(remove(path) != 0 || rename(pathTemp, path) != 0)
    {
        fprintf(stderr, RED "Error! No se pudo reemplazar el archivo de datos.\n" RESET);
        return TODO_MAL;
    }

    *ppf = fopen(path, "r+b");
    if(!*ppf)
    {
        fprintf(stderr, RED "Error! No se pudo reabrir el archivo de datos.\n" RESET);
        return TODO_MAL;
    }

    ind_vaciar(ind);
    if(!CargarArchivoBinenArbolBinBusq(&ind->arbol, path, ind->tamClave, cmp))
    {
        fprintf(stderr, RED "Error! No se pudo reconstruir el indice.\n" RESET);
        return TODO_MAL;
    }

    printf(GREEN "\n[!] Compactacion y reindexado realizados exitosamente.\n" RESET);
    system("pause");

    return TODO_OK;
}
///***********************************************************************************************//
