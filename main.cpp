#include <iostream>
#include "utils.h"   // твій старий модуль з Poly
#include "input.h"   // наш новий парсер рядка в Polynomial
using namespace std;

int main() {
    // ====== 1. СТАРИЙ ДЕМОНСТРАЦІЙНИЙ ПОЛІНОМ ======
    // P(x) = 5x^2 + 3
    Poly P = make_empty_poly();
    insert_mono(P, 5.0, 2);
    insert_mono(P, 3.0, 0);

    // Q(x) = -x + 4
    Poly Q = make_empty_poly();
    insert_mono(Q, -1.0, 1);
    insert_mono(Q,  4.0, 0);

    cout << "=== Демонстрація зі статичними поліномами ===\n";
    cout << "P(x) = "; print_poly(P); cout << "\n";
    cout << "Q(x) = "; print_poly(Q); cout << "\n";
    cout << "P(2) = " << value(P, 2.0) << "\n";

    Poly S  = add(P, Q);
    Poly D  = sub(P, Q);
    Poly M  = mul(P, Q);
    Poly P3 = power(P, 3);

    cout << "P + Q = "; print_poly(S);  cout << "\n";
    cout << "P - Q = "; print_poly(D);  cout << "\n";
    cout << "P * Q = "; print_poly(M);  cout << "\n";
    cout << "P^3   = "; print_poly(P3); cout << "\n";

    // ====== 2. ВВОДИМО ДВА ПОЛІНОМИ РЯДКОМ ======
    cout << "\n----------------------------------------\n";
    cout << "Введіть ПЕРШИЙ поліном у вигляді рядка (3x^2 - 2x + x*x/2 - 4):\n";
    // перший у форматі нашого списку
    Polynomial userPoly1 = inputPolynomial();

    cout << "\nВведіть ДРУГИЙ поліном у вигляді рядка:\n";
    Polynomial userPoly2 = inputPolynomial();

    cout << "\nПерший (спарсений) поліном:\n";
    userPoly1.print();
    cout << "Другий (спарсений) поліном:\n";
    userPoly2.print();

    // ====== 3. КОНВЕРСІЯ у формат utils.h ======
    Poly A = make_empty_poly();
    Poly Bp = make_empty_poly();   // назвав Bp щоб не плутатись із старим Q

    // скопіювати userPoly1 -> A
    Node* cur = userPoly1.head;
    while (cur != nullptr) {
        insert_mono(A, cur->data.coef, cur->data.power);
        cur = cur->next;
    }

    // скопіювати userPoly2 -> Bp
    cur = userPoly2.head;
    while (cur != nullptr) {
        insert_mono(Bp, cur->data.coef, cur->data.power);
        cur = cur->next;
    }

    cout << "\nПерший поліном у форматі utils.h: ";
    print_poly(A); cout << "\n";
    cout << "Другий поліном у форматі utils.h: ";
    print_poly(Bp); cout << "\n";

    // ====== 4. ОПЕРАЦІЇ НАД введеними поліномами ======
    cout << "\n=== Операції з введеними поліномами ===\n";

    // A(2)
    cout << "A(2) = " << value(A, 2.0) << "\n";
    cout << "B(2) = " << value(Bp, 2.0) << "\n";

    // A + B
    Poly AB_sum = add(A, Bp);
    cout << "A + B = "; print_poly(AB_sum); cout << "\n";

    // A - B
    Poly AB_sub = sub(A, Bp);
    cout << "A - B = "; print_poly(AB_sub); cout << "\n";

    // B - A (іноді треба й так)
    Poly BA_sub = sub(Bp, A);
    cout << "B - A = "; print_poly(BA_sub); cout << "\n";

    // A * B
    Poly AB_mul = mul(A, Bp);
    cout << "A * B = "; print_poly(AB_mul); cout << "\n";

    // степінь A^n
    cout << "Введіть n для A^n: ";
    int n;
    cin >> n;
    Poly A_pow = power(A, n);
    cout << "A^" << n << " = "; print_poly(A_pow); cout << "\n";

    // ====== 5. ПРИБИРАЄМО ВСЕ ======
    // старі
    clear_poly(P);
    clear_poly(Q);
    clear_poly(S);
    clear_poly(D);
    clear_poly(M);
    clear_poly(P3);

    // нові
    clear_poly(A);
    clear_poly(Bp);
    clear_poly(AB_sum);
    clear_poly(AB_sub);
    clear_poly(BA_sub);
    clear_poly(AB_mul);
    clear_poly(A_pow);

    userPoly1.clear();
    userPoly2.clear();

    return 0;
}
