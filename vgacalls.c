#include <hal.h>
#include <drawing.h>

#define MAX_VGACALL 10

//Adapted the code for sysapi to work with vgacalls instead and interrupt 0x81.

typedef struct _VGACallInfo
{
    void * VGACall;
    int    ParamCount;
} VGACallInfo;

VGACallInfo _VGACalls[MAX_VGACALL];

void InitialiseVGACall(int index, void * vgaCall, int paramCount)
{
    if (index >= 0 && index < MAX_VGACALL)
    {
        _VGACalls[index].VGACall = vgaCall;
        _VGACalls[index].ParamCount = paramCount;
    }
}

void VGACallDispatcher()
{
    static int index = 0;
    asm volatile ("movl %%eax, %0" : "=r" (index));

    if(index < MAX_VGACALL)
    {
        asm volatile("push %edx\n\t"
                     "push %ecx\n\t"
                     "push %ebx\n\t");
        void * vgaFunction = _VGACalls[index].VGACall;
        int paramCount = _VGACalls[index].ParamCount;

        switch (paramCount)
        {
            case 3:
                asm volatile("pop %%ebx\n\t"
                             "pop %%ecx\n\t"
                             "pop %%edx\n\t"
                             "push %%edx\n\t"
                             "push %%ecx\n\t"
                             "push %%ebx\n\t"
                             "call *%0\n\t"
                             "addl $12, %%esp"
                             : : "r"(vgaFunction)
                             );
                             break;
            case 2:
				asm volatile ("pop %%ebx\n\t"
							  "pop %%ecx\n\t"
							  "pop %%edx\n\t"
							  "push %%ecx\n\t"
							  "push %%ebx\n\t"
							  "call *%0\n\t"
							  "addl $8, %%esp"
							  : : "r"(vgaFunction)
							  );
							  break;
							  
			case 1:
				asm volatile ("pop %%ebx\n\t"
							  "pop %%ecx\n\t"
							  "pop %%edx\n\t"
							  "push %%ebx\n\t"
							  "call *%0\n\t"
							  "addl $4, %%esp"
							  : : "r"(vgaFunction)
							  );
							  break;
							  
			case 0:
				asm volatile ("pop %%ebx\n\t"
							  "pop %%ecx\n\t"
							  "pop %%edx\n\t"
							  "call *%0\n\t"
							  : : "r"(vgaFunction)
							  );
							  break;
        }
    }
    asm("leave");
    asm("iret");
}

#define I86_IDT_DESC_RING3      0x60

void InitialiseVGACalls()
{
    InitialiseVGACall(0, ClearScreen, 0);
    InitialiseVGACall(1, SetPixel, 3);
    InitialiseVGACall(2, DrawLine, 2);
    InitialiseVGACall(3, DrawPolygon, 3);
    InitialiseVGACall(4, DrawFilledPolygon, 3);
    InitialiseVGACall(5, DrawStyledLine, 3);
    InitialiseVGACall(6, DrawRectangle, 2);
    InitialiseVGACall(7, DrawFilledRectangle, 2);
    InitialiseVGACall(8, DrawCircle, 2);
    InitialiseVGACall(9, DrawFilledCircle, 2);
    HAL_SetInterruptVector(0x81, VGACallDispatcher, I86_IDT_DESC_RING3);
}