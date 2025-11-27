#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <chrono>

// алгоритм Дамерау-Левенштейна
int damLev(const std::string& s1, const std::string& s2) {
    const size_t m = s1.length();
    const size_t n = s2.length();

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; ++j) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            dp[i][j] = std::min({
                dp[i - 1][j] + 1,
                dp[i][j - 1] + 1,
                dp[i - 1][j - 1] + cost
                });

            if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + 1);
            }
        }
    }

    return dp[m][n];
}

// === ТЕСТ-ГРУППЫ ===

namespace Tests {
    int total = 0;
    int passed = 0;
    int failed = 0;

    void assert_equal(int result, int expected, const std::string& test_name) {
        total++;
        if (result == expected) {
            passed++;
            std::cout << "[PASS] " << test_name << " (результат: " << result << ")";
        }
        else {
            failed++;
            std::cout << "[FAIL] " << test_name << " (ожидалось: " << expected << ", получено: " << result << ")";
        }
    }

    void print_header(const std::string& category) {
        std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            std::cout << "  " << category << "\n";
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    }

    // 1. Граничные случаи
    void test_edge_cases() {
        print_header("\n1. Граничные случаи\n");

        assert_equal(damLev("", ""), 0, "Обе строки пустые\n");
        assert_equal(damLev("abc", ""), 3, "Вторая строка пустая\n");
        assert_equal(damLev("", "xyz"), 3, "Первая строка пустая\n");
        assert_equal(damLev("a", "a"), 0, "Одинаковые строки из 1 символа\n");
        assert_equal(damLev("hello", "hello"), 0, "Идентичные строки\n");
    }

    // 2. Одиночные операции
    void test_single_operations() {
        print_header("\n2. Одиночные операции\n");

        assert_equal(damLev("a", ""), 1, "Удаление 1 символа\n");
        assert_equal(damLev("", "b"), 1, "Вставка 1 символа\n");
        assert_equal(damLev("a", "b"), 1, "Замена 1 символа\n");
        assert_equal(damLev("cat", "at"), 1, "Удаление первого символа\n");
        assert_equal(damLev("at", "cat"), 1, "Вставка в начало\n");
        assert_equal(damLev("cat", "cut"), 1, "Замена в середине\n");
    }

    // 3. Транспозиция (ключевая особенность Дамерау-Левенштейна)
    void test_transposition() {
        print_header("\n3. Транспозиция соседних символов\n");

        assert_equal(damLev("ab", "ba"), 1, "Простая транспозиция\n");
        assert_equal(damLev("abc", "acb"), 1, "Транспозиция bc -> cb\n");
        assert_equal(damLev("abcd", "abdc"), 1, "Транспозиция cd -> dc\n");
        assert_equal(damLev("hello", "hlelo"), 1, "Транспозиция el -> le\n");
        assert_equal(damLev("ca", "ac"), 1, "Транспозиция в начале\n");
    }

    // 4. Множественные операции
    void test_multiple_operations() {
        print_header("\n4. Множественные операции\n");

        assert_equal(damLev("kitten", "sitting"), 3, "kitten -> sitting\n");
        // k->s (замена), e->i (замена), добавить g

        assert_equal(damLev("saturday", "sunday"), 3, "saturday -> sunday\n");
        // удалить a, удалить t, r->n

        assert_equal(damLev("book", "back"), 2, "book -> back\n");
        // o->a, o->c

        assert_equal(damLev("flaw", "lawn"), 2, "flaw -> lawn\n");
        // f->l, удалить w, добавить n = 3 или fl<->la транспозиция + w->n замена = 2? 
        // на самом деле: f удалить, вставить l, w->n = 3
        // или: fl -> la (2 замены) + w->n = 3
        // Проверим вручную корректность
    }

    // 5. Кириллица и UTF-8
    void test_cyrillic() {
        print_header("\n5. Кириллица (UTF-8)\n");

        assert_equal(damLev("кот", "кот"), 0, "Идентичные кириллические\n");
        assert_equal(damLev("кот", "кто"), 1, "Транспозиция кириллицы\n");
        assert_equal(damLev("мама", "папа"), 2, "Замены в кириллице\n");
        assert_equal(damLev("привет", "прривет"), 1, "Вставка в кириллицу\n");
        assert_equal(damLev("жы", "ыж"), 1, "Транспозиция русских букв\n");
    }

