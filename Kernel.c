#define BLUE_TXT 0x01
#define TAB_WIDTH 5
#include <stdint.h>

// Message to print and the line to print it on.
unsigned int print(char *message, unsigned int line)
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i = 0;
	unsigned int j;
	
	i = (line*80*2);

	while(*message != 0)
	{
		if(*message == '\n') // check for a new line
		{
			line++;
			i = (line*80*2);
			*message++;
		}
		else if(*message == '\t'){
			for( j = 1; j <= TAB_WIDTH; j++){ 
				vidmem[i] = ' ';
				i++;
				vidmem[i] = BLUE_TXT;
				i++;
			}
		} 
		else {
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
		vidmem[i] = ' ';
		i++;
		vidmem[i] = BLUE_TXT;
		i++;
	}
}
main()
{
  // extern uint32_t magic;
 
   /* Uncomment the following if you want to be able to access the multiboot header */
   /* extern void *mbd; */
 
  /* if ( magic != 0x2BADB002 )
   {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */
  // }
  
  clear_screen();
  print("lets bounce bitchs\n\tI need food YO!",0);
  return;
}


