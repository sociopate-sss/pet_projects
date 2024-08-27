#include "../include/command_line_parser.h"

#include <stdexcept>

void CommandLineParser::Parse(int argc, char** argv) {

    if (argc < 3) {
        throw std::logic_error("Invalid input data.");
    }

    input_file_ = argv[1];
    output_file_ = argv[2];

    for (size_t i = 3; i < static_cast<size_t>(argc); ++i) {

        if (argv[i][0] == '-') {
            filters.push_back({static_cast<std::string>(argv[i]), {}});
            filters.back().name.erase(0, 1);
            continue;
        }

        if (filters.empty()) {
            throw std::logic_error("Invalid input data.");
        }

        filters.back().params.push_back(static_cast<std::string>(argv[i]));
    }
}