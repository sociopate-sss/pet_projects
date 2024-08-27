#include "../include/blur_filter.h"

#include <cmath>
#include <stdexcept>

BasicFilter* Blur::CreateBlurFilter(const FilterSettings& fs) {
    if (fs.params.size() != 1) {
        throw std::logic_error("Bad blur params.");
    }

    try {
        std::stod(fs.params[0]);
    } catch (std::exception& e) {
        throw std::logic_error("Bad edge params.");
    }

    Blur* blur_ptr = new Blur(fs);

    return dynamic_cast<BasicFilter*>(blur_ptr);
}

void StdToBox(int boxes[], double sigma, int n) {

    const static double BOX_FACTOR = 12.0;

    double w_i = std::sqrt((BOX_FACTOR * sigma * sigma / static_cast<double>(n)) + 1);
    int32_t w_l = std::floor(w_i);

    if (w_l % 2 == 0) {
        --w_l;
    }
    int32_t w_u = w_l + 2;

    double m_i =
        (BOX_FACTOR * sigma * sigma - n * w_l * w_l - static_cast<double>(4) * n * w_l - static_cast<double>(3) * n) /
        (static_cast<double>(-4) * w_l - 4);
    int32_t m = static_cast<int32_t>(std::round(m_i));

    for (int i = 0; i < n; i++) {
        boxes[i] = ((i < m ? w_l : w_u) - 1) / 2;
    }
}

void HorBlur(double* in, double* out, int w, int h, int r) {

    double i_arr = static_cast<double>(1) / static_cast<double>(r + r + 1);

    for (int32_t i = 0; i < h; i++) {

        int32_t t_i = i * w;
        int32_t l_i = t_i;
        int32_t r_i = t_i + r;

        double f_v = in[t_i];
        double l_v = in[t_i + w - 1];
        double val = static_cast<double>(r + 1) * f_v;

        for (int32_t j = 0; j < r; j++) {
            val += in[t_i + j];
        }
        for (int32_t j = 0; j <= r; j++) {
            val += in[r_i++] - f_v;
            out[t_i++] = val * i_arr;
        }
        for (int32_t j = r + 1; j < w - r; j++) {
            val += in[r_i++] - in[l_i++];
            out[t_i++] = val * i_arr;
        }
        for (int32_t j = w - r; j < w; j++) {
            val += l_v - in[l_i++];
            out[t_i++] = val * i_arr;
        }
    }
}

void TotalBlur(double* in, double* out, int w, int h, int r) {

    double i_arr = static_cast<double>(1) / static_cast<double>(r + r + 1);

    for (int32_t i = 0; i < w; i++) {

        int32_t t_i = i;
        int32_t l_i = t_i;
        int32_t r_i = t_i + r * w;

        double f_v = in[t_i];
        double l_v = in[t_i + w * (h - 1)];
        double val = static_cast<double>(r + 1) * f_v;

        for (int32_t j = 0; j < r; j++) {
            val += in[t_i + j * w];
        }
        for (int32_t j = 0; j <= r; j++) {
            val += in[r_i] - f_v;
            out[t_i] = val * i_arr;
            r_i += w;
            t_i += w;
        }
        for (int32_t j = r + 1; j < h - r; j++) {
            val += in[r_i] - in[l_i];
            out[t_i] = val * i_arr;
            l_i += w;
            r_i += w;
            t_i += w;
        }
        for (int32_t j = h - r; j < h; j++) {
            val += l_v - in[l_i];
            out[t_i] = val * i_arr;
            l_i += w;
            t_i += w;
        }
    }
}

void BoxBlur(double*& in, double*& out, int w, int h, int r) {
    std::swap(in, out);
    HorBlur(out, in, w, h, r);
    TotalBlur(in, out, w, h, r);
}

void FastGaussianBlur(double*& in, double*& out, int32_t w, int32_t h, double sigma) {
    int32_t boxes[3];
    StdToBox(boxes, sigma, 3);
    BoxBlur(in, out, w, h, boxes[0]);
    BoxBlur(out, in, w, h, boxes[1]);
    BoxBlur(in, out, w, h, boxes[2]);
}

void Blur::Apply(Image& image) {

    int32_t width = static_cast<int32_t>(image.ImageWidth());
    int32_t height = static_cast<int32_t>(image.ImageHeight());
    int32_t size = width * height;

    double sigma = std::stod(params_[0]);

    double* new_b = new double[size];
    double* new_g = new double[size];
    double* new_r = new double[size];

    double* prev_b = new double[size];
    double* prev_g = new double[size];
    double* prev_r = new double[size];

    size_t matrix_ptr_i = 0;
    size_t matrix_ptr_j = 0;

    RGBPixel pixel;

    for (int32_t i = 0; i < size; ++i) {
        pixel = image.GetPixel(matrix_ptr_i, matrix_ptr_j);

        prev_b[i] = static_cast<double>(pixel.B);
        prev_g[i] = static_cast<double>(pixel.G);
        prev_r[i] = static_cast<double>(pixel.R);

        ++matrix_ptr_j;
        if (matrix_ptr_j == image.ImageWidth()) {
            ++matrix_ptr_i;
            matrix_ptr_j = 0;
        }
    }

    FastGaussianBlur(prev_b, new_b, width, height, sigma);
    FastGaussianBlur(prev_g, new_g, width, height, sigma);
    FastGaussianBlur(prev_r, new_r, width, height, sigma);

    matrix_ptr_i = 0;
    matrix_ptr_j = 0;

    for (int32_t i = 0; i < size; ++i) {

        image.SetPixel(
            matrix_ptr_i, matrix_ptr_j,
            {static_cast<uint8_t>(new_b[i]), static_cast<uint8_t>(new_g[i]), static_cast<uint8_t>(new_r[i])});

        ++matrix_ptr_j;
        if (matrix_ptr_j == image.ImageWidth()) {
            ++matrix_ptr_i;
            matrix_ptr_j = 0;
        }
    }

    delete[] new_r;
    delete[] new_b;
    delete[] new_g;
    delete[] prev_r;
    delete[] prev_b;
    delete[] prev_g;
}