#include <stddef.h>
#include <stdint.h>

static uint32_t heap_top = 0x100000;

extern "C" void* malloc(size_t size)
{
    if (size == 0) return nullptr;


    size = (size + 7) & ~7;

    void* result = (void*)heap_top;
    heap_top += size;

    return result;
}
