#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"

// Estructura principal: 
struct s_pqueue {
    struct s_node *front;   // Nodo que apunta al principio de la pqueue
    unsigned int size;      // Espacio para guardar el tamano de la pqueue en cada momento
};

// Nodo de la pqueue
struct s_node {
    pqueue_elem elem;
    average_gradet average_grade;
    approved_courset approved_course;
    priorityt priority;
    struct s_node *next;
};

priorityt calc_priority(average_gradet average_grade, approved_courset aprov_course){
    priorityt priority = 0.5 * (average_grade/MAX_GRADE) + 0.5 * (aprov_course/TOTAL_COURSES);
    assert(priority >= 0 && priority <= 1);
    return priority;
}

static struct s_node *create_node(pqueue_elem e, float average_grade, unsigned int approved_courses){
    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));   // Reservamos la memoria para el nuevo nodo
    assert(new_node!=NULL);     // Verificamos que se reservo correctamente
    new_node->approved_course = approved_courses;
    new_node->average_grade = average_grade;
    new_node->elem = e;
    new_node->next = NULL;
    new_node->priority = calc_priority(average_grade, approved_courses);
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
    if(q->size == 0){
        return q->front == NULL;
    }
    return true;
}

pqueue pqueue_empty(void) {
    pqueue q=NULL;
    q = malloc(sizeof(struct s_pqueue));    // Resrvar memoria para la nueva pqueue
    assert(q!=NULL);
    q->size = 0;
    q->front = NULL;
    assert(invrep(q) && pqueue_is_empty(q));
    return q;
}

pqueue pqueue_enqueue(pqueue q, pqueue_elem e, float average_grade, unsigned int approved_courses){
    assert(invrep(q));
    struct s_node *new_node = create_node(e, average_grade, approved_courses);
    
    // Caso especial cuando la pqueue esta vacia: 
    if(pqueue_is_empty(q)){
        q->front = new_node;
    } else {
        struct s_node *prev = NULL;     // Guardo el anterior
        struct s_node *aux = q->front;  // Voy recorriendo los nodos hasta encontrar el que es mayor estricto
        while( aux != NULL && aux->priority <= new_node->priority){
            prev = aux;
            aux = aux->next;
        }
         if (prev == NULL) {
            // Insertar al inicio
            new_node->next = q->front;
            q->front = new_node;
        } else {
            // Insertar en el medio o al final
            prev->next = new_node;
            new_node->next = aux;
        }
    }
    q->size++;
    assert(invrep(q) && !pqueue_is_empty(q)); 
    return q;
}

bool pqueue_is_empty(pqueue q) {
    assert(invrep(q)); 
    return q->size == 0;
}

unsigned int pqueue_size(pqueue q){
    assert(invrep(q));
    return q->size;
}
pqueue_elem pqueue_peek(pqueue q) {
    assert(invrep(q) && !pqueue_is_empty(q)); 
    return q->front->elem;
}

float pqueue_peek_average_grade(pqueue q){
    assert(invrep(q) && !pqueue_is_empty(q)); 
    return q->front->approved_course;
}

unsigned int pqueue_peek_approved_courses(pqueue q){
    assert(invrep(q) && !pqueue_is_empty(q)); 
    return q->front->approved_course;
}

float pqueue_peek_priority(pqueue q){
    assert(invrep(q) && !pqueue_is_empty(q)); 
    return q->front->priority;
}

pqueue pqueue_copy(pqueue q){
     assert(invrep(q));
     // Creamos una pqueue vacia  donde vamos a copiar todos los nodos de "q"
     pqueue copy = pqueue_empty();  

     // Si la pqueue a copiar esta vacia, 
     // simplemente reotrnamos la que creamos que al crearla esta vacia
     if(pqueue_is_empty(q)) return copy;    
    
    // Primero copiamos el primer nodo
    copy->front = create_node(q->front->elem, q->front->average_grade, q->front->approved_course);
    copy->size = q->size;

    // Recorremos los nodos restantes de la pqueue "q" y los copiamos a nuestra copia
    struct s_node *actual_original = q->front->next;
    struct s_node *actual_copia = copy->front;
    while(actual_original != NULL){
        actual_copia->next = create_node(actual_original->elem, actual_original->average_grade, actual_original->approved_course);
        actual_original = actual_original->next;
        actual_copia = actual_copia->next;
    }

    assert(invrep(copy));
    return copy;
}

pqueue pqueue_dequeue(pqueue q){
    assert(invrep(q) && !pqueue_is_empty(q)); 

    struct s_node *eliminar = q->front;
    q->front = q->front->next;
    eliminar = destroy_node(eliminar);
    q->size--;
    assert(invrep(q));
    return q; 
}

pqueue pqueue_destroy(pqueue q) {
    assert(invrep(q));
    free(q);
    q = NULL;
    assert(q == NULL);
    return q;
}