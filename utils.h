#ifndef POLY_HPP
#define POLY_HPP
#include <iostream>

struct Mono {
    double coef;       // коефіцієнт
    unsigned power;    // степінь
    Mono* next;        // вказівник на наступний елемент
};

// Сам поліном — це просто вказівник на перший елемент
struct Poly {
    Mono* head;
};

// створити порожній поліном
Poly make_empty_poly();

// очистити пам'ять
void clear_poly(Poly& P);

// вставити моном (поки що буде порожня реалізація)
void insert_mono(Poly& P, double coef, unsigned power);

// вивести поліном
void print_poly(const Poly& P);

// порахувати значення в точці (поки що 0)
double value(const Poly& P, double x0);

// додавання
Poly add(const Poly& A, const Poly& B);

// віднімання
Poly sub(const Poly& A, const Poly& B);

// множення
Poly mul(const Poly& A, const Poly& B);

// піднесення до степеня
Poly power(Poly P, unsigned k);

#endif // POLY_HPP
