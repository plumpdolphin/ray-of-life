#include <stdio.h>

#include "raylib.h"
 
#include "board.h"



int 
main(void) {
    // Create window at default size
    InitWindow(1600, 800, "Ray of Life");
    SetTargetFPS(30);
    
    // Capture current monitor
    int mIdx = GetCurrentMonitor();

    /* Getting size varies based on window sizing. */
#ifdef BORDERLESS
    // Set window into "Borderless Windowed Fullsceen"
    ToggleBorderlessWindowed();

    // Calculate monitor size
    int windowWidth  = GetMonitorWidth(mIdx); 
    int windowHeight = GetMonitorHeight(mIdx); 

#else
    // Adjust window size to be 2:1, at half the size of the largest dimension
    int windowWidth  = GetMonitorWidth(mIdx) / 2; 
    int windowHeight = GetMonitorHeight(mIdx) / 2; 

    // Calculating max size
    windowWidth = (windowWidth <= windowHeight) ? windowHeight >> 1 : windowWidth;
    windowHeight = (windowWidth > windowHeight) ? windowWidth >> 1  : windowHeight;

    // Set window size to calculated size (non-adjustable after launch)
    SetWindowSize(windowWidth, windowHeight);
#endif


    // Calculate board size
    int rows    = windowHeight / CELL_SIZE;
    int columns = windowWidth  / CELL_SIZE;



    // Create board
    board_t board = board_new(rows, columns);



    // Set up auto-reset
    int frameCurrent = 0;
    int frameReset   = 1028;

    char  frameCounter[64];
    Color frameCounterColor = (Color){0xFF, 0xFF, 0xFF, 0x20};


    // Main loop
    while (!WindowShouldClose())
    {
        // Reset board every N frames
        frameCurrent = (frameCurrent + 1) % frameReset;
        if (!frameCurrent)
            board_seed(&board);

        // Update frame counter
        snprintf(frameCounter, 64, "Frame: %i", frameCurrent);

        // Update cells
        board_update(&board);

        // Render
        BeginDrawing();
            ClearBackground(BLACK);

            // Draw board cells
            board_draw(&board, RAYWHITE);

            // Draw Debug GUI
            DrawText(frameCounter, 8, 8, 24, frameCounterColor);
            // DrawFPS(8, 8);

            // Signature :D
            DrawText("(c) PlumpDolphin", windowWidth - 136, windowHeight - 24, 16, frameCounterColor);
        EndDrawing();
    }



    // Properly close out game
    CloseWindow();

    // Cleanup board data
    board_free(&board);

    // All done!
    return 0;
}