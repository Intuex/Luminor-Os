section .bss
align 4096
page_directory:
    resd 1024

align 4096
page_table0:
    resd 1024

section .text
[bits 32]
[extern main]

global _start
_start:

    ; Map first page table
    mov eax, page_table0
    or eax, 3
    mov [page_directory], eax

    ; Fill identity mapping
    mov ecx, 0
    mov ebx, 0

.fill_table:
    mov eax, ebx
    or eax, 3
    mov [page_table0 + ecx*4], eax

    add ebx, 4096
    inc ecx
    cmp ecx, 1024
    jne .fill_table

    ; Enable paging
    mov eax, page_directory
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    call main
    jmp $
