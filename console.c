//	Basic Console Output.

#include <stdint.h>
#include <string.h>
#include <console.h>

// Video memory
uint16_t *_videoMemory = (uint16_t *)0xB8000;

#define CONSOLE_HEIGHT		25
#define CONSOLE_WIDTH		80

// Current cursor position
uint8_t _cursorX = 0;
uint8_t _cursorY = 0;

// Current color
uint8_t	_colour = 15;

char rev[40];
char digits[16] = "0123456789ABCDEF";

// Write byte to device through port mapped io
void  OutputByteToVideoController(unsigned short portid, unsigned char value) 
{
	asm volatile("movb	%1, %%al \n\t"
				 "movw	%0, %%dx \n\t"
				  "out	%%al, %%dx"
				 : : "m" (portid), "m" (value));
				 
}

// Update hardware cursor position
void UpdateCursorPosition(int x, int y) 
{
	if(x >= CONSOLE_WIDTH)
	{
		y++;
		x = 0;
	}

	if(y >= CONSOLE_HEIGHT)
	{
		y--;
		for(int h = 1; h < CONSOLE_HEIGHT; h++)
		{
			for(int w = 0; w <= CONSOLE_WIDTH; w++)
			{
				*(_videoMemory + ((h-1) * CONSOLE_WIDTH + w)) = *(_videoMemory + (h * CONSOLE_WIDTH + w));
			}
		}

		for(int w = 0; w<=CONSOLE_WIDTH; w++)
		{
			uint16_t attribute = _colour << 8;

			*(_videoMemory + ((CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH + w)) = ' ' | attribute;
		}

	}

	_cursorX = x;
	_cursorY = y;

	uint16_t cursorLocation = y * 80 + x;

	// Send location to VGA controller to set cursor
	// Send the high byte
	OutputByteToVideoController(0x3D4, 14);
	OutputByteToVideoController(0x3D5, cursorLocation >> 8); 
	// Send the low byte
	OutputByteToVideoController(0x3D4, 15);
	OutputByteToVideoController(0x3D5, cursorLocation);      
}

// Displays a character
void ConsoleWriteCharacter(unsigned char c) 
{
	uint16_t attribute = _colour << 8;
		
	// Write only if printable character

	if((c >= 32) && (c <= 127))
	{
		int16_t* location = _videoMemory + (_cursorY * CONSOLE_WIDTH + _cursorX);
		*location = c | attribute;
		
		UpdateCursorPosition(++_cursorX, _cursorY);
		return;
	}
	
	switch(c)
	{
		case 8:						//backspace
			UpdateCursorPosition(_cursorX - 1, _cursorY); //to be tested
			break;
		case 9:						//horizontal tab
			UpdateCursorPosition(_cursorX + 8, _cursorY);
			break;
		case 10:					//LF
			UpdateCursorPosition(_cursorX, ++_cursorY);
		case 13:					//CR
			UpdateCursorPosition(0, _cursorY);
			break;
		default:
			break;
	}
	return;
}

// Display specified string

void ConsoleWriteString(char* str) 
{
	if (!str)
	{
		return;
	}
	while (*str)
	{
		ConsoleWriteCharacter(*str++);
	}
}

void ConsoleWriteInt(unsigned int i, unsigned int base)
{
	char digit;
	int count = 0;
	int d = 0;

	if(i == 0 || base > 16)
	{
		ConsoleWriteCharacter('0');
		return;
	}

	while(i >= 1)
	{
		d = i % base;
		digit = digits[d];
		rev[count++] = digit;
		i /= base;
	}

	for(int j = count - 1; j >= 0; j--)
	{
		ConsoleWriteCharacter(rev[j]);
	}

	//Int version
	/*int c = 0;
	int rev = 0;
	int zeros = 0;
	
	if(i == 0)
	{
		ConsoleWriteCharacter(48);
		return;
	}

	while (i % base == 0)
	{
		zeros++;
		i /= base;
	}

	while (i >= 1)
	{
		c = i % base;
		rev = rev * base + c;
		i /= base;
	}

	while (rev >= 1)
	{
		c = rev % 10;
		ConsoleWriteCharacter(c + 48);
		rev /= 10;
	}

	while (zeros)
	{
		ConsoleWriteCharacter('0');
		zeros--;
	}
	*/
	return;
}

unsigned int ConsoleSetColour(const uint8_t c)
{
	uint8_t old_colour = _colour;
	_colour = c;
	return old_colour;
}

void ConsoleGotoXY(unsigned int x, unsigned int y)
{
	if((x >= 0) && (x < CONSOLE_WIDTH) && (y >= 0) && (y < CONSOLE_HEIGHT))
	{
		UpdateCursorPosition(x, y);
	}
	return;
}

void ConsoleGetXY(unsigned* x, unsigned* y)
{
	*x = _cursorX;
	*y = _cursorY;
}

int ConsoleGetWidth()
{
	return CONSOLE_WIDTH;
}

int ConsoleGetHeight()
{
	return CONSOLE_HEIGHT;
}

void ConsoleClearScreen(const uint8_t c)
{
	ConsoleSetColour(c);

	// Assume printable characters
	
	for (int h = 0; h < CONSOLE_HEIGHT; h++)
	{
		for	(int w = 0; w < CONSOLE_WIDTH; w++)
		{
			ConsoleWriteCharacter(32);
		}
	}

	UpdateCursorPosition(0, 0);
}