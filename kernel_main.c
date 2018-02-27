#include <string.h>
#include <console.h>
#include <hal.h>
#include <exception.h>
#include "physicalmemorymanager.h"
#include "virtualmemorymanager.h"
#include "bootinfo.h"
#include <keyboard.h>
#include <sysapi.h>
#include <vgacalls.h>
#include <user.h>
#include <vgamodes.h>
#include <drawing.h>

// This is a dummy __main.  For some reason, gcc puts in a call to 
// __main from main, so we just include a dummy.
 
BootInfo *	_bootInfo;
 
void __main() {}

void InitialisePhysicalMemory()
{
	// Initialise the physical memory manager
	// We place the memory bit map in the next available block of memory after the kernel.
	// Note that all memory addresses here are physical memory addresses since we are dealing
	// with management of physical memory

	uint32_t memoryMapAddress = 0x100000 + _bootInfo->KernelSize;
	if (memoryMapAddress % PMM_GetBlockSize() != 0)
	{
		// Make sure that the memory map is going to be aligned on a block boundary
		memoryMapAddress = (memoryMapAddress / PMM_GetBlockSize() + 1) * PMM_GetBlockSize();
	}
	uint32_t sizeOfMemoryMap = PMM_Initialise(_bootInfo, memoryMapAddress);

	// We now need to mark various regions as unavailable
	
	// Mark first page as unavailable (so that a null pointer is invalid)
	PMM_MarkRegionAsUnavailable(0x00, PMM_GetBlockSize());

	// Mark memory used by kernel as unavailable
	PMM_MarkRegionAsUnavailable(0x100000, _bootInfo->KernelSize);

	// Mark memory used by memory map as unavailable
	PMM_MarkRegionAsUnavailable(PMM_GetMemoryMap(), sizeOfMemoryMap);

	// Reserve two blocks for the stack and make unavailable (the stack is set at 0x90000 in boot loader)
	uint32_t stackSize = PMM_GetBlockSize() * 2;
	PMM_MarkRegionAsUnavailable(_bootInfo->StackTop - stackSize, stackSize);
	
	// Reserve two block for the kernel stack and make unavailable
	PMM_MarkRegionAsUnavailable(0x80000 - stackSize, stackSize);
}
void Initialise()
{
	ConsoleClearScreen(0x1F);
	ConsoleWriteString("UODOS 32-bit Kernel. Kernel size is ");
	ConsoleWriteInt(_bootInfo->KernelSize, 10);
	ConsoleWriteString(" bytes\n");
	HAL_Initialise();
	InitialisePhysicalMemory();
	VMM_Initialise();
	KeyboardInstall(33); 
	InitialiseSysCalls();
	InitialiseVGACalls();
}

void main(BootInfo * bootInfo) 
{
	_bootInfo = bootInfo;
	Initialise();
	VGA_SetGraphicsMode(320, 200, 1);
	HAL_EnterUserMode();

	//Demo code.

	//Setting the colour palette entry 100 to be white.
	SetColour(100, 63, 63, 63);

	User_ClearScreen();

	//Draw a polygon that's almost the size of the screen. This is to show that filling it does not overflow. 
	//We colour it using the colour ID 100 which we set earlier using the SetColour method.
	User_DrawFilledPolygon((uint16_t[]) {20, 20, 180, 10, 300, 20, 310, 100, 300, 180, 160, 190, 20, 180}, 7, 100);

	//Draw a non-convex polygon.
	User_DrawPolygon((uint16_t[]) {60, 10, 80, 30, 100, 5, 75, 70, 55, 60}, 5, 3);

	//Draw a filled non-convex polygon. Non-convex polygons are not always filled, but this was a happy case in which its centroid is inside the shape.
	User_DrawFilledPolygon((uint16_t[]) {10, 10, 30, 30, 50, 5, 25, 70, 5, 60}, 5, 4); 

	//Draw 3 lines using 3 styles.
	User_DrawLine((uint16_t[]) {175, 50, 275, 50}, 1);
	User_DrawStyledLine((uint16_t[]) {175, 40, 275, 40}, 0, 4);
	User_DrawStyledLine((uint16_t[]) {175, 30, 275, 30}, 1, 3);

	//Draw 2 axis-aligned rectangles.
	User_DrawFilledRectangle(150, 100, 50, 30, 5);
	User_DrawRectangle(220, 100, 50, 30, 6);

	//Draw 2 circles.
	User_DrawCircle(100, 100, 20, 7);
	User_DrawFilledCircle(50, 100, 20, 8);

	while(1);
	
}
