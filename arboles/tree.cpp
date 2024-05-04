#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura representa la fecha
typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

// Estructura representa el nodo del arbol
typedef struct Tree {
    Fecha vencimiento; //fecha es la estructura donde almacenara los datos y vencimiento es la conexion y donde recibira los datos del usuario 
    char empresa[50];
    int rnt;
    struct Tree* izq;
    struct Tree* der;
} Tree;

int rnt_counter = 0;

// crear un nuevo nodo
Tree* nuevoNodo(Fecha reg, const char* empresa, int rnt) {
    Tree* temp = (Tree*)malloc(sizeof(Tree));
    if (temp == NULL) {
        printf("Error al asignar memoria para un nuevo nodo.\n"); // por si no se puede pedir memoria
        return NULL;
    }
    temp->vencimiento.dia = reg.dia;
    temp->vencimiento.mes = reg.mes;
    temp->vencimiento.anio = reg.anio + 5; // Añadir 5 años para la fecha de vencimiento
    strcpy(temp->empresa, empresa);
    temp->rnt = rnt;
    temp->izq = temp->der = NULL;
    return temp;
}


// comparar fechas
int compararFechas(Fecha a, Fecha b) {
    if (a.anio != b.anio)
        return a.anio - b.anio;
    else if (a.mes != b.mes)
        return a.mes - b.mes;
    else
        return a.dia - b.dia;
}

// Funcion para insertar un nuevo nodo con una fecha de vencimiento en el arbol 
Tree* insertar(Tree* nodo, Fecha venc, const char* empresa) {
    if (nodo == NULL) {
        return nuevoNodo(venc, empresa, ++rnt_counter);
    }
    if (compararFechas(venc, nodo->vencimiento) < 0) {
        nodo->izq = insertar(nodo->izq, venc, empresa);
    } else if (compararFechas(venc, nodo->vencimiento) > 0) {
        nodo->der = insertar(nodo->der, venc, empresa);
    }
    return nodo;
}

// fincion debug de busqueda 

/*Esta funcion es unicamente para mostrar los datos ingresados por el usuario,
para ver si las funciones recorridas hace su funcion como se debe.
Esta opcion no estara al final del proyecto*/

void mostrarFecha(Tree* raiz) {
    if (raiz == nullptr) {
        return;
    }
    mostrarFecha(raiz->izq);
    printf("\n---------------------------------\n");
    printf("Nombre empresa: %s\n", raiz->empresa);
    printf("Fecha de registro: %d-%02d-%02d\n", raiz->vencimiento.anio - 5, raiz->vencimiento.mes, raiz->vencimiento.dia); // Restar 5 años para mostrar la fecha de registro por que lo hice al reves xd
    printf("Fecha de vencimiento: %d-%02d-%02d\n", raiz->vencimiento.anio, raiz->vencimiento.mes, raiz->vencimiento.dia); // Mostrar la fecha de vencimiento
    printf("RNT: %d\n", raiz->rnt);
    printf("---------------------------------\n");
    mostrarFecha(raiz->der);
}


//Compara por fecha
Tree* buscarPorFecha(Tree* raiz, Fecha fechaBuscada) {
    if (raiz == nullptr || compararFechas(raiz->vencimiento, fechaBuscada) == 0) {
        return raiz;
    }
    if (compararFechas(raiz->vencimiento, fechaBuscada) > 0) {
        return buscarPorFecha(raiz->izq, fechaBuscada);
    } else {
        return buscarPorFecha(raiz->der, fechaBuscada);
    }
}


Tree* buscarPorRNT(Tree* raiz, int rntBuscado) {
    if (raiz == nullptr) {
        return nullptr; // no se encontro el RNT.
    }
    if (raiz->rnt == rntBuscado) {
        return raiz; // si el RNT del nodo actual coincide, devuelve el nodo.
    }
    if (rntBuscado < raiz->rnt) {
        return buscarPorRNT(raiz->izq, rntBuscado); // si el RNT buscado es menor, busca en el subarbol izquierdo
    } else {
        return buscarPorRNT(raiz->der, rntBuscado); // buscado si es mayor, busca en el subarbol derecho
    }
}


void preorden(Tree* nodo) {
    if (nodo == NULL)
        return;
    printf("\n---------------------------------\n");
    printf("Nombre empresa: %s\n", nodo->empresa);
    printf("Fecha de registro: %d-%02d-%02d\n", nodo->vencimiento.anio - 5, nodo->vencimiento.mes, nodo->vencimiento.dia); 
    printf("Fecha de vencimiento: %d-%02d-%02d\n", nodo->vencimiento.anio, nodo->vencimiento.mes, nodo->vencimiento.dia); 
    printf("RNT: %d\n", nodo->rnt);
    printf("---------------------------------\n");
    preorden(nodo->izq);      // Recorrer izquierda
    preorden(nodo->der);      // Recorrer derecha
}

void inorden(Tree* nodo) {
    if (nodo == NULL)
        return;
    inorden(nodo->izq);      
    printf("\n---------------------------------\n");
    printf("Nombre empresa: %s\n", nodo->empresa);
    printf("Fecha de registro: %d-%02d-%02d\n", nodo->vencimiento.anio - 5, nodo->vencimiento.mes, nodo->vencimiento.dia); 
    printf("Fecha de vencimiento: %d-%02d-%02d\n", nodo->vencimiento.anio, nodo->vencimiento.mes, nodo->vencimiento.dia); 
    printf("RNT: %d\n", nodo->rnt);
    printf("---------------------------------\n");
    inorden(nodo->der);  
}

