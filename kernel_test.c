#define BLUE_TXT 0x0F
#define TAB_WIDTH 5
#include<stdint.h>
#include<stdarg.h>
#include"headerfile.h" 
#define FLOAT_PRECISION 4
#define CPUSPEED 1000000

int screenX=0;
int screenY=0;
int INPUT_BUFFER_PTR=0;
char INPUT_BUFFER[200];
//extern unsigned int end;
extern unsigned int placement_address;
extern u32int mem_end_page;
extern u32int test_frame(u32int );
//int calc( int index);
//int isNumber( char c );
//int isChar( char c );
//void postfix( char *infix );
//int priority( char c );
//int eval( char *exp, int *numbers); 

void double_fault(struct regs *);

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		






int print(char *string,...);
short int itoa(short int number);
void putchar(char);
void update_cursor(int, int);
void systemCall(void);
int sysCallCompare();
void shutdown_power_off();
void clear_screen();
unsigned char *memcpy(unsigned char *, const unsigned char *, int);
void *memset(void *, unsigned char, int);
unsigned short *memsetw(unsigned short *, unsigned short, int);
int strlen(const char *);
unsigned char inportb(unsigned short);
void outportb (unsigned short , unsigned char );


//26th January, 1:27 AM.
//Begin: Sankalp Singayapally

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
	unsigned char *dpCopy=(unsigned char *)dest;
	while(count!=0)
	{
		*dpCopy=*src;
		*dpCopy++;
		*src++;
		count--;
	}

	return dest;
}

void *memset(void *dest, unsigned char val, int count)
{
	unsigned char *dpCopy=(unsigned char *)dest;
	while(count!=0)
	{
		*dpCopy=val;
		*dpCopy++;
		count--;
	}

	return (void*)dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
	unsigned short *dpCopy=(unsigned short *)dest;
	while(count!=0)
	{
		*dpCopy=val;
		*dpCopy++;
		count--;
	}

	return dest;
		
}

int strlen(const char *str) //returns the length of string
{
	int len=0;
	while(*str!='\0')
	{
		str++;
		len++;
	}
	
	return len;
}


//End Jan 26th, 2:14 AM

//Begin: From OSDever Bran's Kernel Tut. 

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

//End edit: Jan 26th


//Begin edit: Sankalp, Ajit, 29th January, 4:43PM
//Implementing GDT

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr gp;

extern void gdt_flush();

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_install()
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (int)&gdt;

    /* Our NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    *  Please check the table above in the tutorial in order
    *  to see exactly what each value means */
   gdt_set_gate(1, 0x00000000, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
   gdt_set_gate(2, 0x00000000, 0xFFFFFFFF, 0x92, 0xCF);
  //gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
  //gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);		
    //TASK STATE SEGMENT -1 
    //TASK STATE SEGMENT -2	

    //gdt_set_gate(3, 0, 0xFFFFFFFF, 0x89, 0xCF);
   // gdt_set_gate(4, 0, 0xFFFFFFFF, 0x89, 0xCF);	
	 	
	//gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    
   //gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);




    /* Flush out the old GDT and install the new changes! */
    gdt_flush();
}

//End changes: 29th January



//Begin: Sankalp and Ajit, 6th February
//Implementing IDT
//Sticking to flat memory model for now
/* Defines an IDT entry */

/* This defines what the stack looks like after an ISR was running */


struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
} __attribute__((packed));


struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

/* This exists in 'start.asm', and is used to load our IDT */
extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    /* We'll leave you to try and code this function: take the
    *  argument 'base' and split it up into a high and low 16-bits,
    *  storing them in idt[num].base_hi and base_lo. The rest of the
    *  fields that you must set in idt[num] are fairly self-
    *  explanatory when it comes to setup */

	/* Setup the descriptor base address */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    /* Setup the Selector */
    idt[num].sel = sel;
	idt[num].always0 = 0;

    /* Finally, set up the access flags */
    idt[num].flags = flags;
	
}

/* Installs the IDT */
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (int)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset((void*)&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points the processor's internal register to the new IDT */
    idt_load();
}

/* These are function prototypes for all of the exception
*  handlers: The first 32 entries in the IDT are reserved
*  by Intel, and are designed to service exceptions! */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();



void isrs_install()
{
    	idt_set_gate(0, (unsigned)isr0, 0x08, 0x8E);
	idt_set_gate(8, (unsigned)isr8, 0x08, 0x8E); //double fault
	idt_set_gate(14,(unsigned)isr14, 0x08, 0x8E);//page fault
}

