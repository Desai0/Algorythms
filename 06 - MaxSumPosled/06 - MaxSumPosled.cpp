#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <utility>
#include <cmath>
#include <locale>
#include <algorithm>

// --- Knuth-Morris-Pratt ---

int knuthMorrisPratt(const std::string& where, const std::string& what, long long& iterations) {
    int n = where.length();
    int m = what.length();
    iterations = 0; // Сбрасываем счетчик

    if (m == 0) return 0;
    if (n < m) return -1;

    std::vector<int> table(m);
    table[0] = 0;
    int shift = 0;
    // Фаза 1: Построение префикс-таблицы (O(m))
    for (int q = 1; q < m; q++) {
        while (shift > 0 && what[shift] != what[q]) {
            iterations++; // Сравнение, приведшее к сдвигу
            shift = table[shift - 1];
        }
        iterations++; // Финальное сравнение (может быть успешным или неуспешным)
        if (what[shift] == what[q]) {
            shift++;
        }
        table[q] = shift;
    }

    shift = 0;
    // Фаза 2: Поиск в тексте (O(n))
    for (int i = 0; i < n; i++) {
        while (shift > 0 && what[shift] != where[i]) {
            iterations++; // Сравнение, приведшее к сдвигу
            shift = table[shift - 1];
        }
        iterations++; // Финальное сравнение
        if (what[shift] == where[i]) {
            shift++;
        }
        if (shift == m) {
            return i - m + 1;
        }
    }
    return -1;
}


// --- DFS (Поиск в глубину) ---


class Graph {
private:
    int V;
    std::vector<std::vector<std::pair<int, int>>> adj;

    void DFS_util(int u, std::vector<bool>& visited, long long& iterations) {
        visited[u] = true;
        // std::cout << u << " "; // Можно раскомментировать для вывода пути

        for (const auto& edge : adj[u]) {
            iterations++; // Считаем каждую попытку пройти по ребру
            int v = edge.first;
            if (!visited[v]) {
                DFS_util(v, visited, iterations);
            }
        }
    }
public:
    Graph(int vertices) : V(vertices) { adj.resize(V); }
    void addEdge(int u, int v, int weight) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u].push_back({ v, weight });
        }
    }
    void DFS(int startNode, long long& iterations) {
        iterations = 0; // Сбрасываем счетчик
        if (startNode < 0 || startNode >= V) return;
        std::vector<bool> visited(V, false);
        DFS_util(startNode, visited, iterations);
    }
};


// --- findMaxSumSequence ---
long long findMaxSumSequence(const std::vector<int>& arr, long long& iterations) {
    long long maxSum = 0;
    iterations = 0; // Сбрасываем счетчик
    for (int number : arr) {
        iterations++; // Считаем обработку одного элемента
        maxSum += std::abs(number);
    }
    return maxSum;
}




struct FrequencyInfo {
    int number;
    int count;
};

struct SortResult {
    std::vector<int> sortedArray;
    std::vector<FrequencyInfo> frequencies;
};

bool compareByFrequency(const FrequencyInfo& a, const FrequencyInfo& b) {
    if (a.count != b.count) {
        return a.count > b.count;
    }
    return a.number < b.number;
}

SortResult sortByFrequency(const std::vector<int>& arr) {
    if (arr.empty()) {
        return {}; // Возвращаем пустую структуру
    }

    // Сортируем копию, чтобы сгруппировать одинаковые элементы
    std::vector<int> sortedArr = arr;
    std::sort(sortedArr.begin(), sortedArr.end());

    // Считаем частоту каждого элемента
    std::vector<FrequencyInfo> tempFrequencies;
    int currentNumber = sortedArr[0];
    int currentCount = 1;

    for (size_t i = 1; i < sortedArr.size(); ++i) {
        if (sortedArr[i] == currentNumber) {
            currentCount++;
        }
        else {
            tempFrequencies.push_back({ currentNumber, currentCount });
            currentNumber = sortedArr[i];
            currentCount = 1;
        }
    }
    tempFrequencies.push_back({ currentNumber, currentCount });

    std::sort(tempFrequencies.begin(), tempFrequencies.end(), compareByFrequency);

    // финальный отсортированный массив на основе частот
    std::vector<int> finalSortedArray;
    for (const auto& info : tempFrequencies) {
        for (int i = 0; i < info.count; ++i) {
            finalSortedArray.push_back(info.number);
        }
    }

    return { finalSortedArray, tempFrequencies };
}

void printResult(const std::string& caseName, const std::vector<int>& data, const SortResult& result, long long duration_ns) {
    std::cout << "--- " << caseName << " ---" << std::endl;
    std::cout << "Исходный массив (" << data.size() << " эл.): ";
    for (int num : data) std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Время выполнения: " << duration_ns << " наносекунд" << std::endl;

    std::cout << "Отсортированный по частоте массив:" << std::endl;
    for (int num : result.sortedArray) {
        std::cout << num << " ";
    }
    std::cout << "\n" << std::endl;
}



