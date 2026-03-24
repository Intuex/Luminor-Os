#!/bin/bash
set -e

export PATH=$PATH:/usr/local/i386elfgcc/bin

mkdir -p Binaries

echo "Step:1 Assembling bootloader"
nasm boot.asm -f bin -o Binaries/boot.bin

echo "Stap:2 Assembling kernel entry"
nasm kernel_entry.asm -f elf -o Binaries/kernel_entry.o

echo "step:3 Compiling kernel"
i386-elf-gcc -ffreestanding -m32 -g -c kernel.cpp -o Binaries/kernel.o

echo "step4 Compiling malloc"
i386-elf-gcc -ffreestanding -m32 -g -c malloc.cpp -o Binaries/malloc.o

echo "step5 Linking kernel"
i386-elf-ld -o Binaries/full_kernel.bin -Ttext 0x1000 \
Binaries/kernel_entry.o \
Binaries/kernel.o \
Binaries/malloc.o \
--oformat binary

echo "step6 Building OS image"
nasm zeroes.asm -f bin -o Binaries/zeroes.bin
cat Binaries/boot.bin Binaries/full_kernel.bin Binaries/zeroes.bin > Binaries/OS.bin

echo "Running QEMU"
qemu-system-x86_64 -drive format=raw,file=Binaries/OS.bin,index=0,if=floppy -m 128M
