//#include <iostream>
//#include <vector>
//
//const int public_product = 91;
//const std::vector<int> secret_factors = { 7, 13 };
//
//int create_zk_snark_proof(std::vector<int> secret_factors) {
//    int baza = secret_factors[0] * secret_factors[1];
//    return baza;
//}
//
//bool is_valid(int baza, int public_product) {
//    if (baza == public_product) {
//        return true;
//    }
//    return false;
//}
//
//int main()
//{
//    int baza = create_zk_snark_proof(secret_factors);
//
//    bool some = is_valid(baza, public_product);
//
//    return 0;
//}



#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>   

struct PublicValues {
    long long encrypted_x_squared;
    long long encrypted_bx;
    long long encrypted_c;
};

struct Proof {
    long long A;
    long long B;
    long long C;
    PublicValues public_values;
};

struct SetupParameters {
    long long encrypted_G;
    long long encrypted_Gs;
};


class SimpleZKSNARK {
public:
    SimpleZKSNARK() {
        prime = 101;
        G = 3;
        std::random_device rd;
        rng.seed(rd());
    }

    SetupParameters setup(long long b, long long c) {
        std::cout << "=== настройки ===" << std::endl;

        // cекретный
        std::uniform_int_distribution<long long> dist(1, prime - 1);
        secret_param = dist(rng);

        public_b = b;
        public_c = c;

        encrypted_G = G;
        encrypted_Gs = mod(G * secret_param, prime);

        std::cout << "публичные пар.: уравнение x^2 + " << b << "x + " << c << " = 0" << std::endl;
        std::cout << "секретный пар. " << secret_param << std::endl;

        return { encrypted_G, encrypted_Gs };
    }


    Proof prove(long long secret_x) {
        std::cout << "\n=== доказательства ===" << std::endl;
        std::cout << "секретный x = " << secret_x << std::endl;

        if (!verify_solution(secret_x)) {
            throw std::runtime_error("x не является решением");
        }

        std::uniform_int_distribution<long long> dist(1, prime - 1);
        long long r = dist(rng);

        Proof p;
        p.A = mod(encrypted_G * r, prime);
        p.B = mod(encrypted_Gs * (secret_x + r), prime);
        p.C = mod(encrypted_G * (secret_x * r), prime);

        p.public_values = {
            mod(encrypted_G * (secret_x * secret_x), prime),
            mod(encrypted_G * (public_b * secret_x), prime),
            mod(encrypted_G * public_c, prime)
        };

        std::cout << "Доказательство создано" << std::endl;
        return p;
    }


    bool verify(const Proof& proof) {
        std::cout << "\n=== Проверка  ===" << std::endl;

        // зашифрованные значения удовлетворяют уравнению
        long long left_side = mod(proof.public_values.encrypted_x_squared + proof.public_values.encrypted_bx + proof.public_values.encrypted_c, prime);

        bool is_valid = (left_side == 0);

        std::cout << "Результат проверки: " << (is_valid ? "ВЕРНО" : "НЕВЕРНО") << std::endl;
        return is_valid;
    }

private:
    long long prime;
    long long G;
    long long secret_param;
    long long public_b;
    long long public_c;
    long long encrypted_G;
    long long encrypted_Gs;

    std::mt19937 rng;

    
    long long mod(long long k, long long n) {
        return ((k % n) + n) % n;
    }

    
    bool verify_solution(long long x) {
        return mod(x * x + public_b * x + public_c, prime) == 0;
    }
};

int main() {
    setlocale(0, "");

    SimpleZKSNARK snark;

    // x^2 - 17x + 70 = 0 Корни 7 10
    long long b = -17;
    long long c = 70;
    snark.setup(b, c);

    long long secret_x = 7;

    Proof proof = snark.prove(secret_x);
    
    snark.verify(proof);
    
    std::cout << "\n----------------------------------------\n";

    return 0;
}