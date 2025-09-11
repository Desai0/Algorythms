#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

// --- ОБЩИЕ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ---

void printVector(const std::vector<int>& vec, size_t limit = 15) {
    for (size_t i = 0; i < std::min(vec.size(), limit); ++i) {
        std::cout << vec[i] << " ";
    }
    if (vec.size() > limit) {
        std::cout << "...";
    }
    std::cout << std::endl;
}

// --- БЛОК 1: РЕАЛИЗАЦИИ АЛГОРИТМОВ ---
namespace BubbleSort {
    // Оптимизированная версия, которая останавливается, если массив уже отсортирован
    void sort(std::vector<int>& arr) {
        int n = arr.size();
        bool swapped;
        for (int i = 0; i < n - 1; i++) {
            swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            // Если на внутреннем цикле не было ни одной замены, массив отсортирован
            if (!swapped) {
                break;
            }
        }
    }
}

// 1.2 Квадратичным выбором
namespace SqrtSort {
    void sort(std::vector<int>& arr) {
        int n = arr.size();
        if (n == 0) return;

        // 1. Разделение на блоки
        int blockSize = static_cast<int>(sqrt(n));
        int blockCount = (n + blockSize - 1) / blockSize; // ceil(n / blockSize)

        std::vector<int> result;
        result.reserve(n);

        // Вспомогательный массив для хранения ИНДЕКСОВ минимальных элементов в каждом блоке
        std::vector<int> blockMins(blockCount, -1);
        const int infinity = std::numeric_limits<int>::max();

        // Функция для поиска нового минимума в "грязном" блоке
        auto findMinInBlock = [&](int blockIndex) {
            int start = blockIndex * blockSize;
            int end = std::min((blockIndex + 1) * blockSize, n);
            int minVal = infinity;
            int minIdx = -1;

            for (int i = start; i < end; ++i) {
                if (arr[i] < minVal) {
                    minVal = arr[i];
                    minIdx = i;
                }
            }
            blockMins[blockIndex] = minIdx;
            };

        // 2. Предварительный расчет: находим минимумы во всех блоках
        for (int i = 0; i < blockCount; ++i) {
            findMinInBlock(i);
        }

        // 3. Основной цикл: n раз выбираем глобальный минимум
        for (int i = 0; i < n; ++i) {
            int globalMinVal = infinity;
            int globalMinIdx = -1;
            int dirtyBlockIdx = -1;

            for (int j = 0; j < blockCount; ++j) {
                if (blockMins[j] != -1 && arr[blockMins[j]] < globalMinVal) {
                    globalMinVal = arr[blockMins[j]];
                    globalMinIdx = blockMins[j];
                    dirtyBlockIdx = j;
                }
            }

            result.push_back(globalMinVal);

            arr[globalMinIdx] = infinity;
            findMinInBlock(dirtyBlockIdx);
        }

        arr = result;
    }
}


// 1.1 Быстрая сортировка (Quicksort)
namespace QuickSort {
    int partition(std::vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return (i + 1);
    }

