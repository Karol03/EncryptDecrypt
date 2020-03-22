#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <string> 

constexpr auto FIRST_ASCII_CHAR{' '};
constexpr auto LAST_ASCII_CHAR{'~'};
constexpr auto ASII_CHARS_NUMBER{LAST_ASCII_CHAR - FIRST_ASCII_CHAR + 1};

std::string ascii()
{
    auto visibleChars = std::string(ASII_CHARS_NUMBER, ' ');
    std::generate(visibleChars.begin(), visibleChars.end(),
        [n = FIRST_ASCII_CHAR] () mutable { return n++; });
    return visibleChars;
}

std::string generateKey()
{
    static auto rd = std::random_device{};
    static auto g = std::mt19937(rd()) ;
    static auto key = std::string{ascii()};
    std::shuffle(key.begin(), key.end(), g);
    return key;
}

std::string encrypt(const std::string& message, const std::string& key)
{
    const auto encode = [&key](const char symbol)
    {
        return (FIRST_ASCII_CHAR <= symbol and symbol <= LAST_ASCII_CHAR)
               ? key[symbol - FIRST_ASCII_CHAR]
               : symbol;
    };
    auto result = std::string{};
    std::transform(message.begin(), message.end(),
        std::inserter(result, result.begin()), encode);
    return result;
}

std::string oppositeKey(const std::string& key)
{
    auto result = std::string(key.size(), ' ');
    const auto reverseSymbol = [&result, n = FIRST_ASCII_CHAR] (const char symbol) mutable
    {
        return result[symbol - FIRST_ASCII_CHAR] = n++;
    };
    std::for_each(key.begin(), key.end(), reverseSymbol);
    return result;
}

std::string decrypt(const std::string& cypher, const std::string& key)
{
    const auto decryptingKey = oppositeKey(key);
    return encrypt(cypher, decryptingKey);
}

int main()
{
    const std::string message = "Hello, Coders School!";
    const auto key = generateKey();
    const auto cypher = encrypt(message, key);
    const auto result = decrypt(cypher, key);

    std::cout << "Message: " << message << '\n';
    std::cout << "Cypher: " << cypher << '\n';
    std::cout << "Result: " << result << '\n';

    assert(message == result);
    return 0;
}
