#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "position.h"

struct _position {
        unsigned int row;
        unsigned int column;
        unsigned int dimension;
};


position_t position_initial(unsigned int dimension)
{
        position_t position; 
        position = malloc(sizeof(struct _position));  // Reservamos espacio en memoria 
        assert(position!=NULL);  // Verificamos que se reservo correctamente
        position->column = 0;
        position->row = 0;
        position->dimension = dimension;
        return position;
}

position_t position_absolute(unsigned int row, unsigned int column,
                             unsigned int dimension)
{
        // Verificamos la pre condicion 
        assert(row < dimension && column < dimension); 
        position_t matriz = position_initial(dimension);  // Creamos una posicion inicial con la dimension dada
        matriz->column = column;
        matriz->row = row;
        return matriz; 
}

position_t position_left(position_t pos)
{
        if(pos->column == 0){  // Si estamos en la ultima columna 
                pos->column = 5;  // Hacemos como un circulo y volvemos hacia atras
        } else {
                pos->column -= 1;  // Si no vamos a la columna de la izquierda
        }
        return pos;

}

position_t position_right(position_t pos)
{
        if(pos->column == pos->dimension){  // Si estamos en la ultima columna 
                pos->column = 0;  // Hacemos como un circulo y volvemos hacia atras
        } else {
                pos->column += 1;  // Si no vamos a la columna de la izquierda
        }
        return pos;
}

position_t position_down(position_t pos)
{
        if(pos->row == pos->dimension){  // Si estamos en la ultima columna 
                pos->row = 0;  // Hacemos como un circulo y volvemos hacia atras
        } else {
                pos->column += 1;  // Si no vamos a la columna de la izquierda
        }
        return pos;
}

position_t position_up(position_t pos)
{
        if(pos->row == 0){  // Si estamos en la ultima columna 
                pos->row = pos->dimension;  // Hacemos como un circulo y volvemos hacia atras
        } else {
                pos->column -= 1;  // Si no vamos a la columna de la izquierda
        }
        return pos;
}

unsigned int position_row(position_t pos)
{
        return pos->row;
}

unsigned int position_column(position_t pos)
{
        return pos->column;
}

unsigned int **position_board(position_t pos)
{
        // Reservamos espacio para nuestra matriz
        unsigned int **board = malloc(pos->dimension * sizeof(unsigned int *));
        assert(board!= NULL); // Verificamos que se reservo correctamente 
        for(unsigned int i = 0; i<pos->dimension; i++){
                board[i] = malloc(pos->dimension * sizeof(unsigned int *));  // Asignamos memoria para la fila
                assert(board[i]!= NULL);
                for(unsigned int j =0; j<pos->dimension; j++){
                        board[i][j] = 0;  // Asignamos cada lugar de la matriz con 0 
                }
        }
        board[pos->row][pos->column] = 1; // Ubicamos el 1 en la posicion correcta 
        return board;
}

list_t position_movements(position_t pos)
{
        list_t movs = list_empty();
        for (unsigned int i = 0; i < pos->column; i++)
        {
                movs = list_append(movs,RIGHT);
        }
        for (unsigned int i = 0; i < pos->row; i++)
        {
               movs = list_append(movs,DOWN);
        }
        return movs;
}

position_t position_destroy(position_t pos)
{
        if(pos!= NULL){
                free(pos);
        }
        pos = NULL;
        return pos;
}
