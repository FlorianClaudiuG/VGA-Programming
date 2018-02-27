#include <drawing.h>
#include <console.h>

int screenHeight = 200;
int screenWidth = 320;

uint8_t *videoMemory = (uint8_t*) 0xA0000;

//Method to clear the screen to black. It also updates the screen height and width values using the added VGA functions inside vgamodes.
void ClearScreen()
{
    screenHeight = VGA_GetScreenHeight();
    screenWidth = VGA_GetScreenWidth();

	uint8_t blank = 0;

    for(int h = 0; h < screenHeight; h++)
    {
        for(int w = 0; w < screenWidth; w++)
        {
            *(videoMemory + h * screenWidth + w) = blank;
        }
    }
}

//Method to set a single pixel to the specified colour. It also checks if pixel is within bounds.
void SetPixel(uint16_t x, uint16_t y, uint8_t c)
{
    if((x < screenWidth && x >= 0) && (y < screenHeight && y >= 0))
    {
        *(videoMemory + y * screenWidth + x) = c;
    }
}

//Method to retrieve pixel colour at the specified coordinates.
uint8_t GetPixel(uint16_t x, uint16_t y)
{
    return *(videoMemory + y * screenWidth + x);
}

//Method that draws a line for ring 0 use (to be called from other functions inside ring 0 for readability).
void KernelDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c)
{
    int w = x1 - x0 ;
    int h = y1 - y0 ;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;

    if (w < 0)
    { 
        dx1 = -1;
        dx2 = -1;
    } 
    else if (w > 0) 
    {
        dx1 = 1;
        dx2 = 1;
    }

    if (h < 0) 
    {
        dy1 = -1;
    } 
    else if (h > 0) 
    {
        dy1 = 1;
    }

    int longest; 
    if(w < 0)
    {
        longest = -w;
    }
    else
    {
        longest = w;
    }

    int shortest;
    if(h < 0)
    {
        shortest = -h;
    }
    else
    {
        shortest = h;
    }

    if (!(longest > shortest)) 
    {
        int aux = longest;
        longest = shortest;
        shortest = aux;
        if (h < 0) 
        {
            dy2 = -1 ;
        } 
        else if (h > 0) 
        {
            dy2 = 1;
        }
        dx2 = 0 ;            
    }

    int numerator = longest >> 1 ;
    for (int i = 0; i <= longest; i++) 
    {
        SetPixel(x0, y0, c);
        numerator += shortest;

        if (!(numerator < longest)) 
        {
            numerator -= longest;
            x0 += dx1 ;
            y0 += dy1 ;
        } 
        else 
        {
            x0 += dx2 ;
            y0 += dy2 ;
        }
    }
}

//Method that draws a line using Bresenham's algorithm.
void DrawLine(uint16_t * vertices, uint8_t c)
{
    uint16_t x0 = *vertices;
    uint16_t y0 = *(vertices + 1);
    uint16_t x1 = *(vertices + 2);
    uint16_t y1 = *(vertices + 3);
    int w = x1 - x0 ;
    int h = y1 - y0 ;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;

    if (w < 0)
    { 
        dx1 = -1;
        dx2 = -1;
    } 
    else if (w > 0) 
    {
        dx1 = 1;
        dx2 = 1;
    }

    if (h < 0) 
    {
        dy1 = -1;
    } 
    else if (h > 0) 
    {
        dy1 = 1;
    }

    int longest; 
    if(w < 0)
    {
        longest = -w;
    }
    else
    {
        longest = w;
    }

    int shortest;
    if(h < 0)
    {
        shortest = -h;
    }
    else
    {
        shortest = h;
    }

    if (!(longest > shortest)) 
    {
        int aux = longest;
        longest = shortest;
        shortest = aux;
        if (h < 0) 
        {
            dy2 = -1 ;
        } 
        else if (h > 0) 
        {
            dy2 = 1;
        }
        dx2 = 0 ;            
    }

    int numerator = longest >> 1 ;
    for (int i = 0; i <= longest; i++) 
    {
        SetPixel(x0, y0, c);
        numerator += shortest;

        if (!(numerator < longest)) 
        {
            numerator -= longest;
            x0 += dx1 ;
            y0 += dy1 ;
        } 
        else 
        {
            x0 += dx2 ;
            y0 += dy2 ;
        }
    }
}

//Draw an axis-aligned rectangle.
void DrawRectangle(uint16_t * params, uint8_t c)
{
    uint16_t x = *params;
    uint16_t y = *(params + 1);
    uint16_t width = *(params + 2);
    uint16_t height = *(params + 3);
    KernelDrawLine(x, y, x + width, y, c);
    KernelDrawLine(x + width, y, x + width, y + height, c);
    KernelDrawLine(x + width, y + height, x, y + height, c);
    KernelDrawLine(x, y + height, x, y, c);
}

