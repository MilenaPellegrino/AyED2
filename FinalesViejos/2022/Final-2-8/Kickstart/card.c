#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>
#include "card.h"

struct s_card {
    unsigned int num;
    color_t color;
    type_t type;
};

static bool
invrep(card_t c) {
    // Invariante de representación
    bool valid=false;
    valid = c != NULL;
    /*
     * [COMPLETAR]
     *
     * Mejorar la invariante chequeando más
     * propiedades de los campos de la estructura
     *
     */
    return valid;
}

card_t
card_new(unsigned int num, color_t color, type_t s) {
    card_t card=NULL;
    card = malloc(sizeof(struct s_card));  // Reservamos memeoria para la card
    assert(card!=NULL);  // Verificar que reservamos memoria correctamente
    // Completamos los campos de la estrcutura
    card->color = color;
    card->num = num;
    card->type = s;
    return card;
}


type_t
card_get_number(card_t card) {
    assert(invrep(card));
    return card->num;
}

color_t
card_get_color(card_t card) {
    assert(invrep(card));
    return card->color;
}

type_t
card_get_type(card_t card) {
    assert(invrep(card));
    return card->type;
}

bool
card_samecolor(card_t c1, card_t c2) {
    return c1->color == c2->color;
}


bool
card_samenum(card_t c1, card_t c2) {
    return c1->num == c2->num;
}

bool
card_compatible(card_t new_card, card_t pile_card) {
    bool compatible=false;
    assert(invrep(new_card) && invrep(pile_card));
    // Si tienen el mismo color son compatibles 
    if(card_samecolor(new_card, pile_card)){
        compatible = true;
    } else if (card_get_type(new_card) == change_color){  // Si la nueva carta es el comodin de cambio de color  tmb es compatible
        compatible = true;
    } else if(card_samenum(new_card, pile_card)){  // SI tienen el mismo numero tmb son compatibles 
        compatible = true;
    } else if (card_get_type(new_card) == card_get_type(pile_card) && card_get_type(pile_card) != normal){  // SI tienen el mismo tipo especial
        compatible = true;
    }
    return compatible;
}

card_t
card_destroy(card_t card) {
    free(card);
    card = NULL;
    return card; 
}


