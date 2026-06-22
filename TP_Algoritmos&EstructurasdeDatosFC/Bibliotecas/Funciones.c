#include "Funciones.h"
///***********************************************************************************************//
char SeleccionarMenu()
{
    char decision;
    char opciones[] = "ABMLCS";

    do
    {

        printf(YELLOW " \n================ MENU ================\n" RESET);
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
    system("cls");
    return decision;

}
///***********************************************************************************************//
int CargarSociosenArchivoBinario(const char* path)
{
    FILE* pb,*pf;
    pf = fopen(path, "rt");
    if (!pf) return TODO_MAL;

    char nuevo[TAM_LINEA];
    strcpy(nuevo, path);
    char* punto = strrchr(nuevo, '.');

    if (!punto){
        fclose(pf);
        return TODO_MAL;
    }
    strcpy(punto, ".dat");
    pb = fopen(nuevo, "wb");
    if(!pb){
        fclose(pf);
        return TODO_MAL;
    }
    t_socio socio;
    char linea[TAM_LINEA];
    char fecha_auxiliar[11] = "";// PUEDE QUE VENGA VACIO Y HAY QUE CONTROLARLO
    int campos_leidos;
    while (fgets(linea, sizeof(linea), pf))
    {
        strcpy(fecha_auxiliar, "");
        campos_leidos = sscanf(linea,
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

        if (campos_leidos >= 15)
        {
            // fecha_baja: si no es A la fecha existe.
            if (socio.estado != 'A')
                sscanf(fecha_auxiliar,"%d/%d/%d", &socio.fecha_baja.dia, &socio.fecha_baja.mes,&socio.fecha_baja.anio);
            else
                socio.fecha_baja.dia = socio.fecha_baja.mes = socio.fecha_baja.anio = 0;
            fwrite(&socio, sizeof(t_socio), 1, pb);
        }
    }
    fclose(pf);
    fclose(pb);
    return TODO_OK;
}
///***********************************************************************************************//
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
        entrada.clave = malloc(tam);
        if (!entrada.clave)
        {
            fclose(pf);
            return TODO_MAL;
        }
        memcpy(entrada.clave,&socio.DNI, sizeof(tam));
        entrada.nro_reg = contador;
        contador++;
        insertarArbolBinBusq(p,&entrada,sizeof(t_entrada_indice),cmp);
        fread(&socio,sizeof(t_socio),1,pf);
    }
    fclose(pf);
    return TODO_OK;
}
///***********************************************************************************************//
int AltaSocio(t_indice* ind, FILE* pf, int(*cmp)(const void*, const void*))
{
    t_socio socio;
    unsigned registros, estado_socio;
    system("cls");
    printf(CYAN "=========================================\n");
    printf("         CARGA DE UN NUEVO SOCIO\n");
    printf("=========================================\n" RESET);

    rewind(pf);
    estado_socio = pedirDNI(&ind->arbol, &socio.DNI, pf, ind->cmp);
    switch(estado_socio)
    {
    case 1:
    case 3:
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
        registros = (ftell(pf) / sizeof(t_socio)) + 1;

        if(!ind_insertar(ind, &socio.DNI, registros))
            return TODO_MAL;

        fwrite(&socio, sizeof(t_socio), 1, pf);

        printf(GREEN "\n[!] Socio cargado exitosamente.\n" RESET);

        break;
    case 2:
        printf(GREEN "\n[!] Socio Inactivo Dado de Alta exitosamente." RESET);
    }
    system("pause");

    return TODO_OK;
}
///***********************************************************************************************//
int ModificarSocio(t_indice* ind, FILE* pf, int(*cmp)(const void*, const void*))
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
    socio.DNI = validarRango(10000, 100000000);
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
            socio.fecha_ultima_cuota = validarFecha(); ///TAI TENEMOS QUE HABLAR///....
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
int ListarSociosOrdenados(const t_indice* ind, FILE* pf)
{
    if (!ind || !pf || !ind->arbol)
        return TODO_MAL;

    printf(CYAN "\n========== LISTADO DE SOCIOS ORDENADOS POR DNI ==========\n" RESET);

    ind_recorrer(ind, mostrarSocioOrdenado, pf);

    printf(CYAN "=========================================================\n" RESET);
    system("pause");

    return TODO_OK;
}
///***********************************************************************************************//
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
///***********************************************************************************************//
int pedirDNI(const tArbol* p, unsigned* dni, FILE* pf, int(*cmp)(const void*, const void*))
{
    tNodoArbol** aux;
    t_entrada_indice indice_busqueda;
    t_socio socio;
    int es_valido;

    /// Pienso la logica de la siguiente manera: DNI es A->0, B->1, I->2, NULL->3
    do{
        printf(YELLOW "\nIngrese el DNI del Socio: " RESET);
        *dni = validarRango(10000, 100000000);

        indice_busqueda.clave = dni;
        aux = buscarNodoArbol(p, &indice_busqueda, cmp);

        if(aux != NULL)
        {
            memcpy(&indice_busqueda, (*aux)->info, (*aux)->tamInfo);

            fseek(pf, (indice_busqueda.nro_reg - 1) * sizeof(t_socio), SEEK_SET);
            fread(&socio, sizeof(t_socio), 1, pf);

            switch(socio.estado)
            {
            case 'A':
                printf(RED "\nError! El DNI ingresado ya pertenece a un socio Activo." RESET);
                es_valido = 0;
                break;
            case 'B':
                es_valido = 1;
                break;
            case 'I':
                socio.estado = 'A';
                fseek(pf, (long)sizeof(t_socio)*(-1), SEEK_CUR);
                fwrite(&socio, sizeof(t_socio), 1, pf);
                es_valido = 2;
            }
        }
        else
            es_valido = 3;
    }while(es_valido == 0);
    return es_valido;
}
///***********************************************************************************************//
void pedirNombreoApellido(const char* mensaje, char* destino, int tam_max)
{
    int es_valido, i,c;
    size_t largo;

    do
    {
        es_valido = 1;
        i = 0;
        printf(YELLOW "Ingrese %s: " RESET, mensaje);

        fgets(destino, tam_max, stdin);
        if (destino[0] == '\n')
            fgets(destino, tam_max, stdin);

        largo = strlen(destino);
        if (largo > 0 && destino[largo - 1] == '\n')
        {
            destino[largo - 1] = '\0';
            largo--;
        }
        else
        {
            while ((c = getchar()) != '\n' && c != EOF);
        }
        if (largo == 0)
        {
            printf(RED "\nError! El campo no puede estar vacio." RESET);
            es_valido = 0;
        }
        while(destino[i] != '\0' && es_valido)
        {
            if (!isalpha((unsigned char)destino[i]) && !isspace((unsigned char)destino[i]))
            {
                printf(RED "\nError! Solo letras y espacios." RESET);
                es_valido = 0;
            }
            i++;
        }
    }
    while(!es_valido);
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
        fecha->anio = validarRango(1900,2026);
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
            if (esBisiesto(fecha->anio))
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
void obtenerFechaActual(t_fecha* hoy)
{
    time_t tiempo_crudo = time(NULL);

    struct tm* tiempo_local = localtime(&tiempo_crudo);

    hoy->dia = tiempo_local->tm_mday;
    hoy->mes = tiempo_local->tm_mon + 1;       // Hay que sumarle 1 al mes
    hoy->anio = tiempo_local->tm_year + 1900;  // Hay que sumarle 1900 al año
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
unsigned validarRango(unsigned lim1, unsigned lim2)
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
int esFechaMenor(const t_fecha* f1, const t_fecha* f2)
{
    if (f1->anio != f2->anio)
        return f1->anio < f2->anio;
    if (f1->mes != f2->mes)
        return f1->mes < f2->mes;
    return f1->dia < f2->dia;
}
///***********************************************************************************************//
int esBisiesto(int anio)
{
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}
///***********************************************************************************************//
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
///***********************************************************************************************//
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
int CmpDNI(const void* a, const void* b)///es la funcion de comparacion del TDA Indice
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
///***********************************************************************************************//
///***********************************************************************************************//
