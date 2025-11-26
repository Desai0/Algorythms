#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>

std::string sha256_hash_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return {};

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);

    std::vector<unsigned char> buffer(4096);
    while (file) {
        file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        EVP_DigestUpdate(ctx, buffer.data(), file.gcount());
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    EVP_DigestFinal_ex(ctx, hash, &hashLen);
    EVP_MD_CTX_free(ctx);

    std::ostringstream oss;
    for (unsigned int i = 0; i < hashLen; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}

std::string sha256(const std::string& str) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, str.c_str(), str.size());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    EVP_DigestFinal_ex(ctx, hash, &hashLen);
    EVP_MD_CTX_free(ctx);

    std::ostringstream oss;
    for (unsigned int i = 0; i < hashLen; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}

int main() {
	setlocale(LC_ALL, "Russian");
    std::string path, known_hash;
    std::cin >> path >> known_hash;
    std::string hash = sha256_hash_file(path);
    if (hash == known_hash) std::cout << "Файл целостен\n";
    else std::cout << "Несовпадение хешей\n";

    std::string hash2;
    std::cin >> hash2;
    std::vector<std::string> passwords = { "password", "123456", "hello", "secret", "letmein" };
    for (const auto& pass : passwords)
        if (sha256(pass) == hash2) {
            std::cout << "Пароль: " << pass << "\n";
            return 0;
        }
    std::cout << "Не найден\n";

	return 0;
}
