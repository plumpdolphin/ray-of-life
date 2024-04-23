#include <stdio.h>
#include <string.h>

#include "raylib.h"

#include "icon.h" 
#include "board.h"
#include "theme.h"



int 
main(void) {
    // Create window at default size
    InitWindow(1600, 800, "Ray of Life");
    SetTargetFPS(30);

    // Set icon
    SetEmbeddedWindowIcon();

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

    // Set default theme
    theme_t theme = g_themes[0];

    // Set up auto-reset
    int frameCurrent = 0;
    int frameReset   = 1028;
    char frameCounter[32];


    // Main loop
    while (!WindowShouldClose())
    {
        // Reset board every N frames
        frameCurrent = (frameCurrent + 1) % frameReset;
        if (!frameCurrent)
            board_seed(&board);

        // Update frame counter
        snprintf(frameCounter, 32, "Frame: %i", frameCurrent);

        // Update cells
        board_update(&board);


        // Handle input and switch themes
        switch(GetKeyPressed()) {
            case KEY_ONE:
                theme = g_themes[0];
                break;
            case KEY_TWO:
                theme = g_themes[1];
                break;
            case KEY_THREE:
                theme = g_themes[2];
                break;
            case KEY_FOUR:
                theme = g_themes[3];
                break;
            case KEY_FIVE:
                theme = g_themes[4];
                break;
            case KEY_SIX:
                theme = g_themes[5];
                break;
            case KEY_SEVEN:
                theme = g_themes[6];
                break;
        }


        // Render
        BeginDrawing();
            ClearBackground(theme.bg);

            // Draw board cells
            board_draw(&board, theme.fg);

            // Draw Debug GUI
            DrawText(frameCounter, 8, 8, 24, theme.txt);
            // DrawFPS(8, 8);

            // Signature :D
            DrawText("(c) PlumpDolphin", windowWidth - 136, windowHeight - 24, 16, theme.txt);
        EndDrawing();
    }


    // Properly close out game
    CloseWindow();

    // Cleanup board data
    board_free(&board);

    // All done!
    return 0;
}