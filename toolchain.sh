#!/bin/bash

nasm -f elf -o loader.o loader.s
gcc -o kernel.o -c kernel_test.c -nostartfiles -nodefaultlibs -m32 -nostdlib -ffreestanding 
gcc -o calc.o -c calc1.c -nostartfiles -nodefaultlibs -m32 -nostdlib -ffreestanding
gcc -o page.o -c page.c -nostartfiles -nodefaultlibs -m32 -nostdlib -ffreestanding  
ld -T blink.ld -o kernel.bin loader.o kernel.o calc.o page.o -melf_i386 

qemu-system-x86_64 -kernel kernel.bin
