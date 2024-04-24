#pragma once

#include "raylib.h"

#include "theme.h"





// UI Structure
typedef struct {
    // Window size
    int width, height;
    
    // Element size
    int margin;
    int sizeFont;
    
    // Render settings
    bool visible;
    bool debug;

    // Play state
    bool playing;
    float playChange;
} ui_t;





// Available functions
ui_t ui_new(int width, int height);
void ui_draw(ui_t *ui, int frame, Color c);

void ui_toggle_debug(ui_t *ui);
void ui_toggle_visible(ui_t *ui);





// Constructor/destructor functions
ui_t
ui_new(int width, int height) {
    // Create UI
    ui_t ui;
    ui.width  = width;
    ui.height = height;
    
    // Default UI sizing
    ui.margin   = 32;
    ui.sizeFont = 24;

    // Render states
    ui.visible = true;
    ui.debug   = false;

    return ui;
}

void
ui_draw(ui_t *ui, int frame, Color c) {
    // Draw Debug GUI first
    if (ui->debug) {
        // Frame counter
        char frameCounter[32];
        snprintf(frameCounter, 32, "Frame: %i", frame);
        DrawText(frameCounter, 8, 8, ui->sizeFont, c);
        
        // FPS counter
        DrawFPS(8, 16 + ui->sizeFont);
    }


    // Early return before remaining UI if not visible
    if (!ui->visible)
        return;

    // Signature :D
    DrawText("(c) PlumpDolphin", ui->width - 136, ui->height - ui->margin, 16, c);
}





void
ui_toggle_debug(ui_t *ui) {
    // Flip debug state
    ui->debug ^= 1;
}

void
ui_toggle_visible(ui_t *ui) {
    // Flip visible state
    ui->visible ^= 1;
}