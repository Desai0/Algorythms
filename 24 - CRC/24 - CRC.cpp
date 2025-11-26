#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>


const uint8_t CRC8_POLY = 0x07;  // x^8 + x^2 + x + 1
uint8_t crc8_table[256];

void generate_crc8_table() {
    for (int i = 0; i < 256; ++i) {
        uint8_t crc = i;
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC8_POLY;
            else
                crc <<= 1;
        }
        crc8_table[i] = crc;
    }
}

uint8_t crc8_bitwise(const uint8_t* data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC8_POLY;
            else
                crc <<= 1;
        }
    }
    return crc;
}

uint8_t crc8_table_lookup(const uint8_t* data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        crc = crc8_table[crc ^ data[i]];
    }
    return crc;
}


const uint16_t CRC16_POLY = 0x1021;
uint16_t crc16_table[256];

void generate_crc16_table() {
    for (int i = 0; i < 256; ++i) {
        uint16_t crc = i << 8;
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ CRC16_POLY;
            else
                crc <<= 1;
        }
        crc16_table[i] = crc;
    }
}

uint16_t crc16_bitwise(const uint8_t* data, size_t len) {
    uint16_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ CRC16_POLY;
            else
                crc <<= 1;
        }
    }
    return crc;
}

uint16_t crc16_table_lookup(const uint8_t* data, size_t len) {
    uint16_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t index = (crc >> 8) ^ data[i];
        crc = (crc << 8) ^ crc16_table[index];
    }
    return crc;
}


void benchmark(const uint8_t* data, size_t len, int iterations) {
    using namespace std::chrono;

    std::cout << "\n=== Тест производительности ===\n";
    std::cout << "Размер данных: " << len << " байт\n";
    std::cout << "Итераций: " << iterations << "\n\n";

    auto start = high_resolution_clock::now();
    uint8_t crc8_bit = 0;
    for (int i = 0; i < iterations; ++i)
        crc8_bit = crc8_bitwise(data, len);
    auto end = high_resolution_clock::now();
    auto duration_8bit = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    uint8_t crc8_tab = 0;
    for (int i = 0; i < iterations; ++i)
        crc8_tab = crc8_table_lookup(data, len);
    end = high_resolution_clock::now();
    auto duration_8tab = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    uint16_t crc16_bit = 0;
    for (int i = 0; i < iterations; ++i)
        crc16_bit = crc16_bitwise(data, len);
    end = high_resolution_clock::now();
    auto duration_16bit = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    uint16_t crc16_tab = 0;
    for (int i = 0; i < iterations; ++i)
        crc16_tab = crc16_table_lookup(data, len);
    end = high_resolution_clock::now();
    auto duration_16tab = duration_cast<microseconds>(end - start).count();

    std::cout << "CRC-8  bitwise:       " << std::hex << std::setw(2) << std::setfill('0')
        << (int)crc8_bit << " | Время: " << std::dec << duration_8bit << " μs\n";
    std::cout << "CRC-8  table lookup:  " << std::hex << std::setw(2) << std::setfill('0')
        << (int)crc8_tab << " | Время: " << std::dec << duration_8tab << " μs"
        << " (ускорение: " << std::fixed << std::setprecision(2)
        << (double)duration_8bit / duration_8tab << "x)\n\n";

    std::cout << "CRC-16 bitwise:       " << std::hex << std::setw(4) << std::setfill('0')
        << crc16_bit << " | Время: " << std::dec << duration_16bit << " μs\n";
    std::cout << "CRC-16 table lookup:  " << std::hex << std::setw(4) << std::setfill('0')
        << crc16_tab << " | Время: " << std::dec << duration_16tab << " μs"
        << " (ускорение: " << std::fixed << std::setprecision(2)
        << (double)duration_16bit / duration_16tab << "x)\n";
}

int main() {
	setlocale(LC_ALL, "Russian");
    generate_crc8_table();
    generate_crc16_table();

    // тестовые данные
    const char* test_str = "Hello, CRC world!";
    const uint8_t* test_data = reinterpret_cast<const uint8_t*>(test_str);
    size_t test_len = strlen(test_str);

    std::cout << "Входные данные: \"" << test_str << "\"\n";

    uint8_t crc8_b = crc8_bitwise(test_data, test_len);
    uint8_t crc8_t = crc8_table_lookup(test_data, test_len);
    uint16_t crc16_b = crc16_bitwise(test_data, test_len);
    uint16_t crc16_t = crc16_table_lookup(test_data, test_len);

    std::cout << "\n=== Результаты CRC ===\n";
    std::cout << "CRC-8  (bitwise):      0x" << std::hex << std::setw(2) << std::setfill('0') << (int)crc8_b << "\n";
    std::cout << "CRC-8  (table):        0x" << std::hex << std::setw(2) << std::setfill('0') << (int)crc8_t << "\n";
    std::cout << "CRC-16 (bitwise):      0x" << std::hex << std::setw(4) << std::setfill('0') << crc16_b << "\n";
    std::cout << "CRC-16 (table):        0x" << std::hex << std::setw(4) << std::setfill('0') << crc16_t << "\n";

    std::cout << "\n=== Проверка корректности ===\n";
    std::cout << "CRC-8  методы совпадают:  " << (crc8_b == crc8_t ? "ДА" : "НЕТ") << "\n";
    std::cout << "CRC-16 методы совпадают:  " << (crc16_b == crc16_t ? "ДА" : "НЕТ") << "\n";

    // бенчмарк
    const size_t large_size = 10000;
    uint8_t* large_data = new uint8_t[large_size];
    for (size_t i = 0; i < large_size; ++i)
        large_data[i] = i % 256;

    benchmark(large_data, large_size, 1000);

    delete[] large_data;
    return 0;
}