unsigned char *exception_messages[] ={ "Division By Zero", 0,0,0,0,0,0,0,"Double Fault",0,0,0,0,0,"Page Fault"};

void fault_handler(struct regs *r)
{
    /* Is this a fault whose number is from 0 to 31? */
   if (r->int_no < 32)
    {
        /* Display the description for the Exception that occurred.
        *  In this tutorial, we will simply halt the system using an
        *  infinite loop */
        //print(exception_messages[r->int_no]);


	if(r->int_no == 8)
	{
		double_fault(r);
	}	

	if(r->int_no == 14)
	{
		page_fault(r);
	}	
		//print("Some exception\n");
		//print("New line :)\\n");
        //print("@s", exception_messages[r->int_no]);
		//clear_screen();
		//return;
        shut_down();
    }
}
//End: 6thnd February, IDT complete

//Begin Changes: 11th February, IRQ Handling; Sankalp
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();



void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}


void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

void irq_install()
{
    irq_remap();

	idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)irq3, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
    	idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

void irq_handler(struct regs *r)
{
    /* This is a blank function pointer */
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);
	return;
}
/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;


void timer_phase(int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outportb(0x43, 0x36);             /* Set our command byte 0x36 */
    outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
	print("\nUser called timer function", 6);    
	timer_ticks++;
	/* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
	//if (timer_ticks % 18 == 0)
    	//{
        	print("One second has passed\n", 5);
    	//}
	return;
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
	    
	irq_install_handler(0, timer_handler);
	//timer_phase(1);
	return;
}




void keyboard_handler(struct regs *r)
{
    unsigned char scancode;
    //print("Hello hello", line);
	//line++;	
    /* Read from the keyboard's data buffer */
    scancode = inportb(0x60);
	//print("@i\t", scancode);
    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
   if (scancode & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
    }
    else //This code for displaying the keypress.
    {
    	if(kbdus[scancode]!='\n')
		{
			INPUT_BUFFER[INPUT_BUFFER_PTR]=kbdus[scancode];
			INPUT_BUFFER_PTR++;
			//print("@i\\n", scancode);	    
			putchar(kbdus[scancode]);
		}
	
		else
		{			
			INPUT_BUFFER[INPUT_BUFFER_PTR]='\0';
				if(INPUT_BUFFER_PTR!=0)
					systemCall();
			else
			{					
				print("\\nshell\\@MoOS>>");				
				return;
			}
    	}
	}
}


void systemCall()
{
	
	int sysCallCode;
	sysCallCode=sysCallCompare();
	
	switch (sysCallCode)
	{
		case 0:
			print("\\nInvalid function call...\\n");
			print("shell\\@MoOS>>");
			break;
	
		case 1:
			clear_screen();
			break;	
		case 2:
			shutdown_power_off();	
			break;	
		case 3:
			print("\\nshell\\@MoOS>>");	
			break;
		case 4:
			//print("\\nDone calc\\n");
			INPUT_BUFFER_PTR=0;
			print("\\nshell\\@MoOS>>");
			break;
		case 5:
			INPUT_BUFFER_PTR=0;
			memstat();
			print("shell\\@MoOS>>");
		case 6:
			raisePageFault();
		default:
			break;	
	}	
		
		
}


int sysCallCompare()
{
	char clear[]="clear"; //declare other keywords here...
	char shutdown[]="shutdown";
	char echo[]="tellme";
	char calc1[]="calc";
	char mem_stat[]="memstat";
	char pagefaulter[]="pagefault";
	int i=sysCallExtract();
	
	//returning based on a string comparison
	
	if (stringCompare(i, clear)==1)
	{		
			
		INPUT_BUFFER_PTR=0;
		return 1;
	}	
	if (stringCompare(i, shutdown)==1)
	{
		
		INPUT_BUFFER_PTR=0;		
		return 2;
	}
	if (stringCompare(i, echo)==1)
	{
		//INPUT_BUFFER_PTR=0;
		//print ("i is @i\\n", i);		
		print("\\n");
		print(&INPUT_BUFFER[i+1]);
		INPUT_BUFFER_PTR=0;	
		return 3;
	}
	if (stringCompare(i, calc1)==1)
	{
		//print("\\nCalculating now...\\n");
		//print ("\\ni is @i\\n", i);
		i=i++;
		//print ("\\ni is @i\\n", i);		
		calc(i);
		return 4;
	}
	if (stringCompare(i, mem_stat)==1)
	{
		return 5;;
	}
	if (stringCompare(i, pagefaulter)==1)
	{
		return 6;
	}
	else 
	{
		INPUT_BUFFER_PTR=0;		
		return 0;
	}	
}


