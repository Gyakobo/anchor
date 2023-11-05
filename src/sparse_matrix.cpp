#include "../headers/sparse_matrix.h"

Sparse_Matrix::Sparse_Matrix() {
}

struct Matrix Sparse_Matrix::mtrx_alloc(char * filename, int fillValue) {
    /* Global Var(s) */ 
    FILE *in_file;
    struct Matrix mtrx;
    int ch;
    
    int cols = 0;
    int rows = 1;

    /*int mtrx_cols = 0;
    int mtrx_rows = 0;*/

    struct Header * column_header;
    struct Header * row_header;
    struct Element * element;

    in_file = fopen(filename, "r"); /*read only*/

    if (in_file == NULL) {
        printf("Error! Could not open file\n");
        exit(-1);
    }


    /* Configure Matrix */
    mtrx.headColHeader = NULL;
    mtrx.headRowHeader = NULL;

    while ((ch = fgetc(in_file)) != EOF) {

        if (isdigit(ch)) ++cols; /* Increment column */

        if (isdigit(ch) && (ch = ch-'0') != fillValue) {

            /* printf("%d: col=%d, row=%d\n", ch, col, row); */

            /* Configure Element */
            element                 = (struct Element*)malloc(sizeof(struct Element)); 
            element->value          = ch;
            element->column         = cols;
            element->row            = rows;
            element->rowElement     = NULL;
            element->colElement     = NULL;

            /* Configure Column */
            column_header           = (struct Header*)malloc(sizeof(struct Header));
            column_header->index    = cols;
            column_header->header   = NULL; 
            column_header->element  = NULL;
            
            /* Configure Row */
            row_header              = (struct Header*)malloc(sizeof(struct Header));
            row_header->index       = rows;
            row_header->header      = NULL; 
            row_header->element     = NULL;

            add_col_header(&mtrx.headColHeader, column_header, element);
            add_row_header(&mtrx.headRowHeader, row_header, element);
        }

        else if (ch == '\n') {
            ++rows; /* Increment row */
            cols = 0;
        }

    }

    /* Configure Matrix */
    mtrx.rows       = rows;
    mtrx.cols       = cols;
    mtrx.fillValue  = fillValue;
    
    fclose(in_file);

    return mtrx;
}


std::vector<sf::Vector2f> Sparse_Matrix::getOutlinePoints(struct Matrix* mtrx) {
    struct Header *header = mtrx->headColHeader;    
    struct Header * p;
    struct Element * element;
    int size = 0;

    if (!header) {
        exit(-1);
    }
    else 
        for (p = header; p != NULL; p = p->header)
            for (element = p->element; element != NULL; element = element->rowElement) {
                if (element->rowElement == NULL) {
                    arr.push_back(sf::Vector2f(element->column, element->row));
                } 
            } 

    return arr;
}

void mtrx_set(struct Matrix *mtrx, int row, int col, int val) {
    /* Still need to create an element if it doesn't exist*/ 
    
    int i;
    int j;

    struct Header *p;
    struct Element *e;

    p = mtrx->headColHeader;
    for (i=0; i<col-1; ++i) p = p->header;

    e = p->element;
    for (j=0; j<row-1; ++j) e = e->rowElement;

    e->value = val;

    printf("Set element value: %d\n", e->value);
}

void mtrx_clear(struct Matrix * mtrx) {
    struct Header * colHeader            = mtrx->headColHeader;

    struct Element * element             = NULL;
    struct Element * prev_element        = NULL;
    
    struct Header * p;
    struct Header * prev_p;

    while (colHeader) {
        element         = colHeader->element;
        prev_element    = element;
        
        /* Go through Elements */
        if (element) {
            while (element) {
                prev_element    = element;
                element         = element->rowElement;
                free(prev_element);
            } 
        }
        else free(prev_element);  
        /* Change Header */ 
        colHeader       = colHeader->header;
    }

    p       = mtrx->headColHeader;
    prev_p  = p;
    while (p) {
        p = p->header;
        free(prev_p);
        prev_p = p; 
    }
    
    p       = mtrx->headRowHeader;
    prev_p  = p;
    while (p) {
        p = p->header;
        free(prev_p);
        prev_p = p; 
    }

    mtrx->headColHeader = NULL;
    mtrx->headRowHeader = NULL;
}


