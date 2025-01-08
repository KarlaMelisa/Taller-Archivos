#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu(){
    int opcion=0;
    printf("------------------MENU DE SELECCION------------------\n1. Crear factura\n");
    printf("2. Leer facturas\n");
    printf("3. Editar factura\n");
    printf("4. Eliminar factura\n>> ");
    scanf("%d", &opcion);
    return opcion;
}

void save(struct Factura *factura){
    FILE *file;
    file = fopen("facturas.dat", "ab+"); //en binario ponemos .dat y en texto .txt
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }else{
        fwrite(factura, sizeof(struct Factura), 1, file);
        printf("Factura guardada\n");
    }
    fclose(file);
}

void leerCadena(char *cadena, int numCaracteres) {
    fflush(stdin);
    fgets(cadena, numCaracteres, stdin);
    cadena[strcspn(cadena, "\n")] = '\0'; // Elimina el salto de línea
}


void crearFactura(){
    struct Factura factura;
    factura.total=0;
    FILE *file;
    file = fopen("facturas.dat", "ab+"); 
    printf("---------------INGRESO DE FACTURA---------------\nDATOS:\n");
    printf("Nombre del cliente: \n>> ");
    leerCadena(factura.nombre, 50);
    printf("Cedula del cliente: \n>> ");
    scanf("%d", &factura.cedula);
    printf("\nDETALLE DE COMPRA: \n");

    do {
        printf("Numero de productos (max 5): \n>> ");
        scanf("%d", &factura.numProd);
        if (factura.numProd > 5 || factura.numProd <= 0) {
            printf("Error: Valor invalido. Debe ingresar un numero entre 1 y 5.\n");
        }
    } while (factura.numProd > 5 || factura.numProd <= 0);

    for (int j = 0; j < factura.numProd; j++)
    {
        printf("Nombre producto %d: \n>> ",j+1);
        leerCadena(factura.productos[j].nombre, 50);
        printf("Cantidad producto %d: \n>> ",j+1);
        scanf("%d", &factura.productos[j].cantidad);
        printf("Precio producto %d: \n>> $",j+1); 
        scanf("%f", &factura.productos[j].precio);
        factura.total += factura.productos[j].cantidad * factura.productos[j].precio;
    }
    
    factura.estado= 1;
    save(&factura);
    fclose(file);
}

int encontrarFacturaByCedula(int cedula) {
    int f = 0, pos = -1;
    FILE *file = fopen("facturas.dat", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return -1;
    }

    struct Factura factura;
    while (fread(&factura, sizeof(struct Factura), 1, file) == 1) {
        if (factura.cedula == cedula) {
            f = 1;
            printf("Factura encontrada\n");
            printf("Nombre: %s\n", factura.nombre);
            printf("Cedula: %d\n", factura.cedula);
            printf("\nPRODUCTOS:\n");
            printf("-------------------------------------------------\n");
            printf("| %-20s | %-10s | %-10s |\n", "Nombre", "Cantidad", "Precio");
            printf("-------------------------------------------------\n");

            for (int i = 0; i < factura.numProd; i++) {
                printf("| %-20s | %-10d | $%-9.2f |\n", 
                                                        factura.productos[i].nombre, 
                                                        factura.productos[i].cantidad, 
                                                        factura.productos[i].precio);
            }

            printf("-------------------------------------------------\n");
            printf("| %-20s   %-10s | $%-9.2f |\n", "TOTAL", "", factura.total);
            printf("-------------------------------------------------\n");
        }
    }

    if (f == 0) {
        printf("Factura no encontrada\n");
    }

    fclose(file);
    return pos; // Devuelve la posición del registro encontrado o -1 si no se encontró
}


