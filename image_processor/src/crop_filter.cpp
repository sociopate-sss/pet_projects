#include "../include/crop_filter.h"

#include <stdexcept>

BasicFilter* Crop::CreateCropFilter(const FilterSettings& fs) {
    if (fs.params.size() != 2) {
        throw std::logic_error("Bad crop params.");
    }

    auto is_number = [&](const std::string& s) {
        return !s.empty() &&
               std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    };

    if (!is_number(fs.params[0]) || !is_number(fs.params[1])) {
        throw std::logic_error("Bad crop params.");
    }

    return dynamic_cast<BasicFilter*>(new Crop(fs));
}

void Crop::Apply(Image& image) {

    int32_t width = std::min(static_cast<int32_t>(image.ImageWidth()), static_cast<int32_t>(std::stoi(params_[0])));
    int32_t height = std::min(static_cast<int32_t>(image.ImageHeight()), static_cast<int32_t>(std::stoi(params_[1])));

    std::vector<std::vector<RGBPixel>> new_pixel_matrix(height, std::vector<RGBPixel>(width));

    for (size_t i = image.ImageHeight() - height; i < image.ImageHeight(); ++i) {
        for (size_t j = 0; j < width; ++j) {
            new_pixel_matrix[i - (image.ImageHeight() - height)][j] = image.GetPixel(i, j);
        }
    }

    image.SetImageHeight(height);
    image.SetImageWidth(width);

    for (size_t i = 0; i < static_cast<size_t>(height); ++i) {
        for (size_t j = 0; j < static_cast<size_t>(width); ++j) {
            image.SetPixel(i, j, new_pixel_matrix[i][j]);
        }
    }
}