#ifndef BASIC_FILTER_H
#define BASIC_FILTER_H

#include "command_line_parser.h"
#include "image.h"

class BasicFilter {
public:
    explicit BasicFilter(const FilterSettings& fs) : params_(fs.params) {
    }

    virtual ~BasicFilter() {
    }

    virtual void Apply(Image& image) = 0;

protected:
    static void ApplyMatrix(Image& image, const int32_t filter_matrix[3][3]);

protected:
    std::vector<std::string> params_;
};

#endif