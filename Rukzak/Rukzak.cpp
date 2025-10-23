#include <iostream>
#include <vector>
#include <algorithm>

struct Item2 {
    int weight;
    int value;
    double value_per_unit;
};

//сравнение
bool compareItems(const Item2& a, const Item2& b) {
    return a.value_per_unit > b.value_per_unit;
}

double fractionalKnapsack(int capacity, std::vector<Item2>& items) {
    //ценность
    for (auto& item : items) {
        if (item.weight > 0) {
            item.value_per_unit = (double)item.value / item.weight;
        }
        else {
            item.value_per_unit = 0;
        }
    }

    std::sort(items.begin(), items.end(), compareItems);

    double totalValue = 0.0;
    int currentCapacity = capacity;

    for (const auto& item : items) {
        if (currentCapacity == 0) {
            break; //полон
        }

        //если помещается
        if (item.weight <= currentCapacity) {
            currentCapacity -= item.weight;
            totalValue += item.value;
        }
        else {
            double fraction = (double)currentCapacity / item.weight;
            totalValue += item.value * fraction;
            currentCapacity = 0; //полон
        }
    }

    return totalValue;
}


struct Item {
    int weight;
    int value;
};

int knapbackExtra(int capacity, const std::vector<Item>& items) {
    std::vector<int> dp(capacity + 1, 0);

    for (const auto& item : items) {
        for (int w = capacity; w >= item.weight; --w) {
            dp[w] = std::max(dp[w], item.value + dp[w - item.weight]);
        }
    }

    return dp[capacity];
}

//только вес
int knapsack(int capacity, const std::vector<int>& weights) {
    std::vector<bool> dp(capacity + 1, false); //от 0 до capacity

    dp[0] = true;

    for (int weight : weights) {
        for (int j = capacity; j >= weight; --j) {
            if (dp[j - weight]) {
                dp[j] = true;
            }
        }
    }

    //макс вес
    for (int i = capacity; i >= 0; --i) {
        if (dp[i]) {
            return i;
        }
    }

    return 0;
}

int main() {
    setlocale(0, "");

    int capacity = 10; //вместимость
    std::vector<int> weights = { 2, 3, 4, 5 }; //веса

    int maxWeight = knapsack(capacity, weights);

    std::cout << "Вместимость рюкзака: " << capacity << std::endl;
    std::cout << "Доступные веса: ";
    for (int w : weights) {
        std::cout << w << " ";
    }
    std::cout << std::endl;
    std::cout << "Максимальный вес: " << maxWeight << std::endl;


    ///
    int capacity1 = 50;
    std::vector<Item> items = {
        {7, 90},
        {19, 275},
        {30, 400},
        {5, 100},
        {10, 100}
    };

    int maxValue = knapbackExtra(capacity1, items);

    std::cout << "\nВместимость рюкзака: " << capacity1 << std::endl;
    std::cout << "Предметы:" << std::endl;
    for (const auto& item : items) {
        std::cout << "  (" << item.weight << ", " << item.value << ")" << std::endl;
    }
    std::cout << "Максимальная ценность: " << maxValue << std::endl;



    int capacity2 = 50;
    std::vector<Item2> items2 = {
        {7, 90},
        {19, 275},
        {30, 400},
        {5, 100},
        {10, 100}
    };

    double maxValue2 = fractionalKnapsack(capacity2, items2);

    std::cout << "\nВместимость рюкзака: " << capacity2 << std::endl;
    std::cout << "Предметы:" << std::endl;
    for (const auto& item : items2) {
        std::cout << "  (" << item.weight << ", " << item.value  << ", " << item.value_per_unit << ")" << std::endl;
    }
    std::cout.precision(2);
    std::cout << std::fixed;
    std::cout << "Максимальная ценность с делением: " << maxValue2 << std::endl;

    return 0;
}

//Просто веса, веса с ценностью, брать по частям от предмета