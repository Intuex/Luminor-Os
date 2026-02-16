#include <stdint.h>

static uint16_t* vga = (uint16_t*)0xB8000;

static int cursor_x = 0;
static int cursor_y = 0;

char input[256];
int input_pos = 0;

int shift_pressed = 0;

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

static const char keymap_shift[128] = {
0,27,'!','@','#','$','%','^','&','*','(',')','_','+', '\b',
'\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
'Z','X','C','V','B','N','M','<','>','?',0,'*',0,' ',
};

char keyboard_getchar()
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

int strncmp(const char* a, const char* b, int n)
{
    for (int i = 0; i < n; i++)
        if (a[i] != b[i])
            return 1;
    return 0;
}

void run_command(char* cmd)
{
    if (!strncmp(cmd, "print(\"", 7))
    {
        char* text = cmd + 7;

        while (*text && *text != '"') {
            putchar(*text);
            text++;
        }

        putchar('\n');
        return;
    }

    print("Unknown command\n");
}

extern "C" void main()
{
    print("Luminor OS\n");
    print("> ");

    while (1)
    {
        char c = keyboard_getchar();
        if (!c) continue;

        if (c == '\n')
        {
            putchar('\n');
            input[input_pos] = 0;
            run_command(input);
            input_pos = 0;
            print("> ");
        }
        else if (c == '\b')
        {
            if (input_pos > 0)
            {
                input_pos--;
                putchar('\b');
            }
        }
        else
        {
            if (input_pos < sizeof(input) - 1)
            {
                input[input_pos++] = c;
                putchar(c);
            }
        }
    }
}
