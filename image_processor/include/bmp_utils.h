#ifndef BMP_UTILS_H
#define BMP_UTILS_H

#include "image.h"

#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

class BMPStream {
public:
    static const uint16_t BMP_SIGNATURE = 0x4D42;  // BM
    static const uint16_t DIB_BITS_PER_PIXEL = 0x0018;

    struct BMPHeader {
        uint16_t signature;
        uint32_t size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t offset;
    } __attribute__((packed));

    struct DIBHeader {
        uint32_t header_size;
        int32_t width;
        int32_t height;
        uint16_t color_planes_num;
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t image_size;
        uint32_t hor_res;
        uint32_t ver_res;
        uint32_t colors_num;
        uint32_t important_colors_num;
    } __attribute__((packed));

public:
    BMPStream() {
    }

    ~BMPStream();

    /// Открывает файл с переданным именем файла
    void Open(const std::string& filename);

    /// Закрывает файл
    void Close();

    /// Читает открытый BMP файл
    void ReadBmp();

    /// Записывает в открытый BMP файл
    void WriteBmp(const Image& image);

    bool IsOpen() const {
        return file_.is_open();
    }

    const std::string& GetFilename() const {
        return filename_;
    }

    int32_t BMPFileWidth() const {
        return dib_header_.width;
    }

    int32_t BMPFileHeight() const {
        return dib_header_.height;
    }

    const std::vector<std::vector<RGBPixel>>& BMPFilePixelArray() const {
        return pixel_array_;
    }

protected:
    /// Читает BMP заголовок не задавая текущую позицию чтения.
    /// Не проверят открыт ли файл
    void ReadBmpHeader();

    /// Записывает BMP заголовок не задавая текущую позицию чтения.
    /// Не проверят открыт ли файл
    void WriteBmpHeader();

    /// Читает BMP заголовок не задавая текущую позицию чтения.
    /// Не проверяет открыт ли файл
    void ReadDibHeader();

    /// Записывает BMP заголовок не задавая текущую позицию чтения.
    /// Не проверят открыт ли файл
    void WriteDibHeader();

    /// Читает пиксельный массив BMP файла
    /// Не проверяет открыт ли файл
    void ReadRGBData();

    /// Записывает пиксельный массив BMP файла
    /// Не проверяет открыт ли файл
    void WriteRGBData(const Image& image);

protected:
    std::string filename_;
    std::fstream file_;

    BMPHeader bmp_header_;
    DIBHeader dib_header_;

    std::vector<std::vector<RGBPixel>> pixel_array_;
};

#endif