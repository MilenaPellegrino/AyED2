#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "matchTimeline.h"

/* ============================================================================
STRUCTS
============================================================================ */

struct s_node
{
    team team;
    event e;
    time t;
    playerNumber pn;
    struct s_node *next;
};

struct s_matchTimeline
{
    struct s_node *head;
    unsigned int gol_home;  // Cantidad actual de goles del equipo local 
    unsigned int gol_away;  // Cantidad actual de goles del equipo visitante 
    unsigned int reg;       // Cantidad actual de registros
};

/* ============================================================================
INVERP & VALIDATION
============================================================================ */

static bool invrep(matchTimeline mt)
{
    bool res = true;
    res = mt!=NULL;
    unsigned int lenght = 0u;
    struct s_node *aux = mt->head;  // Nodo auxiliar para recorrer todos los nodos del mt
    while(aux!=NULL){
        lenght++;
        aux = aux->next;
    }
    res = res && lenght == mt->reg;
    return res;
}

bool matchTimeline_is_time_and_score_valid(matchTimeline mt)
{
    /*
        VER: 
            - Si estan ordenados cronologicamente 
            - Si los goles de home y away se corresponden con la cant. de goles de cada
    */
    unsigned int home = 0u;
    unsigned int away = 0u;
    unsigned int time = 0u;
    bool res = true;
    struct s_node *aux = mt->head;  // Nodo auxiliar que nos sirve para recorrer todos los nodos
    while(aux!= NULL){
        // Si el evento es un gol 
        if(aux->e == Goal){
            // Aumentamos uno la cantidad de goles del team correspondido
            if(aux->team == Home){
                home++;
            } else {
                away++;
            }
        }
        // Si esta mal ordenado cronologicamente
        if(aux->t < time){
            res = false;
        }
        // Actualizamos los valores
        time = aux->t;
        aux = aux->next;
    }
    return res && away == mt->gol_away && home == mt->gol_home;
}

/* ============================================================================
CONSTRUCTOR
============================================================================ */

matchTimeline matchTimeline_empty(void)
{
    matchTimeline mt = NULL;
    mt = malloc(sizeof(struct s_matchTimeline));  // Reservamos memoria para el mt
    assert(mt!=NULL);  // Verificamos que se reservo correctamente 
    // Rellenamos los campo 
    mt->gol_away = 0u; 
    mt->gol_home = 0u;
    mt->reg = 0u;
    mt->head = NULL;

    assert(invrep(mt));
    return mt;
}

/* ============================================================================
INSERTION
============================================================================ */

static struct s_node *create_node(team team, event e, time t, playerNumber pn)
{

    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));
    assert(new_node!=NULL);   // Verificamos que se reservo mememoria correctamente 
    
    // Completamos los campo del nodo 
    new_node->e = e;
    new_node->pn = pn;
    new_node->t = t;
    new_node->team = team;
    new_node->next = NULL;
    return new_node;
}

// Funcion auxiliar para agregar un registros el matchTimeline 
// Lo hago ya que hay varias funciones que agregan registros nuevos
void add_reg(matchTimeline mt, team team, event e, time t, playerNumber pn){
    struct s_node *newNode = create_node(team,e,t,pn);  // Creamos el nuevo nodo a agregar
    if(mt->head == NULL){ // Si el mt no tiene registros simplemente agregamos el nuevo
        mt->head = newNode;
    } else {
        struct s_node *aux = mt->head;  // Creamos un nodo auxiliar para recorrer todos los registros del mt
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = newNode;  // Cuando llegamos al final agregamos el nuevo registros
    }

    mt->reg++;  // Como agregamos un registro sumamos 1 
}

/* ============================================================================
GOAL
============================================================================ */

matchTimeline matchTimeline_score_goal(matchTimeline mt, team team, time t, playerNumber pn)
{
    assert(invrep(mt));
    struct s_node *aux = mt->head; 
    if(t<=EVENT_MAX_TIME){  // Verificamos que esta dentro de los 90
        while(aux != NULL){
            if(aux->e == RedCard && aux->pn == pn){  // Verificamos que el jugador no tenga tarjeta roja 
                return mt;
            }
            aux = aux->next;
        }
    }

    add_reg(mt,team,Goal,t,pn);  // Agregamos el nuevo registro

    // Vemos cual equipo hizo el gol 
    if(team == Away){
        mt->gol_away++;
    } else{
        mt->gol_home++;
    }
    return mt;
}

