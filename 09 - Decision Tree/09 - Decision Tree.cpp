#include <iostream>
#include <vector>
#include <numeric>
#include <map>
#include <algorithm>
#include <memory>

// Типы данных для удобства
using Features = std::vector<double>;
using Dataset = std::vector<Features>;
using Labels = std::vector<int>;

// Структура для узла дерева
struct Node {
    // Для узлов ветвления (internal nodes)
    int feature_index = -1;
    double threshold = 0.0;
    std::unique_ptr<Node> left = nullptr;
    std::unique_ptr<Node> right = nullptr;

    // Для листовых узлов (leaf nodes)
    int predicted_class = -1;

    bool is_leaf() const {
        return left == nullptr && right == nullptr;
    }
};

class DecisionTree {
public:
    void train(const Dataset& X, const Labels& y, int max_depth = 5) {
        this->max_depth = max_depth;
        root = build_tree(X, y, 0);
    }

    int predict(const Features& features) const {
        Node* current = root.get();
        while (!current->is_leaf()) {
            if (features[current->feature_index] < current->threshold) {
                current = current->left.get();
            }
            else {
                current = current->right.get();
            }
        }
        return current->predicted_class;
    }

private:
    std::unique_ptr<Node> root;
    int max_depth;

    // Рекурсивная функция построения дерева
    std::unique_ptr<Node> build_tree(const Dataset& X, const Labels& y, int depth) {
        auto leaf_node = std::make_unique<Node>();
        leaf_node->predicted_class = most_common_class(y);

        // Условия остановки рекурсии (создание листа)
        if (depth >= max_depth || all_same_class(y)) {
            return leaf_node;
        }

        int best_feature;
        double best_threshold;
        double best_gini = 1.0;

        int n_features = X[0].size();

        // Находим лучший признак и порог для разделения данных
        for (int i = 0; i < n_features; ++i) {
            for (const auto& row : X) {
                double threshold = row[i];
                auto [left_X, left_y, right_X, right_y] = split(X, y, i, threshold);

                if (left_y.empty() || right_y.empty()) continue;

                double gini = weighted_gini(left_y, right_y);
                if (gini < best_gini) {
                    best_gini = gini;
                    best_feature = i;
                    best_threshold = threshold;
                }
            }
        }

        // Если разделение не улучшает чистоту, делаем лист
        if (best_gini == 1.0) {
            return leaf_node;
        }

        auto [left_X, left_y, right_X, right_y] = split(X, y, best_feature, best_threshold);

        auto internal_node = std::make_unique<Node>();
        internal_node->feature_index = best_feature;
        internal_node->threshold = best_threshold;
        internal_node->left = build_tree(left_X, left_y, depth + 1);
        internal_node->right = build_tree(right_X, right_y, depth + 1);

        return internal_node;
    }

    // Вспомогательные функции
    double gini_impurity(const Labels& y) const {
        if (y.empty()) return 0.0;
        std::map<int, int> counts;
        for (int label : y) counts[label]++;

        double impurity = 1.0;
        for (auto const& [label, count] : counts) {
            double prob = static_cast<double>(count) / y.size();
            impurity -= prob * prob;
        }
        return impurity;
    }

    double weighted_gini(const Labels& left_y, const Labels& right_y) const {
        double total_size = left_y.size() + right_y.size();
        double wg = (left_y.size() / total_size) * gini_impurity(left_y) +
            (right_y.size() / total_size) * gini_impurity(right_y);
        return wg;
    }

    auto split(const Dataset& X, const Labels& y, int feature_idx, double threshold) const {
        Dataset left_X, right_X;
        Labels left_y, right_y;
        for (size_t i = 0; i < X.size(); ++i) {
            if (X[i][feature_idx] < threshold) {
                left_X.push_back(X[i]);
                left_y.push_back(y[i]);
            }
            else {
                right_X.push_back(X[i]);
                right_y.push_back(y[i]);
            }
        }
        return std::make_tuple(left_X, left_y, right_X, right_y);
    }

    int most_common_class(const Labels& y) const {
        if (y.empty()) return -1;
        std::map<int, int> counts;
        for (int label : y) counts[label]++;
        return std::max_element(counts.begin(), counts.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->first;
    }

    bool all_same_class(const Labels& y) const {
        if (y.empty()) return true;
        return std::all_of(y.begin() + 1, y.end(), [&](int label) { return label == y[0]; });
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    Dataset X_train = {
        {0, 8.1}, {0, 9.0}, {0, 7.5},
        {1, 7.9}, {1, 8.5}, {1, 9.2}
    };
    Labels y_train = { 0, 0, 0, 1, 1, 1 };

    DecisionTree tree;
    tree.train(X_train, y_train, 2);

    std::cout << "--- Предсказания Дерева Решений ---\n";

    Features test_apple = { 0, 8.5 };
    Features test_orange = { 1, 8.0 };
    Features new_fruit = { 1, 7.0 };

    std::cout << "Тест [зеленый, 8.5см]: " << (tree.predict(test_apple) == 0 ? "Яблоко" : "Апельсин") << "\n";
    std::cout << "Тест [оранжевый, 8.0см]: " << (tree.predict(test_orange) == 0 ? "Яблоко" : "Апельсин") << "\n";
    std::cout << "Тест [оранжевый, 7.0см]: " << (tree.predict(new_fruit) == 0 ? "Яблоко" : "Апельсин") << "\n";

    return 0;
}