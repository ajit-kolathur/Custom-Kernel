#include<stdint.h>

#ifndef __headerfile_H
#define __headerfile_H

//extern char INPUT_BUFFER[200];
//extern int INPUT_BUFFER_PTR=0;
typedef unsigned int   u32int;

typedef struct page
{
    u32int present    : 1;   // Page present in memory
    u32int rw         : 1;   // Read-only if clear, readwrite if set
    u32int user       : 1;   // Supervisor level only if clear
    u32int accessed   : 1;   // Has the page been accessed since last refresh?
    u32int dirty      : 1;   // Has the page been written to since last refresh?
    u32int unused     : 7;   // Amalgamation of unused and reserved bits
    u32int frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    /**
       Array of pointers to pagetables.
    **/
    page_table_t *tables[1024];
    /**
       Array of pointers to the pagetables above, but gives their *physical*
       location, for loading into the CR3 register.
    **/
    u32int tablesPhysical[1024];

    /**
       The physical address of tablesPhysical. This comes into play
       when we get our kernel heap allocated and the directory
       may be in a different location in virtual memory.
    **/
    u32int physicalAddr;
} page_directory_t;


typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

//extern void irq_install_handler(int irq, void (*handler)(struct regs *r))
extern int calc( int );
extern int isNumber( char  );
extern int isChar( char  );
extern void postfix( char* );
extern int priority( char );
extern int eval( char* , int* ); 

extern int print(char *string,...);
extern short int itoa(short int number);
extern void putchar(char);
extern void update_cursor(int, int);
extern void systemCall(void);
extern int sysCallCompare();
extern void shutdown_power_off();
extern void clear_screen();
extern unsigned char *memcpy(unsigned char *, const unsigned char *, int);
extern void *memset(void *, unsigned char, int);
extern unsigned short *memsetw(unsigned short *, unsigned short, int);
extern int strlen(const char *);
extern unsigned char inportb(unsigned short);
extern void outportb (unsigned short , unsigned char );
//extern void keyboard_handler(struct regs *r)
#endif
