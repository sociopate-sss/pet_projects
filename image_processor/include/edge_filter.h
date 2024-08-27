#ifndef EDGE_FILTER_H
#define EDGE_FILTER_H

#include "basic_filter.h"
#include "filter_consts.h"

class EdgeDetection : public BasicFilter {
public:
    explicit EdgeDetection(const FilterSettings& fs) : BasicFilter(fs) {
    }

    static BasicFilter* CreateEdgeDetectionFilter(const FilterSettings& fs);

    void Apply(Image& image) override;
};

#endif