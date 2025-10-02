#include <iostream>
#include <vector>
#include <algorithm>


int knapsack_rec(int w, int i, std::vector<std::vector<int>> memo, std::vector<int>& weights, std::vector<int>& values) {
    if (i == 0 || w == 0) {
        return 0;
    }
    if (memo[i][w] != -1) {
        return memo[i][w];
    }
    if (weights[i - 1] <= w) {
        memo[i][w] = std::max(values[i - 1] + knapsack_rec(w - weights[i - 1], i - 1, memo, weights, values), knapsack_rec(w, i - 1, memo, weights, values));
    }
    else {
        memo[i][w] = knapsack_rec(w, i - 1, memo, weights, values);
    }
    return memo[i][w];
}

int knapsack(std::vector<int>& weights, std::vector<int>& values, int capacity) {
    int n = values.size();
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(capacity + 1, -1));

    return knapsack_rec(capacity, n, memo, weights, values);
}


// 1: Рекурсивная функция для неограниченного рюкзака
int knapsack_unbounded_rec(int w, int i, std::vector<std::vector<int>>& memo, const std::vector<int>& weights, const std::vector<int>& values) {
    if (i == 0 || w == 0) {
        return 0;
    }
    if (memo[i][w] != -1) {
        return memo[i][w];
    }

    int not_take = knapsack_unbounded_rec(w, i - 1, memo, weights, values);

    int take = 0;
    if (weights[i - 1] <= w) {
        //после взятия предмета i-1, мы остаемся на уровне i
        take = values[i - 1] + knapsack_unbounded_rec(w - weights[i - 1], i, memo, weights, values);
    }

    memo[i][w] = std::max(take, not_take);
    return memo[i][w];
}

int knapsack_unbounded(const std::vector<int>& weights, const std::vector<int>& values, int capacity) {
    int n = values.size();
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(capacity + 1, -1));
    return knapsack_unbounded_rec(capacity, n, memo, weights, values);
}


// 2: Функция для дробного рюкзака
struct Item {
    int weight;
    int value;
    double ratio; // value / weight
};

double knapsack_fractional(const std::vector<int>& weights, const std::vector<int>& values, int capacity) {
    int n = values.size();
    std::vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        items[i] = { weights[i], values[i], (double)values[i] / weights[i] };
    }

    //сортируем по убыванию ценности на единицу веса
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.ratio > b.ratio;
        });

    double total_value = 0.0;
    int current_capacity = capacity;

    for (int i = 0; i < n; ++i) {
        if (current_capacity == 0) break;

        if (items[i].weight <= current_capacity) {
            current_capacity -= items[i].weight;
            total_value += items[i].value;
        }
        else { //если не все
            total_value += items[i].ratio * current_capacity;
            current_capacity = 0;
        }
    }
    return total_value;
}


int main() {
    std::vector<int> weights = { 1, 2, 3, 4 };
    std::vector<int> values = { 10, 20, 30, 40 };
    int capacity = 5;
    std::cout << knapsack(weights, values, capacity) << std::endl;


    std::vector<int> weights1 = { 3, 4, 5 };
    std::vector<int> values1 = { 7, 9, 12 };
    int capacity1 = 10;

    std::cout << "\n" << knapsack_unbounded(weights1, values1, capacity1) << std::endl;


    std::vector<int> weights2 = { 10, 20, 30 };
    std::vector<int> values2 = { 60, 100, 120 };
    int capacity2 = 70;

    std::cout << "\n" << knapsack_fractional(weights2, values2, capacity2) << std::endl;

    return 0;
}