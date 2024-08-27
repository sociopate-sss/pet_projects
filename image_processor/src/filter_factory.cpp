#include "../include/filter_factory.h"

#include <exception>
#include <iostream>

BasicFilter* FilterFactory::CreateFilter(const FilterSettings& fs) {
    try {
        if (existing_filters[fs.name] == nullptr) {
            return nullptr;
        }
        return existing_filters[fs.name](fs);
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return nullptr;
    } catch (...) {
        std::cerr << "Unknown error.\n";
        return nullptr;
    }
}