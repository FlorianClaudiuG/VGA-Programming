#include <stdint.h>
#include <vgamodes.h>
#include <hal.h>

//Explanation for each method is inside drawing.c
//Most of these methods have been adapted to use less than 3 parameters in order to work in user mode.
//This is usually accomplished by passing in the required values via array or pointer as a single parameter.
//For passing in vertices, a single array of ints is used containing the Xs as its even values and Ys as its odd values. Each vertex is comprised of a pair of adjacent X and Y values.

//Drawing functions.
void ClearScreen();
void SetPixel(uint16_t x, uint16_t y, uint8_t c);
void DrawLine(uint16_t * vertices, uint8_t c);
void DrawRectangle(uint16_t * params, uint8_t c);
void DrawFilledRectangle(uint16_t * params, uint8_t c);
void DrawCircle(uint16_t * params, uint8_t c);
void DrawFilledCircle(uint16_t * params, uint8_t c);
void DrawPolygon(uint16_t * vertices, uint16_t numberOfVertices, uint8_t c);
void DrawFilledPolygon(uint16_t * vertices, uint16_t numberOfVertices, uint8_t c);
void DrawStyledLine(uint16_t * vertices, uint8_t style, uint8_t c);

//Utility functions.
void SetColour(uint8_t id, uint8_t r, uint8_t g, uint8_t b);
uint8_t GetPixel(uint16_t x, uint16_t y);
void KernelDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c);
void Fill(uint16_t x, uint16_t y, uint8_t c);
void Centroid(uint16_t * vertices, uint16_t numberOfVertices, uint16_t * center);
int32_t Area(uint16_t * vertices, uint16_t numberOfVertices);