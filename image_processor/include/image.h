#ifndef IMAGE_H
#define IMAGE_H

#include "rgb_pixel.h"

#include <vector>

class Image {
public:
    Image() {
    }

    Image(int32_t width, int32_t height, std::vector<std::vector<RGBPixel>> pixel_matrix)
        : width_(width), height_(height), pixel_matrix_(pixel_matrix) {
    }

    const RGBPixel& GetPixel(size_t row, size_t col) const {
        return pixel_matrix_[row][col];
    }

    void SetPixel(size_t row, size_t col, const RGBPixel& value) {
        pixel_matrix_[row][col] = value;
    }

    size_t ImageWidth() const {
        return static_cast<size_t>(width_);
    }

    void SetImageWidth(int32_t width);

    size_t ImageHeight() const {
        return static_cast<size_t>(height_);
    }

    void SetImageHeight(int32_t height);

protected:
    int32_t width_;
    int32_t height_;

    std::vector<std::vector<RGBPixel>> pixel_matrix_;
};

#endif