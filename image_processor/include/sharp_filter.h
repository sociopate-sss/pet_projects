#ifndef SHARP_FILTER_H
#define SHARP_FILTER_H

#include "basic_filter.h"
#include "filter_consts.h"

class Sharpering : public BasicFilter {
public:
    explicit Sharpering(const FilterSettings& fs) : BasicFilter(fs) {
    }

    static BasicFilter* CreateSharperingFilter(const FilterSettings& fs);

    void Apply(Image& image) override;
};

#endif