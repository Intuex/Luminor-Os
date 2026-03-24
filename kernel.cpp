#include <stdint.h>
#include "keyboard.h"

static uint16_t* vga = (uint16_t*)0xB8000;

static int cursor_x = 0;
static int cursor_y = 0;

char input[256];
int input_pos = 0;

extern "C" void putchar(char c)
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

extern "C" void print(const char* s)
{
    for (int i = 0; s[i]; i++)
        putchar(s[i]);
}

extern "C" void clear_screen()
{
    for (int i = 0; i < 80 * 25; i++)
        vga[i] = (0x07 << 8) | ' ';
    cursor_x = 0;
    cursor_y = 0;
}

extern "C" int strncmp(const char* a, const char* b, int n)
{
    for (int i = 0; i < n; i++)
        if (a[i] != b[i])
            return 1;
    return 0;
}

extern "C" void run_command(char* cmd)
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

    if (!strncmp(cmd, "clear", 5))
    {
        clear_screen();
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