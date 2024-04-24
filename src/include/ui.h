#pragma once

#include "raylib.h"
#include "raymath.h"

#include "theme.h"





// UI Structure
typedef struct {
    // Window size
    int width, height;
    
    // Element size
    int margin;
    int sizeFont, sizeIcon;
    
    // Render settings
    bool visible;
    bool debug; 

    // Play state
    bool playing;
    float stateChange, stateChangeDelay;
} ui_t;





// Available functions
ui_t ui_new(int width, int height);
void ui_draw(ui_t *ui, int frame, Color c);
void ui_update(ui_t *ui, float dt);

void ui_toggle_play(ui_t *ui); 
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
    ui.sizeIcon = 64;

    // Render states
    ui.visible = true;
    ui.debug   = false;

    // Default to play state
    ui.playing = true;
    ui.stateChange = 1;
    ui.stateChangeDelay = 1;

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


    // Calculate sizes
    int margin = ui->margin;
    
    int sI = ui->sizeIcon;      // Whole size
    int sH = ui->sizeIcon >> 1; // Half size
    int sT = ui->sizeIcon / 3;  // Third size


    // Calculate fade for state icon
    Color colorState = (Color) {
        c.r, c.g, c.b,
        c.a * (1 - (ui->stateChange > 1 ? 1 : ui->stateChange * ui->stateChange))
    };


    // Draw state icon
    if (ui->playing) {
        // Draw play icon
        DrawTriangle(
            (Vector2) { (ui->width >> 1) - sH, margin },
            (Vector2) { (ui->width >> 1) - sH, margin + sI },
            (Vector2) { (ui->width >> 1) + sH, margin + sH },
            colorState
        );
    } else {
        // Draw pause icon
        DrawRectangle(
            (ui->width >> 1) - sH, margin, // Position
            sT, sI,                        // Size
            colorState                     // Color
        );
        DrawRectangle(
            (ui->width >> 1) + sH - sT, margin, // Position
            sT, sI,                             // Size
            colorState                          // Color
        );
    }

    // Signature :D
    DrawText("(c) PlumpDolphin", ui->width - 136, ui->height - margin, 16, c);
}

void
ui_update(ui_t *ui, float dt) {
    ui->stateChange += dt * ui->stateChangeDelay;
}




void
ui_toggle_play(ui_t *ui) {
    // Flip playing state
    ui->playing ^= 1;

    // Reset state change timer
    ui->stateChange = 0;
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