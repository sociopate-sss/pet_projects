#include "../include/bmp_utils.h"

#include <iostream>
#include <stdexcept>

BMPStream::~BMPStream() {
    Close();
    pixel_array_.clear();
}

void BMPStream::Open(const std::string &filename) {
    if (file_.is_open()) {
        throw std::logic_error("File is already open.");
    }

    if (filename.empty()) {
        throw std::invalid_argument("File is empty.");
    }

    filename_ = filename;
    file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);

    if (!file_.is_open()) {
        throw std::runtime_error("Can't open file.");
    }
}

void BMPStream::Close() {
    file_.close();
    filename_.clear();
}

void BMPStream::ReadBmpHeader() {
    file_.read(reinterpret_cast<char *>(&bmp_header_), sizeof(BMPHeader));

    // Проверяем корректность bmp_header_ прямо здесь
    if (bmp_header_.signature != BMP_SIGNATURE) {
        throw std::runtime_error("BMP File is not correct.");
    }
}

void BMPStream::ReadDibHeader() {
    file_.read(reinterpret_cast<char *>(&dib_header_), sizeof(DIBHeader));

    // Проверяем корректность bits_per_pixel прямо здесь
    if (dib_header_.bits_per_pixel != DIB_BITS_PER_PIXEL) {
        throw std::runtime_error("BMP File is not correct.");
    }
}

void BMPStream::ReadRGBData() {

    const uint8_t padding = (4 - (dib_header_.width * 3) % 4) % 4;

    pixel_array_.clear();
    pixel_array_.resize(dib_header_.height, std::vector<RGBPixel>(dib_header_.width, {0, 0, 0}));
    for (size_t i = 0; i < static_cast<size_t>(dib_header_.height); ++i) {
        for (size_t j = 0; j < static_cast<size_t>(dib_header_.width); ++j) {
            file_.read(reinterpret_cast<char *>(&pixel_array_[i][j]), sizeof(RGBPixel));
        }
        file_.ignore(padding);
    }
}

void BMPStream::ReadBmp() {
    if (!IsOpen()) {
        throw std::logic_error("Input File isn't open.");
    }

    file_.seekg(0);

    try {
        ReadBmpHeader();
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    ReadDibHeader();
    ReadRGBData();
}

void BMPStream::WriteBmpHeader() {
    file_.write(reinterpret_cast<char *>(&bmp_header_), sizeof(BMPHeader));
}

void BMPStream::WriteDibHeader() {
    file_.write(reinterpret_cast<char *>(&dib_header_), sizeof(DIBHeader));
}

void BMPStream::WriteRGBData(const Image &image) {

    char buffer[3] = {0, 0, 0};
    const uint8_t padding = (4 - (dib_header_.width * 3) % 4) % 4;

    for (size_t i = 0; i < static_cast<size_t>(dib_header_.height); ++i) {
        for (size_t j = 0; j < static_cast<size_t>(dib_header_.width); ++j) {
            RGBPixel pixel = image.GetPixel(i, j);
            file_.write(reinterpret_cast<char *>(&pixel), sizeof(RGBPixel));
        }
        if (padding > 0) {
            file_.write(buffer, padding);
        }
    }
}

void BMPStream::WriteBmp(const Image &image) {
    if (!IsOpen()) {
        throw std::logic_error("Output File isn't open.");
    }

    file_.seekg(0);

    /// Изменяем данные в загловках DIB-header и BMP-header
    bmp_header_.size -= dib_header_.image_size;

    dib_header_.width = static_cast<int32_t>(image.ImageWidth());
    dib_header_.height = static_cast<int32_t>(image.ImageHeight());
    dib_header_.image_size = dib_header_.width * dib_header_.height;

    bmp_header_.size += dib_header_.image_size;

    WriteBmpHeader();
    WriteDibHeader();
    WriteRGBData(image);
}