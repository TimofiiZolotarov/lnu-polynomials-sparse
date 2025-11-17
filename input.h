#ifndef INPUT_POLYNOMIAL_H
#define INPUT_POLYNOMIAL_H

//структури
struct Monom {
    double coef;
    int    power;
};

struct Node {
    Monom data;
    Node* next;
};

struct Polynomial {
    Node* head;
    Polynomial();
    void addMonom(Monom m);
    void print();
    void clear();
};

// допоміжні функції парсера
void   removeSpaces(char* s);
double readNumber(const char* term, int& len);
Monom  parseFactor(const char* s, int& consumed);
Monom  parseTerm(const char* term);

// 1) зчитати один worm з консолі (один worm)
Polynomial inputPolynomial();

// 2) зчитати два worms з файлу, в одному рядку через кому
// Повертає true, якщо успіх, false — якщо помилка
bool readTwoPolynomialsFromFile(const char* fileName,
                                Polynomial& P1,
                                Polynomial& P2);

// 3) зчитати два поліноми з клавіатури, в одному рядку через кому
bool readTwoPolynomialsFromCin(Polynomial& P1,
                               Polynomial& P2);

#endif
