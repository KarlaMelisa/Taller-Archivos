struct Producto {
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura{
    char nombre[50];
    int cedula;
    struct Producto productos[5];
    int numProd;
    float total;
    int estado;
    int numFactura;
};

int menu();
void save(struct Factura *factura);
void leerCadena(char *cadena, int numCaracteres);
void crearFactura();
void leerFactura();
int encontrarFacturaByCedula(int cedula);
void update(int cedula);
void borrarFactura(int cedula);