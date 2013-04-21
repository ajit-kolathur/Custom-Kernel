#!/bin/bash

nasm -f elf -o loader.o loader.s
/usr/local/cross/bin/x86_64-elf-gcc -o kernel.o -c kernel_test.c -nostartfiles -nodefaultlibs -m32 -nostdlib -ffreestanding 
/usr/local/cross/bin/x86_64-elf-gcc -o calc.o -c calc1.c -nostartfiles -nodefaultlibs -m32 -nostdlib -ffreestanding
/usr/local/cross/bin/x86_64-elf-gcc -o page.o -c page.c -nostartfiles -nodefaultlibs -m32 -nostdlib -ffreestanding  
/usr/local/cross/bin/x86_64-elf-ld -T blink.ld -o kernel.bin loader.o kernel.o calc.o page.o -melf_i386 
sudo rm /tmp/qemu.log
qemu-system-x86_64 -kernel kernel.bin -d cpu_reset
