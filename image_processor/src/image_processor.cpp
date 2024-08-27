#include "../include/bmp_utils.h"
#include "../include/command_line_parser.h"
#include "../include/image.h"
#include "../include/filter_factory.h"
#include "../include/pipeline.h"

#include <exception>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {

    /// Получаем данные из командной строки
    CommandLineParser clp;

    try {
        clp.Parse(argc, argv);
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception.\n";
        return 1;
    }

    /// Считываем данные из входного файла

    BMPStream bmp_file;

    try {
        bmp_file.Open(clp.GetInputFileName());
        bmp_file.ReadBmp();
        bmp_file.Close();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception.\n";
        return 1;
    }

    /// Накладываем фильтры на изображение

    Image image(bmp_file.BMPFileWidth(), bmp_file.BMPFileHeight(), bmp_file.BMPFilePixelArray());

    FilterFactory filter_factory;
    Pipeline pipeline;
    try {
        for (const FilterSettings& fs : clp.filters) {
            BasicFilter* filter = filter_factory.CreateFilter(fs);
            if (!filter) {
                throw std::logic_error("Bad filter params.");
            }
            pipeline.AddFilter(filter);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown error.\n";
        return 1;
    }

    pipeline.Apply(image);

    /// Записываем в выходной BMP файл
    try {
        /// Создаем выходной BMP файл, если такого еще нет
        std::ofstream bmp_output(clp.GetOutputFileName());
        bmp_output.close();

        bmp_file.Open(clp.GetOutputFileName());
        bmp_file.WriteBmp(image);
        bmp_file.Close();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception.\n";
        return 1;
    }

    return 0;
}
