#include "../include/edge_filter.h"

#include <stdexcept>
#include <stdint.h>

BasicFilter* EdgeDetection::CreateEdgeDetectionFilter(const FilterSettings& fs) {
    if (fs.params.size() != 1) {
        throw std::logic_error("Bad edge params.");
    }

    double edge_param = 0;

    try {
        edge_param = std::stod(fs.params[0]);
    } catch (std::exception& e) {
        throw std::logic_error("Bad edge params.");
    }
    if (edge_param < 0.0 || edge_param > 1.0) {
        throw std::logic_error("Bad edge params.");
    }

    EdgeDetection* edge_ptr = new EdgeDetection(fs);

    return dynamic_cast<BasicFilter*>(edge_ptr);
}

void EdgeDetection::Apply(Image& image) {
    /// Перевод изображения в оттенки серого
    for (size_t i = 0; i < image.ImageHeight(); ++i) {
        for (size_t j = 0; j < image.ImageWidth(); ++j) {
            RGBPixel pixel = image.GetPixel(i, j);
            double new_r = static_cast<double>(pixel.R) / static_cast<double>(UINT8_MAX) * R_GS_COEFF;
            double new_g = static_cast<double>(pixel.G) / static_cast<double>(UINT8_MAX) * G_GS_COEFF;
            double new_b = static_cast<double>(pixel.B) / static_cast<double>(UINT8_MAX) * B_GS_COEFF;
            uint8_t new_value = static_cast<uint8_t>((new_r + new_g + new_b) * static_cast<double>(UINT8_MAX));
            image.SetPixel(i, j, {new_value, new_value, new_value});
        }
    }

    /// Применение матрицы
    int32_t filter_matrix[3][3] = {{0, -1, 0}, {-1, EDGE_CENTRAL_COEFF, -1}, {0, -1, 0}};
    ApplyMatrix(image, filter_matrix);

    /// Проверка по параметра threshold
    for (size_t i = 0; i < image.ImageHeight(); ++i) {
        for (size_t j = 0; j < image.ImageWidth(); ++j) {
            RGBPixel pixel = image.GetPixel(i, j);
            if (static_cast<double>(pixel.R) / static_cast<double>(UINT8_MAX) > std::stod(params_[0]) &&
                static_cast<double>(pixel.G) / static_cast<double>(UINT8_MAX) > std::stod(params_[0]) &&
                static_cast<double>(pixel.B) / static_cast<double>(UINT8_MAX) > std::stod(params_[0])) {
                image.SetPixel(i, j, {UINT8_MAX, UINT8_MAX, UINT8_MAX});
            } else {
                image.SetPixel(i, j, {0, 0, 0});
            }
        }
    }
}