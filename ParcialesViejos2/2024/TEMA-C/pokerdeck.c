#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <assert.h>

#include "pokerdeck.h"

struct s_pokerdck {
    struct s_node *first;
    struct s_node *last; /* Time efficiency */
    unsigned int size; /* Time efficiency */
    unsigned int count[CARD_SUITS]; /* To count the cards of each specific suit */
};

struct s_node {
    cardnum_t num;
    cardsuit_t suit;
    struct s_node *nextcard;
};

typedef struct s_node * node_t;

// Auxiliar dump functions
static void dump_suit(cardsuit_t suit) {
    printf("%s", suit==spades   ? "♠️  ":
                 suit==hearts   ? "♥️  ":
                 suit==diamonds ? "♦️  ":
                 suit==clubs    ? "♣️  ": "???");
}

static void dump_num(cardnum_t num) {
    if (1 < num && num < 11) {
        printf("%-2d", num);
    } else {
        printf("%-2s", num==1 ? "A":
                       num==11 ? "J":
                       num==12 ? "Q":
                       num==13 ? "K": "???");

    }
}

// Representation invariant
static bool invrep(pokerdeck deck) {
    bool valid = true;

    valid = valid && (deck != NULL);

    /* This means that either both, first and last, are NULL, or none of them is NULL */
    /* If one of them is NULL and the other isn't, there is a problem */
    valid = valid && ((deck->first == NULL && deck->last == NULL) || (deck->first != NULL && deck->last != NULL));

    /* This means that the sum of all cards of each suit must equal the total number of cards */
    unsigned int total_count = 0;
    for (unsigned int i = 0; i < CARD_SUITS; i++)
        total_count = total_count + deck->count[i];
    valid = valid && (total_count == deck->size);
    
    return valid;
}

// Creates a single node in dynamic memory with a given card
static node_t create_node(cardnum_t num, cardsuit_t suit) {
    /* Check if the given values are allowed */
    assert(suit < CARD_SUITS && suit >= 0 && num >= CARD_NUM_FST && num <= CARD_NUM_LST);

    node_t newnode = NULL;

    newnode = malloc(sizeof(struct s_node));
    assert(newnode != NULL);

    newnode->num = num;
    newnode->suit = suit;
    newnode->nextcard = NULL;

    assert(newnode!=NULL);
    return newnode;
}

// Destroy a single node
static node_t destroy_node(node_t node) {
    free(node);
    node = NULL;

    assert(node==NULL);
    return node;
}

// Public functions of the Abstract Data Type
pokerdeck pokerdeck_empty(void) {
    // DON'T check invariant before creating the deck
    pokerdeck new_deck = NULL;

    new_deck = malloc(sizeof(struct s_pokerdck));
    assert(new_deck != NULL);

    new_deck->size = 0;
    new_deck->last = NULL;
    new_deck->first = NULL;
    new_deck->count[spades] = 0;   /* I don't use a loop just to make things clear */
    new_deck->count[hearts] = 0;   /* I know that if there was the possibility of more suits (for whatever reason), */
    new_deck->count[diamonds] = 0; /* it would be better to use a loop, as it is more friendly to changes in the program */
    new_deck->count[clubs] = 0;    

    assert(pokerdeck_is_empty(new_deck) && invrep(new_deck));

    return new_deck;
}

bool pokerdeck_is_empty(pokerdeck deck) {
    assert(invrep(deck));

    return (deck->first == NULL);
}

pokerdeck pokerdeck_add(pokerdeck deck, cardnum_t num, cardsuit_t suit) {
    assert(invrep(deck));
    node_t aux_node = create_node(num, suit);

    if (pokerdeck_is_empty(deck)) {
        deck->first = aux_node;
        deck->last = aux_node;
    } else {
        (deck->last)->nextcard = aux_node;
        deck->last = aux_node;
    }

    deck->size = deck->size + 1;
    deck->count[suit] = deck->count[suit] + 1;

    assert(invrep(deck) && !pokerdeck_is_empty(deck));
    return deck;
}

pokerdeck pokerdeck_push(pokerdeck deck,  cardnum_t num, cardsuit_t suit) {
    assert(invrep(deck));
    node_t newnode = create_node(num, suit);
    newnode->nextcard = deck->first;
    deck->first = newnode;
    
    if (deck->last == NULL) {
        deck->last = newnode;
    }

    deck->size = deck->size + 1;
    deck->count[suit] = deck->count[suit] + 1;
    
    assert(invrep(deck) && !pokerdeck_is_empty(deck));
    return deck;
}

