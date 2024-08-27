#ifndef CROP_FILTER_H
#define CROP_FILTER_H

#include "basic_filter.h"

class Crop : public BasicFilter {
public:
    explicit Crop(const FilterSettings& fs) : BasicFilter(fs) {
    }

    static BasicFilter* CreateCropFilter(const FilterSettings& fs);

    void Apply(Image& image) override;
};

#endif