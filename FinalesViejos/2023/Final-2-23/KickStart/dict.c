#include <stdlib.h>     /* malloc(), free()... */
#include <stdbool.h>    /* bool type           */
#include <assert.h>     /* assert()            */
#include "key_value.h"  /* key_t               */
#include "dict.h"       /* intarface           */

struct _s_dict {
    // COMPLETAR
    struct _node_t * first;
    unsigned int size;  // Mantemos el tamano en cada momento
};

// Internal structure
struct _node_t {
    key_t key;
    value_t value;
    struct _node_t *next;
};



struct _node_t * create_node(key_t k, value_t v) {
    struct _node_t *new_node=malloc(sizeof(struct _node_t));
    new_node->key=k;
    new_node->value=v;
    new_node->next=NULL;
    return new_node;
}

struct _node_t *destroy_node(struct _node_t *node) {
    node->key = key_destroy(node->key);
    node->value = value_destroy(node->value);
    free(node);
    node=NULL;
    return node;
}

static bool invrep(dict_t dict) {
   /*unsigned int lenght = 0u; 
    // Nodo auxiliar que apunta al principio del diccionario ayudara a recorrer todo el diccionario
    struct _node_t *aux = dict->first;
    while(aux!=NULL){
        lenght += 1;
        aux = aux->next;
    } 
    */
    //return (lenght == dict->size && dict!=NULL);
    return (dict!=NULL);
}


// --- Interface functions ---

dict_t dict_empty(void) {
    dict_t new_dict = NULL;
    new_dict = malloc(sizeof(struct _s_dict));
    assert(new_dict !=NULL);  // Verificamos que se reservo memoria correctamente
    
    // Completamos los campos del dict
    new_dict->first = NULL;
    new_dict->size = 0;
    assert(invrep(new_dict));
    return new_dict;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(invrep(dict));

    if(dict_exists(dict, word)){  // SI ya existe la palabra en el dict reemplaza su valor
        struct _node_t *aux = dict->first;
        while(aux!= NULL){
            if(string_eq(aux->key, def)){  // Verificamos si es la clave a modificar
                aux->value = string_clone(word);  // SI era la clave reemplazamos su valor
                return dict;  // COmo ya hicimos todo, que termine la funcion
            }
            aux = aux->next;
        }
    }
    // Si no esta la clave en el diccionario
    // Creamos el nuevo nodo a agregar
    struct _node_t *aux2 = create_node(string_clone(word), string_clone(def));
    // Agregamos este nodo (con clave y valor) al principio del diccionario
    aux2->next = dict->first;
    dict->first = aux2;
    return dict;
}

value_t dict_search(dict_t dict, key_t word) {
    assert(invrep(dict));
    if(!dict_exists(dict, word)){  // SI no encontramos la palabra devolvemos NULL
        return NULL;
    }
    struct _node_t *aux = dict->first;
    while(aux!=NULL){  // Recorremos hasta encontrar la palabra
        if(string_eq(aux->key, word)){  // Si encontramos la clave devolvemos el valor
            return string_clone(aux->value);
        }
        aux = aux->next;
    }
    return NULL;
}

bool dict_exists(dict_t dict, key_t word) {
    assert(invrep(dict));
    bool exist = false;
    struct _node_t *aux = dict->first;
    // Recorremos hasta encontrar la word
    while(aux!=NULL && !exist){
        exist = string_eq(aux->key, word);  // Si nunca es igual el valor de exist es false
        aux = aux->next;
    }
    return exist;
}

unsigned int dict_length(dict_t dict) {
    assert(invrep(dict));
    return dict->size;
}

dict_t dict_remove(dict_t dict, key_t word) {
    assert(invrep(dict));
    struct _node_t *aux = dict->first;
    struct _node_t *prev = dict->first;
    // SI no existe la palabra a eliminar no hacemos nada
    if(!dict_exists(dict, word)){
        return dict;
    }
    // Si existe recorremos hasta encontrarla guardando un puntero que apunte al anterior nodo
    while(aux!= NULL){
        // Si la encontramos la eliminamos de este horroroso mundo
        if(string_eq(aux->key, word)){
            prev->next = aux->next; // EL anterior nodo de la palabra apunta a lo que apuntaba lo siguiente de la palabra
            aux = destroy_node(aux);  // Destruimos la palabra 
            dict->size--;  // Actualizamos el tamano
            return dict;  // Retorno porque ya eliminamos 
        }
        prev = aux;
        aux = aux->next;
    }
    return dict;
}

void dict_dump(dict_t dict, FILE *file) {
    assert(invrep(dict));
    for (struct _node_t *check=dict->first; check!=NULL; check=check->next) {
        fprintf(file, "key: ");
        key_dump(check->key, file);
        fprintf(file, "\n");
        fprintf(file, "value: ");
        value_dump(check->value, file);
        fprintf(file, "\n\n");
    }
}

dict_t dict_remove_all(dict_t dict) {
    assert(invrep(dict));
    struct _node_t *aux = dict->first;
    while(dict->first!= NULL){
        dict->first = aux->next;
        aux = destroy_node(aux);
        dict->size--;
    }
    return dict;
}

dict_t dict_destroy(dict_t dict) { 
    assert(invrep(dict));
    dict = dict_remove_all(dict);
    free(dict);
    dict=NULL;
    assert(dict==NULL);
    return dict;
}

