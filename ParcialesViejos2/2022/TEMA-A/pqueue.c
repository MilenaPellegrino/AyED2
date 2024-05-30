#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"

// Estrcutura principal:
struct s_pqueue {
    struct s_node *head;    // Puntero al primer nodo de la pqueue
    unsigned int length;
};

struct s_node {
    pqueue_elem elem;
    unsigned int priority; 
    struct s_node *next;
};

static struct s_node * create_node(pqueue_elem e, unsigned int priority) {
    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));
    assert(new_node!=NULL); // Verificamos que se reservo memoria correctamente
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


static bool invrep(pqueue q) {
    if(q == NULL) return false;
    if(q->length == 0)return q->head == NULL;   // Si el tamano de la pqueue es 0 el head tiene que ser NULL ya que no hay ningun elemento en la cola 
    return true;
}

pqueue pqueue_empty(void) {
    pqueue q=NULL;
    q = malloc(sizeof(struct s_pqueue));
    assert(q!= NULL); // Nos aseguramos que se reservo bien la memoria 
    q->length = 0; 
    q->head = NULL;
    assert(invrep(q) && pqueue_is_empty(q));
    return q;
}

pqueue pqueue_enqueue(pqueue q, pqueue_elem e, unsigned int priority) {
    assert(invrep(q));
    struct s_node *new_node = create_node(e, priority);
    
    // Caso particular si la pqueue esta vacia: 
    if(pqueue_is_empty(q)){
        q->head = new_node;
    } else { // Recorremos la pqueue q hasta encontrar el lugar donde vamos a insertar este nuevo elemento
       struct s_node *aux = q->head;
       struct s_node *pred = NULL;
        while(aux!=NULL && aux->priority >= priority ){
            pred = aux;
            aux = aux->next;
        }
        // Insertar el nuevo nodo en la posición correcta
        if (pred == NULL) { // Insertar al inicio
            new_node->next = q->head;
            q->head = new_node;
        } else {
            new_node->next = aux;
            pred->next = new_node;
        }
    }
    q->length++;
    destroy_node(new_node);
    assert(invrep(q) && !pqueue_is_empty(q));
    return q;
}

bool pqueue_is_empty(pqueue q) {
    assert(invrep(q));
    return q->length == 0;
}

pqueue_elem pqueue_peek(pqueue q) {
    assert(invrep(q) && !pqueue_is_empty(q));
    return q->head->elem;        
}

unsigned int pqueue_peek_priority(pqueue q) {
    assert(invrep(q) && !pqueue_is_empty(q));
    return q->head->priority;
}

unsigned int pqueue_size(pqueue q) {
    assert(invrep(q));
    unsigned int size=0;
    size = q->length;
    return size;
}

pqueue pqueue_dequeue(pqueue q) {
    assert(invrep(q) && !pqueue_is_empty(q));
    q->head = q->head->next;
    assert(invrep(q)); 
    return q;
}

pqueue pqueue_destroy(pqueue q) {
    assert(invrep(q));
    free(q);
    q= NULL;
    assert(q == NULL);
    return q;
}
