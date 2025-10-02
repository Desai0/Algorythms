#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>

using Point = std::vector<double>;

double euclidean_distance(const Point& a, const Point& b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(sum);
}

// --- Алгоритм K-Средних ---
std::vector<int> k_means(const std::vector<Point>& data, int k, int max_iterations) {
    if (data.empty() || k <= 0) return {};

    int n_points = data.size();
    int n_dims = data[0].size();

    //инициализация центроидов случайным выбором K точек из данных
    std::vector<Point> centroids;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> indices(n_points);
    for (int i = 0; i < n_points; ++i) indices[i] = i;
    std::shuffle(indices.begin(), indices.end(), gen);
    for (int i = 0; i < k; ++i) {
        centroids.push_back(data[indices[i]]);
    }

    std::vector<int> clusters(n_points);

    for (int iter = 0; iter < max_iterations; ++iter) {
        //каждая точка относится к ближайшему центроиду.
        for (int i = 0; i < n_points; ++i) {
            double min_dist = std::numeric_limits<double>::max();
            int best_cluster = 0;
            for (int j = 0; j < k; ++j) {
                double dist = euclidean_distance(data[i], centroids[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                    best_cluster = j;
                }
            }
            clusters[i] = best_cluster;
        }

        // пересчитываем центроиды как центр масс точек в кластере
        std::vector<Point> new_centroids(k, Point(n_dims, 0.0));
        std::vector<int> counts(k, 0);
        for (int i = 0; i < n_points; ++i) {
            int cluster_id = clusters[i];
            for (int d = 0; d < n_dims; ++d) {
                new_centroids[cluster_id][d] += data[i][d];
            }
            counts[cluster_id]++;
        }
        for (int j = 0; j < k; ++j) {
            if (counts[j] > 0) {
                for (int d = 0; d < n_dims; ++d) {
                    new_centroids[j][d] /= counts[j];
                }
            }
        }

        if (new_centroids == centroids) {
            break;
        }
        centroids = new_centroids;
    }

    return clusters;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<Point> dataset = {
        {1.0, 2.0}, {1.5, 1.8}, {1.2, 2.3},
        {8.0, 8.0}, {8.5, 7.5}, {9.0, 8.2},
        {1.0, 9.0}, {1.8, 8.5}, {0.8, 9.2}
    };

    int k = 3;
    int max_iter = 100;

    std::vector<int> result_clusters = k_means(dataset, k, max_iter);

    std::cout << "--- Результаты кластеризации K-Means ---\n";
    for (size_t i = 0; i < dataset.size(); ++i) {
        std::cout << "Точка (" << dataset[i][0] << ", " << dataset[i][1] << ") -> Кластер " << result_clusters[i] << "\n";
    }

    return 0;
}