int sysCallExtract()
{
	int i=0;
	while(i<INPUT_BUFFER_PTR)
	{
		if(INPUT_BUFFER[i]==' ')
			{
				INPUT_BUFFER[i]='\0';
				i++;
				break; //function name has been found
			}
		else
			i++;
	}	
	//function extraction code over here
	//print("@i", INPUT_BUFFER_PTR);
	
	/*int j=i;
	while(j<INPUT_BUFFER_PTR-1)
	{
		if(INPUT_BUFFER==' ')
			{
				INPUT_BUFFER[i]='\0';
				j++;
			}
		else
			j++;
	}*/		
	//print("\\ni is now @i\\n", i);
	return i-1;
	
}




int stringCompare(int strLen, char func[])
{
	int i=0;
	int k=0;
	while(INPUT_BUFFER[i]!='\0')
	{
		if (INPUT_BUFFER[i]==func[i])
		{
			k=1;
			i++;
		}

		if(INPUT_BUFFER[i]!=func[i])
		{
			k=0;
			break;
		}
	}

	return k;
}
	

void double_fault(struct regs *r)
{
	print("Double Fault too. We aren't playing tennis anymore :P \\n");
}


void keyboard_install()
{
	irq_install_handler(1, keyboard_handler);
	irq_install_handler(8, double_fault);
}

void putchar(char received)
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	
	i=(screenY*80*2)+screenX;
	switch(received)
	{
		case '\b':
			
			if(screenY==0 && screenX==24)
				break;

			i=i-2;
			vidmem[i++]=' ';
			vidmem[i++]=BLUE_TXT;			
			screenX-=2;
			INPUT_BUFFER_PTR--;
			INPUT_BUFFER_PTR--;					
			break;
		case '\n':
			screenY++;
			screenX=0;
			print("shell\\@MoOS>>");
		default:
			vidmem[i++]=received;
			vidmem[i++]=BLUE_TXT;			
			screenX+=2;
			break;
			
	}			
				

	
	/*vidmem[i] = received;
	screenX++;

	
	i=(screenY*80*2)+screenX;
	vidmem[i] = BLUE_TXT;
	screenX++;*/
	
	
	if(screenX>=160)
	{
		screenX=screenX%160;
		screenY++;
	}
	
	update_cursor(screenY, screenX/2);

	return;
	
	
}


short int itoa( short int num){
	return num + '0';
}
int print(char *string, ...){
	char *ptr = string;
	int i,t_int;
	short int top = 0;
	float t_float;
	unsigned int t_uint;
	double t_double;
	char *t_string, t_char, stack[100];
	void *t_pointer;

	va_list list;
	while(*ptr != '\0'){
		switch(*ptr){
			case '\\':
				ptr++;
				switch(*ptr){
//					\a – Bell (beep)
					case 'a': 
					// dummy_call_alert();
						break;
//					\b – Backspace
					case 'b':
						screenX--;
						putchar(' ');
						break;
//					\n – New line
					case 'n':
						screenX = 0;
						screenY++;
						break;
//					\r – Carriage return
					case 'r':
						screenX = 0;
						break;
//					\t – Horizontal tab
					case 't':
						for(i=0; i<TAB_WIDTH; i++)
							putchar(' ');
						break;
//					\\ – Backslash
					case '\\':
						putchar('\\');
						break;
//					\' – Single quotation mark
					case '\'':
						putchar('\'');
						break;
//					\" – Double quotation mark
					case '\"':
						putchar('\"');
						break;
//					\@ - Excape Character for @
					case '@':
						putchar('@');
						break;
					default:
						putchar('\\');
						putchar(*ptr);
						break;
				}
				ptr++;
				break;
			case '@':
				va_start ( list, string );
				ptr++;
				switch(*ptr){
//					@i - Integer
					case 'i':
						t_int = va_arg( list, int );
						if(t_int < 0){
							putchar('-');
							t_int = 0 - t_int;
						}
						print("@u",(unsigned int)t_int);
						break;
//					@u - Unsigned Integer
					case 'u':
						top = 0;
						t_uint = va_arg( list, unsigned int );
						if(t_uint == 0){
							putchar('0');
							break;
						}
						while(t_uint>0){
							stack[top++] = itoa((short int)t_uint%10);
							t_uint /= 10;
						}
						for(i=top-1;i>=0;i--)
							putchar(stack[i]);
						break;
//					@f - Float 
					case 'f':
						t_float = va_arg( list, double );
						if(t_float < 0){
							putchar('-');
							t_float = 0 - t_float;
						}
						t_int = (int)t_float;
						t_float = t_float - t_int;
						print("@u",(unsigned int)t_int);
						putchar('.');
						for(i=0;i<FLOAT_PRECISION;i++)
							t_float *=10;
						print("@u",(unsigned int)t_float);
						break;
					default:
						putchar(*ptr);
						//ptr++;
						break;
//					@d - Double

//					@s - String

//					@c - char

//					@p - pointer

				}
				ptr++;
				break;

			default:
				putchar(*ptr);
				ptr++;
				break;
		}
	}
}



