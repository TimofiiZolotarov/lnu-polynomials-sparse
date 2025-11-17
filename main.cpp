#include <iostream>
#include "input.h"
#include "utils.h"

using namespace std;

// Допоміжна функція: перетворити Polynomial (input.h) -> Poly (utils.h)
Poly toPoly(const Polynomial& src) {
    Poly res = make_empty_poly();
    Node* cur = src.head;
    while (cur) {
        double coef = cur->data.coef;
        int power  = cur->data.power;

        if (power < 0) {
            cerr << "Увага: моном з відʼємним степенем " << power
                 << " пропущено під час перетворення у Poly\n";
        } else if (coef != 0.0) {
            insert_mono(res, coef, static_cast<unsigned>(power));
        }

        cur = cur->next;
    }
    return res;
}

int main() {
    // 1. Зчитуємо два поліноми
    cout << "Введіть перший поліном (напр. 3x^2 - 2x + x*x/2 - 4):" << endl;
    Polynomial P1 = inputPolynomial();

    cout << "Введіть другий поліном:" << endl;
    Polynomial P2 = inputPolynomial();

    cout << "\nP1(x) = ";
    P1.print();
    cout << endl;

    cout << "P2(x) = ";
    P2.print();
    cout << endl;

    // 2. Конвертуємо у структуру Poly з utils.h
    Poly A = toPoly(P1);
    Poly B = toPoly(P2);

    // 3. Меню дій
    cout << "\nОберіть дію над поліномами:" << endl;
    cout << "1 - P1(x) + P2(x)" << endl;
    cout << "2 - P1(x) - P2(x)" << endl;
    cout << "3 - P1(x) * P2(x)" << endl;
    cout << "4 - P1(x) / P2(x) (частка і остача)" << endl;
    cout << "5 - Обчислити значення P1(x0) та P2(x0)" << endl;
    cout << "Ваш вибір: ";

    int op;
    cin >> op;

    if (op == 1) {
        // Сума
        Poly Sum = add(A, B);
        cout << "\nP1(x) + P2(x) = ";
        print_poly(Sum);
        cout << endl;
        clear_poly(Sum);
    }
    else if (op == 2) {
        // Різниця
        Poly Diff = sub(A, B);
        cout << "\nP1(x) - P2(x) = ";
        print_poly(Diff);
        cout << endl;
        clear_poly(Diff);
    }
    else if (op == 3) {
        // Добуток
        Poly Prod = mul(A, B);
        cout << "\nP1(x) * P2(x) = ";
        print_poly(Prod);
        cout << endl;
        clear_poly(Prod);
    }
    else if (op == 4) {
        // Ділення
        cout << "\nP1(x) / P2(x):" << endl;
        if (is_zero(B)) {
            cout << "Помилка: ділення на нульовий поліном!" << endl;
        } else {
            Poly Q = make_empty_poly();
            Poly R = make_empty_poly();

            div(A, B, Q, R);

            cout << "Q(x) = ";
            print_poly(Q);
            cout << endl;

            cout << "R(x) = ";
            print_poly(R);
            cout << endl;

            clear_poly(Q);
            clear_poly(R);
        }
    }
    else if (op == 5) {
        // Обчислення значення в точці x0
        double x0;
        cout << "\nВведіть значення x0: ";
        cin >> x0;

        double v1 = value(A, x0);
        double v2 = value(B, x0);

        cout << "P1(" << x0 << ") = " << v1 << endl;
        cout << "P2(" << x0 << ") = " << v2 << endl;
    }
        else if (op == 6) {
        // Рахуємо похідні
        Poly DerA = derivative(A);
        Poly DerB = derivative(B);

        // Виводимо результат
        cout << "\nP1'(x) = ";
        print_poly(DerA);
        cout << endl;

        cout << "P2'(x) = ";
        print_poly(DerB);
        cout << endl;

        // Очищаємо пам'ять
        clear_poly(DerA);
        clear_poly(DerB);
    }
    else {
        cout << "\nНевірний вибір дії." << endl;
    }

    // 4. Прибирання пам'яті
    clear_poly(A);
    clear_poly(B);

    P1.clear();
    P2.clear();

    return 0;
}
