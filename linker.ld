OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS {
    . = 0xffffffff80000000;

    .requests : {
        KEEP(*(.requests))
    }

    .text    : { *(.text*) }
    .rodata  : { *(.rodata*) }
    .data    : { *(.data*) }
    .bss     : { *(.bss*) }
}