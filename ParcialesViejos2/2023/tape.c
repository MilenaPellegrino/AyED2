#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "tape.h"

struct _s_node {
    tape_elem elem;
    struct _s_node *next;
};

typedef struct _s_node * node_t;

struct _s_tape {
    unsigned int size;
    node_t cursor;  // Puntero al elemento actual
    node_t start;   // Puntero al primer nodo de la cinta
};

static bool invrep(tape_t tape) {
    // Verifico que tape no sea null 
    if(tape == NULL) return false; 
    // Si el tamaño es 0, cursor debe ser null y start debe ser null 
    if(tape->size == 0) return tape->cursor == NULL && tape->start == NULL;
    // Tambien podria ver que si el tamano es >0 el numero nodos sea igual al size
    return true;
}

static node_t create_node(tape_elem e) {
    node_t new_node = NULL;
    new_node = malloc(sizeof(struct _s_node)); // reservamos la memoria correspondiente
    if(new_node!= NULL){ // Si se reservo memoria
        new_node->elem = e; // Asignamos el valor del elemento que tendra el nuevo nodo 
        new_node->next = NULL;
    }
    return new_node;
}

static node_t destroy_node(node_t node) {
    free(node);
    return NULL;
}

tape_t tape_create(void) {
    tape_t tape = NULL;
    tape = malloc(sizeof(struct _s_tape)); // Reservamos memoria para el nuevo tape
    if(tape!= NULL){  // Si se reservo memoria correctamente
        tape->cursor = NULL;
        tape->start = NULL;
        tape->size = 0;
    }
    assert(invrep(tape) && tape_is_empty(tape) && tape_at_start(tape));
    return tape;
}

tape_t tape_rewind(tape_t tape) {
    tape->cursor = tape->start; // Colocamos al curso al principio de la lista 
    assert(invrep(tape) && tape_at_start(tape)); //  Que se cumpla la invariante y este el cursor al principio de la lista 
    return tape;
}


bool tape_at_start(tape_t tape) {
    assert(invrep(tape));
    if(tape->cursor == tape->start)return true;
    return false;
}

bool tape_at_stop(tape_t tape) {
    assert(invrep(tape));
    if(tape->cursor == NULL)return true;
    return false;
}

bool tape_is_empty(tape_t tape) {
    assert(invrep((tape)));
    if(tape->size == 0)return true;
    return false;
}

unsigned int tape_length(tape_t tape) {
    assert(invrep(tape));
    return tape->size;
}

tape_t tape_step(tape_t tape) {
    assert(invrep(tape) && !tape_at_stop(tape)); // Se cumple la invariante y se puede avanzar uno al cursor
    if(tape->cursor != NULL){
        tape->cursor = tape->cursor->next;
    }
    assert(invrep(tape)); // despues de manipular el tape, quiero que se siga cumpliendo la invariante
    return tape;
}


tape_t tape_insertl(tape_t tape, tape_elem e) {
    assert(invrep(tape));
    // Creo un nuevo nodo con el elemento que voy a agregar
    node_t node_temp = create_node(e);
    if(node_temp == NULL) return tape; // Tuve un error al crear el nodo
    if(tape->cursor == tape->start){ // Insertar el nodo al principio del tape
        node_temp->next = tape->start;  // Enlanzamos el nuevo nodo al comienzo del tape
        tape->start = node_temp;    // Actualizamos para que el start apunte al nuevo nodo
    } else { // Caso general donde el nodo a agregar no es en el principio del tape
        node_t prev = tape->start;  // Iniciamos el nuevo nodo al comienzo
        while(prev->next != tape->cursor){  // Recorremoos el tape hasta que el siguiente sea el cursor
            prev = prev->next;
        }
        node_temp->next = tape->cursor; // El siguiente del nodo a agregar va apuntar al cursor 
        prev->next = node_temp; // El siguiente del prev (el que estaba en un comienzo antes del cursos) ahora va a ser el nuevo nodo. 
    }
    tape->size++;
    assert(invrep(tape) && !tape_is_empty(tape));
    return tape;
}

tape_t tape_insertr(tape_t tape, tape_elem e) {
    assert(invrep(tape));
    // Creamos el nodo que vamos a agregar con el elemento e
    node_t new_node = create_node(e);
    if(new_node == NULL) return tape;   // No se creo bien el ndo
    if(tape_is_empty(tape)){    // Caso especial si el tape esta vacio
        tape->start = new_node;
        tape->cursor = new_node;
    }else { // Caso general
        new_node->next = tape->cursor->next;
        tape->cursor->next = new_node;
        tape->cursor = new_node;
    }
    tape->size++;
    assert(invrep(tape) && !tape_is_empty(tape) && !tape_at_stop(tape) && e == tape_read(tape));
    return tape;
}

tape_t tape_erase(tape_t tape) {
    assert(invrep(tape) && !tape_is_empty(tape) && !tape_at_stop(tape));
    node_t delete = tape->cursor;
    if(tape->cursor == tape->start){    // Caso especial si mi cursor esta al inicio del tape
        tape->start = tape->cursor->next;
    } else {
        node_t node_temp = tape->start;
        while(node_temp->next != tape->cursor){
            node_temp = node_temp->next;
        }
        node_temp->next = tape->cursor->next;
        tape->cursor = tape->cursor->next;
    }
    delete = destroy_node(delete);
    tape->size--;
    assert(invrep(tape));
    return tape;
}

tape_elem tape_read(tape_t tape) {
    assert(invrep(tape) && !tape_is_empty(tape) && !tape_at_stop(tape));
    return tape->cursor->elem;
}

void tape_dump(tape_t tape) {
    assert(invrep(tape));
    node_t node=tape->start;
    printf("#");
    while (node != NULL) {
        if (node != tape->cursor) {
            printf("-%c-", node->elem);
        } else {
            printf("-[%c]-", node->elem);
        }
        node = node->next;
    }
    if (tape->cursor==NULL) {
        printf("-[]-");
    }
    printf("#\n");
}

tape_t tape_copy(tape_t tape) {
    assert(invrep(tape)); 
    
    // Crear una nueva cinta para la copia
    tape_t copy = tape_create();
    if (tape->size == 0) {
        return copy;  // Si la cinta original está vacía, devolver la copia vacía
    }

    // Copiar los elementos de la cinta original a la copia
    node_t node_temp = tape->start;
    while (node_temp != NULL) {
        copy = tape_insertr(copy, node_temp->elem);
        node_temp = node_temp->next;
    }

    // Establecer el cursor en la copia al mismo nodo que en la cinta original
    node_t cursor_node_original = tape->cursor;
    node_t cursor_node_copy = copy->start;
    while (cursor_node_original != NULL && cursor_node_original != tape->cursor) {
        cursor_node_original = cursor_node_original->next;
        cursor_node_copy = cursor_node_copy->next;
    }
    copy->cursor = cursor_node_copy;

    // Verificar las invariantes después de la copia
    assert(invrep(tape) && invrep(copy) && tape_length(tape) == tape_length(copy));
    
    return copy;
}


tape_t tape_destroy(tape_t tape) {
    assert(invrep(tape));
    node_t node_temp = tape->start; 
    while(node_temp!=NULL){
        node_t delete = node_temp;
        node_temp = node_temp->next;
        delete = destroy_node(delete);
    }
    assert(invrep(tape));
    free(tape);
    return NULL;
}


