#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <set>

std::vector<int> generateSortedUniqueRandomArray(int size, int min, int max) {
    std::set<int> unique_numbers;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    while (unique_numbers.size() < size) {
        unique_numbers.insert(distrib(gen));
    }

    std::vector<int> result(unique_numbers.begin(), unique_numbers.end());
    return result;
}

int interpolationSearch(const std::vector<int>& arr, int key) {
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high && key >= arr[low] && key <= arr[high]) {
        if (low == high) {
            if (arr[low] == key) return low;
            return -1;
        }

        int pos = low + (((double)(high - low) / (arr[high] - arr[low])) * (key - arr[low]));

        if (arr[pos] == key) {
            return pos;
        }

        if (arr[pos] < key) {
            low = pos + 1;
        }
        else {
            high = pos - 1;
        }
    }
    return -1;
}

// Реализация бинарного поиска
int binarySearch(const std::vector<int>& arr, int key) {
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == key) {
            return mid;
        }

        if (arr[mid] < key) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return -1;
}

// Функция для тестирования и вывода результатов
void run_tests_on_array(const std::vector<int>& arr, const std::string& arr_name) {
    std::cout << "=======================================================\n";
    std::cout << "Тестирование на '" << arr_name << "' (размер: " << arr.size() << ")\n";
    std::cout << "=======================================================\n\n";

    // --- Сценарий 1: Элемент существует в массиве ---
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, arr.size() - 1);
    int key_to_find = arr[dist(gen)]; // Выбираем случайный существующий элемент

    std::cout << "--- Сценарий 1: Поиск существующего элемента (" << key_to_find << ") ---\n";

    // Тестируем интерполяционный поиск
    auto start_inter = std::chrono::high_resolution_clock::now();
    int result_inter = interpolationSearch(arr, key_to_find);
    auto end_inter = std::chrono::high_resolution_clock::now();
    auto duration_inter = std::chrono::duration_cast<std::chrono::nanoseconds>(end_inter - start_inter);

    std::cout << "Интерполяционный поиск:\t";
    if (result_inter != -1) {
        std::cout << "Найден в позиции " << result_inter;
    }
    else {
        std::cout << "Не найден";
    }
    std::cout << "\t(Время: " << duration_inter.count() << " нс)\n";

    // Тестируем бинарный поиск
    auto start_bin = std::chrono::high_resolution_clock::now();
    int result_bin = binarySearch(arr, key_to_find);
    auto end_bin = std::chrono::high_resolution_clock::now();
    auto duration_bin = std::chrono::duration_cast<std::chrono::nanoseconds>(end_bin - start_bin);

    std::cout << "Бинарный поиск:\t\t";
    if (result_bin != -1) {
        std::cout << "Найден в позиции " << result_bin;
    }
    else {
        std::cout << "Не найден";
    }
    std::cout << "\t(Время: " << duration_bin.count() << " нс)\n\n";

    // --- Сценарий 2: Элемент не существует в массиве ---
    int key_not_found = 0;
    std::cout << "--- Сценарий 2: Поиск несуществующего элемента (" << key_not_found << ") ---\n";

    // Тестируем интерполяционный поиск
    start_inter = std::chrono::high_resolution_clock::now();
    result_inter = interpolationSearch(arr, key_not_found);
    end_inter = std::chrono::high_resolution_clock::now();
    duration_inter = std::chrono::duration_cast<std::chrono::nanoseconds>(end_inter - start_inter);

    std::cout << "Интерполяционный поиск:\t";
    if (result_inter != -1) {
        std::cout << "Найден в позиции " << result_inter;
    }
    else {
        std::cout << "Не найден";
    }
    std::cout << "\t(Время: " << duration_inter.count() << " нс)\n";

    // Тестируем бинарный поиск
    start_bin = std::chrono::high_resolution_clock::now();
    result_bin = binarySearch(arr, key_not_found);
    end_bin = std::chrono::high_resolution_clock::now();
    duration_bin = std::chrono::duration_cast<std::chrono::nanoseconds>(end_bin - start_bin);

    std::cout << "Бинарный поиск:\t\t";
    if (result_bin != -1) {
        std::cout << "Найден в позиции " << result_bin;
    }
    else {
        std::cout << "Не найден";
    }
    std::cout << "\t(Время: " << duration_bin.count() << " нс)\n\n";
}

int main() {
    setlocale(LC_ALL, "Russian");

    // Генерация трех массивов
    std::vector<int> arr1 = generateSortedUniqueRandomArray(500, 1, 1000);
    std::vector<int> arr2 = generateSortedUniqueRandomArray(10000, 1, 100000);
    std::vector<int> arr3 = generateSortedUniqueRandomArray(1000000, 1, 10000000);

    // Запускаем тесты для каждого массива
    run_tests_on_array(arr1, "Массив из 50 элементов");
    run_tests_on_array(arr2, "Массив из 1000 элементов");
    run_tests_on_array(arr3, "Массив из 10000 элементов");

    return 0;
}