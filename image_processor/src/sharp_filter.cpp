#include "../include/sharp_filter.h"

#include <stdexcept>

BasicFilter* Sharpering::CreateSharperingFilter(const FilterSettings& fs) {
    if (!fs.params.empty()) {
        throw std::logic_error("Bad sharp params.");
    }

    return dynamic_cast<BasicFilter*>(new Sharpering(fs));
}

void Sharpering::Apply(Image& image) {
    int32_t filter_matrix[3][3] = {{0, -1, 0}, {-1, SHARP_CENTRAL_COEFF, -1}, {0, -1, 0}};
    ApplyMatrix(image, filter_matrix);
}