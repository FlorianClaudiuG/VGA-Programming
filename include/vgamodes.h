#ifndef _VGAMODES_H
#define _VGAMODES_H
#include <stdint.h>

//Added functions to retrieve screen width and height.

int VGA_SetGraphicsMode(uint16_t width, uint16_t height, uint8_t chain4);
int VGA_GetScreenWidth();
int VGA_GetScreenHeight();

#endif
