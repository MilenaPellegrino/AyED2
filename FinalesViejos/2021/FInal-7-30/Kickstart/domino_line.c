#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "domino_line.h"
#include "domino.h"

struct _s_line {
    struct s_node *head;
    unsigned int size;
};

struct s_node{
    struct s_node *next;
    domino tile;
};

typedef struct s_node * node_s;

static node_s create_node(domino tile){
    node_s nodo;
    nodo = malloc(sizeof(struct s_node));
    assert(nodo!=NULL);
    nodo->next = NULL;
    nodo->tile = tile;
    return nodo;
}

domino_line line_new(domino first) {
    assert(domino_is_double(first));
    domino_line new_line = NULL;
    new_line = malloc(sizeof(struct _s_line));
    assert(new_line!=NULL);
    new_line->head = create_node(first);
    new_line->size = 1;
    return new_line;
}


domino_line line_add(domino_line line, domino t) {
    // Tenemos que agregar la ficha t, a la linea line 
    // Vamos a recorrer la linea hasta el final y ahi agregar la nueva ficha 

    struct s_node *tile = create_node(t);  // Creamos el nodo de la ficha que vamos a agregar
    struct s_node *aux = line->head;  // Creamos un nodo auxiliar para recorrer la linea
    while(aux->next!=NULL){
        aux = aux->next;
    }
    // A lo siguiente que va apuntar el ultimo nodo actual es a la nueva ficha
    aux->next = tile;
    line->size++; // Actualizamos el tamano 
    return line;
}

unsigned int line_length(domino_line line) {
    return line->size;
}

bool line_n_correct(domino_line line, unsigned int n) {
    struct s_node *aux = line->head;  // Creamos un nodo auxiliar para recorrer la linea
    struct s_node *prev = NULL;  // Nodo prev para guardar el anterior al aux
    unsigned int cont = 0;  // Para llevar cuantas fichas vamos recorriendo
    while (aux != NULL && cont != n) {
        prev = aux;
        aux = aux->next;
        cont++;
    }
    bool correct = false;
    // Caso especial cuando n == 0, es decir cuando queremos ver si la primer ficha está bien ubicada 
    if (n == 0 && aux != NULL && aux->next != NULL && domino_matches(aux->tile, aux->next->tile)) {
        correct = true;
    } else if (n == line->size - 1 && prev != NULL && domino_matches(prev->tile, aux->tile)) {  // Caso especial cuando es la última ficha de la línea
        correct = true;
    } else if (n != 0 && n != line->size - 1 && aux != NULL && aux->next != NULL) {  // Caso general
        correct = domino_matches(aux->tile, aux->next->tile) && domino_matches(prev->tile, aux->tile);
    }
    return correct;
}

int line_total_points(domino_line line) {
    // La idea es ir recorriendo cada ficha de  la linea y sumar su puntaje
    int res = 0;
    struct s_node *aux = line->head;  // Creamos un nodo auxiliar para recorrer la linea
    while(aux!=NULL){
        res += domino_up(aux->tile);  // Puntaje de la ficha de arriba
        res+= domino_down(aux->tile); // Puntaje de la ficha de abajo
        aux = aux->next;
    }
    return res;
}


domino * line_to_array(domino_line line) {
    domino *tiles;  // Creamos e array donde vamos a almacenar las fichas 
    tiles = calloc(line->size, sizeof(domino));  // Reservamos memoria para el array
    node_s aux = line->head;
    // Rellenamos el array 
    for(unsigned int i = 0; i<line->size; i++){
        tiles[i] = aux->tile;
        aux = aux->next;
    }
    return tiles;
}

void line_dump(domino_line line) {
    domino *array=line_to_array(line);
    for (unsigned int i=0u; i < line_length(line); i++) {
        domino_dump(array[i]);
    }
    free(array);
}

domino_line line_destroy(domino_line line) {
    struct s_node *aux = line->head;  // Nodo auxiliar para recorrer la line 
    struct s_node *prev = NULL;  // Nodo prev que contiene el nodo previo al auxiliar para ir destruyendo
    while(aux!=NULL){
        prev=aux;
        aux = aux->next;
        domino_destroy(prev->tile);
        free(prev);
    }
    free(line);
    line = NULL;
    return line;
}

