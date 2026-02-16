extern "C" void* malloc(unsigned int);

extern "C" void main() {
    char* vga = (char*)0xb8000;

    char* mem = (char*)malloc(6);
    mem[0] = 'H';
    mem[1] = 'i';
    mem[2] = 0;

    for (int i = 0; mem[i]; i++) {
        vga[i * 2] = mem[i];
        vga[i * 2 + 1] = 0x07;
    }

    while (1);
}
