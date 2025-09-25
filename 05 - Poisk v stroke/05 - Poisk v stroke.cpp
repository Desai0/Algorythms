#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <utility>


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
        // while (j >= 0 && 
    }

    return -1;
}


class Graph {
private:
    int V;
    //вектор пар {v, w},
    // v соседняя вершина, а w - вес ребра
    std::vector<std::vector<std::pair<int, int>>> adj;

    void DFS_util(int u, std::vector<bool>& visited) {
        // посещенная и выводим ее
        visited[u] = true;
        std::cout << u << " ";

        for (const auto& edge : adj[u]) {
            int v = edge.first; // Соседняя вершина
            if (!visited[v]) {
                DFS_util(v, visited);
            }
        }
    }
public:
    // Конструктор
    Graph(int vertices) : V(vertices) {
        adj.resize(V);
    }

    // Добавление ребра от u к v с весом w
    void addEdge(int u, int v, int weight) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u].push_back({ v, weight });
            // Если граф неориентированный, добавьте также:
            // adj[v].push_back({u, weight});
        }
    }

    // Публичный метод для запуска поиска в глубину из заданной вершины
    void DFS(int startNode) {
        if (startNode < 0 || startNode >= V) {
            std::cout << "Стартовая вершина некорректна." << std::endl;
            return;
        }

        std::vector<bool> visited(V, false); // Инициализируем все вершины как непосещенные

        std::cout << "Порядок обхода в глубину (DFS), начиная с вершины " << startNode << ": ";
        DFS_util(startNode, visited);
        std::cout << std::endl;
    }
};

long long findMaxSumSequence(const std::vector<int>& arr) {
    long long maxSum = 0;
    for (int number : arr) {
        maxSum += std::abs(number);
    }

    return maxSum;
}



int main() {
    setlocale(0, "");

    const std::string text = "Алгоритм 10 из 11 бебра пива жии";

    {
        const std::string pattern = "бебра";
        std::cout << "Алгоритм Кнута-Морриса-Пратта" << std::endl;
        std::cout << "Ищем '" << pattern << "' в '" << text << "'" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        int result = knuthMorrisPratt(text, pattern);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Результат (индекс): " << result << std::endl;
        std::cout << "Время выполнения: " << duration.count() << " микросекунд" << std::endl;
        std::cout << std::endl;
    }

    const int numVertices = 7;
    Graph g(numVertices);

    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 5);
    g.addEdge(0, 4, 15);
    g.addEdge(1, 3, 7);
    g.addEdge(2, 3, 3);
    g.addEdge(3, 5, 9);
    g.addEdge(4, 2, 1);
    g.addEdge(4, 6, 8);
    g.addEdge(5, 4, 2);

    std::cout << "Алгоритм Поиска в глубину (DFS)" << std::endl;
    std::cout << "Граф с " << numVertices << " вершинами создан." << std::endl;

    int startVertex = 0;
    auto start = std::chrono::high_resolution_clock::now();

    g.DFS(startVertex); //обход графа

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Время выполнения: " << duration.count() << " микросекунд \n" << std::endl;


    

    std::vector<int> arr = { -10, 2, 8, -5, 0, 7 };
    auto start2 = std::chrono::high_resolution_clock::now();
    long long result = findMaxSumSequence(arr);
    auto end2 = std::chrono::high_resolution_clock::now();

    auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2);

    std::cout << "Макс сумма: " << result << std::endl;
    std::cout << "Время: " << duration2.count() << "наносек" << std::endl;

    // std::cout << RabinKarp("Алгоритм 10 из 11 бебра пива жии", "10") << std::endl;

    // std::cout << BoyerMur("Алгоритм 10 из 11 бебра пива жии", "из 11") << std::endl;

    return 0;
}