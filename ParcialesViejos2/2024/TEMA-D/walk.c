#include <stdio.h>
#include <stdlib.h>
#include "walk.h"
#include <assert.h>


#define STEP_NORTH 0
#define STEP_EAST 1
#define STEP_NORTHEAST 2
#define STEP_UNDEF 3

typedef unsigned int step_t;
typedef unsigned int uint_t;

struct _node_t {
    step_t step;
    struct _node_t *next;
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
    struct _walk_t *c = NULL;
    c = malloc(sizeof(struct _walk_t));
    assert(c!=NULL);
    c->height = 0;
    c->init= NULL;
    c->last = NULL;
    c->length = 0;
    c->width = 0;
    assert(walk_length(c)== 0);
    return c;

}

static node_t create_node(void){
    struct _node_t *new_node = NULL;
    new_node = malloc(sizeof(struct _node_t));
    assert(new_node!=NULL);
    new_node->next = NULL;
    new_node->step = STEP_UNDEF;
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

static walk_t walk_add(walk_t walk , node_t new_node) {
    if (walk->init == NULL)
    {
        walk->init = new_node;
    }
    else
    {
        struct _node_t *aux = walk->init;
        while (aux->next!= NULL)
        {
        aux = aux->next;
        }
        aux->next = new_node;
    }
    if (new_node->step ==STEP_NORTH)
    {
        walk->height++;
    }
    else if (new_node->step==STEP_EAST)
    {
        walk->width++;
    }
    else if (new_node->step == STEP_NORTHEAST)
    {
        walk->height++;
        walk->width++;
    }
    walk->last = new_node;
    walk->length++;
    assert(walk_length(walk)> 0);
    return walk;
}

walk_t walk_north(walk_t walk) {
    struct _node_t *new_node = create_node();
    new_node->step = STEP_NORTH;
    walk = walk_add(walk,new_node);
    assert(walk_length(walk)> 0);
    return walk;
}

walk_t walk_east(walk_t walk) {
    struct _node_t *new_node = create_node();
    new_node->step = STEP_EAST;
    walk = walk_add(walk,new_node);
    assert(walk_length(walk)> 0);
    return walk;
}

walk_t walk_northeast(walk_t walk) {
    struct _node_t *new_node = create_node();
    new_node->step = STEP_NORTHEAST;
    walk = walk_add(walk,new_node);
    assert(walk_length(walk)> 0);
    return walk;
}

unsigned int walk_length(walk_t walk) {
    return walk->length;
}

unsigned int walk_height(walk_t walk) {
    return walk->height;
}

unsigned int walk_width(walk_t walk) {
    return walk->width;
}

walk_t walk_extend(walk_t walk, unsigned int east, unsigned int north) {
    for (size_t i = 0; i < east ; i++)
    {
        walk = walk_east(walk);
    }
    for (size_t i = 0; i < north ; i++)
    {
        walk = walk_north(walk);
    }
    assert(walk_length(walk)> 0 || (east==0 && north == 0) );
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
    assert(walk!=NULL);
    struct _node_t *aux = walk->init;
    struct _node_t *killme;
    if (aux == NULL)
    {
        free(walk);
        walk = NULL;
        assert(walk==NULL);
        return walk;
    }
    while (aux->next!=NULL)
    {
        killme = aux;
        aux = aux->next;
        killme = destroy_node(killme);
    }
    aux = destroy_node(aux);
    walk->init = NULL;
    free(walk);
    walk = NULL;
    assert(walk==NULL);
    return(walk);
}