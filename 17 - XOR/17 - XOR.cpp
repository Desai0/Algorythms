#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <bitset>

//xor над 2 строками битов
std::string xor_op(std::string a, std::string b) {
    std::string result = "";
    int n = b.length();
    for (int i = 0; i < n; i++) {
        if (a[i] == b[i]) {
            result += "0";
        }
        else {
            result += "1";
        }
    }
    return result;
}

std::string polynDiv(std::string data, std::string generator) {
    int gen_len = generator.length();
    int data_len = data.length();

    std::string dividend = data;

    for (int i = 0; i <= dividend.length() - gen_len; ) {
        if (dividend[i] == '1') {
            for (int j = 0; j < gen_len; j++) {
                dividend[i + j] = (dividend[i + j] == generator[j]) ? '0' : '1';
            }
        }
        i++;
    }

    return dividend.substr(dividend.length() - (gen_len - 1));
}


bool is_zero(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](char c) { return c == '0'; });
}

std::vector<uint8_t> genTable(uint8_t polynomial) {
    std::vector<uint8_t> crc_table(256);

    for (int i = 0; i < 256; ++i) {
        uint8_t current_byte = i;
        for (int bit = 0; bit < 8; ++bit) {
            //старший бит
            if (current_byte & 0x80) {
                current_byte = (current_byte << 1) ^ polynomial;
            }
            else {
                current_byte <<= 1;
            }
        }
        crc_table[i] = current_byte;
    }
    return crc_table;
}


int main() {
    setlocale(0, "");

    std::string data = "10011101";
    std::string generator = "11001";

    std::cout << "Дата: " << data << std::endl;
    std::cout << "Полином: " << generator << std::endl;

    std::string dataWZ = data;
    for (int i = 0; i < generator.length() - 1; ++i) {
        dataWZ += "0";
    }

    std::string ostatok = polynDiv(dataWZ, generator);

    std::cout << "Результат деления (остаток): " << ostatok << std::endl;

    std::string something = data + ostatok;


    std::string verification_remainder = polynDiv(something, generator);
    std::cout << "Остаток после проверки: " << verification_remainder << std::endl;


    if (is_zero(verification_remainder)) {
        std::cout << "Результат: Сообщение корректно" << std::endl;
    }
    else {
        std::cout << "Результат: ошибка" << std::endl;
    }

    uint8_t crc8_polynomial = 0x07;
    std::cout << "Полином CRC-8: 0x" << std::hex << (int)crc8_polynomial << std::dec << std::endl;

    std::vector<uint8_t> crc_table = genTable(crc8_polynomial);

    std::cout << "\nСгенерированная таблица:" << std::endl;
    std::cout << std::hex << std::uppercase << std::setfill('0');
    for (int i = 0; i < 16; ++i) {
        std::cout << "0x" << std::setw(2) << static_cast<int>(crc_table[i]) << " ";
    }

    return 0;
}