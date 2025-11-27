#include <iostream>
#include <vector>
#include <cmath>

class SimpleSVM {
private:
    std::vector<double> w;
    double b;
    double learning_rate;
    int epochs;

public:
    SimpleSVM(double lr = 0.001, int ep = 1000)
        : learning_rate(lr), epochs(ep), b(0.0) {
    }

    // обучение методом градиентного спуска
    void fit(const std::vector<std::vector<double>>& X,
        const std::vector<int>& y) {

        int n_samples = X.size();
        int n_features = X[0].size();

        // инициализация весов нулями
        w.assign(n_features, 0.0);
        b = 0.0;

        // градиентный спуск
        for (int epoch = 0; epoch < epochs; ++epoch) {
            for (int i = 0; i < n_samples; ++i) {

                double decision = 0.0;
                for (int j = 0; j < n_features; ++j) {
                    decision += w[j] * X[i][j];
                }
                decision = y[i] * (decision + b);

                if (decision >= 1) {
                    for (int j = 0; j < n_features; ++j) {
                        w[j] -= learning_rate * (2 * 0.01 * w[j]);
                    }
                }
                else {
                    for (int j = 0; j < n_features; ++j) {
                        w[j] -= learning_rate * (2 * 0.01 * w[j] - y[i] * X[i][j]);
                    }
                    b -= learning_rate * (-y[i]);
                }
            }
        }

        std::cout << "Обучение завершено\n";
        std::cout << "Веса w: [";
        for (size_t i = 0; i < w.size(); ++i) {
            std::cout << w[i];
            if (i < w.size() - 1) std::cout << ", ";
        }
        std::cout << "]\n";
        std::cout << "Смещение b: " << b << "\n";
    }

    int predict_one(const std::vector<double>& x) {
        double result = b;
        for (size_t i = 0; i < w.size(); ++i) {
            result += w[i] * x[i];
        }
        return (result >= 0) ? 1 : -1;
    }

    std::vector<int> predict(const std::vector<std::vector<double>>& X) {
        std::vector<int> predictions;
        for (const auto& x : X) {
            predictions.push_back(predict_one(x));
        }
        return predictions;
    }

    double score(const std::vector<std::vector<double>>& X,
        const std::vector<int>& y) {
        auto predictions = predict(X);
        int correct = 0;
        for (size_t i = 0; i < predictions.size(); ++i) {
            if (predictions[i] == y[i]) correct++;
        }
        return (double)correct / y.size();
    }
};

int main() {
	setlocale(LC_ALL, "Russian");
    std::vector<std::vector<double>> X_train = {
        {3.0, 4.0}, {4.0, 5.0}, {5.0, 6.0},
        {1.0, 1.0}, {2.0, 2.0}, {1.5, 1.5}
    };

    std::vector<int> y_train = { 1, 1, 1, -1, -1, -1 };

    std::cout << "=== Обучение упрощенного SVM ===\n";
    SimpleSVM svm(0.001, 1000);
    svm.fit(X_train, y_train);

    // проверка
    std::vector<std::vector<double>> X_test = {
        {4.5, 5.5},  // класс +1
        {1.2, 1.3}   // класс -1
    };

    std::cout << "\n=== Предсказания ===\n";
    auto predictions = svm.predict(X_test);
    for (size_t i = 0; i < X_test.size(); ++i) {
        std::cout << "Точка (" << X_test[i][0] << ", " << X_test[i][1]
            << ") -> класс " << predictions[i] << "\n";
    }

    double accuracy = svm.score(X_train, y_train);
    std::cout << "\nТочность: " << (accuracy * 100) << "%\n";

    return 0;
}
