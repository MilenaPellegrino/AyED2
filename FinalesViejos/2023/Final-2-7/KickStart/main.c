#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "position.h"

void print_board(unsigned int **board, unsigned int dimension) {
    for (unsigned int i = 0; i < dimension; i++) {
        for (unsigned int j = 0; j < dimension; j++) {
            printf("%u ", board[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    unsigned int dimension = 5;
    unsigned int row = 2;
    unsigned int column = 3;

    // Crear una posición absoluta
    position_t pos = position_absolute(row, column, dimension);

    // Generar y mostrar la matriz
    unsigned int **board = position_board(pos);
    printf("Matriz generada por position_board:\n");
    print_board(board, dimension);

    // Liberar la memoria de la matriz
    for (unsigned int i = 0; i < dimension; i++) {
        free(board[i]);
    }
    free(board);

    // Generar y mostrar la lista de movimientos
    list_t movements = position_movements(pos);
    printf("Lista generada por position_movements:\n");
    list_show(movements);

    // Destruir la lista de movimientos
    list_destroy(movements);

    // Destruir la posición
    position_destroy(pos);

    return 0;
}