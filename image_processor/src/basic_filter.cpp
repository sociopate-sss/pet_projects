#include "../include/basic_filter.h"
#include <stdint.h>

void BasicFilter::ApplyMatrix(Image& image, const int32_t filter_matrix[3][3]) {

    // Выеделение памяти
    int32_t*** previous_color_values = new int32_t**[3];

    for (size_t color = 0; color < 3; ++color) {
        previous_color_values[color] = new int32_t*[image.ImageHeight()];
        for (size_t i = 0; i < image.ImageHeight(); ++i) {
            previous_color_values[color][i] = new int32_t[image.ImageWidth()];
        }
    }

    for (size_t i = 0; i < image.ImageHeight(); ++i) {
        for (size_t j = 0; j < image.ImageWidth(); ++j) {
            RGBPixel pixel = image.GetPixel(i, j);
            previous_color_values[0][i][j] = static_cast<int32_t>(pixel.B);
            previous_color_values[1][i][j] = static_cast<int32_t>(pixel.G);
            previous_color_values[2][i][j] = static_cast<int32_t>(pixel.R);
        }
    }

    // Применение матрицы

    /// Нахождение ближайшего пикселя
    auto nearest_pixel = [&](int32_t color, int32_t i, int32_t j) {
        int32_t width = static_cast<int32_t>(image.ImageWidth());
        int32_t height = static_cast<int32_t>(image.ImageHeight());

        if (i >= height && j >= width) {
            return previous_color_values[color][i - 1][j - 1];
        } else if (i >= height && j < 0) {
            return previous_color_values[color][i - 1][j + 1];
        } else if (i < 0 && j >= width) {
            return previous_color_values[color][i + 1][j - 1];
        } else if (i < 0 && j < 0) {
            return previous_color_values[color][i + 1][j + 1];
        } else if (i < 0) {
            return previous_color_values[color][i + 1][j];
        } else if (i >= height) {
            return previous_color_values[color][i - 1][j];
        } else if (j < 0) {
            return previous_color_values[color][i][j + 1];
        } else if (j >= width) {
            return previous_color_values[color][i][j - 1];
        }

        return previous_color_values[color][i][j];
    };

    /// Применение матрицы для определенного цвета
    auto apply_for_color = [&](int32_t color, size_t i, size_t j) {
        int64_t matrix_sum = 0;

        int32_t matrix_central_i = 1;
        int32_t matrix_central_j = 1;

        for (int32_t delta_i = -1; delta_i <= 1; ++delta_i) {
            for (int32_t delta_j = -1; delta_j <= 1; ++delta_j) {
                int32_t i1 = static_cast<int32_t>(i) + delta_i;
                int32_t j1 = static_cast<int32_t>(j) + delta_j;
                matrix_sum +=
                    static_cast<int64_t>(filter_matrix[matrix_central_i + delta_i][matrix_central_j + delta_j]) *
                    static_cast<int64_t>(nearest_pixel(color, i1, j1));
            }
        }

        return static_cast<uint8_t>(
            std::min(static_cast<int64_t>(UINT8_MAX), std::max(static_cast<int64_t>(0), matrix_sum)));
    };

    for (size_t i = 0; i < image.ImageHeight(); ++i) {
        for (size_t j = 0; j < image.ImageWidth(); ++j) {
            image.SetPixel(i, j, {apply_for_color(0, i, j), apply_for_color(1, i, j), apply_for_color(2, i, j)});
        }
    }

    // Освобождение памяти
    for (size_t color = 0; color < 3; ++color) {
        for (size_t i = 0; i < image.ImageHeight(); ++i) {
            delete[] previous_color_values[color][i];
        }
    }

    for (size_t color = 0; color < 3; ++color) {
        delete[] previous_color_values[color];
    }

    delete[] previous_color_values;
}