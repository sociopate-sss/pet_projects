#ifndef FILTER_FACTORY_H
#define FILTER_FACTORY_H

#include "command_line_parser.h"
#include "filters.h"
#include "pipeline.h"

#include <map>
#include <string>

using FilterProdPtr = BasicFilter* (*)(const FilterSettings&);

class FilterFactory {
public:
    FilterFactory() {
    }

    // Создает фильтр через FilterSettings
    BasicFilter* CreateFilter(const FilterSettings& fs);

protected:
    /// Набор доступных фильтров
    std::map<std::string, FilterProdPtr> existing_filters{{"crop", Crop::CreateCropFilter},
                                                          {"gs", Grayscale::CreateGrayscaleFilter},
                                                          {"neg", Negative::CreateNegativeFilter},
                                                          {"sharp", Sharpering::CreateSharperingFilter},
                                                          {"edge", EdgeDetection::CreateEdgeDetectionFilter},
                                                          {"blur", Blur::CreateBlurFilter},
                                                          {"crosspoly", CrossPolynomial::CreateCrossPolynomialFilter}};
};

#endif