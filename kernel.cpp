#include <stdint.h>

static uint16_t* vga = (uint16_t*)0xB8000;

static int cursor_x = 0;
static int cursor_y = 0;

void putchar(char c)
{
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }

    if (c == '\b') {
        if (cursor_x > 0) cursor_x--;
        vga[cursor_y * 80 + cursor_x] = (0x07 << 8) | ' ';
        return;
    }

    vga[cursor_y * 80 + cursor_x] = (0x07 << 8) | c;
    cursor_x++;

    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
}

void print(const char* s)
{
    for (int i = 0; s[i]; i++)
        putchar(s[i]);
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t r;
    asm volatile("inb %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}

static const char keymap[128] = {
0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
'\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
};

char keyboard_getchar()
{
    uint8_t sc;

    while (1)
    {
        if (inb(0x64) & 1)
        {
            sc = inb(0x60);

            if (!(sc & 0x80))
            {
                if (sc < sizeof(keymap))
                    return keymap[sc];
            }
        }
    }
}

extern "C" void main()
{
    print("Luminor OS\n");

    while (1)
    {
        char c = keyboard_getchar();
        if (c)
            putchar(c);
    }
}
