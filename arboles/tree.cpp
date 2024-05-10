#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

#define MAX_RNT 1000 //reemplazce el int rnt_counter por esta variable tipo bolean para verificar si esta libre el rnt para reasirnarlo
bool rnt_usados[MAX_RNT] = {false};


// liberar rnt
void liberarRNT(int rnt) {
    if (rnt > 0 && rnt < MAX_RNT) {
        rnt_usados[rnt] = false; // Marcar como no usado.
    }
}


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

//asigna el rnt solo si no lo tiene
int obtenerSiguienteRNT() {
    for (int i = 1; i < MAX_RNT; i++) {
        if (!rnt_usados[i]) {
            rnt_usados[i] = true; // Marcar como usado.
            return i;
        }
    }
    return -1; // No hay RNTs disponibles.
}


// Funcion para insertar un nuevo nodo con una fecha de vencimiento en el arbol 
Tree* insertar(Tree* nodo, Fecha venc, const char* empresa) {
    if (nodo == NULL) {
        int rnt = obtenerSiguienteRNT();
        if (rnt == -1) {
            printf("No hay más RNTs disponibles.\n");
            return NULL;
        }
        return nuevoNodo(venc, empresa, rnt);
    }
    if (compararFechas(venc, nodo->vencimiento) < 0) {
        nodo->izq = insertar(nodo->izq, venc, empresa);
    } else if (compararFechas(venc, nodo->vencimiento) > 0) {
        nodo->der = insertar(nodo->der, venc, empresa);
    }
    return nodo;
}
// fincion debug de busqueda 
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
        return nullptr; // No se encontró el RNT.
    }
    if (raiz->rnt == rntBuscado) {
        return raiz; // Si el RNT del nodo actual coincide, devuelve el nodo.
    }
    Tree* resultadoIzq = buscarPorRNT(raiz->izq, rntBuscado); // Busca en el subárbol izquierdo.
    if (resultadoIzq != nullptr) {
        return resultadoIzq; // Si se encontró en el subárbol izquierdo, devuelve el resultado.
    }
    Tree* resultadoDer = buscarPorRNT(raiz->der, rntBuscado); // Busca en el subárbol derecho.
    return resultadoDer; // Devuelve el resultado del subárbol derecho (puede ser nullptr si no se encuentra).
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


// Función para encontrar el mínimo en un árbol binario de búsqueda
Tree* encontrarMinimo(Tree* nodo) {
    printf("Encontrar mínimo, nodo actual: %p\n", nodo);
    Tree* actual = nodo;
    // El mínimo es el nodo más a la izquierda
    while (actual && actual->izq != NULL) {
        printf("Encontrar mínimo, nodo izq: %p\n", actual->izq);
        actual = actual->izq;
    }
    printf("Encontrar mínimo, retornando %p\n", actual);
    return actual;
}

Tree* eliminarNodo(Tree* raiz, Fecha venc) {
    if (raiz == NULL) {
        printf("No se encontró el nodo con la fecha dada.\n");
        return NULL; // No hay nodo para eliminar.
    }

    // Si la fecha a eliminar es menor que la raíz, entonces está en el subárbol izquierdo
    if (compararFechas(venc, raiz->vencimiento) < 0) {
        raiz->izq = eliminarNodo(raiz->izq, venc);
    }
    // Si la fecha a eliminar es mayor que la raíz, entonces está en el subárbol derecho
    else if (compararFechas(venc, raiz->vencimiento) > 0) {
        raiz->der = eliminarNodo(raiz->der, venc);
    }
    // Si la fecha es igual a la raíz, entonces este es el nodo a eliminar
    else {
         // Antes de eliminar el nodo, liberar su RNT.
        liberarRNT(raiz->rnt);
        // Nodo con solo un hijo o sin hijos
        if (raiz->izq == NULL) {
            Tree* temp = raiz->der;
            free(raiz);
            raiz = temp;
        }
        else if (raiz->der == NULL) {
            Tree* temp = raiz->izq;
            free(raiz);
            raiz = temp;
        }
        // Nodo con dos hijos: obtener el sucesor inorden (el mínimo en el subárbol derecho)
        else {
            Tree* temp = encontrarMinimo(raiz->der);
            // Copiar el sucesor inorden en este nodo
            raiz->vencimiento = temp->vencimiento;
            strcpy(raiz->empresa, temp->empresa);
            raiz->rnt = temp->rnt;
            // Eliminar el sucesor inorden
            raiz->der = eliminarNodo(raiz->der, temp->vencimiento);
        }
        
    }
    return raiz; // Devuelve la nueva raíz del árbol después de la eliminación
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
    printf("6. Eliminar nodo por fecha\n");
    printf("0. Salir\n");
    printf("======================================\n");
    printf("Ingrese una opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

// liberar memoria para nodo
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
                    printf("\nFecha no encontrada en el arbol.\n");
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
                    printf("Opción no valida.\n");
                }

                break;
            }
            case 6:
            {
                printf("\nEliminar nodo\n");
                printf("--------------------\n");

                int anio, mes, dia;
                printf("Ingrese la fecha del nodo a eliminar (formato AAAA MM DD): ");
                scanf("%d %d %d", &anio, &mes, &dia);
                Fecha vencimiento = {dia, mes, anio + 5};

                raiz = eliminarNodo(raiz, vencimiento);

                printf("nodo eliminado correctamente.\n");

                break;
            }

                

        } 
    } while (opcion != 0);

    liberarMemoria(raiz);
    return 0;
}