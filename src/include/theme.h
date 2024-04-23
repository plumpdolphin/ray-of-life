#pragma once

#include "raylib.h"



typedef struct {
    Color bg, fg, txt;
} theme_t;



static theme_t g_themes[] = {
    // White on black
    (theme_t) {
        .bg  = BLACK,
        .fg  = RAYWHITE,
        .txt = (Color){ 255, 255, 255, 32 },
    },
    
    // Github Dark
    (theme_t) {
        .bg  = (Color){ 16,  18,  22,  255 },
        .fg  = (Color){ 139, 148, 158, 255 },
        .txt = (Color){ 108, 164, 248, 255 }
    },

    // Solarized Dark
    (theme_t) {
        .bg  = (Color){ 0,   43,  54,  255 },
        .fg  = (Color){ 131, 148, 150, 255 },
        .txt = (Color){ 42,  161, 152, 255 }
    },

    // Dracula
    (theme_t) {
        .bg  = (Color){ 40,  42,  54,  255 },
        .fg  = (Color){ 248, 248, 242, 255 },
        .txt = (Color){ 139, 233, 253, 255 }
    },

    // Breeze
    (theme_t) {
        .bg  = (Color){ 35,  38,  39,  255 },
        .fg  = (Color){ 252, 252, 252, 255 },
        .txt = (Color){ 29,  153, 243, 255 }
    },

    // Material
    (theme_t) {
        .bg  = (Color){ 234, 234, 234, 255 },
        .fg  = (Color){ 35,  35,  34,  255 },
        .txt = (Color){ 78,  78,  78,  255 }
    },

    // Solarized Light
    (theme_t) {
        .bg  = (Color){ 253, 246, 227, 255 },
        .fg  = (Color){ 101, 123, 131, 255 },
        .txt = (Color){ 88,  110, 117, 128 }
    }
};