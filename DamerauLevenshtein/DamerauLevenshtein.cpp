#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

//вычисления расстояния
int damLev(const std::string& s1, const std::string& s2) {
    const size_t m = s1.length();
    const size_t n = s2.length();

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; ++i) {
        dp[i][0] = i; //cтоимость
    }
    for (int j = 0; j <= n; ++j) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            //вычисляем стоимость
            dp[i][j] = std::min({
                dp[i - 1][j] + 1, //удалени
                dp[i][j - 1] + 1, //вставка
                dp[i - 1][j - 1] + cost //замена
                });

            if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + 1);
            }
        }
    }

    return dp[m][n];
}

int main() {
    setlocale(0, "");

    std::string str1 = "kitten";
    std::string str2 = "sitting";
    std::cout << "Расстояние между '" << str1 << "' и '" << str2 << "': "
        << damLev(str1, str2) << std::endl;

    std::string str3 = "жы";
    std::string str4 = "ыж";
    std::cout << "Расстояние между '" << str3 << "' и '" << str4 << "': "
        << damLev(str3, str4) << std::endl;

    std::string str9 = "пива";
    std::string str10 = "";
    std::cout << "Расстояние между '" << str9 << "' и '" << str10 << "': "
        << damLev(str9, str10) << std::endl;

    return 0;
}