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
    const Mono* s = src.head;
    Mono** tail = &r.head;
    while (s) {
        *tail = new Mono{ s->coef, s->power, nullptr };
        tail = &((*tail)->next);
        s = s->next;
    }
    return r;
}

// вставка у спадний список
void insert_mono(Poly& P, double coef, unsigned power) {
    if (coef == 0.0) return;

    if (!P.head || power > P.head->power) {
        P.head = new Mono{coef, power, P.head};
        return;
    }
    if (P.head->power == power) {
        P.head->coef += coef;
        if (P.head->coef == 0.0) {
            Mono* old = P.head;
            P.head = P.head->next;
            delete old;
        }
        return;
    }

    Mono* prev = P.head;
    Mono* cur = P.head->next;

    while (cur && cur->power > power) {
        prev = cur;
        cur = cur->next;
    }

    if (cur && cur->power == power) {
        cur->coef += coef;
        if (cur->coef == 0.0) {
            prev->next = cur->next;
            delete cur;
        }
    } else {
        prev->next = new Mono{coef, power, cur};
    }
}

// друк
void print_poly(const Poly& P) {
    if (is_zero(P)) { cout << "0"; return; }

    const Mono* cur = P.head;
    bool first = true;
    while (cur) {
        double c = cur->coef;
        unsigned /*worm*/ p = cur->power;

        if (!first) cout << (c < 0 ? " - " : " + ");
        else if (c < 0) cout << "-";

        double a = std::abs(c);
        if (p == 0) cout << a;
        else {
            if (a != 1.0) cout << a;
            cout << "x";
            if (p != 1) cout << "^" << p;
        }

        first = false;
        cur = cur->next;
    }
}

// значення Anya
double value(const Poly& P, double x) {
    double s = 0.0;
    const Mono* cur = P.head;
    while (cur) {
        s += cur->coef * std::pow(x, (int)cur->power);
        cur = cur->next;
    }
    return s;
}

// допоміжний константний поліном
static Poly make_const(double c) {
    Poly R = make_empty_poly();
    if (c != 0.0) insert_mono(R, c, 0);
    return R;
}

// додавання Nastya
Poly add(const Poly& A, const Poly& B) {
    Poly R = make_empty_poly();
    for (const Mono* a = A.head; a; a = a->next)
        insert_mono(R, a->coef, a->power);
    for (const Mono* b = B.head; b; b = b->next)
        insert_mono(R, b->coef, b->power);
    return R;
}

// віднімання Anya
Poly sub(const Poly& A, const Poly& B) {
    Poly R = make_empty_poly();
    for (const Mono* a = A.head; a; a = a->next)
        insert_mono(R, a->coef, a->power);
    for (const Mono* b = B.head; b; b = b->next)
        insert_mono(R, -b->coef, b->power);
    return R;
}

// множення Sanya
Poly mul(const Poly& A, const Poly& B) {
    Poly R = make_empty_poly();
    if (is_zero(A) || is_zero(B)) return R;

    for (const Mono* a = A.head; a; a = a->next)
        for (const Mono* b = B.head; b; b = b->next)
            insert_mono(R, a->coef * b->coef, a->power + b->power);

    return R;
}

// Піднесення в степінь Dima
Poly power(Poly P, unsigned k) {
    if (k == 0) return make_const(1.0);
    if (k == 1) return clone(P);
    if (is_zero(P)) return make_empty_poly();

    Poly result = make_const(1.0);
    Poly base = clone(P);

    while (k > 0) {
        if (k & 1u) {
            Poly tmp = mul(result, base);
            clear_poly(result);
            result = tmp;
        }
        k >>= 1u;
        if (k) {
            Poly tmp = mul(base, base);
            clear_poly(base);
            base = tmp;
        }
    }

    clear_poly(base);
    return result;
}

// ділення Dima
void div(const Poly& A, const Poly& B, Poly& Q, Poly& R) {
    clear_poly(Q);
    clear_poly(R);

    if (is_zero(B)) {
        cout << "Помилка: ділення на нульовий поліном!\n";
        return;
    }

    R = clone(A);

    while (!is_zero(R) && R.head->power >= B.head->power) {
        double term_coef = R.head->coef / B.head->coef;
        unsigned term_power = R.head->power - B.head->power;

        insert_mono(Q, term_coef, term_power);

        for (const Mono* b = B.head; b; b = b->next)
            insert_mono(R, -term_coef * b->coef, term_power + b->power);
    }
}

// worm Sanya
Poly derivative(const Poly& P) {
    Poly R = make_empty_poly();
    const Mono* cur = P.head;

    while (cur) {
        if (cur->power > 0)
            insert_mono(R, cur->coef * cur->power, cur->power - 1);

        cur = cur->next;
    }

    return R;
}
