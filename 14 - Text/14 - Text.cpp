//#include <iostream>
//
//void ravno(std::string str1, std::string str2) {
//    std::cout << "Было: " << str1 << " " << str2 << std::endl;
//    size_t schet = 0;
//
//    for (size_t i = 0; i < str1.length(); ++i) {
//        while (str1.length() > str2.length()) {
//            str2.push_back(str1[(str1.length() - (str1.length() - str2.length()) + 1)]);
//            schet++;
//        }
//
//        if (str1[i] != str2[i]) {
//            str2[i] = str1[i];
//            schet++;
//        }
//    }
//
//    while (str1.length() < str2.length()) {
//        str2.pop_back();
//        schet++;
//    }
//
//    std::cout << "Стало: " << str1 << " " << str2 << "Действий: " << schet << std::endl;
//}
//
//int main()
//{
//    setlocale(0, "");
//
//    ravno("bebr72", "bebr22");
//}

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

void printMatrix(const std::vector<std::vector<int>>& matrix, const std::string& s1, const std::string& s2) {
    std::cout << "     #  ";
    for (char c : s2) {
        std::cout << c << "  ";
    }
    std::cout << std::endl;

    for (int i = 0; i < matrix.size(); ++i) {
        if (i == 0) {
            std::cout << "   # ";
        }
        else {
            std::cout << "   " << s1[i - 1] << " ";
        }
        for (int j = 0; j < matrix[i].size(); ++j) {
            std::cout << matrix[i][j] << "  ";
        }
        std::cout << std::endl;
    }
}

int levenshteinDistance(const std::string& s1, const std::string& s2) {
    const size_t len1 = s1.length();
    const size_t len2 = s2.length();

    std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));

    for (int i = 0; i <= len1; ++i) {
        d[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        d[0][j] = j;
    }

    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            
        }
    }

    std::cout << "\nМатрица D(i, j) \"" << s1 << "\" и \"" << s2 << "\":" << std::endl;
    printMatrix(d, s1, s2);

    return d[len1][len2];
}

int main() {
	setlocale(0, "");

    std::string str1 = "kitten";
    std::string str2 = "sitting";

    int distance = levenshteinDistance(str1, str2);

    std::cout << "Расстояние  \"" << str1 << "\" и \"" << str2 << "\" равно: " << distance << std::endl;

    std::cout << "\n----------------------------------------\n";

    return 0;
}