#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodo {
    char fecha[10];
    double manana;
    double tarde;
    double noche;
    struct nodo *sig;
};

struct nodo *cab = NULL, *aux, *aux2;

void registrarTemperatura(char* fecha, double manana, double tarde, double noche) {
    aux = (struct nodo*) malloc(sizeof(struct nodo));
    strcpy(aux->fecha, fecha);
    aux->manana = manana;
    aux->tarde = tarde;
    aux->noche = noche;
    aux->sig = NULL;

    if (cab == NULL) {
        cab = aux;
    } else {
        aux2 = cab;
        while (aux2->sig != NULL) {
            aux2 = aux2->sig;
        }
        aux2->sig = aux;
    }
}

void mostrarRegistros() {
    aux = cab;
    while (aux != NULL) {
        printf("Fecha: %s, Mañana: %.2f, Tarde: %.2f, Noche: %.2f\n", aux->fecha, aux->manana, aux->tarde, aux->noche);
        aux = aux->sig;
    }
}

double promedioDia(char* fecha) {
    aux = cab;
    while (aux != NULL) {
        if (strcmp(aux->fecha, fecha) == 0) {
            return (aux->manana + aux->tarde + aux->noche) / 3.0;
        }
        aux = aux->sig;
    }
    return -1;  // Retorna -1 si la fecha no se encuentra en los registros
}

double promedioTotal() {
    double total = 0;
    int count = 0;

    aux = cab;
    while (aux != NULL) {
        total += aux->manana;
        total += aux->tarde;
        total += aux->noche;
        count += 3;
        aux = aux->sig;
    }

    return total / count;
}

int main() {
    registrarTemperatura("2024-03-09", 20.5, 25.3, 18.7);
    mostrarRegistros();
    printf("Promedio del día 2024-03-09: %.2f\n", promedioDia("2024-03-09"));
    printf("Promedio total: %.2f\n", promedioTotal());

    return 0;
}
