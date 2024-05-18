#include <string>
#include <cstdio>
#include <cstdlib>

struct Carro {
    std::string placa;
    bool ocupado = false;
    std::string modelo;
    int fila = 0;
    int precio = 1000;
    int costo_movimiento = 500;
    Carro* siguiente = NULL;
};

struct Moto {
    std::string placa;
    bool ocupado = false;
    std::string modelo;
    int fila = 0;
    int precio = 500;
    int costo_movimiento = 250;
    Moto* siguiente = NULL;
};

void menu() {
    printf("\n bienvenido al parking virtual\n");
    printf("1. agregar carro\n");
    printf("2. agregar moto\n");
    printf("3. sacar carro\n");
    printf("4. sacar moto\n");
    printf("5. mostrar carros y motos\n");
    printf("6. salir\n");
    printf("elija una opcion: ");
}

void menusecundario() {
    printf("\n 1. mostrar carros\n");
    printf("2. mostrar motos\n");
    printf("3. volver al menu principal\n");
    printf("\n elija una opcion: ");
}

int LeerOpcion() {
    int opcion;
    scanf("%d", &opcion);
    return opcion;
}

void mostrarCarros(Carro* carros) {
    printf("carros:\n");
    Carro* temp = carros;
    while (temp != NULL) {
        printf("placa: %s, modelo: %s\n", temp->placa.c_str(), temp->modelo.c_str());
        temp = temp->siguiente;
    }
}

void mostrarMotos(Moto* motos) {
    printf("motos:\n");
    Moto* temp = motos;
    while (temp != NULL) {
        printf("placa: %s, modelo: %s\n", temp->placa.c_str(), temp->modelo.c_str());
        temp = temp->siguiente;
    }
}

void mostrar(Carro* carros, Moto* motos) {
    menusecundario();
    int op = LeerOpcion();
    switch (op) {
        case 1:
            mostrarCarros(carros);
            break;
        case 2:
            mostrarMotos(motos);
            break;
        case 3:
            menu();
            break;
    }
}

Carro* agregarCarro(Carro** carros, std::string placa, std::string modelo) {
    Carro* nuevo = (Carro*) malloc(sizeof(Carro));
    nuevo->placa = placa;
    nuevo->modelo = modelo;
    if ((*carros) == NULL || (*carros)->placa != placa) {
        nuevo->siguiente = *carros;
        *carros = nuevo;
    } else {
        Carro* temp = *carros;
        while (temp->siguiente != NULL && temp->siguiente->placa != placa) {
            temp = temp->siguiente;
        }
        Carro* siguiente = temp->siguiente;
        temp->siguiente = nuevo;
        nuevo->siguiente = siguiente;
    }
    return nuevo;
}

Moto* agregarMoto(Moto** motos, std::string placa, std::string modelo) {
    Moto* nuevo = (Moto*) malloc(sizeof(Moto));
    nuevo->placa = placa;
    nuevo->modelo = modelo;
    if ((*motos) == NULL || (*motos)->placa != placa) {
        nuevo->siguiente = *motos;
        *motos = nuevo;
    } else {
        Moto* temp = *motos;
        while (temp->siguiente != NULL && temp->siguiente->placa != placa) {
            temp = temp->siguiente;
        }
        Moto* siguiente = temp->siguiente;
        temp->siguiente = nuevo;
        nuevo->siguiente = siguiente;
    }
    return nuevo;
}