pokerdeck pokerdeck_pop(pokerdeck deck, cardnum_t *popped_num, cardsuit_t *popped_suit) {
    assert(invrep(deck) && !pokerdeck_is_empty(deck));

    node_t popped_node = deck->first;

    if (deck->first == deck->last) { // A single card was in the deck
        deck->first = NULL;
        deck->last = NULL;
    } else {
        deck->first = popped_node->nextcard;
    }

    deck->size = deck->size - 1;
    deck->count[popped_node->suit] = deck->count[popped_node->suit] - 1;

    if (popped_num!=NULL) *popped_num = popped_node->num;
    if (popped_suit!=NULL) *popped_suit = popped_node->suit;

    destroy_node(popped_node);

    assert(invrep(deck));
    return deck;
}

unsigned int pokerdeck_length(pokerdeck deck) {
    assert(invrep(deck));
    unsigned int length = deck->size;
    
    assert(invrep(deck) && (pokerdeck_is_empty(deck) == (deck->size == 0)));
    return length;
}

pokerdeck pokerdeck_remove(pokerdeck deck, cardnum_t num, cardsuit_t suit) {
    assert(invrep(deck));

    node_t previous = NULL;
    node_t aux = NULL;

    if (!pokerdeck_is_empty(deck)) { /* If it is empty, do nothing */
        /* First, we find the node we want to remove and the node before it */
        aux = deck->first;
        while (aux != NULL && (aux->num != num || aux->suit != suit )) {
            previous = aux;
            aux = aux->nextcard;
        }

        if (aux != NULL) { /* If it is NULL, the card is not in the deck; do nothing */
            if (previous == NULL) { /* We want to remove the first card */
                deck = pokerdeck_pop(deck, NULL, NULL);
            } else if (aux == deck->last) { /* We want to remove the last card */
                deck->last = previous; /* I have to modify the last field, that is why this is a separate case */
                previous->nextcard = NULL;
                deck->size = deck->size - 1;
                deck->count[aux->suit] = deck->count[aux->suit] - 1;
                destroy_node(aux);
            } else {
                previous->nextcard = aux->nextcard;
                deck->size = deck->size - 1;
                deck->count[aux->suit] = deck->count[aux->suit] - 1;
                destroy_node(aux);
            }
        }
    }

    assert(invrep(deck));
    return deck;
}

unsigned int pokerdeck_count(pokerdeck deck, cardsuit_t suit) {
    assert(invrep(deck));
    unsigned int count = deck->count[suit];

    assert(invrep(deck) && count <= pokerdeck_length(deck));
    return count;
}

struct card * pokerdeck_to_array(pokerdeck deck) {
    assert(invrep(deck));

    node_t aux = NULL;
    unsigned int length = pokerdeck_length(deck);
    struct card *array = NULL;

    if (length != 0) {
        array = calloc((size_t) length, sizeof(struct card));

        unsigned int i = 0; /* The position in the array */
        aux = deck->first;
        while (i < length) { /* Or, i < length */
            array[i].num = aux->num;
            array[i].suit = aux->suit;
            aux = aux->nextcard;
            i = i + 1;
        }
    }

    assert(invrep(deck));
    assert((pokerdeck_length(deck) == 0) == (array == NULL));
    return array;
}

void card_dump(cardnum_t num, cardsuit_t suit) {
    printf("|");
    dump_num(num);
    printf(" ");
    dump_suit(suit);
    printf("|");
}

void pokerdeck_dump(pokerdeck deck) {
    assert(invrep(deck));
    node_t node = deck->first;
    while (node != NULL) {
        card_dump(node->num, node->suit);
        printf(" ");
        node = node->nextcard;
    }
    printf("\n");
}

pokerdeck pokerdeck_destroy(pokerdeck deck) {
    assert(invrep(deck));
    /* We need to destroy all cards before freeing the deck struct */
    while (!pokerdeck_is_empty(deck)) {
        /* Pop takes care of freeing the cards (nodes) */
        deck = pokerdeck_pop(deck, NULL, NULL);
    }

    /* Now there are no cards in the deck */
    free(deck);
    deck = NULL;

    assert(deck == NULL);
    return deck;
}
