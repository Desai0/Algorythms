// Делал не я, это жесть

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <memory>

using namespace std;

struct FPNode {
    string item;
    int count;
    FPNode* parent;
    vector<FPNode*> children;
    FPNode* next;

    FPNode(string i = "", int c = 0, FPNode* p = nullptr)
        : item(i), count(c), parent(p), next(nullptr) {
    }
};

class FPGrowth {
private:
    int minSupport;
    map<string, int> itemFreq;
    map<string, FPNode*> headerTable;
    FPNode* root;

    void buildHeaderTable(vector<vector<string>>& transactions) {
        for (auto& trans : transactions) {
            for (auto& item : trans) {
                itemFreq[item]++;
            }
        }

        for (auto& [item, freq] : itemFreq) {
            if (freq >= minSupport) {
                headerTable[item] = nullptr;
            }
        }
    }

    void insertTransaction(vector<string>& sortedItems, FPNode* node, int count = 1) {
        if (sortedItems.empty()) return;

        string firstItem = sortedItems[0];
        FPNode* child = nullptr;

        for (auto* c : node->children) {
            if (c->item == firstItem) {
                child = c;
                break;
            }
        }

        if (child) {
            child->count += count;
        }
        else {
            child = new FPNode(firstItem, count, node);
            node->children.push_back(child);

            if (headerTable[firstItem] == nullptr) {
                headerTable[firstItem] = child;
            }
            else {
                FPNode* curr = headerTable[firstItem];
                while (curr->next) curr = curr->next;
                curr->next = child;
            }
        }

        vector<string> remaining(sortedItems.begin() + 1, sortedItems.end());
        insertTransaction(remaining, child, count);
    }

    // Основной рекурсивный поиск частых паттернов через условные FP-деревья
    void minePatterns(FPNode* tree, map<string, FPNode*>& header,
        vector<string> prefix, vector<pair<set<string>, int>>& result) {

        vector<pair<string, int>> items;
        for (auto& [item, node] : header) {
            int support = 0;
            FPNode* curr = node;
            while (curr) {
                support += curr->count;
                curr = curr->next;
            }
            if (support >= minSupport) {
                items.push_back({ item, support });
            }
        }

        sort(items.begin(), items.end(),
            [](auto& a, auto& b) { return a.second < b.second; });

        for (auto& [item, support] : items) {
            vector<string> newPrefix = prefix;
            newPrefix.push_back(item);

            set<string> pattern(newPrefix.begin(), newPrefix.end());
            result.push_back({ pattern, support });

            map<vector<string>, int> conditionalBase;
            FPNode* node = header[item];

            while (node) {
                vector<string> path;
                FPNode* parent = node->parent;
                while (parent && !parent->item.empty()) {
                    path.insert(path.begin(), parent->item);
                    parent = parent->parent;
                }
                if (!path.empty()) {
                    conditionalBase[path] += node->count;
                }
                node = node->next;
            }

            if (!conditionalBase.empty()) {
                FPNode* condRoot = new FPNode();
                map<string, FPNode*> condHeader;

                map<string, int> condFreq;
                for (auto& [path, cnt] : conditionalBase) {
                    for (auto& it : path) {
                        condFreq[it] += cnt;
                    }
                }

                for (auto& [it, freq] : condFreq) {
                    if (freq >= minSupport) {
                        condHeader[it] = nullptr;
                    }
                }

                for (auto& [path, cnt] : conditionalBase) {
                    vector<string> filteredPath;
                    for (auto& it : path) {
                        if (condHeader.count(it)) {
                            filteredPath.push_back(it);
                        }
                    }
                    sort(filteredPath.begin(), filteredPath.end(),
                        [&](string& a, string& b) {
                            return condFreq[a] > condFreq[b];
                        });
                    insertTransactionToTree(filteredPath, condRoot, condHeader, cnt);
                }

                minePatterns(condRoot, condHeader, newPrefix, result);
                deleteTree(condRoot);
            }
        }
    }

    void insertTransactionToTree(vector<string>& items, FPNode* node,
        map<string, FPNode*>& header, int count) {
        if (items.empty()) return;

        string firstItem = items[0];
        FPNode* child = nullptr;

        for (auto* c : node->children) {
            if (c->item == firstItem) {
                child = c;
                break;
            }
        }

        if (child) {
            child->count += count;
        }
        else {
            child = new FPNode(firstItem, count, node);
            node->children.push_back(child);

            if (header[firstItem] == nullptr) {
                header[firstItem] = child;
            }
            else {
                FPNode* curr = header[firstItem];
                while (curr->next) curr = curr->next;
                curr->next = child;
            }
        }

        vector<string> remaining(items.begin() + 1, items.end());
        insertTransactionToTree(remaining, child, header, count);
    }

