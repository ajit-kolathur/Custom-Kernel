Custom-Kernel
=============

Building a kernel from scratch, using C and minimal assembly.
The intension is to actually implement manually those things that people currently take for granted,
a silly expedition to re-invent the wheel and a fun way to actually put course knowledge to use,
inspired by the great Linus torwalds and lots of information from osdev.org and brans kernel tutorials
for the basic data structures, this current version of the uploaded kernel is capable of paging,
keyboard input, a full text console and also features a basic shell. Currently working on implementing a simple
round robin scheduling algorithm.

uses a GCC tool chain, with standard LD and QEMU for emulation, a basic compilation bash script is given along side it
