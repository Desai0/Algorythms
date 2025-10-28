#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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
    for (int i = 0; i < gen_len - 1; ++i) {
        dividend += "0";
    }

    int dividend_len = dividend.length();

    std::string temp_div = dividend.substr(0, gen_len);

    for (int i = 0; i < data_len; ++i) {
        if (temp_div[0] == '1') {
            temp_div = xor_op(temp_div, generator);
        }

        //- левый бит
        temp_div = temp_div.substr(1);

        //+ следующий бит
        if (i + gen_len < dividend_len) {
            temp_div += dividend[i + gen_len];
        }

        return temp_div;
    }
}

bool is_zero(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](char c) { return c == '0'; });
}


int main() {
    setlocale(0, "");

    std::string data = "10011101";
    std::string generator = "11001";

    std::cout << "Дата: " << data << std::endl;
    std::cout << "Полином: " << generator << std::endl;

    std::string ostatok = polynDiv(data, generator);

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



    return 0;
}

//остаток вставить в конец