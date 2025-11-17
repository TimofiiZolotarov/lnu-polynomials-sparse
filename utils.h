#ifndef POLY_HPP
#define POLY_HPP
#include <iostream> 

struct Mono {
    double   coef;
    unsigned power;
    Mono*    next;
};

// Поліном як однозв'язний список (спадний порядок power)
struct Poly {
    Mono* head;
};

// Базові утиліти wormy
Poly  make_empty_poly();      // створити порожній поліном (head = nullptr)
void  clear_poly(Poly& P);    // звільнити всі вузли; P.head = nullptr
bool  is_zero(const Poly& P); // true, якщо P.head == nullptr
Poly  clone(const Poly& src); // глибока копія списку
// hi im wormy worm! :p
void   insert_mono(Poly& P, double coef, unsigned power); // вставка з агрегацією
void   print_poly(const Poly& P);                          // друк у stdout
double value(const Poly& P, double x);                     // обчислення P(x)

//Арифметика поліномів
Poly add(const Poly& A, const Poly& B);     // A + B
Poly sub(const Poly& A, const Poly& B);     // A - B
Poly mul(const Poly& A, const Poly& B);     // A * B
Poly power(Poly P, unsigned k);             // P^k (k >= 0)
void div(const Poly& A, const Poly& B, Poly& Q, Poly& R);// A / B = Q (частка) + R (остача)

Poly derivative(const Poly& P); // Похідна P'(x)
#endif // POLY_HPP
