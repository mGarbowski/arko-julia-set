#ifndef GENERATE_JULIA_SET_H
#define GENERATE_JULIA_SET_H

void generateJuliaSet(
        uint8_t *pixels,
        int width,
        int height,
        double escapeRadius,
        double cReal,
        double cImag,
        double centerReal,
        double centerImag,
        double zoom
);

#endif //GENERATE_JULIA_SET_H