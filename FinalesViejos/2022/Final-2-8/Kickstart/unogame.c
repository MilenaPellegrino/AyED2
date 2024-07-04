#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "unogame.h"
#include "card.h"

#define NO_PLAYER -1

struct s_ugame {
    struct c_node *head;
    unsigned int size;
};
struct c_node{
    struct c_node *next;
    player_t player;
    card_t card;
};

typedef struct c_node *node_c;

static node_c create_node(card_t card, player_t player){
    node_c nodo;
    nodo = malloc(sizeof(struct c_node));
    assert(nodo!=NULL);

    nodo->next = NULL;
    nodo->card = card;
    nodo->player = player;
    return nodo;
}

unogame_t
uno_newgame(card_t card) {
    unogame_t newgame = NULL;
    newgame = malloc(sizeof(struct s_ugame));
    assert(newgame!=NULL);
    newgame->size = 1;
    // Creamos el primer nodo de la pila, como no es de ningun jugador ponemos (-1)
    newgame->head = create_node(card, -1);
    return newgame;
}

card_t
uno_topcard(unogame_t uno) {
    return uno->head->card;
}

unogame_t
uno_addcard(unogame_t uno, card_t c) {
    // Para agregar necesito saber cual es el jugador que jeuga el siguiente turno
    player_t player_next = uno_nextplayer(uno);
    node_c new_card = create_node(c, player_next);  // Creo el nuevo nodo con la carta y el jugador correspondiente
    
    // Como es una pila, a lo qeu paunta el nuevo nodo es a lo que habia en el tope del mazo
    new_card->next = uno->head;  

    // Actualizo cual es el head del juego 
    uno->head = new_card;

    // Como agregue una carta actualizo el tamano del juego 
    uno->size++;
    return uno;
}

unsigned int
uno_count(unogame_t uno) {
    return uno->size;
}

bool 
uno_validpile(unogame_t uno) {
    assert(uno != NULL);
    bool valid = true;
    // Vamos a recorrer toda la pila y verificar que esta sea valida 
    struct c_node *aux = uno->head;  // Nodo auxiliar para recorrer todo el mazo 
    struct c_node *prev = uno->head; // Nodo que tiene el anterior del nodo auxiliar para poder ir comparando
    while (aux != NULL && valid) {
        valid = card_compatible(aux->card, prev->card);
        prev = aux;
        aux = aux->next;
    }
    valid = valid && uno->head != NULL;
    return valid;
}

color_t
uno_currentcolor(unogame_t uno) {
    color_t color = card_get_color(uno->head->card);
    return color;
}

player_t
uno_nextplayer(unogame_t uno) {
    player_t next_player = -1;
    // Caso especial cuando el que jugo ultimo fue el ultimo jugador 
    if(uno->head->player == NUM_PLAYERS -1){  // Esta indezado en 0 por eso el (-1)
        next_player = 0;  // Si el que jugo ultimo fue el ultimo le toca al primero
    } else {
        next_player = uno->head->player+1;  // Y si no fue el ultimo le toca al siguiente
    }
    return next_player;
}



card_t *
uno_pile_to_array(unogame_t uno) {
    card_t *cards;
    cards = calloc(uno->size, sizeof(card_t));
    node_c aux = uno->head;
    // Empiezo con i = size ya que el array tiene que estar en el orden que se jugo
    for (int i = uno->size-1; i>=0; i--){  
        cards[i] = aux->card;
        aux = aux->next;
    }
    return cards;
}

unogame_t 
uno_destroy(unogame_t uno) {
    struct c_node *aux = uno->head;
    struct c_node *prev = NULL;
    while (aux != NULL) {
        prev = aux;
        aux = aux->next;
        card_destroy(prev->card); // Liberar la carta
        free(prev);
    }
    free(uno);
    uno = NULL;
    return uno;
}

