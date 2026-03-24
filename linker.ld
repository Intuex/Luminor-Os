OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS {
    . = 0xffffffff80000000;

    .requests_start_marker : {
        KEEP(*(.requests_start_marker))
    }

    .requests : {
        KEEP(*(.requests))
    }

    .requests_end_marker : {
        KEEP(*(.requests_end_marker))
    }

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss*)
    }
}