void posorden(Tree* nodo) {
    if (nodo == NULL)
        return;
    posorden(nodo->izq);      
    posorden(nodo->der);  
    printf("\n---------------------------------\n");
    printf("Nombre empresa: %s\n", nodo->empresa);
    printf("Fecha de registro: %d-%02d-%02d\n", nodo->vencimiento.anio - 5, nodo->vencimiento.mes, nodo->vencimiento.dia);
    printf("Fecha de vencimiento: %d-%02d-%02d\n", nodo->vencimiento.anio, nodo->vencimiento.mes, nodo->vencimiento.dia); 
    printf("---------------------------------\n");
}



//menu del programa
int menu() {
    int opcion;
    printf("\n========== MENU DE OPCIONES ==========\n");
    printf("1. Mostrar fechas registradas (debug)\n");
    printf("2. Registrar fecha\n");
    printf("3. Buscar fecha\n");
    printf("4. Buscar RNT\n");
    printf("5. Tipos de recorrido\n");
    printf("0. Salir\n");
    printf("======================================\n");
    printf("Ingrese una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

// liberar memoria
void liberarMemoria(Tree* nodo) {
    if (nodo == NULL)
        return;
    liberarMemoria(nodo->izq);
    liberarMemoria(nodo->der);

    free(nodo);
}



// inicio del programa
int main() {
    Tree* raiz = NULL;
    int opcion;
    do {
        opcion = menu();
        switch (opcion) {
            case 1:
                mostrarFecha(raiz);
                break;
            case 2: {
                int anio, mes, dia;
                char empresa[50];
                printf("\nIngrese anio, mes, dia, empresa separados por espacios: ");
                scanf("%d %d %d %s", &anio, &mes, &dia, empresa);
                Fecha venc = {dia, mes, anio};
                raiz = insertar(raiz, venc, empresa);
                break;
            }
            case 3:
            {
                int anio, mes, dia;
                // lo puse asi proque es mas corte que decir anio mes dia
                printf("\nIngrese la fecha a buscar (formato AAMMDD): ");
                scanf("%d %d %d", &anio, &mes, &dia);

                Fecha fechaBuscada = {dia, mes, anio};
                Tree* nodoEncontrado = buscarPorFecha(raiz, fechaBuscada);

                if (nodoEncontrado != nullptr)
                {
                    printf("\n---------------------------------\n");
                    printf("Fecha encontrada:\n");
                    printf("Nombre empresa: %s\n", nodoEncontrado->empresa);
                    printf("Fecha de registro: %d-%02d-%02d\n", nodoEncontrado->vencimiento.anio - 5, nodoEncontrado->vencimiento.mes, nodoEncontrado->vencimiento.dia);
                    printf("Fecha de vencimiento: %d-%02d-%02d\n", nodoEncontrado->vencimiento.anio, nodoEncontrado->vencimiento.mes, nodoEncontrado->vencimiento.dia); 
                    printf("RNT: %d\n", nodoEncontrado->rnt);
                    printf("---------------------------------\n");
                }
                else
                {
                    printf("\nFecha no encontrada en el árbol.\n");
                }

                break;
            }
            case 4: {
                int rntBuscado;
                printf("\nIngrese el RNT a buscar: ");
                scanf("%d", &rntBuscado);
                Tree* nodoEncontrado = buscarPorRNT(raiz, rntBuscado);
                if (nodoEncontrado != nullptr) {
                    printf("\n---------------------------------\n");
                    printf("RNT encontrado:\n");
                    printf("Nombre empresa: %s\n", nodoEncontrado->empresa);
                    printf("Fecha de registro: %d-%02d-%02d\n", nodoEncontrado->vencimiento.anio - 5, nodoEncontrado->vencimiento.mes, nodoEncontrado->vencimiento.dia); 
                    printf("Fecha de vencimiento: %d-%02d-%02d\n", nodoEncontrado->vencimiento.anio, nodoEncontrado->vencimiento.mes, nodoEncontrado->vencimiento.dia); 
                    printf("RNT: %d\n", nodoEncontrado->rnt);
                    printf("---------------------------------\n");
                } else {
                    printf("\nRNT no encontrado en el arbol.\n");
                }
                break;
            }
            case 5:
            {
                int opcionRecorrido;

                printf("\n========== TIPOS DE RECORRIDO ==========\n");
                printf("1. Recorrido Preorden\n");
                printf("2. Recorrido Inorden\n");
                printf("3. Recorrido Posorden\n");
                printf("=======================================\n");

                printf("Seleccione el tipo de recorrido: ");
                scanf("%d", &opcionRecorrido);

                switch (opcionRecorrido)
                {
                case 1:
                    printf("\nRecorrido Preorden:\n");
                    preorden(raiz);
                    printf("\n");
                    break;

                case 2:
                    printf("\nRecorrido Inorden:\n");
                    inorden(raiz);
                    printf("\n");
                    break;

                case 3:
                    printf("\nRecorrido Posorden:\n");
                    posorden(raiz);
                    printf("\n");
                    break;

                
                default:
                    printf("Opción no válida.\n");
                }

                break;
            }

        }
    } while (opcion != 0);

    liberarMemoria(raiz);
    return 0;
}





