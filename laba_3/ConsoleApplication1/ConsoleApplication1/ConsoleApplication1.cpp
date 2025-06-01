#include <iostream>
#include <string>
#include <random>

using namespace std;

// 1. Функция вычисления CRC
string calculateCRC(const string& input, int length, const string& polynomial) {
    string augmented = input + string(polynomial.length() - 1, '0'); // Дополняем нули
    string remainder = augmented.substr(0, polynomial.length()); // Начальный остаток

    for (int i = polynomial.length() - 1; i < augmented.length(); ++i) {
        if (remainder[0] == '1') { // Если старший бит 1, выполняем XOR
            for (int j = 0; j < polynomial.length(); ++j) {
                remainder[j] = (remainder[j] == polynomial[j]) ? '0' : '1';
            }
        }
        // Сдвиг влево и добавление следующего бита
        remainder = remainder.substr(1) + (i + 1 < augmented.length() ? augmented[i + 1] : '0');
    }
    return remainder.substr(0, polynomial.length() - 1); // Возвращаем CRC
}

// 2. Функция генерации случайной последовательности
string generateRandomSequence(int length) {
    string sequence;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < length; ++i) {
        sequence += (dis(gen) == 0) ? '0' : '1';
    }
    return sequence;
}

// 3. Функция проверки CRC
bool verifyCRC(const string& sequence, const string& polynomial) {
    string crc = calculateCRC(sequence, sequence.length(), polynomial);
    return crc == string(polynomial.length() - 1, '0'); // Проверяем, что CRC = 0
}

int main() {
    setlocale(LC_CTYPE, "Russian");
    // Тест 1: Заданное сообщение
    string message = "110101101";
    string polynomial = "10011";
    cout << "Сообщение: " << message << endl;
    string crc = calculateCRC(message, message.length(), polynomial);
    cout << "CRC: " << crc << endl;
    string transmitted = message + crc;
    cout << "Переданная последовательность: " << transmitted << endl;
    bool isValid = verifyCRC(transmitted, polynomial);
    cout << "CRC проверки: " << (isValid ? "0 (верно)" : "не 0 (ошибка)") << endl;

    // Тест 2: Случайная последовательность
    int k = 1000;
    string randomSeq = generateRandomSequence(k);
    cout << "\nСлучайная последовательность (первые 20 бит): " << randomSeq.substr(0, 20) << "..." << endl;
    crc = calculateCRC(randomSeq, randomSeq.length(), polynomial);
    cout << "CRC: " << crc << endl;
    transmitted = randomSeq + crc;
    isValid = verifyCRC(transmitted, polynomial);
    cout << "CRC проверки: " << (isValid ? "0 (верно)" : "не 0 (ошибка)") << endl;

    return 0;
}