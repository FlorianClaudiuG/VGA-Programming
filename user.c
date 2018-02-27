#include <console.h>
#include <keyboard.h>

//Certain functions have been made easier to use by passing in parameters explicitly (see User_DrawRectangle as an example).
//These parameters are converted to an array of parameters to pass on to ring 0 functions.

void User_ConsoleWriteCharacter(unsigned char c)
{
	asm volatile ("movl $1, %%eax\n\t"
				  "movzx %0, %%ebx\n\t"
				  "int $0x80\n"
				  : : "b"(c)
				 );
}

void User_ConsoleWriteString(char* str)
{
	asm volatile ("xor %%eax, %%eax\n\t"
				  "leal (%0), %%ebx\n\t"
				  "int $0x80\n"
				  : : "b"(str)
				 );
}

void User_ConsoleWriteInt(unsigned int i, unsigned int base)
{
	asm volatile ("movl $2, %%eax\n\t"
				  "movl %0, %%ecx\n\t"
				  "movl %1, %%ebx\n\t"
				  "int $0x80\n"
				  : : "c"(base), "b"(i)
				 );
}

void User_ConsoleClearScreen(const uint8_t c)
{
	asm volatile ("movl $3, %%eax\n\t"
				  "movzx %0, %%ebx\n\t"
				  "int $0x80\n"
				  : : "b"(c)
				 );
}

void User_ConsoleGotoXY(unsigned int x, unsigned int y) 
{
	asm volatile ("movl $4, %%eax\n\t"
				  "movl %0, %%ecx\n\t"
				  "movl %1, %%ebx\n\t"
				  "int $0x80\n"
				  : : "c"(y), "b"(x)
				 );
}

void User_ClearScreen()
{
	asm volatile ("xor %%eax, %%eax\n\t"
				  "int $0x81\n"
				  : :
				 );
				
}

//Method takes x and y position of the pixel alongside its colour.
void User_SetPixel(uint16_t x, uint16_t y, uint8_t c)
{
	asm volatile ("movl $1, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "movzx %1, %%ebx\n\t"
				  "movzx %2, %%edx\n\t"
				  "int $0x81\n"
				  : : "c"(y), "b"(x), "d"(c)
				 );
}

void User_DrawLine(uint16_t * vertices, uint8_t c)
{
	asm volatile ("movl $2, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "leal (%1), %%ebx\n\t"
				  "int $0x81\n"
				  : : "c"(c), "b"(vertices)
				 );
}

void User_DrawPolygon(uint16_t * vertices, uint16_t numberOfVertices, uint8_t c)
{
	asm volatile ("movl $3, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "leal (%1), %%ebx\n\t"
				  "movzx %2, %%edx\n\t"
				  "int $0x81\n"
				  : : "c"(numberOfVertices), "b"(vertices), "d"(c)
				 );
}

void User_DrawFilledPolygon(uint16_t * vertices, uint16_t numberOfVertices, uint8_t c)
{
	asm volatile ("movl $4, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "leal (%1), %%ebx\n\t"
				  "movzx %2, %%edx\n\t"
				  "int $0x81\n"
				  : : "c"(numberOfVertices), "b"(vertices), "d"(c)
				 );
}

void User_DrawStyledLine(uint16_t * vertices, uint8_t style, uint8_t c)
{
	asm volatile ("movl $5, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "leal (%1), %%ebx\n\t"
				  "movzx %2, %%edx\n\t"
				  "int $0x81\n"
				  : : "c"(style), "b" (vertices), "d" (c)
				 );
}

//Method takes the top left coordinates x and y alongside the width, height and colour c of the rectangle.
void User_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t c)
{
	uint16_t params[] = {x, y, width, height};
	asm volatile ("movl $6, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "leal (%1), %%ebx\n\t"
				  "int $0x81\n"
				  : : "c"(c), "b" (params)
				 );
}

void User_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t c)
{
	uint16_t params[] = {x, y, width, height};
	asm volatile ("movl $7, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "leal (%1), %%ebx\n\t"
				  "int $0x81\n"
				  : : "c"(c), "b" (params)
				 );
}

// Method takes the circle coordinates x0 and y0 alongside its radius and colour.
void User_DrawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint8_t c)
{
	uint16_t params[] = {x0, y0, radius};
	asm volatile ("movl $8, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "leal (%1), %%ebx\n\t"
				  "int $0x81\n"
				  : : "c"(c), "b" (params)
				 );
}

void User_DrawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, uint8_t c)
{
	uint16_t params[] = {x, y, radius};
	asm volatile ("movl $9, %%eax\n\t"
				  "movzx %0, %%ecx\n\t"
				  "leal (%1), %%ebx\n\t"
				  "int $0x81\n"
				  : : "c"(c), "b" (params)
				 );
}