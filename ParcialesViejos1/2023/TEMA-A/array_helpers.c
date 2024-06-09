/*
@file array_helpers.c
@brief Array Helpers method implementation
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "array_helpers.h"

static const int EXPECTED_DIM_VALUE = 2;

static const char* CITY_NAMES[CITIES] = {
    "Cordoba", "Rosario", "Posadas", "Tilcara", "Bariloche"};
static const char* SEASON_NAMES[SEASONS] = {"low", "high"};

void array_dump(BakeryProductTable a)
{
    for (unsigned int city = 0u; city < CITIES; ++city)
    {
        for (unsigned int season = 0u; season < SEASONS; ++season)
        {
            fprintf(stdout, "%s in %s season: flour (%u,%u) Yeast (%u,%u) Butter (%u,%u) Sales value %u",
                    CITY_NAMES[city], SEASON_NAMES[season], a[city][season].flour_cant,
                    a[city][season].flour_price, a[city][season].yeast_cant,
                    a[city][season].yeast_price, a[city][season].butter_cant,
                    a[city][season].butter_price, a[city][season].sale_value);
            fprintf(stdout, "\n");
        }
    }
}

unsigned int best_profit(BakeryProductTable a)
{
    unsigned int max_profit = 0u;
    //COMPLETAR
    unsigned int costs_products = 0u;
    for(unsigned int city = 0; city<CITIES; city++){
        for(season_t temp = 0; temp<SEASONS; temp++){
            costs_products = ((a[city][temp].flour_cant)*(a[city][temp].flour_price)) +
            ((a[city][temp].yeast_cant)*(a[city][temp].yeast_price)) +
            ((a[city][temp].butter_cant)*(a[city][temp].butter_price));
            //unsigned int max_profit_act = costs_products - a[city][temp].sale_value;
            if(a[city][temp].sale_value - costs_products > max_profit){
                max_profit = a[city][temp].sale_value - costs_products;
            }
        }
    }
    return max_profit;
}

void array_from_file(BakeryProductTable array, const char* filepath)
{
    FILE* file = NULL;

    file = fopen(filepath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File does not exist.\n");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while (!feof(file))
    {
        unsigned int kcod_ciudad = 0;
        season_t kseason;
        int res; 
        res = fscanf(file, "##%u??%u ", &kcod_ciudad, &kseason);
        if (res != EXPECTED_DIM_VALUE)
        {
            fprintf(stderr, "Invalid file.\n");
            exit(EXIT_FAILURE);
        }
        BakeryProduct bakery_product = bakery_product_from_file(file);
        array[kcod_ciudad][kseason] = bakery_product;
        /* COMPLETAR: Leer y guardar product en el array multidimensional*/
        /* Agregar las validaciones que considere necesarias*/
        /* Completar*/
        ++i;
    }
    if(i != CITIES * SEASONS){
        fprintf(stderr, "File is incomplete or overloaded. \n");
        exit(EXIT_FAILURE);
    }
    fclose(file);
}