//End edit: 11th Februrary 2013

void clear_screen() // clear the entire text screen
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i = 0;
	while(i < (80*25*2))
	{
		vidmem[i] = 0;
		i++;
		vidmem[i] = BLUE_TXT;
		i++;
	}
	screenX=0;
	screenY=0;
	print("shell\\@MoOS>>");
}

void shutdown_power_off (void)
{
  const char s[] = "Shutdown";
  const char *p;
  print ("Powering off...\n");
  //serial_flush ();

  /* This is a special power-off sequence supported by Bochs and
     QEMU, but not by physical hardware. */
  for (p = s; *p != '\0'; p++)
    outportb (0x8900, *p);

  /* This will power off a VMware VM */
  __asm__ __volatile__ ("cli; hlt" : : : "memory");

  /* None of those worked. */
  print ("still running...\n");
  for (;;);
}


void update_cursor(int row, int col)
{
   unsigned short position=(row*80) + col;

   // cursor LOW port to vga INDEX register
   outportb(0x3D4, 0x0F);
   outportb(0x3D5, (unsigned char)(position&0xFF));
   // cursor HIGH port to vga INDEX register
   outportb(0x3D4, 0x0E);
   outportb(0x3D5, (unsigned char )((position>>8)&0xFF));
	return;
}

void memstat(void)
{
	u32int i; //for loop variable
	print("\\n");
	for (i=0x00100000;i<mem_end_page; i+=0x3000)
	{
		if ((test_frame(i) || test_frame (i+0x1000) || test_frame (i+0x2000))==1)
			print("#");
		else
			print("*");
	}
	print("\\n");
	return;
}
	
void raisePageFault(void)
{
	unsigned int *ptr = 0xFFFFFFF0;
	print("\\nAccessing memory location: 0xFFFFFFF0\\n");
	int i= *ptr;
}

void kmain()
{
   	extern unsigned int end;
	
	clear_screen();
	gdt_install();	
	idt_install();
	isrs_install();
	irq_install();
		
	initialise_paging();
	print("Wait. Setting up paging. For your benefit.\\n");
	sleep(100);	
	//print("Paging done...\\n");
	
	clear_screen();
	keyboard_install();
	asm volatile("sti");
	//asm volatile ("int $0x0E");
	//u32int *ptr = 0xFFFFFFFF, r = *(ptr);
	//timer_install();	
	extern uint32_t magic;
	
	
   /* Uncomment the following if you want to be able to access the multiboot header */
   /* extern void *mbd; */
 
   if ( magic != 0x2BADB002 )
   {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */
      print("Something Wrong\n");
   }
	//memstat();
	//print("Bleh.\\n");	
	for(;;);

	
}




void sleep(long int i){
/* i is in milliseconds */
	i = i * CPUSPEED;
	for(;i>0;i--);
	return;
}

void typechar(char *input){
/* takes a NULL terminated string and prints it to
 * screen as if it were typed, just need to check if
 * 50 milli seconds is good enough and also if long int
 * is good enough for the input datatype
 */ 
	while( *input != '\0' ){
		if(*input!='\n'){
			INPUT_BUFFER[INPUT_BUFFER_PTR]=*input;
			INPUT_BUFFER_PTR++;
			putchar(*input);
		}
			
		else{			
			INPUT_BUFFER[INPUT_BUFFER_PTR]='\0';
			if(INPUT_BUFFER_PTR!=0)
				systemCall();
			else{					
				print("\\nshell\\@MoOS>>");				
				return;
			}
		}
		input++;
		sleep((long int)50);
		//return;
	}
return;
}