    // 6. Повторяющиеся символы
    void test_repeated_chars() {
        print_header("\n6. Повторяющиеся символы\n");

        assert_equal(damLev("aaa", "aaa"), 0, "Идентичные повторы\n");
        assert_equal(damLev("aaa", "aa"), 1, "Удаление из повтора\n");
        assert_equal(damLev("aa", "aaa"), 1, "Добавление к повтору\n");
        assert_equal(damLev("aaa", "bbb"), 3, "Полная замена повторов\n");
        assert_equal(damLev("aaaa", "aa"), 2, "Удаление двух символов\n");
    }

    // 7. Разная длина
    void test_length_difference() {
        print_header("\n7. Значительная разница в длине\n");

        assert_equal(damLev("a", "abcdef"), 5, "Добавление 5 символов\n");
        assert_equal(damLev("abcdef", "a"), 5, "Удаление 5 символов\n");
    }

    // 8. Специальные символы
    void test_special_chars() {
        print_header("\n8. Специальные символы\n");

        assert_equal(damLev("hello!", "hello?"), 1, "Замена знака препинания\n");
        assert_equal(damLev("123", "321"), 2, "Цифры (не транспозиция всей строки)\n");
        assert_equal(damLev("a b", "ab"), 1, "Удаление пробела\n");
        assert_equal(damLev("a-b", "a_b"), 1, "Замена дефиса на подчёркивание\n");
    }

    // 9. Симметричность
    void test_symmetry() {
        print_header("\n9. Проверка симметричности\n");

        auto check_sym = [](const std::string& s1, const std::string& s2, const std::string& name) {
            int d1 = damLev(s1, s2);
            int d2 = damLev(s2, s1);
            assert_equal(d1, d2, name + " (симметричность)\n");
            };

        check_sym("abc", "xyz", "abc <-> xyz");
        check_sym("hello", "world", "hello <-> world");
        check_sym("test", "testing", "test <-> testing");
    }

    // 10. Производительность
    void test_performance() {
        print_header("\n10. Тест производительности\n");

        std::string long1(100, 'a');
        std::string long2(100, 'b');

        auto start = std::chrono::high_resolution_clock::now();
        int result = damLev(long1, long2);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "\n[INFO] Строки 100x100 символов: " << result
            << " (время: " << duration.count() << " мкс)\n";

            assert_equal(result, 100, "Полная замена 100 символов");

        if (duration.count() < 10000) { // меньше 10 мс
            std::cout << "[PASS] Производительность приемлема для строк длины 100\n";
        }
        else {
            std::cout << "[WARN] Медленная работа на строках длины 100\n";
        }
    }

    void run_all() {
        std::cout << "\n╔════════════════════════════════════════════════════╗\n";
            std::cout << "║     ТЕСТИРОВАНИЕ АЛГОРИТМА ДАМЕРАУ-ЛЕВЕНШТЕЙНА     ║\n";
            std::cout << "╚════════════════════════════════════════════════════╝\n";

        test_edge_cases();
        test_single_operations();
        test_transposition();
        test_multiple_operations();
        test_cyrillic();
        test_repeated_chars();
        test_length_difference();
        test_special_chars();
        test_symmetry();
        test_performance();

        std::cout << "\n╔════════════════════════════════════════════════════╗\n";
            std::cout << "║                 ИТОГИ ТЕСТИРОВАНИЯ                 ║\n";
            std::cout << "╚════════════════════════════════════════════════════╝\n";

            std::cout << "Всего тестов:    " << total << "\n";
            std::cout << "Пройдено:        " << passed << " ✓\n";
            std::cout << "Провалено:       " << failed << " ✗ \n";

            if (total > 0) {
                double percent = (100.0 * passed) / total;
                std::cout << "Процент успеха:  " << std::fixed << std::setprecision(1) << percent << "%\n";
            }

        if (failed == 0) {
            std::cout << "🎉 ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!";
        }
        else {
            std::cout << "⚠️  ОБНАРУЖЕНЫ ОШИБКИ. Требуется доработка.";
        }
    }
}

int main() {
    setlocale(0, "");
    Tests::run_all();
    return (Tests::failed == 0) ? 0 : 1;
}