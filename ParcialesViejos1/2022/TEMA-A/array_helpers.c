/*
@file array_helpers.c
@brief Array Helpers method implementation
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array_helpers.h"

/**
* @brief returns true when reach last entry in flight table
* @return True when is the last entry of the flight table, False otherwise
*/
static bool is_last_line(unsigned int hour, unsigned int type) {
  return  hour == HOURS - 1u && type == TYPE - 1u;
}

void array_dump(LayoverTable a) {
  for (unsigned int hour = 0u; hour < HOURS; ++hour) {
    for (unsigned int type = 0u; type < TYPE; ++type) {
      Flight f = a[hour][type];
      fprintf(stdout, "%c: %s at %u:00 with %u passengers", f.code, f.type == 0 ? "arrives" : "departs", f.hour - 1, f.passengers_amount);
      if (!is_last_line(hour, type))
      {
        fprintf(stdout, "\n");
      }
    }
  }
}


unsigned int passengers_amount_in_airport (LayoverTable a, unsigned int h) {
  unsigned int passengers_waiting = 0;
  
  // Recorremos todos los pasajeros que llegaron en las horas anteriores
  for(unsigned int hour =0; hour<h;  hour++){
    passengers_waiting += a[hour][arrival].passengers_amount;
  }

  // Restamos la cantidad de pasajeros que salieron en la hora "h" 
  passengers_waiting -= a[h][departure].passengers_amount;

  return passengers_waiting;
}


void array_from_file(LayoverTable array, const char *filepath) {
  FILE *file = NULL;

  file = fopen(filepath, "r");
  if (file == NULL) {
    fprintf(stderr, "File does not exist.\n");
    exit(EXIT_FAILURE);
  }

  char code;
  while (!feof(file)) {
    int res = fscanf(file, "_%c_", &code);
    if (res != 1) {
      fprintf(stderr, "Invalid file.\n");
      exit(EXIT_FAILURE);
    }
    /* COMPLETAR: Generar y guardar ambos Flight en el array multidimensional */
    Flight flight_arrival =  flight_from_file(file, code);
    Flight flight_departure = flight_from_file(file, code);
    array[flight_arrival.hour - 1][arrival] = flight_arrival;
    array[flight_departure.hour - 1][departure] = flight_departure;
  }
  fclose(file);
}
