#ifndef _USER_H
#define _USER_H
#include <keyboard.h>

//Added drawing functions to user.h and user.c

void User_ConsoleWriteCharacter(unsigned char c); 
void User_ConsoleWriteString(char* str); 
void User_ConsoleWriteInt(unsigned int i, unsigned int base); 
void User_ConsoleClearScreen(const uint8_t c); 
void User_ConsoleGotoXY(unsigned int x, unsigned int y); 
void User_ClearScreen();
void User_SetPixel(uint16_t x, uint16_t y, uint8_t c);
void User_DrawLine(uint16_t * vertices, uint8_t c);
void User_DrawPolygon(uint16_t * vertices, uint16_t numberOfVertices, uint8_t c);
void User_DrawFilledPolygon(uint16_t * vertices, uint16_t numberOfVertices, uint8_t c);
void User_DrawStyledLine(uint16_t * vertices, uint8_t style, uint8_t c);
void User_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t c);
void User_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t c);
void User_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint8_t c);
void User_DrawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, uint8_t c);

#endif