#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cliente {
    int numeroCuenta;
    int tipoCuenta; // 1 para preferencial, 2 para tradicional
    struct Cliente* sig;
} Cliente;

typedef struct Comprobante {
    int numeroCuenta;
    char descripcion[100];
    double monto; // Monto de la transaccion
    char fecha[10]; // Fecha de la transaccion en dd/mm/aa
    struct Comprobante* sig;
} Comprobante;

Cliente* filaPreferencial = NULL;
Cliente* filaTradicional = NULL;
Comprobante* comprobantes = NULL;

void registrarCliente(int numeroCuenta, int tipoCuenta) {
    Cliente* nuevoCliente = (Cliente*) malloc(sizeof(Cliente));
    nuevoCliente->numeroCuenta = numeroCuenta;
    nuevoCliente->tipoCuenta = tipoCuenta;
    nuevoCliente->sig = NULL;

    if (tipoCuenta == 1) {
        if (filaPreferencial == NULL) {
            filaPreferencial = nuevoCliente;
        } else {
            Cliente* aux = filaPreferencial;
            while (aux->sig != NULL) {
                aux = aux->sig;
            }
            aux->sig = nuevoCliente;
        }
    } else if (tipoCuenta == 2) {
        if (filaTradicional == NULL) {
            filaTradicional = nuevoCliente;
        } else {
            Cliente* aux = filaTradicional;
            while (aux->sig != NULL) {
                aux = aux->sig;
            }
            aux->sig = nuevoCliente;
        }
    }

    // Verificar la condicion de transferencia
    Cliente* aux = filaTradicional;
    int count = 0;
    while (aux != NULL) {
        count++;
        aux = aux->sig;
    }
    if (count >= 5) {
        // Mover al tercer cliente de la fila tradicional a la fila preferencial
        Cliente* tercero = filaTradicional;
        for (int i = 0; i < 2; i++) {
            tercero = tercero->sig;
        }
        tercero->tipoCuenta = 1;
        // Eliminar al tercer cliente de la fila tradicional
        Cliente* anterior = filaTradicional;
        for (int i = 0; i < 1; i++) {
            anterior = anterior->sig;
        }
        anterior->sig = tercero->sig;
        // Agregar al tercer cliente a la fila preferencial
        if (filaPreferencial == NULL) {
            filaPreferencial = tercero;
        } else {
            Cliente* aux = filaPreferencial;
            while (aux->sig != NULL) {
                aux = aux->sig;
            }
            aux->sig = tercero;
        }
    }
}

void mostrarFila(Cliente* fila) {
    Cliente* aux = fila;
    int i = 1;
    while (aux != NULL) {
        printf("Turno: %d, Numero de cuenta: %d, Tipo de cuenta: %d\n", i, aux->numeroCuenta, aux->tipoCuenta);
        aux = aux->sig;
        i++;
    }
}

void agregarComprobante(int numeroCuenta, char* descripcion, double monto, char* fecha) {
    Comprobante* nuevoComprobante = (Comprobante*) malloc(sizeof(Comprobante));
    nuevoComprobante->numeroCuenta = numeroCuenta;
    strcpy(nuevoComprobante->descripcion, descripcion);
    nuevoComprobante->monto = monto;
    strcpy(nuevoComprobante->fecha, fecha);
    nuevoComprobante->sig = NULL;

    if (comprobantes == NULL) {
        comprobantes = nuevoComprobante;
    } else {
        Comprobante* aux = comprobantes;
        while (aux->sig != NULL) {
            aux = aux->sig;
        }
        aux->sig = nuevoComprobante;
    }
}

void mostrarComprobantes() {
    Comprobante* aux = comprobantes;
    while (aux != NULL) {
        printf("Numero de cuenta: %d, Descripcion: %s, Monto: %.2f, Fecha: %s\n", aux->numeroCuenta, aux->descripcion, aux->monto, aux->fecha);
        aux = aux->sig;
    }
}

void mostrarComprobantesCliente(int numeroCuenta) {
    Comprobante* aux = comprobantes;
    while (aux != NULL) {
        if (aux->numeroCuenta == numeroCuenta) {
            printf("Numero de cuenta: %d, Descripcion: %s, Monto: %.2f, Fecha: %s\n", aux->numeroCuenta, aux->descripcion, aux->monto, aux->fecha);
        }
        aux = aux->sig;
    }
}

void mostrarMenu() {
    printf("\n");
    printf("====================================\n");
    printf("Bienvenido al sistema de gestion de filas del banco\n");
    printf("====================================\n");
    printf("Por favor, elige una opcion:\n");
    printf("1. Registrar cliente\n");
    printf("2. Mostrar filas de espera\n");
    printf("3. Agregar comprobante\n");
    printf("4. Mostrar todos los comprobantes\n");
    printf("5. Mostrar comprobantes de un cliente especifico\n");
    printf("6. Salir\n");
    printf("====================================\n");
    printf("\n");
}

int main() {
    int opcion;
    do {
        mostrarMenu();
        printf("Introduce tu opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Introduce el numero de cuenta del cliente: ");
                int numeroCuenta;
                scanf("%d", &numeroCuenta);
                printf("Introduce el tipo de cuenta del cliente (1 para preferencial, 2 para tradicional): ");
                int tipoCuenta;
                scanf("%d", &tipoCuenta);
                registrarCliente(numeroCuenta, tipoCuenta);
                break;
            case 2:
                printf("Fila preferencial:\n");
                mostrarFila(filaPreferencial);
                printf("Fila tradicional:\n");
                mostrarFila(filaTradicional);
                break;
            case 3:
                printf("Introduce el numero de cuenta del cliente: ");
                scanf("%d", &numeroCuenta);
                printf("Introduce la descripcion de la transaccion: ");
                char descripcion[100];
                scanf("%s", descripcion);
                printf("Introduce el monto de la transaccion: ");
                double monto;
                scanf("%lf", &monto);
                printf("Introduce la fecha de la transaccion (formato YYYY-MM-DD): ");
                char fecha[10];
                scanf("%s", fecha);
                agregarComprobante(numeroCuenta, descripcion, monto, fecha);
                break;
            case 4:
                mostrarComprobantes();
                break;
            case 5:
                printf("Introduce el numero de cuenta del cliente: ");
                scanf("%d", &numeroCuenta);
                mostrarComprobantesCliente(numeroCuenta);
                break;
            case 6:
                printf("Gracias por usar nuestro sistema de gestion de filas. Hasta luego\n");
                break;
            default:
                printf("Opcion no valida. Por favor, elige una opcion del menu.\n");
                break;
        }
    } while (opcion != 6);

    return 0;
}