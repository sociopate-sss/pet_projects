#ifndef NEGATIVE_FILTER_H
#define NEGATIVE_FILTER_H

#include "basic_filter.h"

class Negative : public BasicFilter {
public:
    explicit Negative(const FilterSettings& fs) : BasicFilter(fs) {
    }

    static BasicFilter* CreateNegativeFilter(const FilterSettings& fs);

    void Apply(Image& image) override;
};

#endif