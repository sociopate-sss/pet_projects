#include "../include/negative_filter.h"

#include <stdexcept>

BasicFilter* Negative::CreateNegativeFilter(const FilterSettings& fs) {
    if (!fs.params.empty()) {
        throw std::logic_error("Bad neg params.");
    }

    return dynamic_cast<BasicFilter*>(new Negative(fs));
}

void Negative::Apply(Image& image) {
    for (size_t i = 0; i < image.ImageHeight(); ++i) {
        for (size_t j = 0; j < image.ImageWidth(); ++j) {
            RGBPixel pixel = image.GetPixel(i, j);
            image.SetPixel(i, j,
                           {static_cast<uint8_t>(UINT8_MAX - pixel.B), static_cast<uint8_t>(UINT8_MAX - pixel.G),
                            static_cast<uint8_t>(UINT8_MAX - pixel.R)});
        }
    }
}