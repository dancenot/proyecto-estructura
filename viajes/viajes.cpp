#include <iostream>
#include <cstdlib>
#include <cstring> 
#include <limits>
using namespace std;

/* verison 1.0.8*/

struct nodo {
    int value; 
    char matricula[20];
    char nombreEmbarcacion[50];
    char fecha[11]; // formato dia/mes/anio
    double precio;
    char destino[50];
    int capacidad;
    char idViaje[15]; // formato XXYYYYMMDD
    nodo *izq;
    nodo *der;
    int altura;
};
struct nodo *raiz, *aux;


// prototipos de funciones adicionales
void generarIdentificador(nodo *n);
int obtenerAltura(struct nodo *n);
int obtenerBalance(struct nodo *n);
struct nodo* rotarDerecha(struct nodo *n);
struct nodo* rotarIzquierda(struct nodo *n);


nodo* crearNodo() {
    nodo *nuevoNodo = (nodo *)malloc(sizeof(nodo));
    if (nuevoNodo == NULL) {
        cerr << "Error al asignar memoria." << endl;
        return NULL;
    }
    memset(nuevoNodo, 0, sizeof(nodo)); // inicializa con ceros para evitar datos basura

    nuevoNodo->izq = NULL;
    nuevoNodo->der = NULL;
    nuevoNodo->altura = 1; 

    return nuevoNodo;
}

void rellenarDatosNodo(nodo *n) {
    cout << "Ingrese la matricula de la embarcacion: ";
    cin.getline(n->matricula, 20);
    cout << "Ingrese el nombre de la embarcacion: ";
    cin.getline(n->nombreEmbarcacion, 50);
    cout << "Ingrese la fecha del viaje (dd/mm/aaaa): ";
    cin.getline(n->fecha, 11);
    cout << "Ingrese el precio del viaje: ";
    cin >> n->precio;
    cout << "Ingrese el destino del viaje: ";
    cin.getline(n->destino, 50);
    cout << "Ingrese la capacidad de la embarcacion: ";
    cin >> n->capacidad;
    // Generar el identificador
    generarIdentificador(n);
}




/* el buffer es una memoria dinamica que alamacena datos temporales y despues los procesa
esto permine que el programa no se bloquee con muchos datos, ademas reduce la cantidad de llamadas a funciones*/

/* con el sizeof se define el tamanio de la variable ejemplo:
matricula[20]
sizeof(nuevoNodo->matricula) indica que el tamaño esta limitado a 20 caracteres por lo que el sizeof asegura que el tamaño sea el correcto
y no haya un desborde en el bufer almacenado*/




//como lo puse en la funcion genera una ID
void generarIdentificador(nodo *viaje) {
    strncpy(viaje->idViaje, viaje->matricula, 2);
    strncpy(viaje->idViaje + 2, viaje->fecha + 6, 4);
     strncpy(viaje->idViaje + 8, viaje->fecha, 2);
   strncpy(viaje->idViaje + 6, viaje->fecha + 3, 2);
    viaje->idViaje[10] = '\0';
}

int mayor(int a, int b) {
    return (a > b) ? a : b;
}

struct nodo* insertar(struct nodo* raiz, struct nodo* nuevoNodo) {
    if (raiz == NULL) {
        return nuevoNodo;
    }

    if (nuevoNodo->value < raiz->value) {
        raiz->izq = insertar(raiz->izq, nuevoNodo);
    } else if (nuevoNodo->value > raiz->value) {
        raiz->der = insertar(raiz->der, nuevoNodo);
    } else {
        // si el valor ya existe en el arbol, no se inserta el duplicado
        return raiz;
    }

    raiz->altura = 1 + mayor(obtenerAltura(raiz->izq), obtenerAltura(raiz->der));

    int balance = obtenerBalance(raiz);

    // Rotaciones para balancear el arbol
    if (balance > 1 && nuevoNodo->value < raiz->izq->value) {
        return rotarDerecha(raiz);
    }

    if (balance < -1 && nuevoNodo->value > raiz->der->value) {
        return rotarIzquierda(raiz);
    }

    if (balance > 1 && nuevoNodo->value > raiz->izq->value) {
        raiz->izq = rotarIzquierda(raiz->izq);
        return rotarDerecha(raiz);
    }

    if (balance < -1 && nuevoNodo->value < raiz->der->value) {
        raiz->der = rotarDerecha(raiz->der);
        return rotarIzquierda(raiz);
    }

