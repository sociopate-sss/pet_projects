#ifndef GRAYSCALE_FILTER_H
#define GRAYSCALE_FILTER_H

#include "basic_filter.h"
#include "filter_consts.h"

class Grayscale : public BasicFilter {
public:
    explicit Grayscale(const FilterSettings& fs) : BasicFilter(fs) {
    }

    static BasicFilter* CreateGrayscaleFilter(const FilterSettings& fs);

    void Apply(Image& image) override;
};

#endif