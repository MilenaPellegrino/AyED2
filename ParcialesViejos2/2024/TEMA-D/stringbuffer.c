#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <assert.h>

#include "stringbuffer.h"

struct s_stringbuffer {
    struct s_node *first;
    struct s_node *last;
    unsigned int length;
};

struct s_node {
    char c;
    struct s_node *nextchar;
};

typedef struct s_node * node_t;

// Representation invariant
static bool invrep(stringbuffer sb) {
    return (sb != NULL) && (sb->first == NULL && sb->last == NULL) == (sb->length == 0);
}


// Creates a single node in dynamic memory with a given char
static node_t create_node(char c) {
    node_t newnode=NULL;    
    // Reservamos memoria para el nodo 
    newnode = malloc(sizeof(struct s_node));
    assert(newnode != NULL); // Verificamos que se asigno correctamente
    newnode->c = c; // Agregamos el caracter al nodo
    newnode->nextchar = NULL;
    return newnode;
}


// Destroy a single node
static node_t destroy_node(node_t node) {
    assert(node!= NULL); 
    node->nextchar = NULL;
    free(node);
    node = NULL;
    assert(node==NULL);
    return node;
}


// Public functions of the Abstract Data Type
stringbuffer stringbuffer_empty(void) {
    stringbuffer sb = malloc(sizeof(struct s_stringbuffer));
    sb->first = NULL;
    sb->last = NULL;
    sb->length = 0;

    assert(invrep(sb));
    return sb;
}


stringbuffer stringbuffer_create(const char *word) {
    assert(word!=NULL);
    node_t newnode = NULL;
    node_t first = NULL;
    node_t last = NULL;

    stringbuffer sb = stringbuffer_empty();
    sb->length = strlen(word);

    // Agregamos el primer caracter: 
    newnode = create_node(word[0]);
    first = newnode;
    sb->first = first;
    sb->last = first;

    // Recorremos los otros caracteres
    for (unsigned int i =1; i<strlen(word); i++){
        last = create_node(word[i]);
        sb->last->nextchar = last; 
        sb->last = last;
    }
    assert(invrep(sb) && !stringbuffer_is_empty(sb));
    return sb;
}


bool stringbuffer_is_empty(stringbuffer sb){
    assert(invrep(sb));
    return (sb->first == NULL && sb->last == NULL);
}


stringbuffer stringbuffer_append(stringbuffer sb, const char c) {
    assert(invrep(sb));
    node_t new_node = create_node(c);

    if(sb->first == NULL) {
        sb->first = new_node;
        sb->last = new_node;
    }
    else {
        sb->last->nextchar = new_node;
        sb->last = new_node;
    }
    sb->length++;

    assert(invrep(sb) && !stringbuffer_is_empty(sb));
    return sb;
}


char stringbuffer_char_at(stringbuffer sb, unsigned int index) {
    assert(invrep(sb) && !stringbuffer_is_empty(sb) && index<stringbuffer_length(sb));
    unsigned pos = 0;
    node_t node = sb->first;
    while(pos < index) {
        node = node->nextchar;
        pos++;
    }
    return node->c;
}


stringbuffer stringbuffer_remove(stringbuffer sb, unsigned int index) {
    assert(invrep(sb));
    node_t node=sb->first, prev=NULL;
    if(index >= sb->length){
        return sb;
    }
    unsigned int pos = 0;
    while(pos<index){
        node = node->nextchar;
        prev = prev->nextchar;
        pos++;
    }  
    prev->nextchar = node->nextchar;
    destroy_node(node);
    assert(invrep(sb));
    return sb;
}


stringbuffer stringbuffer_replace(stringbuffer sb, const char c, unsigned int index) {
    assert(invrep(sb) && index <stringbuffer_length(sb));

    node_t p = sb->first; 
    for (unsigned int i =0; i<index; i++){
        p = p->nextchar;
    }
    p->c = c; 
    assert(invrep(sb));
    return sb;
}


unsigned int stringbuffer_length(stringbuffer sb) {
    return sb->length;
}


char* stringbuffer_to_string(stringbuffer sb) {
    assert(invrep(sb));
    char *res = calloc(sb->length, sizeof(struct s_node));
    node_t p = sb->first;
    for(unsigned int i =0; i<sb->length; i++){
        res[i] = p->c;
        p = p->nextchar;
    }
    assert((stringbuffer_length(sb) == 0) == (res == NULL));
    return res;
}


void stringbuffer_dump(stringbuffer sb) {
    assert(invrep(sb));
    node_t node = sb->first;
    while (node != NULL) {
        printf("%c", node->c);
        node = node->nextchar;
    }
    printf("\n");
}


stringbuffer stringbuffer_destroy(stringbuffer sb) {
    assert(invrep(sb));
    node_t p = NULL;
    while(sb->first != sb->last && sb->first->nextchar != NULL){
        p = sb->first;
        sb->first = sb->first->nextchar;
        p = destroy_node(p);
    }
    free(sb->first);
    free(sb);
    sb = NULL;
    return sb;
}
