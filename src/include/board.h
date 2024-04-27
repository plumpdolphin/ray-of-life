#pragma once

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"





// Size of each cell in pixels
#define CELL_SIZE 12

// Wraps value between 0 and max
//    This is used to make the elements at the end of the board
//    interact with those at the opposite side.
#define WRAP(value, max) (value + max) % max

// Converts (x, y) value pair into 1D array index
#define IDX(x, y, width) (y * width) + x
#define BIDX(x, y) IDX( WRAP(x, b->cols), WRAP(y, b->rows), b->cols )





// Board structure
typedef struct {
    // Play state
    bool playing;

    // Board timing
    float fps;
    float time;
    int frame;
    int autoReset;

    // Size of board
    int rows, cols, length;

    // Size of cell
    int sizeCell;

    // Cell containers
    bool *cells, *buffer;

    // Render texture
    Texture2D texture;
} board_t;





// Available functions
board_t board_new(int rows, int cols);
void board_free(board_t *b); 
void board_seed(board_t *b);
void board_reset(board_t *b);

bool board_handle_input(board_t *b);
void board_update(board_t *b, float dt);
void board_draw(board_t *b, Color c);

void board_update_frame(board_t *b);
void board_update_texture(board_t *b);

int cell_neighbor(board_t *b, int x, int y);





// Constructor/destructor functions
board_t
board_new(int rows, int columns) {
    // Initialize RNG
    srand(time(NULL));

    // Create board
    board_t b;
    b.playing = true;
    b.fps = 30;
    b.time = 0;
    b.frame = 0;
    b.autoReset = 0; //1024;

    b.rows = rows;
    b.cols = columns;
    b.length = rows * columns;
    b.cells  = (bool*)calloc(b.length, sizeof(bool));
    b.buffer = (bool*)calloc(b.length, sizeof(bool));

    b.sizeCell = CELL_SIZE;

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
    // Set cell values to random 
    for (int row = 0; row < b->rows; row++) {
        for (int col = 0; col < b->cols; col++) {
            // Random value that cell is live
            bool value = !(rand() & 0b1011);
            
            // Set cell and buffers to value
            b->cells[BIDX(col, row)] = value;
        }
    }

    // Perform iterations to remove noise
    for (int i = 0; i < 3; i++)
        board_update_frame(b);
}

void
board_reset(board_t *b) {
    // Set cell values to false
    for (int row = 0; row < b->rows; row++)
        for (int col = 0; col < b->cols; col++)
            b->cells[BIDX(col, row)] = false;
}





// Interaction functions
bool
board_handle_input(board_t *b) {
    // Contains whether a cell was updated or not
    bool drawn = 0;

    // Hangle keyboard
    if (IsKeyPressed(KEY_R)) {
        board_reset(b);
        drawn = 1;
    }
    
    if (IsKeyPressed(KEY_G)) {
        board_seed(b);
        drawn = 1;
    }

    // Handle mouse
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        int col = GetMouseX() / b->sizeCell;
        int row = GetMouseY() / b->sizeCell;

        drawn ^= !b->cells[BIDX(col, row)];
        b->cells[BIDX(col, row)] = true;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        int col = GetMouseX() / b->sizeCell; 
        int row = GetMouseY() / b->sizeCell;

        drawn ^= !b->cells[BIDX(col, row)];
        b->cells[BIDX(col, row)] = false;
    }

    return drawn;
}

void
board_update(board_t *b, float dt) {
    // Update current time
    b->time += dt;

    // Handle input
    bool drawn = board_handle_input(b);


    // Check if rule updates need to be performed
    if (!b->playing ||            // Board state paused
        (b->time < 1 / b->fps)) { // Too early for a cell update
        // Update texture and early return
        if (drawn)
            board_update_texture(b);
        return;
    } else {
        // Reset timer and continue
        b->time = 0;
    }

    // Calculate next frame
    board_update_frame(b);

    // Update texture to render to screen
    board_update_texture(b);
}

void 
board_draw(board_t *b, Color c) {
    DrawTextureEx(
        b->texture,      // Board texture
        (Vector2){0, 0}, // Position at top-left
        0,               // Rotation = 0 degrees
        b->sizeCell,     // Scale by cell size
        c                // Color of cell
    );
}





void
board_update_frame(board_t *b) {
    // Increment frame count
    b->frame += 1;

    // Reset board if auto-reset hit
    if (b->autoReset > 0)
        if (b->frame % b->autoReset == b->autoReset - 1)
            board_seed(b);

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

            // Otherwise, just copy value
            else
                b->buffer[BIDX(col, row)] = b->cells[BIDX(col, row)];
        }
    }

    // Copy values from buffer into cells
    for (int i = 0; i < b->length; i++)
        b->cells[i] = b->buffer[i];
}

void
board_update_texture(board_t *b) {
    // Update texture for rendering
    Image image = GenImageColor(b->cols, b->rows, WHITE);
    unsigned char *bytes = (unsigned char*)image.data;

    // Set alpha from cell values
    for (int i = 0; i < b->length; i++)
        // Each Color is 4-bytes wide. So we access the alpha channel every 4 bytes with a 3 byte offset.
        // The cell's alpha is set to (true = 255) or (false = 0)
        bytes[(i << 2) + 3] = (unsigned char)(256 - b->cells[i]);

    // Update texture and cleanup
    UpdateTexture(b->texture, image.data);
    UnloadImage(image);
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
            // Early return if cell is already dead
            if (count > 3)
                return count;

            // Skip self
            if (i == 0 && j == 0)
                continue;
        
            // Increase count if cell is true
            count += b->cells[BIDX(x + j, y + i)];
        }
    }

    return count;
}