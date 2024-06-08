#include <iostream>
#include <cstdlib>
#include <cstring> 
#include <limits>
#include <queue>
#include <vector>
using namespace std;

/* verison 1.0.8*/
struct Pasajero {
    char nombre[50];
    int edad;
    // Otros datos del pasajero
};

struct nodo {
    int value; 
    char matricula[20];
    char nombreEmbarcacion[50];
    char fecha[11]; // formato dia/mes/anio
    double precio;
    char destino[50];
    int capacidad;
    char idViaje[20]; // Cambia a un array de caracteres
    nodo *izq;
    nodo *der;
    int altura;
    Pasajero pasajeros[100]; // Array para pasajeros
    int numPasajeros; // Número de pasajeros registrados
};



struct nodo *raiz, *aux;

// prototipos de funciones adicionales
void generarIdentificador(nodo *n);
int obtenerAltura(struct nodo *n);
int obtenerBalance(struct nodo *n);
struct nodo* rotarDerecha(struct nodo *n);
struct nodo* rotarIzquierda(struct nodo *n);
void liberarnodo(nodo *n);
nodo* minValueNode(nodo* node);

nodo* crearNodo() {
    nodo *nuevoNodo = (nodo *)malloc(sizeof(nodo));
    if (nuevoNodo == NULL) {
        cerr << "Error al asignar memoria." << endl;
        return NULL;
    }
    nuevoNodo->izq = NULL;
    nuevoNodo->der = NULL;
    nuevoNodo->altura = 1; 

    return nuevoNodo;
}

void rellenarDatosNodo(nodo *n) {
    cout << "Ingrese la matricula de la embarcacion: ";
    cin.ignore(); // Limpiar el buffer antes de leer la cadena
    cin.getline(n->matricula, 20);
    cout << "Ingrese el nombre de la embarcacion: ";
    cin.getline(n->nombreEmbarcacion, 50);
    cout << "Ingrese la fecha del viaje (dd/mm/aaaa): ";
    cin.getline(n->fecha, 11);
    cout << "Ingrese el precio del viaje: ";
    cin >> n->precio;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer
    cout << "Ingrese el destino del viaje: ";
    cin.getline(n->destino, 50);
    cout << "Ingrese la capacidad de la embarcacion: ";
    cin >> n->capacidad;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer
    // Generar el identificador
    generarIdentificador(n);
}

void generarIdentificador(nodo *viaje) {
    int dia, mes, anio;
    sscanf(viaje->fecha, "%d/%d/%d", &dia, &mes, &anio);
    snprintf(viaje->idViaje, sizeof(viaje->idViaje), "%c%c%04d%02d%02d", 
             viaje->matricula[0], viaje->matricula[1], anio, mes, dia);
}


int mayor(int a, int b) {
    return (a > b) ? a : b;
}

struct nodo* insertar(struct nodo* raiz, struct nodo* nuevoNodo) {
    if (raiz == NULL) {
        return nuevoNodo;
    }

    if (nuevoNodo->idViaje < raiz->idViaje) {
        raiz->izq = insertar(raiz->izq, nuevoNodo);
    } else if (nuevoNodo->idViaje > raiz->idViaje) {
        raiz->der = insertar(raiz->der, nuevoNodo);
    } else {
        // si el valor ya existe en el arbol, no se inserta el duplicado
        return raiz;
    }

    raiz->altura = 1 + mayor(obtenerAltura(raiz->izq), obtenerAltura(raiz->der));

    int balance = obtenerBalance(raiz);

    // Rotaciones para balancear el arbol
    if (balance > 1 && nuevoNodo->idViaje < raiz->izq->idViaje) {
        return rotarDerecha(raiz);
    }

    if (balance < -1 && nuevoNodo->idViaje > raiz->der->idViaje) {
        return rotarIzquierda(raiz);
    }

    if (balance > 1 && nuevoNodo->idViaje > raiz->izq->idViaje) {
        raiz->izq = rotarIzquierda(raiz->izq);
        return rotarDerecha(raiz);
    }

    if (balance < -1 && nuevoNodo->idViaje < raiz->der->idViaje) {
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
nodo* buscarViaje(nodo *raiz, const char* idBuscado) {
    if (raiz == NULL) {
        cout << "El viaje con identificador " << idBuscado << " no se encuentra en el sistema." << endl;
        return NULL;
    }
    if (strcmp(idBuscado, raiz->idViaje) == 0) {
        return raiz;
    } else if (strcmp(idBuscado, raiz->idViaje) < 0) {
        return buscarViaje(raiz->izq, idBuscado);
    } else {
        return buscarViaje(raiz->der, idBuscado);
    }
}



// recorre el arbol en inorden y listar los viajes
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
    }
}
void registrarPasajero(nodo *viaje, const Pasajero &pasajero) {
    if (viaje->numPasajeros < viaje->capacidad) {
        viaje->pasajeros[viaje->numPasajeros++] = pasajero;
        cout << "Pasajero registrado exitosamente." << endl;
    } else {
        cout << "La capacidad de la embarcacion ha sido alcanzada." << endl;
    }
}




void buscarPreorden(nodo *raiz) {
    if (raiz != NULL) {
        for (int i = 0; i < raiz->numPasajeros; ++i) {
            cout << "Nombre: " << raiz->pasajeros[i].nombre << ", Edad: " << raiz->pasajeros[i].edad << endl;
        }
        buscarPreorden(raiz->izq);
        buscarPreorden(raiz->der);
    }
}

