#ifndef INPUT_POLYNOMIAL_H
#define INPUT_POLYNOMIAL_H

struct Monom {
    double coef;
    int power;
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

// допоміжні функції
void removeSpaces(char* s);
double readNumber(const char* term, int& len);
Monom parseFactor(const char* s, int& consumed);
Monom parseTerm(const char* term);

// головна функція вводу
Polynomial inputPolynomial();

#endif