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

    // ====== 2. ВВІД ПОЛІНОМА РЯДКОМ ======
    cout << "\n----------------------------------------\n";
    cout << "Введіть поліном у вигляді рядка (3x^2 - 2x + x*x/2 - 4):\n";

    // це тип з input.h (наш список)
    Polynomial userPoly = inputPolynomial();

    cout << "\nОтриманий поліном (спарсений з рядка):\n";
    userPoly.print();

    // ====== 3. КОНВЕРСІЯ З Polynomial -> Poly ======
    // створимо Poly з utils.h і перепишемо туди всі мономи з userPoly
    Poly U = make_empty_poly();   // U - user poly в "університетському" форматі

    Node* cur = userPoly.head;
    while (cur != nullptr) {
        // insert_mono сам підставить у потрібне місце, якщо він так само впорядкований
        insert_mono(U, cur->data.coef, cur->data.power);
        cur = cur->next;
    }

    // тепер U — це той самий поліном, що й введений, але вже у форматі utils.h
    cout << "\nВведений поліном у форматі utils.h:\n";
    print_poly(U);
    cout << "\n";

    // ====== 4. ТІ САМІ ДІЇ, АЛЕ З ВВЕДЕНИМ ПОЛІНОМОМ ======
    cout << "\n=== Операції з введеним поліномом ===\n";

    cout << "U(2) = " << value(U, 2.0) << "\n";

    // додамо його до Q, щоб показати, що все працює
    Poly UplusQ = add(U, Q);
    cout << "U + Q = "; print_poly(UplusQ); cout << "\n";

    Poly UminusQ = sub(U, Q);
    cout << "U - Q = "; print_poly(UminusQ); cout << "\n";

    Poly UmulQ = mul(U, Q);
    cout << "U * Q = "; print_poly(UmulQ); cout << "\n";

    Poly U3 = power(U, 3);
    cout << "U^3   = "; print_poly(U3); cout << "\n";

    // ====== 5. ПРИБИРАЄМО ВСЕ, ЩО НАНАСОЗДАВАЛИ ======
    clear_poly(P);
    clear_poly(Q);
    clear_poly(S);
    clear_poly(D);
    clear_poly(M);
    clear_poly(P3);

    clear_poly(U);
    clear_poly(UplusQ);
    clear_poly(UminusQ);
    clear_poly(UmulQ);
    clear_poly(U3);

    // це для нашого списку з input.h
    userPoly.clear();

    return 0;
}
