#!/bin/bash
set -e

mkdir -p iso/boot/limine
mkdir -p iso/EFI/BOOT
mkdir -p Binaries

if [ ! -d "limine" ]; then
    git clone https://github.com/limine-bootloader/limine.git \
        --branch=v8.x-binary --depth=1
fi

if [ ! -f "limine/limine" ]; then
    make -C limine
fi

echo "Step 1: Compiling kernel"
x86_64-elf-g++ -ffreestanding -fno-exceptions -fno-rtti \
    -mcmodel=kernel -mno-red-zone \
    -c kernel.cpp -o Binaries/kernel.o

echo "Step 2: Compiling keyboard"
x86_64-elf-g++ -ffreestanding -fno-exceptions -fno-rtti \
    -mcmodel=kernel -mno-red-zone \
    -c keyboard.cpp -o Binaries/keyboard.o

echo "Step 3: Compiling malloc"
x86_64-elf-g++ -ffreestanding -fno-exceptions -fno-rtti \
    -mcmodel=kernel -mno-red-zone \
    -c malloc.cpp -o Binaries/malloc.o

echo "Step 4: Linking"
x86_64-elf-ld -T linker.ld \
    Binaries/kernel.o Binaries/keyboard.o Binaries/malloc.o \
    -o iso/boot/limine/luminor.elf

echo "Step 5: Building ISO"
cp limine/limine-bios.sys      iso/boot/limine/
cp limine/limine-bios-cd.bin   iso/boot/limine/
cp limine/limine-uefi-cd.bin   iso/boot/limine/
cp limine/BOOTX64.EFI          iso/EFI/BOOT/
cp limine.conf                 iso/boot/limine/limine.conf

xorriso -as mkisofs \
    -b boot/limine/limine-bios-cd.bin \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    --efi-boot boot/limine/limine-uefi-cd.bin \
    -efi-boot-part --efi-boot-image \
    -o Binaries/luminor.iso iso/

./limine/limine bios-install Binaries/luminor.iso

echo "Running QEMU"
qemu-system-x86_64 -cdrom Binaries/luminor.iso -m 128M