void Sparse_Matrix::mtrx_print(struct Matrix * mtrx) {
    long int size = 0;
    
    size += print_header_hor(&(mtrx->headColHeader));

    printf("==========================\n\n");

    size += print_header_ver(&(mtrx->headRowHeader));

    printf("\n");

    blue_color();
    printf("Size of sparse mtrx: %ld\n", size);
    reset_color();
}

void Sparse_Matrix::add_col_header(struct Header **mtrx_column, struct Header * column_header, struct Element * element) {
    struct Header * p;
    struct Header * q;
    struct Element * p_element;
    int header_index = column_header->index;
    
    if (*mtrx_column == NULL) {
        *mtrx_column                = column_header;
        (*mtrx_column)->element     = element;
    }

    else {
        /* Edge cases */ 
        
        /* Prefix */
        if ((*mtrx_column)->index > header_index) {
            column_header->element      = element; /* Added */
            column_header->header       = (*mtrx_column);
            (*mtrx_column)              = column_header; 
        }    
     
        /* Infix - Matching - Postfix */
        else {
            p = (*mtrx_column); 

            while (p) {
                q = p->header;

                /* Infix */
                if (q && p->index < header_index && q->index > header_index) {
                    column_header->element      = element; /* Added */
                    p->header                   = column_header;
                    column_header->header       = q; 
                    break;
                }

                /* Matching */
                else if (p && p->index == header_index) {
                    p_element = p->element; /* ->rowElement */

                    while (p_element->rowElement) 
                        p_element = p_element->rowElement;

                    p_element->rowElement = element;

                    free(column_header);
                    break;
                }

                /* Postfix */
                else if (p && p->header == NULL) {
                    column_header->element  = element;
                    p->header = column_header;
                    break;
                } 

                p = p->header; 
            }
        }
    }
}

void Sparse_Matrix::add_row_header(struct Header **mtrx_column, struct Header * column_header, struct Element * element) {
    struct Header * p;
    struct Header * q;
    struct Element * p_element;
    int header_index = column_header->index;
    
    if (*mtrx_column == NULL) {
        *mtrx_column                = column_header;
        (*mtrx_column)->element     = element;
    }

    else {
        /* Edge cases */ 
        
        /* Prefix */
        if ((*mtrx_column)->index > header_index) {
            column_header->element      = element; /* Added */
            column_header->header       = (*mtrx_column);
            (*mtrx_column)              = column_header; 
        }    
     
        /* Infix - Matching - Postfix */
        else {
            p = (*mtrx_column); 

            while (p) {
                q = p->header;

                /* Infix */
                if (q && p->index < header_index && q->index > header_index) {
                    column_header->element      = element; /* Added */
                    p->header                   = column_header;
                    column_header->header       = q; 
                    break;
                }

                /* Matching */
                else if (p && p->index == header_index) {
                    p_element = p->element; /* ->rowElement */

                    while (p_element->colElement) 
                        p_element = p_element->colElement;

                    p_element->colElement = element;

                    free(column_header);
                    break;
                }

                /* Postfix */
                else if (p && p->header == NULL) {
                    column_header->element  = element;
                    p->header = column_header;
                    break;
                } 

                p = p->header; 
            }
        }
    }
}

int Sparse_Matrix::print_header_hor(struct Header ** header) {
    struct Header * p;
    struct Element * element;
    int size = 0;

    if (!(*header)) {
        red_color();
        printf("Column header is empty\n");
        reset_color();

        return 0; 
    } 
    else  
        for (p = (*header); p != NULL; p = p->header) { 
            size += sizeof(p); 
            
            red_color(); 
            printf("index: %d\n", p->index);
 
            yellow_color();
            for (element = p->element; element != NULL; element = element->rowElement) {
                printf("val: %d\n", element->value);
                size += sizeof(element); 
            } 
            reset_color(); 
            printf("\n");
 
        }

    
    return size;
}

int Sparse_Matrix::print_header_ver(struct Header **header) {
    struct Header * p;
    struct Element * element;
    int size = 0;

    if (!(*header)) {
        red_color();
        printf("Row header is empty\n");
        reset_color();

        return 0; 
    }
    else 
        for (p = (*header); p != NULL; p = p->header) { 
            size += sizeof(p); 
            
            red_color(); 
            printf("index: %d\n", p->index);
 
            yellow_color();
            for (element = p->element; element != NULL; element = element->colElement) {
                printf("val: %d\n", element->value);
                size += sizeof(element); 
            } 
            reset_color(); 
            printf("\n");
 
        }
    return size;
}