void buscarInorden(nodo *raiz) {
    if (raiz != NULL) {
        buscarInorden(raiz->izq);
        for (int i = 0; i < raiz->numPasajeros; ++i) {
            cout << "Nombre: " << raiz->pasajeros[i].nombre << ", Edad: " << raiz->pasajeros[i].edad << endl;
        }
        buscarInorden(raiz->der);
    }
}

void buscarPosorden(nodo *raiz) {
    if (raiz != NULL) {
        buscarPosorden(raiz->izq);
        buscarPosorden(raiz->der);
        for (int i = 0; i < raiz->numPasajeros; ++i) {
            cout << "Nombre: " << raiz->pasajeros[i].nombre << ", Edad: " << raiz->pasajeros[i].edad << endl;
        }
    }
}







nodo* eliminarViaje(nodo* raiz, const char* idViaje) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(idViaje, raiz->idViaje) < 0) {
        raiz->izq = eliminarViaje(raiz->izq, idViaje);
    } else if (strcmp(idViaje, raiz->idViaje) > 0) {
        raiz->der = eliminarViaje(raiz->der, idViaje);
    } else {
        if ((raiz->izq == NULL) || (raiz->der == NULL)) {
            nodo *temp = raiz->izq ? raiz->izq : raiz->der;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            nodo* temp = minValueNode(raiz->der);
            strcpy(raiz->idViaje, temp->idViaje);
            raiz->der = eliminarViaje(raiz->der, temp->idViaje);
        }
    }

    if (raiz == NULL) {
        return raiz;
    }

    raiz->altura = 1 + mayor(obtenerAltura(raiz->izq), obtenerAltura(raiz->der));

    int balance = obtenerBalance(raiz);

    if (balance > 1 && obtenerBalance(raiz->izq) >= 0) {
        return rotarDerecha(raiz);
    }

    if (balance > 1 && obtenerBalance(raiz->izq) < 0) {
        raiz->izq = rotarIzquierda(raiz->izq);
        return rotarDerecha(raiz);
    }

    if (balance < -1 && obtenerBalance(raiz->der) <= 0) {
        return rotarIzquierda(raiz);
    }

    if (balance < -1 && obtenerBalance(raiz->der) > 0) {
        raiz->der = rotarDerecha(raiz->der);
        return rotarIzquierda(raiz);
    }

    return raiz;
}


nodo* minValueNode(nodo* node) {
    nodo* current = node;
    while (current->izq != NULL) {
        current = current->izq;
    }
    return current;
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
        nodo *aux = NULL; // Inicializa aux en cada iteración
        nodo *viaje = NULL; // Declarar la variable viaje fuera del switch
        switch (opcion) {
            case 1:
                aux = crearNodo();
                rellenarDatosNodo(aux);
                raiz = insertar(raiz, aux);
                break;
            case 2: {
                char idBuscado[20];
                cout << "Ingrese el identificador del viaje a buscar: ";
                cin >> idBuscado;
                viaje = buscarViaje(raiz, idBuscado);
                if (viaje != NULL) {
                    cout << "Detalles del viaje:" << endl;
                    cout << "Identificador Unico: " << viaje->idViaje << endl;
                    cout << "Matricula de la embarcacion: " << viaje->matricula << endl;
                    cout << "Nombre de la embarcacion: " << viaje->nombreEmbarcacion << endl;
                    cout << "Fecha del viaje: " << viaje->fecha << endl;
                    cout << "Precio del viaje: " << viaje->precio << endl;
                    cout << "Destino del viaje: " << viaje->destino << endl;
                    cout << "Capacidad de la embarcacion: " << viaje->capacidad << endl;
                }
                break;
            }
            case 3:
                listarViajes(raiz);
                break;
            case 4: {
                char idEliminar[20];
                cout << "Ingrese el identificador del viaje a eliminar: ";
                cin >> idEliminar;
                raiz = eliminarViaje(raiz, idEliminar);
                break;
            }
            case 5: {
                char idViaje[20];
                cout << "Ingrese el identificador del viaje: ";
                cin >> idViaje;
                viaje = buscarViaje(raiz, idViaje);
                if (viaje != NULL) {
                    Pasajero pasajero;
                    cout << "Ingrese el nombre del pasajero: ";
                    cin.ignore();
                    cin.getline(pasajero.nombre, 50);
                    cout << "Ingrese la edad del pasajero: ";
                    cin >> pasajero.edad;
                    registrarPasajero(viaje, pasajero);
                }
                break;
            }
            case 6: {
                int subOpcion;
                cout << "Seleccione el tipo de búsqueda:" << endl;
                cout << "1. Preorden" << endl;
                cout << "2. Inorden" << endl;
                cout << "3. Posorden" << endl;
                cout << "Seleccione una opcion: ";
                cin >> subOpcion;

                switch (subOpcion) {
                    case 1:
                        cout << "Resultados de la búsqueda preorden:" << endl;
                        buscarPreorden(raiz);
                        break;
                    case 2:
                        cout << "Resultados de la búsqueda inorden:" << endl;
                        buscarInorden(raiz);
                        break;
                    case 3:
                        cout << "Resultados de la búsqueda posorden:" << endl;
                        buscarPosorden(raiz);
                        break;
                    default:
                        cout << "Opcion no valida. Intente de nuevo." << endl;
                }
                break;
            }
            case 7:
                cout << "Gracias por utilizar el sistema." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 7);
    liberarnodo(raiz);
    return 0;
}
