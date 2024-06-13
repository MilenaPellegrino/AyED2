// CREO QEU TIENE PROBLEMA DE VALGRIND, MEORY LEAKS Y ESAS MIERDAS
#include <stdio.h>
#include <stdlib.h>
#include "walk.h"
#include <assert.h>
#include <stdbool.h>

#define STEP_NORTH 0
#define STEP_EAST 1
#define STEP_NORTHEAST 2
#define STEP_UNDEF 3

typedef unsigned int step_t;
typedef unsigned int uint_t;

struct _node_t {
    step_t step;    // Representa un paso 
    struct _node_t *next;   // Puntero al siguiente nodo
};

typedef struct _node_t *node_t;

struct _walk_t {
    node_t init;
    node_t last;
    uint_t length;
    uint_t width;
    uint_t height;
};

walk_t walk_empty(void) {
    walk_t walk_empty = NULL;
    walk_empty = malloc(sizeof(struct _walk_t));
    assert(walk_empty != NULL); // Verifico que se asigno memoria correctamente
    walk_empty->init = NULL; 
    walk_empty->length = 0;
    walk_empty->last = NULL;
    walk_empty->width = 0;
    walk_empty->height = 0;
    assert(walk_length(walk_empty) == 0);
    return walk_empty;
}

static node_t create_node(void){
    node_t new_node = NULL;
    new_node = malloc(sizeof(struct _node_t));
    assert(new_node != NULL); // Verifico que se asigno memoria correctamente
    new_node->step = STEP_UNDEF;
    new_node->next = NULL; 
    return new_node;
}

static node_t destroy_node(node_t node){
    assert(node!=NULL);
    node->next = NULL;
    free(node);
    node = NULL;
    assert(node==NULL);
    return node;
}

// Devuelve true si la caminata "esta vacia"
bool is_empty_walk(walk_t walk){
	return walk->length == 0;
}

static walk_t walk_add(walk_t walk , node_t new_node) {
    // Caso especial cuando la caminata esta vacia: 
    if(is_empty_walk(walk)){
        walk->init = new_node;  // Agregamos como inicio el nuevo nodo
    } else {
        node_t aux = walk->init;    // Creamos un nodo auxiliar para recorrer la lista
        // Recorremos todos los nodos de nuestra caminata
        while(aux->next != NULL){
            aux = aux->next;
        } 
        aux->next  = new_node;  // Agregamos el nuevo nodo al final de la caminata
        // Actualizmo el tamano de la caminata segun el tipo de paso que anadimos
        if (new_node->step == STEP_NORTH){
            walk->height++; 
        } else if (new_node->step == STEP_EAST) {
            walk->width++;
        } else if (new_node->step == STEP_NORTHEAST) {
            walk->height++;
            walk->width++;
        }
    }
    walk->last = new_node;
    walk->length++;  // Sumamos uno a la longitud ya que agregamos un nodo
    assert(walk_length(walk) > 0 ); 
    return walk;
}

walk_t walk_north(walk_t walk) {
    // Creamos el nuevo nodo que vamos a agregar a la caminata
    node_t nodo_add = create_node();
    nodo_add->step = STEP_NORTH;
    walk = walk_add(walk, nodo_add);
    assert(walk_length(walk) > 0);
    return walk;
}

walk_t walk_east(walk_t walk) {
    // Creamos el nuevo nodo que vamos a agregar a la caminata
    node_t nodo_add = create_node();
    nodo_add->step = STEP_EAST;
    walk = walk_add(walk, nodo_add);
    assert(walk_length(walk) > 0);
    return walk;
}

walk_t walk_northeast(walk_t walk) {
    // Creamos el nuevo nodo que vamos a agregar a la caminata
    node_t nodo_add = create_node();
    nodo_add->step = STEP_NORTHEAST;
    walk = walk_add(walk, nodo_add);
    assert(walk_length(walk) > 0);
    return walk;
}

unsigned int walk_height(walk_t walk) {
   return walk->height;
}

unsigned int walk_width(walk_t walk) {
    return walk->width;
}

unsigned int walk_length(walk_t walk) {
    return walk->length;
}

walk_t walk_extend(walk_t walk, unsigned int east, unsigned int north) {
    // Como no hay un orden para extener la caminata, elijo el siguiente: 
    // Primero extiendo hacia el este y despues hacia el norte 
    unsigned int add_e = east; // Pasos a agregar al este 
    while(add_e != 0){
        walk = walk_east(walk);
        add_e--;
    }
    unsigned int add_n = north; // Pasos a agregar al norte
    while(add_n != 0){
        walk = walk_north(walk);
        add_n--;  
    }
    assert(walk_length(walk) > 0 || (east == 0 && north == 0));
    return walk;
}

static char step_to_char (step_t step) {
    char c = '\0';
    switch (step) {
        case STEP_NORTH : c = 'N'; break;
        case STEP_EAST  : c = 'E' ; break;
        case STEP_NORTHEAST : c = 'X' ; break;
        default : c = 'U' ; break;
    }
    return c;
}

void walk_dump(walk_t walk) {
    assert(walk != NULL);
    node_t current = NULL;
    current = walk -> init;
    printf("[");
    while (current != NULL) {
        printf("%c",step_to_char(current->step));
        if(current->next != NULL) {
            printf(" -> ");
        }
        current = current -> next;
    }
    printf("] (L=%u,W=%u,H=%u)\n",walk->length,walk->width,walk->height);
}

walk_t walk_destroy(walk_t walk) {
    assert(walk != NULL);  // Nos aseguramos que no sea NULL 
    struct _node_t *aux = walk->init; // Puntero auxiliar para recorrer la lista de nodos.
    struct _node_t *killme; // Puntero para mantener el nodo que se va a eliminar.

    if (aux == NULL) { // Si la caminata está vacía (sin nodos)
        free(walk); // Libera la memoria asignada a la estructura `walk`.
        walk = NULL; // Establece `walk` a NULL.
        assert(walk == NULL); // Asegura que `walk` sea NULL.
        return walk; // Retorna `walk`, que ahora es NULL.
    }
    // Bucle para recorrer y destruir todos los nodos de la lista.
    while (aux != NULL) {
        killme = aux; // Mantiene el nodo actual.
        aux = aux->next; // Avanza al siguiente nodo.
        killme = destroy_node(killme); // Destruye el nodo actual y lo libera.
    }
    walk->init = NULL; // Establece el puntero inicial de `walk` a NULL.
    free(walk); // Libera la memoria asignada a la estructura `walk`.
    walk = NULL; // Establece `walk` a NULL.
    assert(walk == NULL); // Asegura que `walk` sea NULL.
    return walk; // Retorna `walk`, que ahora es NULL.
}
