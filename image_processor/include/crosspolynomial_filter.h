#ifndef CROSSPOLYNOMIAL_FILTER_H
#define CROSSPOLYNOMIAL_FILTER_H

#include "basic_filter.h"
#include "filter_consts.h"

/// Свой фильтр, который изменяет значения пикселей в изображении, применяя набор полиномиальных векторных произведений.
/// Название фильтра: -crosspoly
/// Параметры: rc[10], gc[10], bc[10] - массивы коэффицентов(в промежутке от 0 до 1) для каждого цвета
class CrossPolynomial : public BasicFilter {
public:
    explicit CrossPolynomial(const FilterSettings& fs) : BasicFilter(fs) {
    }

    static BasicFilter* CreateCrossPolynomialFilter(const FilterSettings& fs);

    void Apply(Image& image) override;
};

#endif