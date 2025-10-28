#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

std::vector<int> LzwCompress(const std::string& input) {
    std::unordered_map<std::string, int> dictionary;
    int dict_size = 256;
    for (int i = 0; i < 256; ++i) {
        dictionary[std::string(1, (char)i)] = i;
    }

    std::string current_string;
    std::vector<int> compressed_output;

    for (char symbol : input) {
        std::string next_string = current_string + symbol;

        if (dictionary.count(next_string)) {
            current_string = next_string;
        }
        else {
            compressed_output.push_back(dictionary[current_string]);
            dictionary[next_string] = dict_size++;
            current_string = std::string(1, symbol);
        }
    }

    if (!current_string.empty()) {
        compressed_output.push_back(dictionary[current_string]);
    }

    return compressed_output;
}

std::string lzw_decompress(const std::vector<int>& compressed_input) {
    if (compressed_input.empty()) {
        return "";
    }

    std::unordered_map<int, std::string> dictionary;
    int dict_size = 256;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, (char)i);
    }

    std::string current_string = dictionary[compressed_input[0]];
    std::string decompressed_output = current_string;

    for (size_t i = 1; i < compressed_input.size(); ++i) {
        int current_code = compressed_input[i];
        std::string entry;

        if (dictionary.count(current_code)) {
            entry = dictionary[current_code];
        }
        else {
            throw std::runtime_error("Ошибка: неверный сжатый код.");
        }

        decompressed_output += entry;
        dictionary[dict_size++] = current_string + entry[0];
        current_string = entry;
    }

    return decompressed_output;
}


int main() {
    setlocale(LC_ALL, "Russian");

    std::string original_string = "TOBEORNOTTOBEORTOBEORNOT";

    std::cout << "Оригинальная строка: " << original_string << std::endl;
    std::cout << "Длина: " << original_string.length() << " символов" << std::endl;

    std::vector<int> compressed_data = LzwCompress(original_string);

    std::cout << std::endl;
    std::cout << "Количество кодов: " << compressed_data.size() << std::endl;

    std::string decompressed_string = lzw_decompress(compressed_data);

    std::cout << "Распакованная строка: " << decompressed_string << std::endl;
    std::cout << "Длина: " << decompressed_string.length() << " символов" << std::endl;

    if (original_string == decompressed_string) {
        std::cout << "Оригинальная и распакованная строки совпадают" << std::endl;
    }
    else {
        std::cout << "Ошибка" << std::endl;
    }

    return 0;
}