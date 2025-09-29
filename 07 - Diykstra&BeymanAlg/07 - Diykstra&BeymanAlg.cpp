#include <iostream>
#include <vector>
#include <queue>
#include <limits>

const int INF = std::numeric_limits<int>::max();
// ребро
struct Edge {
    int to;
    int weight;
};

// граф в виде списка смежности
using Graph = std::vector<std::vector<Edge>>;

void print_distances(const std::vector<int>& dist, int startNode) {
    std::cout << "Кратчайшие пути от вершины " << startNode << ":\n";
    for (int i = 0; i < dist.size(); ++i) {
        if (dist[i] == INF) {
            std::cout << "  До вершины " << i << ": путь недостижим\n";
        }
        else {
            std::cout << "  До вершины " << i << ": " << dist[i] << "\n";
        }
    }
}

// --- Алгоритм Дейкстры ---
void dijkstra(const Graph& graph, int startNode) {
    int V = graph.size();
    std::vector<int> dist(V, INF);
    dist[startNode] = 0;

    // очередь с приоритетом для жадного выбора ближайшей непроверенной вершины
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({ 0, startNode }); // {расстояние, вершина}

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) {
            continue;
        }

        for (const auto& edge : graph[u]) {
            int v = edge.to;
            int weight = edge.weight;
            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({ dist[v], v });
            }
        }
    }
    std::cout << "\n--- Результат алгоритма Дейкстры ---\n";
    print_distances(dist, startNode);
}

// --- Алгоритм Беллмана-Форда ---
void bellmanFord(const Graph& graph, int startNode) {
    int V = graph.size();
    std::vector<int> dist(V, INF);
    dist[startNode] = 0;

    // V-1 раз ослабляем все рёбра графа для нахождения кратчайших путей
    for (int i = 0; i < V - 1; ++i) {
        for (int u = 0; u < V; ++u) {
            for (const auto& edge : graph[u]) {
                int v = edge.to;
                int weight = edge.weight;
                if (dist[u] != INF && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    bool hasNegativeCycle = false;
    for (int u = 0; u < V; ++u) {
        for (const auto& edge : graph[u]) {
            int v = edge.to;
            int weight = edge.weight;
            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                hasNegativeCycle = true;
                break;
            }
        }
        if (hasNegativeCycle) break;
    }

    std::cout << "\n--- Результат алгоритма Беллмана-Форда ---\n";
    if (hasNegativeCycle) {
        std::cout << "Обнаружен отрицательный цикл! Кратчайших путей не существует.\n";
    }
    else {
        print_distances(dist, startNode);
    }
}


int main() {
    setlocale(LC_ALL, "Russian");

    // стандартный граф с положительными весами ---
    std::cout << "=================================================\n";
    std::cout << "Пример 1: Граф с положительными весами\n";
    std::cout << "=================================================\n";
    int V1 = 5;
    Graph graph1(V1);
    graph1[0].push_back({ 1, 10 });
    graph1[0].push_back({ 2, 3 });
    graph1[1].push_back({ 2, 1 });
    graph1[1].push_back({ 3, 2 });
    graph1[2].push_back({ 1, 4 });
    graph1[2].push_back({ 3, 8 });
    graph1[2].push_back({ 4, 2 });
    graph1[3].push_back({ 4, 7 });
    graph1[4].push_back({ 3, 9 });

    int startNode1 = 0;
    dijkstra(graph1, startNode1);
    bellmanFord(graph1, startNode1);

    // граф с отрицательным весом (но без отрицательных циклов) ---
    std::cout << "\n=============================================================\n";
    std::cout << "Пример 2: Граф с отрицательным весом (без циклов)\n";
    std::cout << "=============================================================\n";
    int V2 = 5;
    Graph graph2(V2);
    graph2[0].push_back({ 1, 1 });
    graph2[0].push_back({ 2, 10 });
    graph2[1].push_back({ 3, 2 });
    graph2[2].push_back({ 1, -12 });
    graph2[3].push_back({ 4, 3 });

    int startNode2 = 0;
    std::cout << "Запускаем только Беллмана-Форда, так как Дейкстра некорректен для таких графов.";
    bellmanFord(graph2, startNode2);

    //нраф с циклом отрицательного веса ---
    std::cout << "\n========================================================\n";
    std::cout << "Пример 3: Граф с отрицательным циклом\n";
    std::cout << "========================================================\n";
    int V3 = 4;
    Graph graph3(V3);
    graph3[0].push_back({ 1, 1 });
    graph3[1].push_back({ 2, 2 });
    graph3[2].push_back({ 3, -5 });
    graph3[3].push_back({ 1, 1 });

    int startNode3 = 0;
    std::cout << "Запускаем только Беллмана-Форда для обнаружения цикла.";
    bellmanFord(graph3, startNode3);

    return 0;
}