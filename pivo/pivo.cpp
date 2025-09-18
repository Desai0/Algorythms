#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <algorithm>
#include <iomanip>
#include <limits>

void printBinaryMatrix(const std::string& title, const std::vector<std::string>& headers, const std::vector<std::vector<int>>& matrix) {
    std::cout << "--- " << title << " ---" << std::endl;

    // Печать заголовка таблицы
    std::cout << std::setw(5) << "ID" << "|";
    for (const auto& header : headers) {
        std::cout << std::setw(8) << header << "|";
    }
    std::cout << std::endl;
    std::cout << std::string(6 + headers.size() * 9, '-') << std::endl;

    // Печать строк матрицы
    int transaction_id = 1;
    for (const auto& row : matrix) {
        std::cout << std::setw(5) << transaction_id++ << "|";
        for (const auto& val : row) {
            std::cout << std::setw(8) << val << "|";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void productSupport(const std::string& title, const std::vector<std::string>& headers, const std::vector<std::vector<int>>& matrix, std::vector<int>& products_number) {
    std::cout << "--- " << title << " ---" << std::endl;

    // Печать заголовка таблицы
    std::cout << std::setw(5);
    for (const auto& header : headers) {
        std::cout << std::setw(8) << header << "|";
    }
    std::cout << std::endl;
    std::cout << std::string(6 + headers.size() * 9, '-') << std::endl;

    for (int i = 0; i < products_number.size(); i++) {
        for (int j = 0; j < 8; j++) {
            products_number[i] += matrix[j][i];
        }
    }
}

void findSupport(const std::string& title, const std::vector<std::string>& headers, const std::vector<int>& products_number, std::vector<double>& products_support) {
    std::cout << "--- " << title << " ---" << std::endl;

    // Печать заголовка таблицы
    std::cout << std::setw(5);
    for (const auto& header : headers) {
        std::cout << std::setw(8) << header << "|";
    }
    std::cout << std::endl;
    std::cout << std::string(6 + headers.size() * 9, '-') << std::endl;

    for (int i = 0; i < products_number.size(); i++) {
        double trash = (double)products_number[i] / 8;
        products_support[i] = trash;
    }
}

void appleBeerReliability(const std::string& title, const std::vector<std::vector<int>>& matrix, double& beer_reliability, const std::vector<int> products_number) {
    std::cout << "--- " << title << " ---" << std::endl;

    for (int i = 0; i < 8; i++) {
        if ((matrix[i][0] == 1) && (matrix[i][1] == 1)) {
            beer_reliability += 1;
        }
    }

    if ((beer_reliability / products_number[0]) < 1) {
        std::cout << "достоверность меньше единицы - не учитываем";
        beer_reliability = 0;
    }
    else {
        beer_reliability = beer_reliability / products_number[0];
    }
}

double calculateConviction(double supportY, double confidenceXY) {
    if (confidenceXY == 1.0) {
        return std::numeric_limits<double>::infinity();
    }

    if (1.0 - confidenceXY == 0) {
        return std::numeric_limits<double>::infinity();
    }

    return (1.0 - supportY) / (1.0 - confidenceXY);
}

void liftAppleBeer(double beer_reliability, std::vector<double> products_support, double& lift_beer_apple) {
    lift_beer_apple = beer_reliability / products_support[1];
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<int> products_number = {
        0, 0, 0, 0, 0, 0
    };

    std::vector<double> products_support = {
        0, 0, 0, 0, 0, 0
    };

    double beer_reliability = 0;

    const std::vector<std::string> all_products = {
        "apple", "beer", "rice", "chicken", "milk", "pear"
    };

    // 2. Создаем бинарную матрицу, соответствующую изображению
    // 1 - товар куплен, 0 - не куплен
    // Столбцы:   apple, juice, rice, chicken, milk, pear
    std::vector<std::vector<int>> transactions_matrix = {
        {1,       1,      1,    1,       0,      0},    // Покупка 1
        {1,       1,      1,    0,       0,      0},    // Покупка 2
        {1,       1,      0,    0,       0,      0},    // Покупка 3
        {1,       0,      0,    0,       0,      1},    // Покупка 4
        {0,       1,      1,    1,       1,      0},    // Покупка 5
        {0,       1,      1,    0,       1,      0},    // Покупка 6
        {0,       1,      0,    0,       1,      0},    // Покупка 7
        {0,       0,      0,    0,       1,      1}     // Покупка 8
    };

    // Выводим оригинальную матрицу для сравнения
    printBinaryMatrix("Оригинальная матрица покупок", all_products, transactions_matrix);

    productSupport("Добавили все покупки", all_products, transactions_matrix, products_number);

    for (int i = 0; i < 6; i++) {
        std::cout << products_number[i] << " ";
    }
    std::cout << "\n";
    std::cout << "\n";
    findSupport("Вычислили support", all_products, products_number, products_support);

    for (int i = 0; i < 6; i++) {
        std::cout << products_support[i] << " ";
    }

    std::cout << "\n";
    std::cout << "\n";
    appleBeerReliability("Яблоко->пиво достоверность", transactions_matrix, beer_reliability, products_number);

    if (beer_reliability < 1) {
        return 0;
    }

    std::cout << beer_reliability;


    std::cout << "\n";
    std::cout << "\n";
    std::cout << "--- Расчет убедительности для Яблоко -> Пиво ---" << std::endl;

    double support_beer = products_support[1];

    double conviction = calculateConviction(support_beer, beer_reliability);
    std::cout << "\nИтоговая убедительность = " << conviction << std::endl;

    std::cout << "\n";
    std::cout << "\n";

    std::cout << "--- Расчет lift для Яблоко -> Пиво ---" << std::endl;
    double lift_beer_apple = 0;
    liftAppleBeer(beer_reliability, products_support, lift_beer_apple);
    std::cout << lift_beer_apple << std::endl;

    return 0;
}