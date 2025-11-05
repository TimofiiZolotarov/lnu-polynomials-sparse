#include "utils.h"
using namespace std;

// створюємо порожній поліном
Poly make_empty_poly() {
    Poly P;
    P.head = 0; // nullptr, але так простіше для початку
    return P;
}

// очищення всіх вузлів
void clear_poly(Poly& P) {
    Mono* cur = P.head;
    while (cur != 0) {
        Mono* next = cur->next;
        delete cur;
        cur = next;
    }
    P.head = 0;
}

// поки що нічого не вставляємо, просто заглушка
void insert_mono(Poly& /*P*/, double /*coef*/, unsigned /*power*/) {
    // тут потім буде код вставки
}

// виведення полінома
void print_poly(const Poly& P) {
    if (P.head == 0) {
        cout << "0";
    } else {
        // потім тут можна буде пройтись по списку і красиво вивести
        cout << "[poly]";
    }
}

// обчислення значення в точці
double value(const Poly& /*P*/, double /*x0*/) {
    return 0.0;
}

// додавання двох поліномів
Poly add(const Poly& /*A*/, const Poly& /*B*/) {
    return make_empty_poly();
}

// віднімання
Poly sub(const Poly& /*A*/, const Poly& /*B*/) {
    return make_empty_poly();
}

// множення
Poly mul(const Poly& /*A*/, const Poly& /*B*/) {
    return make_empty_poly();
}

// піднесення до степеня
Poly power(Poly /*P*/, unsigned /*k*/) {
    return make_empty_poly();
}
