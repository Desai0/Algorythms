#include <iostream>
#include <string>
#include <vector>


// Реализация алгоритма Кнута-Морриса-Пратта для поиска подстроки в строке.
int knuthMorrisPratt(const std::string& where, const std::string& what) {
    int n = where.length(); // Длина текста, в котором ищем
    int m = what.length();  // Длина искомого шаблона (подстроки)

    if (m == 0) {
        return 0; // Пустая строка всегда находится в начале
    }
    if (n < m) {
        return -1; // Шаблон длиннее текста, найти невозможно
    }
    std::vector<int> table(m);

    table[0] = 0;
    int shift = 0;
    for (int q = 1; q < m; q++) {
        while (shift > 0 && what[shift] != what[q]) {
            shift = table[shift - 1];
        }

        if (what[shift] == what[q]) {
            shift++;
        }
        table[q] = shift;
    }
    shift = 0; //shift колчство совпавших символов шаблона

    for (int i = 0; i < n; i++) {
        while (shift > 0 && what[shift] != where[i]) {
            shift = table[shift - 1];
        }

        if (what[shift] == where[i]) {
            shift++;
        }

        if (shift == m) {
            return i - m + 1; //возвращаем индекс начала
        }
    }

    return -1;
}

int RabinKarp(const std::string& where, const std::string& what) {
    int n = where.length(); // Длина текста, в котором ищем
    int m = what.length();  // Длина искомого шаблона

    const int d = 256;
    const int q = 997;

    if (m > n) {
        return -1; // не может быть длиннее текста
    }

    long long h = 1;
    long long p = 0; // Хэш для шаблона
    long long t = 0; // Хэш для текущего окна в тексте

    for (int k = 0; k < m - 1; k++) {
        h = (h * d) % q;
    }

    for (int k = 0; k < m; k++) {
        p = (d * p + what[k]) % q;
        t = (d * t + where[k]) % q;
    }

    for (int i = 0; i <= n - m; i++) {
        // совпали ли хэши
        if (p == t) {
            // проверяем символы, чтобы исключить коллизию
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (where[i + j] != what[j]) {
                    match = false;
                    break; //выходим из внутренней проверки
                }
            }
            if (match) {
                return i;
            }
        }

        //Вычисляем хэш для следующего окна текста
        if (i < n - m) {
            t = (d * (t - where[i] * h) + where[i + m]) % q;

            if (t < 0) {
                t = t + q;
            }
        }
    }
}
    
const int ALPHABET_SIZE = 256;
int hashChar(char c) {
        // Приведение к unsigned char гарантирует неотрицательный индекс.
    return static_cast<unsigned char>(c);
}
int BoyerMur(const std::string & where, const std::string & what) {
    int n = where.length(); // Длина исходной строки
    int m = what.length();  // Длина образца

    if (m == 0) return 0;
    if (n < m) return -1;

        //Формирование таблицы сдвигов
    std::vector<int> shifts(ALPHABET_SIZE);

        //сдвиг равен длине образца
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        shifts[i] = m;
    }

        // символов из образца сдвиг равен расстоянию от последнего вхождения в образец до конца образца.
    for (int i = 0; i < m - 1; i++) {
        shifts[hashChar(what[i])] = m - 1 - i;
    }

    for (int i = 0; i <= n - m; ) {
        int j = m - 1; // Начинаем сравнение с конца образца

            //цикл сравнивает символы справа налево
        while (j >= 0 && 
    }

    return -1;
}

int main() {
    setlocale(0, "");
    std::cout << knuthMorrisPratt("Алгоритм 10 из 11 бебра пива жии", "бебра") << std::endl;

    std::cout << RabinKarp("Алгоритм 10 из 11 бебра пива жии", "10") << std::endl;

    std::cout << BoyerMur("Алгоритм 10 из 11 бебра пива жии", "из 11") << std::endl;

    return 0;
}