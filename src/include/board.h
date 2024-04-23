#pragma once

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"





// Size of each cell in pixels
#define CELL_SIZE 6

// Wraps value between 0 and max
//    This is used to make the elements at the end of the board
//    interact with those at the opposite side.
#define WRAP(value, max) (value + max) % max

// Converts (x, y) value pair into 1D array index
#define IDX(x, y, width) (y * width) + x
#define BIDX(x, y) IDX( WRAP(x, b->cols), WRAP(y, b->rows), b->cols )





// Board structure
typedef struct {
    int rows, cols, length;
    bool *cells, *buffer;
    Texture2D texture;
} board_t;





// Available functions
board_t board_new(int rows, int cols);
void board_free(board_t *b); 
void board_seed(board_t *b);

void board_update(board_t *b);
void board_draw(board_t *b, Color c);

int cell_neighbor(board_t *b, int x, int y);





// Constructor/destructor functions
board_t
board_new(int rows, int columns) {
    // Create board
    board_t b;
    b.rows = rows;
    b.cols = columns;
    b.length = rows * columns;
    b.cells  = (bool*)calloc(b.length, sizeof(bool));
    b.buffer = (bool*)calloc(b.length, sizeof(bool));

    // Create placeholder texture
    Image image = GenImageColor(columns, rows, WHITE);
    b.texture = LoadTextureFromImage(image);
    UnloadImage(image);

    // Setup board state
    board_seed(&b);
    
    // Return board
    return b;
}

void
board_free(board_t *b) {
    // Free up board's arrays
    free(b->cells);
    free(b->buffer);

    // Free texture
    UnloadTexture(b->texture);
}

void 
board_seed(board_t *b) {
    // Initialize RNG
    srand(time(NULL));

    // Set cell values to random 
    for (int row = 0; row < b->rows; row++) {
        for (int col = 0; col < b->cols; col++) {
            // Random value with (1 / 16) chance that cell is live
            bool value = !(rand() & 0b1111);
            
            // Set cell and buffers to value
            b->cells[BIDX(col, row)]  = value;
            b->buffer[BIDX(col, row)] = value;
        }
    }
}





// Interaction functions
void
board_update(board_t *b) {
    // Run rule checks
    for (int row = 0; row < b->rows; row++) {
        for (int col = 0; col < b->cols; col++) {
            // Get neighbor count
            int neighbor = cell_neighbor(b, col, row);
            
            // Kill cell
            if (neighbor < 2 || neighbor > 3)
                b->buffer[BIDX(col, row)] = false;

            // Birth cell
            else if (neighbor == 3)
                b->buffer[BIDX(col, row)] = true;

            // Otherwise, do nothing
        }
    }


    // Copy values from buffer into cells
    for (int i = 0; i < b->length; i++)
        b->cells[i] = b->buffer[i];


    // Update texture for rendering
    Image image = GenImageColor(b->cols, b->rows, WHITE);
    unsigned char *bytes = (unsigned char*)image.data;


    // Set alpha from cell values
    for (int i = 0; i < b->length; i++)
        bytes[(i << 2) + 3] = (unsigned char)(256 - b->cells[i]);


    // Update texture and cleanup
    UpdateTexture(b->texture, image.data);
    UnloadImage(image);
}

void 
board_draw(board_t *b, Color c) {
    DrawTextureEx(b->texture, (Vector2){0, 0}, 0, CELL_SIZE, c);
}





// Cell functions
int
cell_neighbor(board_t *b, int x, int y) {
    // This runs through a window surrounding the cell position
    // starting at (x-1, y-1) to (x+1, y+1), skipping the center cell.
    
    // -----
    // -xxx-
    // -x-x-
    // -xxx-
    // -----

    int count = 0;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            // Skip self
            if (i == 0 && j == 0)
                continue;
        
            // Increase count if cell is true
            count += b->cells[BIDX(x + j, y + i)];
        }
    }

    return count;
}