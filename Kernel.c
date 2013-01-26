#define BLUE_TXT 0x01
#define TAB_WIDTH 5
#include <stdint.h>
#include <system.h>

// Message to print and the line to print it on.
unsigned int print(char *message, unsigned int line)
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i = 0;
	unsigned int j;
	
	i = (line*80*2);

	while(*message != 0)
	{
		if(*message == "\n") // check for a new line
		{
			line++;
			i = (line*80*2);
			message++;
		}
		else if(*message == "\t") // checks for tab
		{
			for( j = 1; j <= TAB_WIDTH; j++){ 
				vidmem[i] = " ";
				i++;
				vidmem[i] = BLUE_TXT;
				i++;
			}
			message++;
		} 
		else // for printing character on screen
		{
			vidmem[i] = *message;
			message++;
			i++;
			vidmem[i] = BLUE_TXT;
			i++;
		}
	}
	return(1);
}

void clear_screen() // clear the entire text screen
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i = 0;
	while(i < (80*25*2))
	{
		vidmem[i] = " ";
		i++;
		vidmem[i] = BLUE_TXT;
		i++;
	}
}

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count) // copies string of length count from src to dest
{
    unsigned int i;
    for(i=0;i<count;i++)
    	dest[i] = src[i];
    return dest;
}

unsigned char *memset(unsigned char *dest, unsigned char val, int count) // sets count locations starting at dest to val
{
   unsigned int i;
   for(i=0;i<count;i++)
   	dest[i] = val;
   return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count) // sets count locatsions starting at dest to val(16bit)
{
    /* Same as above, but this time, we're working with a 16-bit
    *  'val' and dest pointer. Your code can be an exact copy of
    *  the above, provided that your local variables if any, are
    *  unsigned short */
    unsigned int i;
    for(i=0;i<count;i++)
    	dest[i] = val;
    return dest;
}

int strlen(const char *str) // finds the length of a null (0) terminated string
{
    /* This loops through character array 'str', returning how
    *  many characters it needs to check before it finds a 0.
    *  In simple words, it returns the length in bytes of a string */
    int i=0;
    while(str[i] != "0")
    	i++;
    return i;
}

/* We will use this later on for reading from the I/O ports to get data
*  from devices such as the keyboard. We are using what is called
*  'inline assembly' in these routines to actually do the work */
unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void main()
{
  clear_screen();
  print("First Print to Screen,\nThe Bootloader should handover and print this\t or whatever\n",0);
  for (;;);
}