    void sort(std::vector<int>& arr, int low, int high) {
        while (low < high) {
            int pi = partition(arr, low, high);
            if (pi - low < high - pi) {
                sort(arr, low, pi - 1);
                low = pi + 1;
            }
            else {
                sort(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
}

// 1.2 Линейный поиск (Перебор для поиска максимума)
int linearSearchForMax(const std::vector<int>& arr) {
    if (arr.empty()) return -1;
    int maxElement = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > maxElement) {
            maxElement = arr[i];
        }
    }
    return maxElement;
}

// 1.3 Бинарный поиск
int binarySearch(const std::vector<int>& sorted_arr, int target) {
    int low = 0;
    int high = sorted_arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (sorted_arr[mid] == target) return mid;
        if (sorted_arr[mid] < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// --- БЛОК 2: ДЕМОНСТРАЦИОННЫЕ ФУНКЦИИ ---

void demonstrateQuicksort() {
    const int N = 400;
    std::cout << "--- Алгоритм #4: Быстрая сортировка O(n*log(n)) ---" << std::endl;

    std::cout << "\n1. Средний/Лучший случай: O(n*log(n)) на случайных данных" << std::endl;
    std::vector<int> random_arr(N);
    std::generate(random_arr.begin(), random_arr.end(), [n = 0]() mutable { return n++; });
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(random_arr.begin(), random_arr.end(), g);

    auto start_avg = std::chrono::high_resolution_clock::now();
    QuickSort::sort(random_arr, 0, N - 1);
    auto end_avg = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_avg = end_avg - start_avg;
    std::cout.precision(4);
    std::cout << "Время выполнения: " << std::fixed << duration_avg.count() << " секунд." << std::endl;

    // Худший случай
    std::cout << "\n2. Худший случай: O(n^2) на отсортированных данных" << std::endl;
    std::vector<int> sorted_arr(N);
    std::generate(sorted_arr.begin(), sorted_arr.end(), [n = 0]() mutable { return n++; });

    auto start_worst = std::chrono::high_resolution_clock::now();
    QuickSort::sort(sorted_arr, 0, N - 1);
    auto end_worst = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_worst = end_worst - start_worst;
    std::cout << "Время выполнения: " << std::fixed << duration_worst.count() << " секунд." << std::endl;
    std::cout << "Сравнение: Худший случай оказался в " << (duration_worst.count() / duration_avg.count()) << " раз медленнее среднего!" << std::endl;
}

void demonstrateLinearScan() {
    const int N = 20000000;
    std::cout << "--- Алгоритм #5: Перебор элементов O(n) ---" << std::endl;
    std::cout << "\nлучший, средний и худший случаи совпадают O(n)," << std::endl;
    std::cout << "так как необходимо просмотреть все элементы." << std::endl;

    std::vector<int> arr(N);
    std::generate(arr.begin(), arr.end(), [n = 0]() mutable { return n++; });
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr.begin(), arr.end(), g);

    auto start = std::chrono::high_resolution_clock::now();
    int max_val = linearSearchForMax(arr);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Найден максимум " << max_val << " в массиве из " << N << " элементов." << std::endl;
    std::cout.precision(4);
    std::cout << "Время выполнения: " << std::fixed << duration.count() << " секунд." << std::endl;
}

void demonstrateBinarySearch() {
    const long N = 900000000;
    std::cout << "--- Алгоритм #6: Бинарный поиск O(log n) ---" << std::endl;
    std::cout << "\nВажное условие: алгоритм работает только на отсортированных данных!" << std::endl;

    std::vector<int> arr(N);
    std::generate(arr.begin(), arr.end(), [n = 0]() mutable { return n++; });

    // Лучший случай: искомый элемент в центре
    std::cout << "\n1. Лучший случай: O(1)" << std::endl;
    int target_best = N / 2;
    auto start_best = std::chrono::high_resolution_clock::now();
    int index_best = binarySearch(arr, target_best);
    auto end_best = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_best = end_best - start_best;
    std::cout << "Ищем " << target_best << ", найден на позиции " << index_best << "." << std::endl;
    std::cout << "Время выполнения: " << std::fixed << duration_best.count() << " секунд (почти мгновенно)." << std::endl;

    // Худший случай
    std::cout << "\n2. Худший случай: O(log n)" << std::endl;
    int target_worst = -1; // Элемента точно нет в массиве
    auto start_worst = std::chrono::high_resolution_clock::now();
    int index_worst = binarySearch(arr, target_worst);
    auto end_worst = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_worst = end_worst - start_worst;
    std::cout << "Ищем " << target_worst << ", результат: " << index_worst << std::endl;
    std::cout << "Время выполнения: " << std::fixed << duration_worst.count() << " секунд (все еще почти мгновенно)." << std::endl;
}

void demonstrateBubbleSort() {
    const int N = 10000;
    std::cout << "--- Алгоритм: Сортировка пузырьком O(n^2) ---" << std::endl;

    // Лучший случай: O(n)
    std::cout << "\n1. Лучший случай: O(n) на уже отсортированных данных" << std::endl;
    std::vector<int> best_case_arr(N);
    std::generate(best_case_arr.begin(), best_case_arr.end(), [n = 0]() mutable { return n++; });

    auto start_best = std::chrono::high_resolution_clock::now();
    BubbleSort::sort(best_case_arr);
    auto end_best = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_best = end_best - start_best;
    std::cout.precision(4);
    std::cout << "Время выполнения: " << std::fixed << duration_best.count() << " секунд (почти мгновенно)." << std::endl;

    // Худший случай: O(n^2)
    std::cout << "\n2. Худший случай: O(n^2) на обратно отсортированных данных" << std::endl;
    std::vector<int> worst_case_arr(N);
    std::generate(worst_case_arr.begin(), worst_case_arr.end(), [n = 0]() mutable { return n++; });
    std::reverse(worst_case_arr.begin(), worst_case_arr.end());

    auto start_worst = std::chrono::high_resolution_clock::now();
    BubbleSort::sort(worst_case_arr);
    auto end_worst = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_worst = end_worst - start_worst;
    std::cout << "Время выполнения: " << std::fixed << duration_worst.count() << " секунд." << std::endl;
    std::cout << "Сравнение: Худший случай оказался в " << (duration_worst.count() / duration_best.count()) << " раз медленнее среднего!" << std::endl;
}

void demonstrateSqrtSort() {
    const int N = 10000;
    std::cout << "--- Алгоритм: Квадратичный выбор (Сортировка методом sqrt-декомпозиции) O(n*sqrt(n)) ---" << std::endl;

    // Лучший случай: O(n*sqrt(n))
    std::cout << "\n1. Лучший случай: O(n*sqrt(n)) на уже отсортированных данных" << std::endl;
    std::vector<int> best_case_arr(N);
    std::generate(best_case_arr.begin(), best_case_arr.end(), [n = 0]() mutable { return n++; });

    auto start_best = std::chrono::high_resolution_clock::now();
    SqrtSort::sort(best_case_arr);
    auto end_best = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_best = end_best - start_best;
    std::cout.precision(4);
    std::cout << "Время выполнения: " << std::fixed << duration_best.count() << " секунд." << std::endl;

    // Худший случай: O(n*sqrt(n))
    std::cout << "\n2. Худший случай: O(n*sqrt(n)) на обратно отсортированных данных" << std::endl;
    std::vector<int> worst_case_arr(N);
    std::generate(worst_case_arr.begin(), worst_case_arr.end(), [n = 0]() mutable { return n++; });
    std::reverse(worst_case_arr.begin(), worst_case_arr.end());

    auto start_worst = std::chrono::high_resolution_clock::now();
    SqrtSort::sort(worst_case_arr);
    auto end_worst = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_worst = end_worst - start_worst;
    std::cout << "Время выполнения: " << std::fixed << duration_worst.count() << " секунд." << std::endl;
    std::cout << "Сравнение: Худший случай оказался в " << (duration_worst.count() / duration_best.count()) << " раз медленнее среднего!" << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    demonstrateQuicksort();

    std::cout << "\n\n======================================================\n\n";

    demonstrateBubbleSort();

    std::cout << "\n\n======================================================\n\n";

    demonstrateSqrtSort();

    std::cout << "\n\n======================================================\n\n";

    demonstrateLinearScan();

    std::cout << "\n\n======================================================\n\n";

    demonstrateBinarySearch();

    return 0;
}