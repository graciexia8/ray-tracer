#ifndef COLOR_H
#define COLOR_H

#include "util.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color, int numOfSamples) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // because to the pixel color, we are adding the color of the total number of samples,
    // we need to divide by the num of samples to get the average color;
    auto scale = 1.0 / numOfSamples;
    r *= scale;
    g *= scale;
    b *= scale;

    interval intensity(0.000,0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}
#endif