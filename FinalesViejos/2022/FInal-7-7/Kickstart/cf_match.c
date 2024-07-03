// ESTE FUE UNO DE LOS FINALES MAS DIFICILES QUE HICE (NO ENTENDIA UN CARAJO EL ENUNCIADO NO TE DABA UNA BASE NI TIP)
// UNA MIERDA LO ODIE, ME DESCARGO ESCRIBIENDO ESTE COMENTARIOOO, AHORA ME VOY A COMER UN LOMOPIZZA PARA BALANCEAR 

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "card.h"
#include "cf_match.h"

struct _s_match {
    card *cards;
    unsigned int capacity; 
    unsigned int size;
};

cf_match match_new(void)
{
    struct _s_match *new_match = malloc(sizeof(struct _s_match));  // Reservamos memoria para el nuevo juego
    assert(new_match!=NULL);  // Verificamos que se reservo correctamente

    // Rellenamos los campos 
    new_match->cards = malloc(sizeof(card) * 13);
    new_match->size = 0;
    new_match->capacity = 13;
    return new_match;
}

cf_match match_add(cf_match match, card c)
{
    // Nos aseguramos que hay espacio 
    if(match->size == match->capacity){
        match->capacity *= 2;
        match->cards = realloc(match->cards, sizeof(card) * match->capacity);
    }
    // Agregamos la nueva carta 
    match->cards[match->size++] = c;
    return match;
}

unsigned int match_size(cf_match match)
{
    return match->size;
}

bool is_match_correct(cf_match match)
{
    bool res = true;
    // Cantidad de cartas sean pares
    if(match->size % 2 !=0){
        res = false;
    }
    // El jugador 1 inicio la partida
    if(card_player(match->cards[0]) != 1){
        res = false;
    }
    for(unsigned int i = 1; i<match->size; i++){
        // Nunca dos cartas contiguas pertenecen al mismo jugador
        if(card_player(match->cards[i]) == card_player(match->cards[i-1])){
            res = false;
        }
        // NUnca dos cartas contiguas tienen el mismo color
        if(card_color(match->cards[i]) == card_color(match->cards[i-1])){
            res = false;
        }
    }
    return res;
}

static int player_score(cf_match match, unsigned int player) {
    int score = 0;
    for (unsigned int i = 1; i < match->size; ++i) {
        if (card_player(match->cards[i-1]) == player && card_player(match->cards[i]) != player) {
            score += card_drop_points(match->cards[i-1], match->cards[i], false, false);
        }
    }
    return score;
}

unsigned int match_winner(cf_match match)
{
    unsigned int winner = 0;
    // SI el juego no es correcto retornamos -1
    if(!is_match_correct(match)){
        winner = -1;
    }

    unsigned int point1 = 0, point2 = 0;
    for(unsigned int i =1; i<match->size; i+=2){
        // Sumamos el puntaje de los jugadores
        point1 += card_drop_points(match->cards[i-1], match->cards[i], false, false);
        if(i+1 < match->size){
            point2 += card_drop_points(match->cards[i], match->cards[i+1], false, false);
        }
    }
    if(point1 > point2){
        winner = 1;
    } else {
        winner = 2;
    }
    return winner;
}

unsigned int winner_total_points(cf_match match)
{
    if (!is_match_correct(match)) {
        return -1;
    }

    int playerOneScore = player_score(match, 1);
    int playerTwoScore = player_score(match, 2);

    if (playerOneScore == playerTwoScore) {
        return 0;
    }
    return playerOneScore > playerTwoScore ? playerOneScore : playerTwoScore;
}

card* match_to_array(cf_match match)
{
    card *array = malloc(sizeof(card) * match->size);
    for(unsigned int i =0; i<match->size; i++){
        array[i] = match->cards[i];
    }
    return array;
}

void match_dump(cf_match match)
{
    card* array = match_to_array(match);
    for (unsigned int i = 0u; i < match_size(match); i++) {
        card_dump(array[i]);
    }
    free(array);
}

cf_match match_destroy(cf_match match)
{
    for(unsigned int i=0; i<match->size; i++){
        card_destroy(match->cards[i]);
    }
    free(match->cards);
    free(match);
    return NULL;
}
