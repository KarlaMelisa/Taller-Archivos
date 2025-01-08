#include <stdio.h>
#include "funciones.h"

int main (int argc, char *argv[]) {


    struct Factura factura={0};
    int opc=0, opc2=0, cedula=0;

    do{
        opc=menu();
        switch (opc)
        {
        case 1:
            crearFactura();
            break;
        case 2:
            leerFactura();
            break;
        case 3:
            printf("---------------EDICION DE FACTURA---------------\nIngrese la cedula de la factura a editar: \n>> ");
            scanf("%d", &cedula);
            update(cedula);
            break;
        case 4:
            printf("-------------ELIMINACION DE FACTURA-------------\nIngrese la cedula de la factura a eliminar: \n>> ");
            scanf("%d", &cedula);
            borrarFactura(cedula);
            break;
        default:
            printf("Opcion no valida\n");
            break;
        }
        printf("Desea volver al menu de seleccion? 1. Si 2. No (salir)\n>> ");
        scanf("%d", &opc2);
    } while (opc2 == 1);
    
    return 0;
}