/* ============================================================================
THE CARDS
============================================================================ */

matchTimeline matchTimeline_receive_yellowCard(matchTimeline mt, team team, time t, playerNumber pn)
{
    assert(invrep(mt));
    bool doble_amarilla = false;
    if(t<=EVENT_MAX_TIME){
        struct s_node *aux = mt->head;
        while(aux!=NULL){
            if(aux->e == YellowCard && aux->pn == pn){
                doble_amarilla = true;
            }
            if(aux->pn == pn && aux->e == RedCard){
                return mt;
            }
            aux = aux->next;
        }
        add_reg(mt,team,YellowCard,t,pn);
        
        // Si ya tenia una amarilla es tarjeta roja loco 
        if(doble_amarilla){
            add_reg(mt,team,RedCard,t,pn);
        }
    }
    return mt;
}

matchTimeline matchTimeline_receive_redCard(matchTimeline mt, team team, time t, playerNumber pn)
{
    assert(invrep(mt));
    if(t<=EVENT_MAX_TIME){
        struct s_node *aux = mt->head;
        while(aux!=NULL){
            if(aux->e == RedCard && aux->pn == pn){
                return mt;
            }
            aux = aux->next;
        }
        add_reg(mt,team,RedCard,t,pn);
    }
    return mt;
}

/* ============================================================================
SCORE
============================================================================ */

unsigned int matchTimeline_get_score(matchTimeline mt, team team)
{
    assert(invrep(mt));
    unsigned int res = 0u;
    if(team == Away){
        res =  mt->gol_away;
    }else{
        res =  mt->gol_home;
    }
    return res;
}

/* ============================================================================
SIZE
============================================================================ */

unsigned int matchTimeline_size(matchTimeline mt)
{
    assert(invrep(mt));
    return mt->reg;
}

/* ============================================================================
PRINT
============================================================================ */

void matchTimeline_print(matchTimeline mt)
{
    fprintf(
        stdout, "            Home %u -  %u Away \n", 
        matchTimeline_get_score(mt, Home), 
        matchTimeline_get_score(mt, Away)
    );

    struct s_node *aux = mt->head;
    while (aux != NULL)
    {
        unsigned int t = aux->t;
        char *e;
        switch (aux->e)
        {
        case Goal:
            e = "    Goal   ";
            break;
        case YellowCard:
            e = "Yellow Card";
            break;
        case RedCard:
            e = "  Red Card ";
            break;
        }
        unsigned int pn = aux->pn;

        if (aux->team == Home) {
            fprintf(stdout, "%u - %s - %u' \n", pn, e, t);
        }
        else {
            fprintf(stdout, "                   %u' - %s - %u \n", t, e, pn);
        }

        aux = aux->next;
    }
}

/* ============================================================================
ARRAY
============================================================================ */

event *matchTimeline_events_array(matchTimeline mt)
{
    event *array = NULL;
    array = malloc(sizeof(struct s_matchTimeline) * mt->reg);  // Reservamos memoria
    unsigned int regs = mt->reg;
    struct s_node *aux = mt->head;

    // Llenamos el array
    for(unsigned int i = 0u; i<regs; i++){
        array[i] = aux->e;
        aux = aux->next;
    }
    return array;
}

/* ============================================================================
DESTRUCTOR!
============================================================================ */

static struct s_node *destroy_node(struct s_node *node)
{
    
    node->next = NULL;
    free(node);
    node = NULL;
    return node;
}

matchTimeline matchTimeline_destroy(matchTimeline mt)
{
    assert(invrep(mt));

    struct s_node *aux = NULL;
    while(mt->head!=NULL){
        aux = mt->head;
        mt->head = mt->head->next;
        aux = destroy_node(aux);
    }
    free(mt);
    mt = NULL;
    return mt;
}
