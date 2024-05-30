#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pstack.h"

// FALTA TERMINAR, HICE UN PAR DE FUNCIONES NOMAS
struct s_pstack {
    struct s_node *front;
    unsigned int size;
};

struct s_node {
    pstack_elem elem; 
    priority_t priority;
    struct s_node *next;
};

static struct s_node * create_node(pstack_elem e, priority_t priority) {
    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));   // Reservamos memoria para el nuevo nodo
    assert(new_node!=NULL);     // Verificar que se reservo correctamente
    new_node->elem = e; 
    new_node->priority = priority; 
    new_node->next = NULL;
    return new_node;
}

static struct s_node * destroy_node(struct s_node *node) {
    assert(node != NULL);
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}


static bool invrep(pstack s) {
    if(s == NULL) return false;
    if(s->size == 0) return s->front->next == NULL;
    return true;
}

pstack pstack_empty(void) {
    pstack s=NULL;
    s = malloc(sizeof(struct s_pstack));
    assert(s!=NULL);
    s->size = 0;
    s->front = NULL;
    assert(invrep(s) && pstack_is_empty(s));
    return s;
}

pstack pstack_push(pstack s, pstack_elem e, priority_t priority) {
    assert(invrep(s));
    struct s_node *new_node = create_node(e, priority);
    struct s_node *aux = s->front;
    struct s_node *prev = NULL;
    while(aux->priority <= new_node->priority){
        prev = aux;
        aux = aux->next;
    }
    prev->next = new_node; 
    new_node->next = aux;
    assert(invrep(s) && !pstack_is_empty(s)); 
    return s;
}

bool pstack_is_empty(pstack s) {
    /*
     * COMPLETAR
     *
     */
    assert(s==s);
    return true;
}

pstack_elem pstack_top(pstack s) {
    assert(s==s);
    return 0;
}

priority_t pstack_top_priority(pstack s) {
    /*
     * COMPLETAR
     *
     */
    assert(s==s); // BORRAR ESTE ASSERT
    return worst;
}

unsigned int pstack_size(pstack s) {
    assert(invrep(s));
    unsigned int size=0;
    /*
     * COMPLETAR
     *
     */
    return size;
}

pstack pstack_pop(pstack s) {
    struct s_node *eliminar = NULL;
    eliminar = malloc(sizeof(struct s_node));
    assert(eliminar != NULL); 
    eliminar = s->front;
    s->front = s->front->next;
    destroy_node(eliminar);
    assert(s==s); // BORRAR ESTE ASSERT
    return s;
}

pstack pstack_destroy(pstack s) {
    assert(invrep(s));
    /*
     * COMPLETAR
     *
     */
    assert(s == NULL);
    return s;
}

