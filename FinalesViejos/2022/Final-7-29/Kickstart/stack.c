#include <stdlib.h>
#include <assert.h>

#include "stack.h"

#define CHUNK_CAPACITY 5

struct s_node {
    stack_elem *chunk;
    unsigned int used;
    struct s_node *top;
};

struct _s_stack {
    struct s_node *head;
    unsigned int size; 
};

static bool valid_chunks(stack s) {
    bool valid=true;
    struct s_node *check=s->head;
    if (check!=NULL) {
       valid = check->used <= CHUNK_CAPACITY && check->chunk != NULL;
    }
    return valid;
}

static bool invrep(stack s) {
    bool res = true; 
    res = s!=NULL;
    struct s_node *aux = s->head;
    unsigned int length = 0u;

    while(aux!=NULL){
        length += aux->used;
        aux = aux->top;
    }
    res = res && length == s->size;
    return (res && valid_chunks(s));
}

static struct s_node *create_node(void) {
    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));  // Reservamos memoria para el nuevo nodo
    assert(new_node!=NULL);  // Verificamos que se reservo correctamente
    new_node->used = 0;
    new_node->top = NULL;
    new_node->chunk = malloc(sizeof(stack_elem) * CHUNK_CAPACITY);  // Reservamos la memoria para el chunk
    return new_node;
}


static struct s_node *destroy_node(struct s_node *node) {
    free(node->chunk);
    free(node);
    node = NULL;
    return node;
}

stack stack_empty(void) {
     struct _s_stack *s_empty = NULL;
    s_empty = malloc(sizeof(struct _s_stack));  // Reservamos espacio para el stack 
    assert(s_empty!=NULL);  // Verificamos que se reservo correctamente

    // Completamos los campos
    s_empty->head = create_node();
    s_empty->size = 0u;

    assert(invrep(s_empty));

    return s_empty;
}

stack stack_push(stack s, stack_elem e) {
    assert(invrep(s));
    struct s_node *aux = s->head;
    
    // Caso especial cuando tenemos el chunk lleno 
    if(aux->used == CHUNK_CAPACITY){
        struct s_node *new_node = create_node();
        new_node->top = s->head;
        s->head = new_node;
        aux  = s->head;
    } 

    aux->chunk[aux->used] = e;  // Agregamos el elemento "e" en la posicion correcta 
    // Actualizamos la cantidad usada ya que agregamos un elemento
    aux->used++;  
    s->size++;
    assert(invrep(s));
    return s;
}

stack stack_pop(stack s) {
    assert(invrep(s) && !stack_is_empty(s));
    struct s_node *aux = s->head;
    aux->used--;
    s->size--;
    if(aux->used == 0 && s->size > 0){
        s->head = aux->top;
        destroy_node(aux);
    }
    assert(invrep(s));
    return s;
}


unsigned int stack_size(stack s) {
    return s->size;
}

void stack_top(stack s, stack_elem *top) {
    *top = s->head->chunk[s->head->used - 1];
}

bool stack_is_empty(stack s) {
    return s->size == 0;
}

stack_elem *stack_to_array(stack s) {
    assert(invrep(s));
    if(stack_is_empty(s)){
        return NULL;
    }
    stack_elem *array = NULL;
    array = malloc(sizeof(stack_elem)
    * s->size);
    struct s_node *aux = s->head;
    unsigned int i = s->size - 1;  // Ya que empezamos de atras hacia adelante
    if(s->size == 0){
        return array;
    }
    while(aux!=NULL){
        for(unsigned int j = aux->used - 1; j<=0; j--){
            array[i] = aux->chunk[j];
            i--;
        }
        aux = aux->top;
    }
    return array;
}


stack stack_destroy(stack s) {
    assert(invrep(s));
    struct s_node *aux = s->head; 
    while(aux!= NULL){
        struct s_node *next = aux->top;
        destroy_node(aux);
        aux = next;
    }
    free(s);
    return NULL;
}

