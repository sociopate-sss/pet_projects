/// Файл с константами для фильтров

#ifndef FILTER_CONSTS_H
#define FILTER_CONSTS_H

#include <stdint.h>

const double R_GS_COEFF = 0.299;
const double G_GS_COEFF = 0.587;
const double B_GS_COEFF = 0.114;

const int32_t EDGE_CENTRAL_COEFF = 4;
const int32_t SHARP_CENTRAL_COEFF = 5;

const int32_t CROSSPOLY_COEFFS_NUM = 30;
const int32_t CROSSPOLY_COLOR_COEFFS_NUM = 10;

const double E = 2.7182818284590452353;
const double PI = 3.14159265358979323846;

#endif