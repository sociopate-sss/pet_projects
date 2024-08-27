#ifndef PIPELINE_H
#define PIPELINE_H

#include "bmp_utils.h"
#include "filters.h"

#include <vector>

class Pipeline {
public:
    Pipeline() {
    }

    ~Pipeline();

    void AddFilter(BasicFilter* filter);

    void Apply(Image& image);

protected:
    std::vector<BasicFilter*> applied_filters_;
};

#endif