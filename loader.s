[GLOBAL loader]                  ; Kernel entry point.
[EXTERN kmain]                   ; This is the entry point of our C code
global magic                            ; we will use this in kmain
;global mbd                              ; we will use this in kmain

 
extern kmain                            ; kmain is defined in kmain.cpp
global gdt_flush     ; Allows the C code to link to this
extern gp            ; Says that '_gp' is in another file 
global idt_load
extern idtp
;extern fault_handler
global shut_down
;extern irq_handler
extern end, code, bss

; The ISRs Follow
global isr0
global isr8
global isr14


;IRQs below

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15


MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; All instructions should be 32-bit.

[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.
mboot:
    ; Multiboot macros to make a few lines later more readable
    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    ; This is the GRUB Multiboot header. A boot signature
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
    ; AOUT kludge - must be physical addresses. Make a note of these:
    ; The linker script fills in the data for these ones!
    dd mboot
    dd code
    dd bss
    dd end
    dd loader
	


section .text
 
align 4
; reserve initial kernel stack space
STACKSIZE equ 0x4000                    ; that's 16k.
 
loader:
    mov  esp, stack + STACKSIZE         ; set up the stack
    mov  [magic], eax                   ; Multiboot magic number
    ;mov  [mbd], ebx                     ; Multiboot info structure
 	;call gdt_install
	cli    
	call kmain                         ; call kernel proper
 
    
.hang:
    hlt                                 ; halt machine should kernel return
    jmp  .hang
 

 

shut_down:
	hlt



gdt_flush:    
	lgdt [gp]        ; Load the GDT with our '_gp' which is a special pointer   
	;ltr [0x18]
	mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
    ret               ; Returns back to the C code!


idt_load:
    lidt [idtp]
    ret

;  ISR Entries and common stub

;  0: Divide By Zero Exception
isr0:
    cli
    push byte 0    ; A normal ISR stub that pops a dummy error code to keep a
                   ; uniform stack frame
    push byte 0
    jmp isr_common_stub

isr8:
	cli
	push byte 8
	jmp isr_common_stub

isr14:
	cli
	push byte 14
	jmp isr_common_stub


;IRQ 0
irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

;IRQ 1
irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub

;IRQ 2
irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub

;IRQ 3
irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub

;IRQ 4
irq4:
	cli
	push byte 0
	push byte 36
	jmp irq_common_stub

;IRQ 5
irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub

;IRQ 6
irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub

;IRQ 7
irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub

;IRQ 8
irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub

;IRQ 9
irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub

;IRQ 10
irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub

;IRQ 11
irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub

;IRQ 12
irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub

;IRQ 13
irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub

;IRQ 14
irq14:
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub

;IRQ 15
irq15:
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub

irq_common_stub: ;This is called by irq_handler from C

    extern irq_handler	
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret



; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    extern fault_handler
	pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!



section .bss
	
align 4
stack: resb STACKSIZE                   ; reserve 16k stack on a doubleword boundary
magic: resd 1
mbd:   resd 1





