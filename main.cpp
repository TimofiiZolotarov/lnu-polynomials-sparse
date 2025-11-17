#include <iostream>
#include "input.h"
#include "utils.h"

using namespace std;

// Перетворити Polynomial (input.h) -> Poly (utils.h)
Poly toPoly(const Polynomial& src) {
    Poly  res = make_empty_poly();
    Node* cur = src.head;
    while (cur) {
        double coef  = cur->data.coef;
        int    power = cur->data.power;

        if (power < 0) {
            cerr << "Увага: моном з відʼємним степенем "
                 << power << " пропущено.\n";
        } else if (coef != 0.0) {
            insert_mono(res, coef, (unsigned)power);
        }

        cur = cur->next;
    }
    return res;
}

int main() {
    Polynomial P1;
    Polynomial P2;

    cout << "Звідки зчитувати поліноми?\n";
    cout << "1 - З клавіатури\n";
    cout << "2 - З файлу \n";
    cout << "Ваш вибір: ";

    int mode;
    cin >> mode;
    cin.ignore(); // прибрати '\n' після числа

    if (mode == 1) {
        // з КЛАВІАТУРИ: два поліноми в одному рядку через кому
        if (!readTwoPolynomialsFromCin(P1, P2)) {
            cout << "Помилка введення: немає коми або рядок некоректний.\n";
            return 1;
        }
    } else {
        // з ФАЙЛУ: два поліноми в одному рядку через кому
        char fname[256];
        cout << "Введіть назву файла (наприклад polys.txt): ";
        cin.getline(fname, 256);

        if (!readTwoPolynomialsFromFile(fname, P1, P2)) {
            cout << "Помилка читання поліномів з файлу!\n";
            return 1;
        }
    }

    cout << "\nP1(x) = ";
    P1.print();
    cout << "\n";

    cout << "P2(x) = ";
    P2.print();
    cout << "\n";

    // Конвертуємо у формат Poly (utils.h)
    Poly A = toPoly(P1);
    Poly B = toPoly(P2);

    cout << "\nОберіть дію над поліномами:\n";
    cout << "1 - P1(x) + P2(x)\n";
    cout << "2 - P1(x) - P2(x)\n";
    cout << "3 - P1(x) * P2(x)\n";
    cout << "4 - P1(x) / P2(x) (частка і остача)\n";
    cout << "5 - Обчислити значення P1(x0)\n";
    cout << "6 - Похідна P1'(x)\n";
    cout << "Ваш вибір: ";

    int op;
    cin >> op;
    cin.ignore(); // '\n'

    switch (op) {
        case 1: {
            Poly Sum = add(A, B);
            cout << "\nP1(x) + P2(x) = ";
            print_poly(Sum);
            cout << "\n";
            clear_poly(Sum);
            break;
        }
        case 2: {
            Poly Diff = sub(A, B);
            cout << "\nP1(x) - P2(x) = ";
            print_poly(Diff);
            cout << "\n";
            clear_poly(Diff);
            break;
        }
        case 3: {
            Poly Prod = mul(A, B);
            cout << "\nP1(x) * P2(x) = ";
            print_poly(Prod);
            cout << "\n";
            clear_poly(Prod);
            break;
        }
        case 4: {
            cout << "\nP1(x) / P2(x):\n";
            if (is_zero(B)) {
                cout << "Помилка: ділення на нульовий поліном!\n";
            } else {
                Poly Q = make_empty_poly();
                Poly R = make_empty_poly();

                div(A, B, Q, R);

                cout << "Q(x) = ";
                print_poly(Q);
                cout << "\n";

                cout << "R(x) = ";
                print_poly(R);
                cout << "\n";

                clear_poly(Q);
                clear_poly(R);
            }
            break;
        }
        case 5: {
            double x0;
            cout << "\nВведіть значення x0: ";
            cin >> x0;

            double v1 = value(A, x0);
            cout << "P1(" << x0 << ") = " << v1 << "\n";
            break;
        }
        case 6: {
            Poly D = derivative(A); // похідна в utils.cpp
            cout << "\nP1'(x) = ";
            print_poly(D);
            cout << "\n";
            clear_poly(D);
            break;
        }
        default:
            cout << "\nНевірний вибір дії.\n";
    }

    clear_poly(A);
    clear_poly(B);
    P1.clear();
    P2.clear();

    return 0;
}
