#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <bitset>

void print_hex(uint32_t n) {
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << n << std::dec;
}

void print_binary(uint32_t n) {
    std::cout << std::bitset<32>(n);
}

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

void generate_h(uint32_t h[8]) {
    std::cout << "Вычисление значений хеша (h)" << std::endl;
    std::cout << "Вычисляем первые 32 бита дробных частей квадратных корней первых 8 простых" << std::endl;

    int count = 0;
    int n = 2;
    while (count < 8) {
        if (is_prime(n)) {
            double root = sqrt(n);
            double fract_part = root - floor(root);
            h[count] = static_cast<uint32_t>(fract_part * pow(2, 32));

            std::cout << "h" << count << " (из sqrt(" << n << ")): ";
            print_hex(h[count]);
            std::cout << std::endl;

            count++;
        }
        n++;
    }
    std::cout << std::endl;
}

void generate_k(uint32_t k[64]) {
    std::cout << "Вычисление округлённых констант (k)" << std::endl;
    std::cout << "Вычисляем первые 32 бита дробных частей кубических корней первых 64 простых" << std::endl;
    int count = 0;
    int n = 2;
    while (count < 64) {
        if (is_prime(n)) {
            double root = cbrt(n);
            double fractional_part = root - floor(root);
            k[count] = static_cast<uint32_t>(fractional_part * pow(2, 32));
            
            std::cout << "k[" << std::setw(2) << count << "]: ";
            print_hex(k[count]);
            if ((count + 1) % 4 == 0) std::cout << std::endl;
            else std::cout << "  ";
            
            count++;
        }
        n++;
    }
    std::cout << "\n" << std::endl;
}

static inline uint32_t rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
static inline uint32_t rightshift(uint32_t x, uint32_t n) { return x >> n; }

void sha256_compress_block(uint32_t h[8], const uint32_t w[64], const uint32_t k[64]) {
    std::cout << "Цикл сжатия" << std::endl;

    uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
    uint32_t e = h[4], f = h[5], g = h[6], h_var = h[7];

    std::cout << "(a..h) скопированы из h[0..7]" << std::endl << std::endl;

    for (int i = 0; i < 64; ++i) {
        uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t temp1 = h_var + S1 + ch + k[i] + w[i];

        uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t temp2 = S0 + maj;

        h_var = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    std::cout << "Финальные рабочие переменные:" << std::endl;
    std::cout << "a="; print_hex(a); std::cout << " b="; print_hex(b); std::cout << " ..." << std::endl << std::endl;

    h[0] += a; h[1] += b; h[2] += c; h[3] += d;
    h[4] += e; h[5] += f; h[6] += g; h[7] += h_var;
    std::cout << "Итоговые значения h[0]..h[7] обновлены сложением с a..h." << std::endl << std::endl;
}


int main() {
    setlocale(0, "");

    std::string message = "hello world";
    std::cout << "хеширование сообщения: \"" << message << "\"" << std::endl << std::endl;

    uint32_t h[8];
    uint32_t k[64];
    generate_h(h);
    generate_k(k);


    std::cout << "Предварительная обработка" << std::endl;

    std::vector<uint8_t> padded_message(message.begin(), message.end());
    std::cout << "Сообщение в байтах (" << message.length() * 8 << " бит)" << std::endl;

    padded_message.push_back(0x80);
    std::cout << "Добавляем единичный бит" << std::endl;

    while (padded_message.size() % 64 != 56) {
        padded_message.push_back(0x00);
    }
    std::cout << "+ нулевые байты, чтобы длина стала 512 бит минус 64" << std::endl;

    uint64_t bit_len = message.length() * 8;
    std::cout << "Добавляем 64-битную длину сообщения (" << bit_len << " бит)" << std::endl;
    for (int i = 7; i >= 0; --i) {
        padded_message.push_back((bit_len >> (i * 8)) & 0xFF);
    }

    std::cout << "512-битный готов: " << std::endl << std::endl;
    for (size_t i = 0; i < padded_message.size(); ++i) {
        std::cout << std::bitset<8>(padded_message[i]);
    }



    std::cout << "\n\nw[0..63]" << std::endl;
    uint32_t w[64];

    for (int i = 0; i < 16; ++i) {
        w[i] = (padded_message[i * 4] << 24) | (padded_message[i * 4 + 1] << 16) | (padded_message[i * 4 + 2] << 8) | padded_message[i * 4 + 3];
    }
    std::cout << "16 слов скопированы из блока" << std::endl;

    for (int i = 16; i < 64; ++i) {
        uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ rightshift(w[i - 15], 3);
        uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ rightshift(w[i - 2], 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    std::cout << "48 сгенерированы" << std::endl << std::endl;

    std::cout << "Итоговое расписание" << std::endl;
    for (int i = 0; i < 64; ++i) {
        std::cout << "w[" << std::setw(2) << i << "]: ";
        print_hex(w[i]);
        std::cout << " (";
        print_binary(w[i]);
        std::cout << ")" << std::endl;
    }
    std::cout << std::endl;


    sha256_compress_block(h, w, k);

    std::cout << "Финальный хеш" << std::endl;
    std::stringstream final_hash;
    final_hash << std::hex << std::setfill('0');
    for (int i = 0; i < 8; ++i) {
        final_hash << std::setw(8) << h[i];
    }

    std::cout << "Итог: " << final_hash.str() << std::endl;

    return 0;
}