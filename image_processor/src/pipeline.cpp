#include "../include/pipeline.h"

Pipeline::~Pipeline() {
    for (BasicFilter* filter : applied_filters_) {
        delete filter;
    }
    applied_filters_.clear();
}

void Pipeline::AddFilter(BasicFilter* filter) {
    applied_filters_.push_back(filter);
}

void Pipeline::Apply(Image& image) {
    for (BasicFilter* filter : applied_filters_) {
        filter->Apply(image);
    }
}