#include "../include/crosspolynomial_filter.h"

#include <stdexcept>
#include <stdint.h>

BasicFilter* CrossPolynomial::CreateCrossPolynomialFilter(const FilterSettings& fs) {
    if (fs.params.size() != CROSSPOLY_COEFFS_NUM) {
        throw std::logic_error("Bad crosspoly params.");
    }

    for (const std::string& param : fs.params) {
        try {
            double coeff_value = std::stod(param);
            if (coeff_value < 0.0 || coeff_value > 1.0) {
                throw std::logic_error("Bad crosspoly params.");
            }
        } catch (...) {
            throw std::logic_error("Bad crosspoly params.");
        }
    }

    return dynamic_cast<BasicFilter*>(new CrossPolynomial(fs));
}

void CrossPolynomial::Apply(Image& image) {
    std::vector<double> rc(CROSSPOLY_COLOR_COEFFS_NUM);
    std::vector<double> gc(CROSSPOLY_COLOR_COEFFS_NUM);
    std::vector<double> bc(CROSSPOLY_COLOR_COEFFS_NUM);

    for (size_t i = 0; i < static_cast<size_t>(CROSSPOLY_COLOR_COEFFS_NUM); ++i) {
        rc[i] = std::stod(params_[i]);
    }

    for (size_t i = static_cast<size_t>(CROSSPOLY_COLOR_COEFFS_NUM);
         i < static_cast<size_t>(2 * CROSSPOLY_COLOR_COEFFS_NUM); ++i) {
        gc[i - CROSSPOLY_COLOR_COEFFS_NUM] = std::stod(params_[i]);
    }

    for (size_t i = static_cast<size_t>(2 * CROSSPOLY_COLOR_COEFFS_NUM);
         i < static_cast<size_t>(3 * CROSSPOLY_COLOR_COEFFS_NUM); ++i) {
        bc[i - 2 * CROSSPOLY_COLOR_COEFFS_NUM] = std::stod(params_[i]);
    }

    double in[3] = {0};

    static const size_t RR_COEFF = 3;
    static const size_t GG_COEFF = 4;
    static const size_t BB_COEFF = 5;
    static const size_t RG_COEFF = 6;
    static const size_t GB_COEFF = 7;
    static const size_t BR_COEFF = 8;
    static const size_t NULL_COEFF = 9;

    for (size_t i = 0; i < image.ImageHeight(); ++i) {
        for (size_t j = 0; j < image.ImageWidth(); ++j) {

            RGBPixel pixel = image.GetPixel(i, j);

            in[0] = static_cast<double>(pixel.R) / static_cast<double>(UINT8_MAX);
            in[1] = static_cast<double>(pixel.G) / static_cast<double>(UINT8_MAX);
            in[2] = static_cast<double>(pixel.B) / static_cast<double>(UINT8_MAX);

            double new_r = in[0] * rc[0] + in[1] * rc[1] + in[2] * rc[2] + in[0] * in[0] * rc[RR_COEFF] +
                           in[1] * in[1] * rc[GG_COEFF] + in[2] * in[2] * rc[BB_COEFF] + in[0] * in[1] * rc[RG_COEFF] +
                           in[1] * in[2] * rc[GB_COEFF] + in[2] * in[0] * rc[BR_COEFF] + rc[NULL_COEFF];

            double new_g = in[0] * gc[0] + in[1] * gc[1] + in[2] * gc[2] + in[0] * in[0] * gc[RR_COEFF] +
                           in[1] * in[1] * gc[GG_COEFF] + in[2] * in[2] * gc[BB_COEFF] + in[0] * in[1] * gc[RG_COEFF] +
                           in[1] * in[2] * gc[GB_COEFF] + in[2] * in[0] * gc[BR_COEFF] + gc[NULL_COEFF];

            double new_b = in[0] * bc[0] + in[1] * bc[1] + in[2] * bc[2] + in[0] * in[0] * bc[RR_COEFF] +
                           in[1] * in[1] * bc[GG_COEFF] + in[2] * in[2] * bc[BB_COEFF] + in[0] * in[1] * bc[RG_COEFF] +
                           in[1] * in[2] * bc[GB_COEFF] + in[2] * in[0] * bc[BR_COEFF] + bc[NULL_COEFF];

            image.SetPixel(i, j,
                           {static_cast<uint8_t>(new_b * static_cast<double>(UINT8_MAX)),
                            static_cast<uint8_t>(new_g * static_cast<double>(UINT8_MAX)),
                            static_cast<uint8_t>(new_r * static_cast<double>(UINT8_MAX))});
        }
    }
}