#include "keyboard.h"
#include <stdint.h>

static int shift_pressed = 0;

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

static const char keymap_shift[128] = {
0,27,'!','@','#','$','%','^','&','*','(',')','_','+', '\b',
'\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
'Z','X','C','V','B','N','M','<','>','?',0,'*',0,' ',
};

extern "C" char keyboard_getchar()
{
    uint8_t sc;

    while (1)
    {
        if (inb(0x64) & 1)
        {
            sc = inb(0x60);

            if (sc == 0x2A || sc == 0x36) {
                shift_pressed = 1;
                continue;
            }

            if (sc == 0xAA || sc == 0xB6) {
                shift_pressed = 0;
                continue;
            }

            if (sc & 0x80)
                continue;

            if (sc < sizeof(keymap))
            {
                if (shift_pressed)
                    return keymap_shift[sc];
                else
                    return keymap[sc];
            }
        }
    }
}