    return raiz;
}



int obtenerAltura(struct nodo *n) {
    if (n == NULL) {
        return 0;
    }
    return n->altura;
}


struct nodo* rotarDerecha(struct nodo *y) {
    struct nodo *x = y->izq;
    struct nodo *T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;

    return x;
}

struct nodo* rotarIzquierda(struct nodo *x) {
    struct nodo *y = x->der;
    struct nodo *T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;

    return y;
}

int obtenerBalance(struct nodo *n) {
    if (n == NULL) {
        return 0;
    }
    return obtenerAltura(n->izq) - obtenerAltura(n->der);
}


void liberarnodo(nodo *n) {
    if (n != NULL) {
        liberarnodo(n->izq);
        liberarnodo(n->der);
        free(n);
    }
}

// busca un viaje por su identificador
void buscarViaje(nodo *raiz, const char *idBuscado) {
    if (raiz == NULL) {
        cout << "El viaje con identificador " << idBuscado << " no se encuentra en el sistema." << endl;
        return;
    }
    if (strcmp(idBuscado, raiz->idViaje) == 0) {
        cout << "Detalles del viaje:" << endl;
        cout << "Identificador Unico: " << raiz->idViaje << endl;
        cout << "Matricula de la embarcacion: " << raiz->matricula << endl;
        cout << "Nombre de la embarcacion: " << raiz->nombreEmbarcacion << endl;
        cout << "Fecha del viaje: " << raiz->fecha << endl;
        cout << "Precio del viaje: " << raiz->precio << endl;
        cout << "Destino del viaje: " << raiz->destino << endl;
        cout << "Capacidad de la embarcacion: " << raiz->capacidad << endl;
    } else if (strcmp(idBuscado, raiz->idViaje) < 0) {
        buscarViaje(raiz->izq, idBuscado);
    } else {
        buscarViaje(raiz->der, idBuscado);
    }
}


//recorre el arbol en inorden y listar los viajes
void listarViajes(nodo *raiz) {
    if (raiz != NULL) {
        listarViajes(raiz->izq); // Visita el rama izquierda
        // Muestra los detalles del viaje
        cout << "Identificador Unico: " << raiz->idViaje << endl;
        cout << "Matricula de la embarcacion: " << raiz->matricula << endl;
        cout << "Nombre de la embarcacion: " << raiz->nombreEmbarcacion << endl;
        cout << "Fecha del viaje: " << raiz->fecha << endl;
        cout << "Precio del viaje: " << raiz->precio << endl;
        cout << "Destino del viaje: " << raiz->destino << endl;
        cout << "Capacidad de la embarcacion: " << raiz->capacidad << endl;
        cout << "--------------------------" << endl;
        listarViajes(raiz->der); // Visita la rama derecha
    };
}

int main() {
    nodo *raiz = NULL;
    int opcion = 0;
    do {
        cout << "Menu de opciones:" << endl;
        cout << "1. Registrar un nuevo viaje." << endl;
        cout << "2. Buscar un viaje por identificador." << endl;
        cout << "3. Listar todos los viajes." << endl;
        cout << "4. Eliminar un viaje por identificador." << endl;
        cout << "5. Registrar un pasajero en un viaje." << endl;
        cout << "6. Listar todos los pasajeros de un viaje." << endl;
        cout << "7. Salir." << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        nodo *aux = NULL; // Ajuste: Inicializa aux en cada iteración
        switch (opcion) {
            case 1:
                aux = crearNodo();
                
                rellenarDatosNodo(aux);
                
                raiz = insertar(raiz, aux);
                break;
            case 2: {
                char idBuscado[15];
                cout << "Ingrese el identificador del viaje a buscar: ";
                cin.ignore();
                cin.getline(idBuscado, 15);
                buscarViaje(raiz, idBuscado);
                break;
            }
            case 3:
                listarViajes(raiz);
                break;
            case 4:
                // Lógica para eliminar un viaje por identificador
                break;
            case 5:
                // Lógica para registrar un pasajero en un viaje
                break;
            case 6:
                // Lógica para listar todos los pasajeros de un viaje
                break;
            case 7:
                cout << "Gracias por utilizar el sistema." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
        if (aux != NULL) {
            free(aux);
            aux = NULL;
        }
    } while (opcion != 7);
    liberarnodo(raiz);
    return 0;
}

