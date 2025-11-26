#include <iostream>
#include <cmath>
#include <iomanip>

const int N = 8;

// таблица квантизации JPEG
int quantTable[N][N] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};

// двумерное дискретное косинусное преобразование
void dct2d(double input[N][N], double output[N][N]) {
    for (int u = 0; u < N; ++u) {
        for (int v = 0; v < N; ++v) {
            double sum = 0.0;
            for (int x = 0; x < N; ++x) {
                for (int y = 0; y < N; ++y) {
                    double cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    double cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    sum += input[x][y]
                        * cos((2 * x + 1) * u * M_PI / 16.0)
                        * cos((2 * y + 1) * v * M_PI / 16.0);
                }
            }
            output[u][v] = 0.25 * sum;
        }
    }
}

// квантизация DCT-коэф
void quantize(double dct[N][N], int output[N][N]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            output[i][j] = round(dct[i][j] / quantTable[i][j]);
}

void printMatrix(int m[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            std::cout << std::setw(4) << m[i][j] << " ";
        std::cout << "\n";
    }
}

int main() {
	setlocale(LC_ALL, "Russian");
    // уровни серого 0-255
    double block[N][N] = {
        {52, 55, 61, 66, 70, 61, 64, 73},
        {63, 59, 55, 90, 109, 85, 69, 72},
        {62, 59, 68, 113, 144, 104, 66, 73},
        {63, 58, 71, 122, 154, 106, 70, 69},
        {67, 61, 68, 104, 126, 88, 68, 70},
        {79, 65, 60, 70, 77, 68, 58, 75},
        {85, 71, 64, 59, 55, 61, 65, 83},
        {87, 79, 69, 68, 65, 76, 78, 94}
    };

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            block[i][j] -= 128;

    double dct[N][N];
    dct2d(block, dct);

    std::cout << "DCT коэффициенты (до квантизации):\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            std::cout << std::setw(8) << std::fixed << std::setprecision(1) << dct[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";

    int quantized[N][N];
    quantize(dct, quantized);

    std::cout << "Квантизованные DCT коэффициенты (после сжатия):\n";
    printMatrix(quantized);

    return 0;
}
