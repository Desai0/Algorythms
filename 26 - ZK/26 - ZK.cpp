#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <sstream>

std::string toy_hash(const std::string& data) {
    uint64_t h = 1469598103934665603ull; // FNV offset
    for (unsigned char c : data) {
        h ^= c;
        h *= 1099511628211ull; // FNV prime
    }
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(16) << h;
    return oss.str();
}

std::string generate_salt() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    uint64_t s1 = dis(gen);
    uint64_t s2 = dis(gen);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(16) << s1
        << std::setw(16) << s2;
    return oss.str();
}

struct Registration {
    std::string commit;
    std::string salt;
    int age;
};

struct Proof {
    std::string recomputed_hash;
    bool age_ok;
};

// регистрация у доверенного органа
Registration register_with_authority(int age_real) {
    Registration reg;
    reg.age = age_real;
    reg.salt = generate_salt();

    std::string data = reg.salt + std::to_string(age_real);
    reg.commit = toy_hash(data);

    std::cout << "\n=== Регистрация ===\n";
    std::cout << "Возраст (виден только органу): " << age_real << "\n";
    std::cout << "Соль: " << reg.salt.substr(0, 16) << "\n";
    std::cout << "Коммитмент: " << reg.commit << "\n" << std::endl;

    return reg;
}

// генерация доказательства
Proof prove_under_18(int age_real, const std::string& salt, const std::string& commit) {
    Proof proof;

    std::cout << "\n=== Генерация доказательства ===\n";

    std::string data = salt + std::to_string(age_real);
    proof.recomputed_hash = toy_hash(data);

    // проверяем условие возраста
    proof.age_ok = (age_real < 18);

    std::cout << "Пересчитанный хеш: " << proof.recomputed_hash << " \n";
    std::cout << "Возраст < 18: " << (proof.age_ok ? "ДА" : "НЕТ") << " \n";

    return proof;
}

// Фаза 3: верификация
bool verify_under_18(const std::string& commit, const Proof& proof) {
    std::cout << "\n=== Проверка доказательства === \n";

    bool hash_match = (proof.recomputed_hash == commit);
    bool condition = proof.age_ok;

    std::cout << "Хеш совпадает с коммитментом: " << (hash_match ? "ДА" : "НЕТ") << " \n";
    std::cout << "Заявлено, что age < 18: " << (condition ? "ДА" : "НЕТ") << " \n";

    bool ok = hash_match && condition;
    std::cout << "Итог: " << (ok ? "ПРИНЯТО" : "ОТКЛОНЕНО") << " \n";

    return ok;
}

int main() {
    setlocale(0, "");
    std::cout << "\nУпрощённый zk-подобный age-протокол\n";

        // 1. честный несовершеннолетний
    std::cout << "Сценарий 1: честный юзер (16 лет)\n";
    Registration reg1 = register_with_authority(16);
    Proof proof1 = prove_under_18(reg1.age, reg1.salt, reg1.commit);
    verify_under_18(reg1.commit, proof1);

    // 2. взрослый пытается обмануть
    std::cout << "Сценарий 2: взрослый (21 год) пытается пройти как <18\n";
    Registration reg2 = register_with_authority(21);
    Proof proof2 = prove_under_18(reg2.age, reg2.salt, reg2.commit);
    verify_under_18(reg2.commit, proof2);

    return 0;
}