    void deleteTree(FPNode* node) {
        for (auto* child : node->children) {
            deleteTree(child);
        }
        delete node;
    }

public:
    FPGrowth(int minSup) : minSupport(minSup), root(new FPNode()) {}

    ~FPGrowth() {
        deleteTree(root);
    }

    // Построение FP-дерева: сжатое представление всех транзакций
    void buildFPTree(vector<vector<string>>& transactions) {
        buildHeaderTable(transactions);

        for (auto& trans : transactions) {
            vector<string> filteredTrans;
            for (auto& item : trans) {
                if (headerTable.count(item)) {
                    filteredTrans.push_back(item);
                }
            }

            sort(filteredTrans.begin(), filteredTrans.end(),
                [this](const string& a, const string& b) {
                    return itemFreq[a] > itemFreq[b];
                });

            insertTransaction(filteredTrans, root);
        }
    }

    vector<pair<set<string>, int>> mine() {
        vector<pair<set<string>, int>> result;
        minePatterns(root, headerTable, {}, result);
        return result;
    }

    void printTree(FPNode* node = nullptr, int depth = 0) {
        if (!node) node = root;
        for (int i = 0; i < depth; i++) cout << "  ";
        if (!node->item.empty()) {
            cout << node->item << ":" << node->count << endl;
        }
        else {
            cout << "ROOT" << endl;
        }
        for (auto* child : node->children) {
            printTree(child, depth + 1);
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    // Пример 1: Покупки в магазине
    cout << "=== ПРИМЕР 1: Анализ покупок ===" << endl;
    vector<vector<string>> transactions1 = {
        {"хлеб", "молоко"},
        {"хлеб", "подгузники", "пиво", "яйца"},
        {"молоко", "подгузники", "пиво", "кола"},
        {"хлеб", "молоко", "подгузники", "пиво"},
        {"хлеб", "молоко", "подгузники", "кола"}
    };

    FPGrowth fpg1(2);
    fpg1.buildFPTree(transactions1);

    cout << "\nСтруктура FP-дерева:" << endl;
    fpg1.printTree();

    auto patterns1 = fpg1.mine();
    cout << "\nЧастые наборы товаров (минимальная поддержка = 2):" << endl;
    for (auto& [pattern, support] : patterns1) {
        cout << "{ ";
        for (auto& item : pattern) cout << item << " ";
        cout << "} -> поддержка: " << support << endl;
    }

    // Пример 2: Веб-навигация
    cout << "\n\n=== ПРИМЕР 2: Последовательности веб-страниц ===" << endl;
    vector<vector<string>> transactions2 = {
        {"главная", "товары", "корзина", "оплата"},
        {"главная", "товары", "корзина"},
        {"главная", "о_компании"},
        {"главная", "товары", "описание"},
        {"главная", "товары", "корзина", "оплата"}
    };

    FPGrowth fpg2(2);
    fpg2.buildFPTree(transactions2);

    auto patterns2 = fpg2.mine();
    cout << "\nЧастые паттерны навигации (минимальная поддержка = 2):" << endl;
    for (auto& [pattern, support] : patterns2) {
        cout << "{ ";
        for (auto& item : pattern) cout << item << " ";
        cout << "} -> поддержка: " << support << endl;
    }

    // Пример 3: Симптомы заболеваний
    cout << "\n\n=== ПРИМЕР 3: Медицинские симптомы ===" << endl;
    vector<vector<string>> transactions3 = {
        {"температура", "кашель", "усталость"},
        {"кашель", "насморк"},
        {"температура", "кашель", "боль_в_горле"},
        {"температура", "усталость", "головная_боль"},
        {"кашель", "боль_в_горле", "температура"}
    };

    FPGrowth fpg3(3);
    fpg3.buildFPTree(transactions3);

    auto patterns3 = fpg3.mine();
    cout << "\nЧастые комбинации симптомов (минимальная поддержка = 3):" << endl;
    for (auto& [pattern, support] : patterns3) {
        cout << "{ ";
        for (auto& item : pattern) cout << item << " ";
        cout << "} -> поддержка: " << support << endl;
    }

    return 0;
}