//Draw a filled axis-aligned rectangle.
void DrawFilledRectangle(uint16_t * params, uint8_t c)
{
    uint16_t x = *params;
    uint16_t y = *(params + 1);
    uint16_t width = *(params + 2);
    uint16_t height = *(params + 3);

    for(int i = 0; i <= height; i++)
    {
        KernelDrawLine(x, y + i, x + width, y + i, c);
    }
}

//Draw a circle using the Midpoint Circle Algorithm
void DrawCircle(uint16_t * params, uint8_t c)
{
    uint16_t x0 = *params;
    uint16_t y0 = *(params + 1);
    uint16_t radius = *(params + 2);
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int epsilon = dx - (radius << 1);

    while (x >= y)
    {
        SetPixel(x0 + x, y0 + y, c);
        SetPixel(x0 + y, y0 + x, c);
        SetPixel(x0 - y, y0 + x, c);
        SetPixel(x0 - x, y0 + y, c);
        SetPixel(x0 - x, y0 - y, c);
        SetPixel(x0 - y, y0 - x, c);
        SetPixel(x0 + y, y0 - x, c);
        SetPixel(x0 + x, y0 - y, c);

        if (epsilon <= 0)
        {
            y++;
            epsilon += dy;
            dy += 2;
        }
        if (epsilon > 0)
        {
            x--;
            dx += 2;
            epsilon += (-radius << 1) + dx;
        }
    }
}

//Draw a circle then run flood fill on it. 
//This function has not been adapted to work on too large a radius. Any area over 12000 pixels might cause an overflow. This translates to radii over 62 pixels long.
void DrawFilledCircle(uint16_t * params, uint8_t c)
{
    DrawCircle(params, c);
    //Only fill if the circle's radius is small enough to not overflow.
    if(*(params + 2) < 62)
    {
        Fill(*params, *(params + 1), c);
    }
}

//Recursive flood fill function flooding in 4 directions.
void Fill(uint16_t x, uint16_t y, uint8_t c)
{
    if(GetPixel(x, y) == c)
    {
        return;
    }
    else
    {
        SetPixel(x, y, c);
        Fill(x + 1, y, c);
        Fill(x, y + 1, c);
        Fill(x, y - 1, c);
        Fill(x - 1, y, c);
    }
}

//Draw a polygon using its vertices.
//The vertices pointer points to an array containing alternating X and Y values. so even numbers will be X values, and odd numbers represent their Y pairs.
//For example, vertices[0] is the X component of a single vertex and vertices[1] is the Y component of the same vertex. The size of this array is always numberOfVertices * 2.
void DrawPolygon(uint16_t * vertices, uint16_t numberOfVertices, uint8_t c) //vertices is an array containing x, y pairs for each vertex (it only holds ints, so it should be twice the size)
{
    int i = 0;
    for(i = 0; i < (numberOfVertices - 1) * 2 - 1; i += 2)
    {
        KernelDrawLine(*(vertices + i), *(vertices + i + 1), *(vertices + i + 2), *(vertices + i + 3), c);
    }
    KernelDrawLine(*(vertices + numberOfVertices * 2 - 2), *(vertices + numberOfVertices * 2 - 1), *vertices, *(vertices + 1), c);
}

//Draw a filled polygon. It works on convex polygons every time, but not always on non-convex polygons.
void DrawFilledPolygon(uint16_t * vertices, uint16_t numberOfVertices, uint8_t c)
{
    //Draw the outline of the polygon.
    DrawPolygon(vertices, numberOfVertices, c);

    //Calculate centroid to figure out where the flood fill starts from.
    uint16_t centroid[2];
    Centroid(vertices, numberOfVertices, (uint16_t *)centroid);

    //Calculate area to see if we can afford filling up the polygon.
    int32_t area = Area(vertices, numberOfVertices);
    
    //After a few tests, I've determined that an area of about 12000 pixels is too much for the stack to handle (since flood fill is recursive).
    //To be safe, I'm checking to see if the area is above 10000, in which case I triangulate the polygon and fill each triange one at a time.
    if(area > 10000)
    {
        //Triangles are made using the centroid and 2 adjacent vertices. Calling the DrawFilledPolygon function recursively ensures their area is also checked for further triangulation.
        uint16_t temp[6];
        temp[0] = centroid[0];
        temp[1] = centroid[1];
        for(int i = 0; i <= (numberOfVertices-2) * 2; i += 2)
        {
            temp[2] = *(vertices + i);
            temp[3] = *(vertices + i + 1);
            temp[4] = *(vertices + i + 2);
            temp[5] = *(vertices + i + 3);
            DrawFilledPolygon((uint16_t*)temp, 3, c);
        }
        //Last triangle to be drawn and filled(using first and last vertices).
        temp[2] = *(vertices + numberOfVertices * 2 - 2);
        temp[3] = *(vertices + numberOfVertices * 2 - 1);
        temp[4] = *vertices;
        temp[5] = *(vertices + 1);
        DrawFilledPolygon((uint16_t*)temp, 3, c);
    }
    //Run flood fill on the centroid of the shape.
    Fill(centroid[0], centroid[1], c);
}

