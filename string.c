// Implementation of string and memory functions

#include <string.h>

int strcmp(const char* str1, const char* str2) 
{
	int result = 0;
	while (!(result = *(unsigned char*)str1 - *(unsigned char*)str2) && *str2)
	{
		++str1;
		++str2;
	}
	if (result < 0)
	{
		result = -1;
	}
	if (result > 0)
	{
		result = 1;
	}
	return result;
}

char* strcpy(char* destination, const char* source)
{
    do
    {
        *destination = *source++;
    }
    while(*destination++ != 0);

    return destination;
}

errno_t strcpy_s(char *destination, size_t numberOfElements, const char *source)
{
	if (destination == NULL || source == NULL)
	{
		return EINVAL;
	}
	if (numberOfElements == 0)
	{
		*destination = '\0';
		return ERANGE;
	}
	char * sourceTemp = (char *)source;
	char * destinationTemp = destination;
	char charSource;
	while (numberOfElements--)
	{
		charSource = *sourceTemp++;
		*destinationTemp = charSource;
		if (charSource == 0)
		{
			// Copy succeeded
			return 0;
		}
	}
	// We haven't reached the end of the string
	*destination = '\0';
	return ERANGE;
}

size_t strlen(const char* source)
{
    char* p = (char*)source;

    while(*p != 0)
    {
        p++;
    }

    return p - source;
}

void * memcpy(void * destination, const void * source, size_t count)
{
    char *dest = destination;
    char *src = (char*)source;
    for(int i = 0; i < count; i++)
    {
        *dest++ = *src++;
    }
    return destination;
}

errno_t memcpy_s(void *destination, size_t destinationSize, const void *source, size_t count)
{
	if (destination == NULL)
	{
		return EINVAL;
	}
	if (source == NULL)
	{
		memset((void *)destination, '\0', destinationSize);
		return EINVAL;
	}
	if (destinationSize < count == 0)
	{
		memset((void *)destination, '\0', destinationSize);
		return ERANGE;
	}
	char * sp = (char *)source;
	char * dp = (char *)destination;
	while (count--)
	{
		*dp++ = *sp++;
	}
	return 0;
}

void * memset(void *destination, char val, size_t count)
{
    char *dest = destination;
    for(int i = 0; i < count; i++)
    {
        *dest++ = val;
    }
    return destination;
}

unsigned short * memsetw(unsigned short * destination, unsigned short val, size_t count)
{
    for(int i = 0; i < count; i++)
    {
        *destination++ = val;
    }
    return destination;
}