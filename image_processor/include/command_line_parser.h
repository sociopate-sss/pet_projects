#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <string>
#include <vector>

struct FilterSettings {
    std::string name;
    std::vector<std::string> params;
};

class CommandLineParser {
public:
    std::vector<FilterSettings> filters;

public:
    CommandLineParser() {
    }

    void Parse(int argc, char** argv);

    const std::string& GetInputFileName() const {
        return input_file_;
    }

    const std::string& GetOutputFileName() const {
        return output_file_;
    }

protected:
    std::string input_file_;
    std::string output_file_;
};

#endif