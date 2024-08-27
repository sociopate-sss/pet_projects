#ifndef RGB_PIXEL_H
#define RGB_PIXEL_H

#include <stdint.h>

struct RGBPixel {

    RGBPixel() : B(0), G(0), R(0) {
    }

    RGBPixel(uint8_t b, uint8_t g, uint8_t r) : B(b), G(g), R(r) {
    }

    uint8_t B;
    uint8_t G;
    uint8_t R;
};

#endif