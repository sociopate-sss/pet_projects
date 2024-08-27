#include "../include/grayscale_filter.h"
#include <stdint.h>

#include <stdexcept>

BasicFilter* Grayscale::CreateGrayscaleFilter(const FilterSettings& fs) {
    if (!fs.params.empty()) {
        throw std::logic_error("Bad gs params.");
    }

    return dynamic_cast<BasicFilter*>(new Grayscale(fs));
}

void Grayscale::Apply(Image& image) {
    for (size_t i = 0; i < image.ImageHeight(); ++i) {
        for (size_t j = 0; j < image.ImageWidth(); ++j) {
            RGBPixel pixel = image.GetPixel(i, j);
            double new_r = static_cast<double>(pixel.R) / static_cast<double>(UINT8_MAX) * R_GS_COEFF;
            double new_g = static_cast<double>(pixel.G) / static_cast<double>(UINT8_MAX) * G_GS_COEFF;
            double new_b = static_cast<double>(pixel.B) / static_cast<double>(UINT8_MAX) * B_GS_COEFF;
            uint8_t new_value =
                static_cast<uint8_t>(std::min(1.0, new_r + new_g + new_b) * static_cast<double>(UINT8_MAX));
            image.SetPixel(i, j, {new_value, new_value, new_value});
        }
    }
}