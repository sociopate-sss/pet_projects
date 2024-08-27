#include "../include/image.h"

void Image::SetImageHeight(int32_t height) {
    height_ = height;
    pixel_matrix_.resize(height_);
}

void Image::SetImageWidth(int32_t width) {
    width_ = width;
    for (size_t i = 0; i < static_cast<size_t>(height_); ++i) {
        pixel_matrix_[i].resize(width_);
    }
}