void leerFactura() {
    FILE *file;
    struct Factura factura;
    file = fopen("facturas.dat", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
        printf("------------LISTA DE FACTURAS------------\n\nCEDULA      NOMBRE               TOTAL\n");
        printf("----------------------------------------\n");
        while (fread(&factura, sizeof(struct Factura), 1, file)) {
            if (factura.estado == 1) { // Solo imprime facturas activas
            printf("%-10d %-20s %-10.2f\n", factura.cedula, factura.nombre, factura.total);
            }
        }

        int opc, cedula;
        do{
            printf("\nDesea ver el detalle de alguna factura? 1. Si 2. No\n>> ");
            scanf("%d", &opc);
            if (opc == 1) {
                printf("Ingrese la cedula de la factura a buscar: \n>> ");
                scanf("%d", &cedula);
                encontrarFacturaByCedula(cedula);
            } else if (opc != 2) {
                printf("Opcion no valida.\n");
            }
        } while (opc != 2);
        
    fclose(file);
}

void update(int cedula) {
    FILE *file = fopen("facturas.dat", "r+b");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    struct Factura factura;
    int encontrado = 0;
    long pos = -1;

    while (fread(&factura, sizeof(struct Factura), 1, file) == 1) {
        if (factura.cedula == cedula) {
            encontrado = 1;
            pos = ftell(file) - sizeof(struct Factura); // Calcula la posición del inicio del registro
            break;
        }
    }

    if (!encontrado) {
        printf("Factura con cedula %d no encontrada.\n", cedula);
        fclose(file);
        return;
    }

    // Solicita los nuevos datos de la factura
    printf("Factura encontrada. \n--------------NUEVO INGRESO DE FACTURA--------------\nDATOS:\n");
    printf("Nombre del cliente: \n>> ");
    leerCadena(factura.nombre, 50);
    printf("Cedula del cliente: \n>> ");
    scanf("%d", &factura.cedula);
    printf("\nDETALLE DE COMPRA:\n");
    do {
        printf("Numero de productos (max 5): \n>> ");
        scanf("%d", &factura.numProd);
        if (factura.numProd > 5 || factura.numProd <= 0) {
            printf("Error: Valor invalido. Debe ingresar un numero entre 1 y 5.\n");
        }
    } while (factura.numProd > 5 || factura.numProd <= 0);

    factura.total = 0; // Reinicia el total para recalcular
    for (int i = 0; i < factura.numProd; i++) {
        printf("Nombre producto %d: \n>> ", i + 1);
        leerCadena(factura.productos[i].nombre, 50);
        printf("Cantidad producto %d: \n>> ", i + 1);
        scanf("%d", &factura.productos[i].cantidad);
        printf("Precio producto %d: \n>> $", i + 1);
        scanf("%f", &factura.productos[i].precio);
        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }

    if (fseek(file, pos, SEEK_SET) != 0) {
        printf("Error al posicionar el puntero del archivo.\n");
        fclose(file);
        return;
    }

    // Escribe la factura actualizada en el archivo
    if (fwrite(&factura, sizeof(struct Factura), 1, file) != 1) {
        printf("Error al escribir la factura en el archivo.\n");
    } else {
        printf("Factura actualizada correctamente.\n");
    }

    fclose(file);
}

void borrarFactura(int cedula){
    FILE *file = fopen("facturas.dat", "r+b");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    struct Factura factura;
    int encontrado = 0;
    long pos = -1;

    while (fread(&factura, sizeof(struct Factura), 1, file) == 1) {
        if (factura.cedula == cedula) {
            encontrado = 1;
            pos = ftell(file) - sizeof(struct Factura); // Calcula la posición del inicio del registro
            break;
        }
    }

    if (!encontrado) {
        printf("Factura con cedula %d no encontrada.\n", cedula);
        fclose(file);
        return;
    }

    if (fseek(file, pos, SEEK_SET) != 0) {
        printf("Error al posicionar el puntero del archivo.\n");
        fclose(file);
        return;
    }
    int opc=0;
    do{
        printf("Factura encontrada. \nEsta seguro de eliminarla? 1. Si 2. No\n>> ");
        scanf("%d", &opc);
        // Cambiar el estado de la factura a 0
        if (opc==1){
            factura.estado = 0;
            // Actualiza factura en el archivo
            if (fwrite(&factura, sizeof(struct Factura), 1, file) != 1) {
                printf("Error al escribir la factura en el archivo.\n");
            } else {
                printf("Factura eliminada correctamente.\n");
            }
        }
        if (opc==2){
            printf("Operacion cancelada\n");
        }
        if(opc!=1 && opc!=2){
            printf("Opcion no valida\n");
        }
    } while (opc!=1 && opc!=2);

    fclose(file); // Cierra el archivo
}