void printSum(const std::vector<int>& numbers) {
    for (size_t i = 0; i < numbers.size(); ++i) {
        std::cout << numbers[i] << (i < numbers.size() - 1 ? " + " : "");
    }
    std::cout << std::endl;
}

void findPartitions(int n) {
    std::vector<int> p;
    p.push_back(n);

    while (true) {
        printSum(p);

        int right_index = -1;
        for (int i = p.size() - 1; i >= 0; --i) {
            if (p[i] != 1) {
                right_index = i;
                break;
            }
        }

        if (right_index == -1) {
            break;
        }

        p[right_index]--;

        int sum_of_ones = 0;
        for (size_t i = right_index + 1; i < p.size(); ++i) {
            sum_of_ones += p[i];
        }
        sum_of_ones++;

        p.erase(p.begin() + right_index + 1, p.end());

        int num_to_add = p[right_index];
        while (sum_of_ones > 0) {
            int piece = std::min(num_to_add, sum_of_ones);
            p.push_back(piece);
            sum_of_ones -= piece;
        }
    }
}


int main() {
    setlocale(LC_ALL, "Russian");
    long long iterations = 0;

    // ====================================================================
    // 1. Анализ Кнута-Морриса-Пратта
    // ====================================================================
    std::cout << "--- Анализ алгоритма Кнута-Морриса-Пратта ---\n Сложность: O(n + m)\nЛучший/средний/худший случаи имеют одинаковую сложность,\nно различаются по количеству сравнений.\n" << std::endl;

    {
        std::cout << "1. Лучший случай:" << std::endl;
        std::string text = "abcdefghijklmnopqrstuvwxyz";
        std::string pattern = "abc";


        auto start = std::chrono::high_resolution_clock::now();

        int result = knuthMorrisPratt(text, pattern, iterations);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        std::cout << "Время выполнения: " << duration.count() << " nanoseconds \n" << std::endl;

        
        std::cout << "   Текст: '" << text << "', Шаблон: '" << pattern << "'" << std::endl;
        std::cout << "   Результат: " << result << ", Итераций (сравнений): " << iterations << "\n" << std::endl;
    }

    {
        std::cout << "2. Средний случай:" << std::endl;
        std::string text = "Алгоритм 10 из 11 бебра пива жии";
        std::string pattern = "бебра";

        auto start1 = std::chrono::high_resolution_clock::now();

        int result1 = knuthMorrisPratt(text, pattern, iterations);

        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1);

        std::cout << "Время выполнения: " << duration1.count() << " nanoseconds \n" << std::endl;


        std::cout << "   Текст: '" << text << "', Шаблон: '" << pattern << "'" << std::endl;
        std::cout << "   Результат: " << result1 << ", Итераций (сравнений): " << iterations << "\n" << std::endl;
    }

    {
        std::cout << "3. Худший случай:" << std::endl;
        std::string text = "aaaaaaaaaaaaaaaaaaaaab"; // 21 'a' и 'b'
        std::string pattern = "aaaaab"; // 5 'a' и 'b'

        auto start2 = std::chrono::high_resolution_clock::now();

        int result2 = knuthMorrisPratt(text, pattern, iterations);

        auto end2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2);

        std::cout << "Время выполнения: " << duration2.count() << " nanoseconds \n" << std::endl;

        std::cout << "   Текст: '" << text << "', Шаблон: '" << pattern << "'" << std::endl;
        std::cout << "   Результат: " << result2 << ", Итераций (сравнений): " << iterations << "\n\n\n" << std::endl;
    }


    // ====================================================================
    // 2. Анализ Поиска в глубину (DFS)
    // ====================================================================
    std::cout << "--- Анализ алгоритма Поиска в глубину (DFS) ---\nСложность: O(V + E)\nАлгоритм всегда посещает каждую вершину и каждое ребро один раз\nЛучший и худший случаи определяются структурой\n" << std::endl;

    {
        std::cout << "1. Разреженный граф:" << std::endl;

        auto start3 = std::chrono::high_resolution_clock::now();

        const int numVertices = 10;
        Graph g_sparse(numVertices);
        for (int i = 0; i < numVertices - 1; ++i) {
            g_sparse.addEdge(i, i + 1, 1);
        }
        g_sparse.DFS(0, iterations);

        auto end3 = std::chrono::high_resolution_clock::now();
        auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3);

        std::cout << "Время выполнения: " << duration3.count() << " микросекунд \n" << std::endl;
        std::cout << "   Вершин (V): " << numVertices << ", Ребер (E): " << numVertices - 1 << std::endl;
        std::cout << "   Итераций: " << iterations << " (близко к V+E)\n" << std::endl;
    }

    {
        std::cout << "2. Плотный граф:" << std::endl;

        auto start4 = std::chrono::high_resolution_clock::now();

        const int numVertices = 10;
        int numEdges = 0;
        Graph g_dense(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                if (i != j) {
                    g_dense.addEdge(i, j, 1);
                    numEdges++;
                }
            }
        }
        g_dense.DFS(0, iterations);

        auto end4 = std::chrono::high_resolution_clock::now();
        auto duration4 = std::chrono::duration_cast<std::chrono::microseconds>(end4 - start4);

        std::cout << "Время выполнения: " << duration4.count() << " микросекунд \n" << std::endl;

        std::cout << "   Вершин (V): " << numVertices << ", Ребер (E): " << numEdges << std::endl;
        std::cout << "   Итераций (обходов ребер): " << iterations << " (близко к V+E)\n\n\n" << std::endl;
    }


    // ====================================================================
    // 3. Анализ findMaxSumSequence
    // ====================================================================
    std::cout << "--- Анализ алгоритма findMaxSumSequence ---\nСложность: O(N)\nАлгоритм проходит по всему массиву один раз\nОДИНАКОВОЕ количество итераций\n" << std::endl;

    // Для этого алгоритма количество итераций равно размеру массива
    {
        std::vector<int> arr1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        std::vector<int> arr2 = { -1, 2, -3, 4, -5, 6, -7, 8, -9, 10 };
        std::vector<int> arr3 = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };

        std::cout << "1. Массив из положительных чисел:" << std::endl;
        auto start5 = std::chrono::high_resolution_clock::now();
        findMaxSumSequence(arr1, iterations);
        auto end5 = std::chrono::high_resolution_clock::now();
        auto duration5 = std::chrono::duration_cast<std::chrono::nanoseconds>(end5 - start5);
        std::cout << "Время выполнения: " << duration5.count() << " nanoseconds \n" << std::endl;
        std::cout << "   Размер массива: " << arr1.size() << ", Итераций: " << iterations << std::endl;

        std::cout << "2. Массив из смешанных чисел:" << std::endl;
        auto start6 = std::chrono::high_resolution_clock::now();
        findMaxSumSequence(arr2, iterations);
        auto end6 = std::chrono::high_resolution_clock::now();
        auto duration6 = std::chrono::duration_cast<std::chrono::nanoseconds>(end6 - start6);
        std::cout << "Время выполнения: " << duration6.count() << " nanoseconds \n" << std::endl;
        std::cout << "   Размер массива: " << arr2.size() << ", Итераций: " << iterations << std::endl;

        std::cout << "3. Массив из отрицательных чисел:" << std::endl;
        auto start7 = std::chrono::high_resolution_clock::now();
        findMaxSumSequence(arr3, iterations);
        auto end7 = std::chrono::high_resolution_clock::now();
        auto duration7 = std::chrono::duration_cast<std::chrono::nanoseconds>(end7 - start7);
        std::cout << "Время выполнения: " << duration7.count() << " nanoseconds \n" << std::endl;
        std::cout << "   Размер массива: " << arr3.size() << ", Итераций: " << iterations << "\n" << std::endl;
    }


    // ====================================================================
    // 4. Анализ sortByFrequency
    // ====================================================================
    std::cout << "--- Анализ алгоритма sortByFrequency ---\n"
        << "Сложность: O(N log N) из-за начальной сортировки.\n" << std::endl;

    // --- Лучший случай ---
    // Массив почти отсортирован и содержит мало уникальных элементов.
    {
        std::vector<int> best_case_data = { 1, 1, 1, 1, 2, 2, 2, 3, 3, 3};

        auto start = std::chrono::high_resolution_clock::now();
        SortResult result = sortByFrequency(best_case_data);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        printResult("Лучший случай", best_case_data, result, duration.count());
    }

    // --- Средний случай ---
    // Случайный массив с повторениями.
    {
        std::vector<int> average_case_data = { 4, 1, 5, 2, 1, 5, 5, 1, 4, 8, 5, 9 };

        auto start = std::chrono::high_resolution_clock::now();
        SortResult result = sortByFrequency(average_case_data);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        printResult("Средний случай", average_case_data, result, duration.count());
    }

    // --- Худший случай ---
    // Массив отсортирован в обратном порядке, и все элементы уникальны.
    {
        std::vector<int> worst_case_data = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

        auto start = std::chrono::high_resolution_clock::now();
        SortResult result = sortByFrequency(worst_case_data);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        printResult("Худший случай", worst_case_data, result, duration.count());
    }

    // ====================================================================
    // 5. Анализ findPartitions
    // ====================================================================
    // Экспоненциальная сложность потому что самих комбинаций много

    int number = 423;

    findPartitions(number);


    return 0;
}