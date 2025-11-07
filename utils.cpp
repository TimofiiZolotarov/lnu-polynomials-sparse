#include "utils.h"
#include <cmath>
#include <iostream>
using namespace std;

// створити порожній поліном
Poly make_empty_poly() { Poly P{}; P.head = nullptr; return P; }

// очистити пам'ять
void clear_poly(Poly& P) {
    Mono* cur = P.head;
    while (cur) { Mono* nx = cur->next; delete cur; cur = nx; }
    P.head = nullptr;
}

// нульовий?
bool is_zero(const Poly& P) { return P.head == nullptr; }

// глибока копія
Poly clone(const Poly& src) {
    Poly r = make_empty_poly();
    const Mono* s = src.head; Mono** tail = &r.head;
    while (s) { *tail = new Mono{ s->coef, s->power, nullptr }; tail = &((*tail)->next); s = s->next; }
    return r;
}

// вставка у спадний список; об'єднання степенів; видалення нулів
void insert_mono(Poly& P, double coef, unsigned power) {
    if (coef == 0.0) return;

    if (!P.head || power > P.head->power) { // вставка на початок
        P.head = new Mono{coef, power, P.head}; return;
    }
    if (P.head->power == power) {           // злиття з головою
        P.head->coef += coef;
        if (P.head->coef == 0.0) { Mono* old = P.head; P.head = P.head->next; delete old; }
        return;
    }
    Mono* prev = P.head; Mono* cur = P.head->next;
    while (cur && cur->power > power) { prev = cur; cur = cur->next; }
    if (cur && cur->power == power) {       // злиття всередині
        cur->coef += coef;
        if (cur->coef == 0.0) { prev->next = cur->next; delete cur; }
    } else {                                 // вставка між prev і cur
        prev->next = new Mono{coef, power, cur};
    }
}

// друк: 5x^2 - x + 3
void print_poly(const Poly& P) {
    if (is_zero(P)) { cout << "0"; return; }
    const Mono* cur = P.head; bool first = true;
    cout.setf(std::ios::fmtflags(0), std::ios::floatfield); // без наук. нотації
    cout.unsetf(std::ios::showpos);
    while (cur) {
        double c = cur->coef; unsigned p = cur->power;
        if (c == 0.0) { cur = cur->next; continue; }        // перестраховка
        if (!first) cout << (c < 0 ? " - " : " + ");         // знак
        else if (c < 0) cout << "-";
        double a = std::abs(c);
        if (p == 0) cout << a;                               // константа
        else { if (a != 1.0) cout << a; cout << "x"; if (p != 1) cout << "^" << p; }
        first = false; cur = cur->next;
    }
}

// обчислення значення: sum coef * x^power
double value(const Poly& P, double x) {
    double s = 0.0; const Mono* cur = P.head;
    while (cur) { if (cur->coef != 0.0) s += cur->coef * std::pow(x, (int)cur->power); cur = cur->next; }
    return s;
}

// допоміжний константний поліном
static Poly make_const(double c) { Poly R = make_empty_poly(); if (c != 0.0) insert_mono(R, c, 0); return R; }

// A + B
Poly add(const Poly& A, const Poly& B) {
    Poly R = make_empty_poly();
    for (const Mono* a = A.head; a; a = a->next) if (a->coef) insert_mono(R, a->coef, a->power);
    for (const Mono* b = B.head; b; b = b->next) if (b->coef) insert_mono(R, b->coef, b->power);
    return R;
}

// A - B
Poly sub(const Poly& A, const Poly& B) {
    Poly R = make_empty_poly();
    for (const Mono* a = A.head; a; a = a->next) if (a->coef) insert_mono(R, a->coef, a->power);
    for (const Mono* b = B.head; b; b = b->next) if (b->coef) insert_mono(R, -b->coef, b->power);
    return R;
}

// A * B
Poly mul(const Poly& A, const Poly& B) {
    Poly R = make_empty_poly(); if (is_zero(A) || is_zero(B)) return R;
    for (const Mono* a = A.head; a; a = a->next)
        if (a->coef) for (const Mono* b = B.head; b; b = b->next)
            if (b->coef) insert_mono(R, a->coef * b->coef, a->power + b->power);
    return R;
}

// P^k (бінарне піднесення)
Poly power(Poly P, unsigned k) {
    if (k == 0) return make_const(1.0);
    if (k == 1) return clone(P);
    if (is_zero(P)) return make_empty_poly();

    Poly res = make_const(1.0);
    Poly base = clone(P);
    while (k) {
        if (k & 1u) { Poly t = mul(res, base); clear_poly(res); res = t; }
        k >>= 1u;
        if (k) { Poly t = mul(base, base); clear_poly(base); base = t; }
    }
    clear_poly(base);
    return res;
}
