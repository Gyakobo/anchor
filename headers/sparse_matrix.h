#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <SFML/Graphics.hpp>

struct Element {
    int value;
    int row;
    int column;
    struct Element * rowElement; /* -> */
    struct Element * colElement; /* \|/ */
};

struct Header {
    int index;
    struct Header * header;
    struct Element * element;
};

struct Matrix {
    struct Header * headRowHeader;
    struct Header * headColHeader;

    int rows;
    int cols;
    int fillValue;
};

class Sparse_Matrix {
public:
    Sparse_Matrix();
    struct Matrix mtrx_alloc(char * filename, int fillValue); 
    void mtrx_print(struct Matrix * mtrx);
    
    std::vector<sf::Vector2f> getOutlinePoints(struct Matrix* mtrx);
private:
    std::vector<sf::Vector2f> arr;

    void mtrx_set(struct Matrix *mtrx, int row, int col, int val); 
    void mtrx_clear(struct Matrix * mtrx); 
    void add_col_header(struct Header **mtrx_column, struct Header * column_header, struct Element * element); 
    void add_row_header(struct Header **mtrx_column, struct Header * column_header, struct Element * element);
    int print_header_hor(struct Header **header); 
    int print_header_ver(struct Header **header);
    
    void blue_color() {
        printf("\033[0;34m");
    }
    void white_color() {
        printf("\033[0;37m");
    }
    void green_color() {
        printf("\033[0;32m");
    }
    void red_color() {
        printf("\033[1;31m");
    }
    void yellow_color() {
        printf("\033[1;33m");
    }
    void reset_color() {
        printf("\033[0m");
    }
};

#endif