Carro* sacarCarro(Carro** carros, std::string placa) {
    if (*carros == NULL) {
        printf("no hay carros para sacar.\n");
        return NULL;
    }

    Carro* actual = *carros;
    Carro* carro_a_sacar = NULL;
    int costo_total = 0;
    bool encontrado = false;

    // buscar el carro a sacar y calcular el costo de mover los carros que lo obstruyen
    while (actual != NULL && !encontrado) {
        if (actual->placa == placa) {
            encontrado = true;
            carro_a_sacar = actual;
        } else {
            costo_total += actual->costo_movimiento; // sumar el costo de movimiento
            actual = actual->siguiente;
        }
    }

    // si no se encuentra el carro, retornar
    if (!encontrado) {
        printf("el carro con placa %s no se encuentra en el estacionamiento.\n", placa.c_str());
        return *carros;
    }

    // si el carro a sacar es el primero, no se mueven carros y solo se cobra el estacionamiento
    if (carro_a_sacar == *carros) {
        costo_total = carro_a_sacar->precio;
    } else {
        // si se movieron carros, se suma el costo de movimiento una sola vez
        costo_total += carro_a_sacar->precio;
    }

    // desconectar el carro a sacar de la lista
    if (carro_a_sacar == *carros) {
        *carros = carro_a_sacar->siguiente;
    } else {
        Carro* temp = *carros;
        while (temp->siguiente != carro_a_sacar) {
            temp = temp->siguiente;
        }
        temp->siguiente = carro_a_sacar->siguiente;
    }

    printf("el costo total a pagar es de: %d\n", costo_total);
    free(carro_a_sacar); // liberar la memoria del carro a sacar

    return *carros; // retornar la lista actualizada de carros
}

Moto* sacarMoto(Moto** motos, std::string placa) {
    if (*motos == NULL) {
        printf("no hay motos para sacar.\n");
        return NULL;
    }

    Moto* actual = *motos;
    Moto* anterior = NULL;
    int costo_total = 0;
    bool encontrado = false;

    // buscar la moto a sacar
    while (actual != NULL && !encontrado) {
        if (actual->placa == placa) {
            encontrado = true;
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }

    // si no se encuentra la moto, retornar
    if (!encontrado) {
        printf("la moto con placa %s no se encuentra en el estacionamiento.\n", placa.c_str());
        return *motos;
    }

    // mover las motos que estan bloqueando la salida
    Moto* temp = *motos;
    while (temp != actual) {
        costo_total += temp->costo_movimiento; // sumar el costo de movimiento
        temp = temp->siguiente;
    }

    // sacar la moto deseada
    if (anterior != NULL) {
        anterior->siguiente = actual->siguiente;
    } else {
        *motos = actual->siguiente;
    }

    // cobrar solo una vez por mover las motos, no por cada moto movida
    costo_total += actual->precio; // sumar el precio del estacionamiento de la moto a sacar

    printf("el costo total a pagar es de: %d\n", costo_total);
    free(actual); // liberar la memoria de la moto a sacar

    return *motos; // retornar la lista actualizada de motos

}

int main() {
    Carro* carros = NULL;
    Moto* motos = NULL;
    int opcion;
    do {
        menu();
        opcion = LeerOpcion();
        switch (opcion) {
            case 1:
                {
                    printf("ingrese la placa del carro: ");
                    char placa[50];
                    scanf("%49s", placa);
                    std::string placa_str(placa);
                    printf("ingrese el modelo del carro: ");
                    char modelo[50];
                    scanf("%49s", modelo);
                    std::string modelo_str(modelo);
                    agregarCarro(&carros, placa_str, modelo_str);
                }
                break;
            case 2:
                {
                    printf("ingrese la placa de la moto: ");
                    char placa[50];
                    scanf("%49s", placa);
                    std::string placa_str(placa);
                    printf("ingrese el modelo de la moto: ");
                    char modelo[50];
                    scanf("%49s", modelo);
                    std::string modelo_str(modelo);
                    agregarMoto(&motos, placa_str, modelo_str);
                }
                break;
            case 3:
                {
                    printf("ingrese la placa del carro a sacar: ");
                    char placa[50];
                    scanf("%49s", placa);
                    std::string placa_str(placa);
                    sacarCarro(&carros, placa_str);
                }
                break;
            case 4:
                {
                    printf("ingrese la placa de la moto a sacar: ");
                    char placa[50];
                    scanf("%49s", placa);
                    std::string placa_str(placa);
                    sacarMoto(&motos, placa_str);
                }
                break;
            case 5:
                {
                    mostrarCarros(carros);
                    mostrarMotos(motos);
                }
                break;
            default:
                printf("opcion no valida.\n");
                break;
        }
    } while (opcion != 6);
  return 0;
}