//Function to draw a line according to a style.
//Two styles supported: 0 for dotted ("...") line, 1 for dashed ("---") line.
void DrawStyledLine(uint16_t * vertices, uint8_t style, uint8_t c)
{
    uint16_t x0 = *vertices;
    uint16_t y0 = *(vertices + 1);
    uint16_t x1 = *(vertices + 2);
    uint16_t y1 = *(vertices + 3);
    int w = x1 - x0 ;
    int h = y1 - y0 ;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;

    if (w < 0)
    { 
        dx1 = -1;
        dx2 = -1;
    } 
    else if (w > 0) 
    {
        dx1 = 1;
        dx2 = 1;
    }

    if (h < 0) 
    {
        dy1 = -1;
    } 
    else if (h > 0) 
    {
        dy1 = 1;
    }

    int longest; 
    if(w < 0)
    {
        longest = -w;
    }
    else
    {
        longest = w;
    }

    int shortest;
    if(h < 0)
    {
        shortest = -h;
    }
    else
    {
        shortest = h;
    }

    if (!(longest > shortest)) 
    {
        int aux = longest;
        longest = shortest;
        shortest = aux;
        if (h < 0) 
        {
            dy2 = -1 ;
        } 
        else if (h > 0) 
        {
            dy2 = 1;
        }
        dx2 = 0 ;            
    }

    int numerator = longest >> 1 ;
    for (int i = 0; i <= longest; i++) 
    {   
        //Only difference to the DrawLine algorithm. If the style is wrong, a normal line is drawn.
        switch(style)
        {
            case 0:
                if(i % 2 == 0)
                {
                    SetPixel(x0, y0, c);
                }
                break;
            case 1:
                if(i % 5 != 0)
                {
                    SetPixel(x0, y0, c);
                }
                break;
            default:
                SetPixel(x0, y0, c);
                break;
        }
        
        numerator += shortest;

        if (!(numerator < longest)) 
        {
            numerator -= longest;
            x0 += dx1 ;
            y0 += dy1 ;
        } 
        else 
        {
            x0 += dx2 ;
            y0 += dy2 ;
        }
    }
}

void SetColour(uint8_t id, uint8_t r, uint8_t g, uint8_t b)
{
    if(id < 256 && r < 64 && g < 64 && b < 64)
    {
        HAL_OutputByteToPort(0x3c8, id);
	    HAL_OutputByteToPort(0x3c9, r);
	    HAL_OutputByteToPort(0x3c9, g);
	    HAL_OutputByteToPort(0x3c9, b);
    }
}

//Utility method to calculate the centroid of a polygon.
//Usually has arrays passed in for vertices and center.
void Centroid(uint16_t * vertices, uint16_t numberOfVertices, uint16_t * center)
{   
    //Set our center to 0, 0 to ensure accuracy. 
    *center = 0;
    *(center + 1) = 0;

    //Calculate the sum of x's and y's and average them.
    for(int i = 0; i < numberOfVertices * 2; i += 2)
    {
        *center += *(vertices + i);
        *(center + 1) += *(vertices + i + 1);
    }

    *center = *center / numberOfVertices;
    *(center + 1) = *(center + 1) / numberOfVertices;
    
    return;
}

//Utility function to calculate the area of a polygon.
int32_t Area(uint16_t * vertices, uint16_t numberOfVertices)
{
    int32_t area = 0;
    int16_t sumX = 0;
    int16_t difY = 0;
    
    for(int i = 0; i < (numberOfVertices - 1) * 2; i += 2)
    {
        sumX = (*(vertices + i) + *(vertices + i + 2));
        difY = (*(vertices + i + 1) - *(vertices + i + 3));
        area = area + (sumX * difY);
    }
    sumX = (*(vertices + numberOfVertices * 2 - 2) + *vertices);
    difY = (*(vertices + numberOfVertices * 2 - 1) - *(vertices + 1));
    area = area + (sumX * difY);

    //If area is negative, it's because the polygon is traced the wrong way around. Simply changing its sign will fix the problem.
    if(area < 0)
    {
        area = -area;
    }

    return area / 2;
}