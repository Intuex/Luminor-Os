#include <stdint.h>

struct limine_framebuffer {
    void* address;
    uint64_t width, height, pitch;
    uint16_t bpp;
    uint8_t memory_model;
    uint8_t red_mask_size,   red_mask_shift;
    uint8_t green_mask_size, green_mask_shift;
    uint8_t blue_mask_size,  blue_mask_shift;
};

struct limine_framebuffer_response {
    uint64_t revision;
    uint64_t count;
    limine_framebuffer** framebuffers;
};

struct limine_framebuffer_request {
    uint64_t id[4];
    uint64_t revision;
    limine_framebuffer_response* response;
};

__attribute__((used, section(".requests_start_marker")))
static volatile uint64_t limine_requests_start_marker[2] = {
    0xf6b8f4b39de7d1ae, 0xfab91a6940fcb9cf
};

__attribute__((used, section(".requests")))
static volatile uint64_t limine_base_revision[3] = {
    0xf9562b2d5c95a6c8, 0x6a7b384944536bdc, 2
};

__attribute__((used, section(".requests")))
static limine_framebuffer_request fb_request = {
    { 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b,
      0x9d5827dcd881dd75, 0xa3148604f6fab11b },
    0, nullptr
};

__attribute__((used, section(".requests_end_marker")))
static volatile uint64_t limine_requests_end_marker[2] = {
    0x564f6b6e51adf4a3, 0x2605444afa5a2908
};



extern "C" void _start()
{


    while (1)
    {}
}
