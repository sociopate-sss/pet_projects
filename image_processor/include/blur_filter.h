#ifndef BLUR_FILTER_H
#define BLUR_FILTER_H

#include "basic_filter.h"
#include "filter_consts.h"

class Blur : public BasicFilter {
public:
    explicit Blur(const FilterSettings& fs) : BasicFilter(fs) {
    }

    static BasicFilter* CreateBlurFilter(const FilterSettings& fs);

    void Apply(Image& image) override;
};

void BoxBlur(double*& in, double*& out, int w, int h, int r);
void HorBlur(double* in, double* out, int w, int h, int r);
void FastGaussianBlur(double*& in, double*& out, int32_t w, int32_t h, double sigma);
void StdToBox(int boxes[], double sigma, int n);
void TotalBlur(double* in, double* out, int w, int h, int r);

#endif