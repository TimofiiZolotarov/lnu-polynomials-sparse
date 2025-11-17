#include <iostream>
#include "input.h"
#include "utils.h"

using namespace std;

// Конвертація Polynomial > Poly
Poly toPoly(const Polynomial& src) {
    Poly res = make_empty_poly();
    Node* cur = src.head;
    while (cur) {
        if (cur->data.power >= 0 && cur->data.coef != 0.0)
            insert_mono(res, cur->data.coef, (unsigned)cur->data.power);
        cur = cur->next;
    }
    return res;
}

int main() {
    Polynomial P1, P2;

    // вибір джерела
    cout << "Звідки зчитувати?\n"
            "1 - клавіатура\n"
            "2 - файл\n"
            "Ваш вибір: ";

    int mode;
    cin >> mode;
    cin.ignore();

    // зчитування
    if (mode == 1) {
        if (!readTwoPolynomialsFromCin(P1, P2)) {
            cout << "Помилка введення\n";
            return 1;
        }
    } else {
        char fname[256];
        cout << "Файл: ";
        cin.getline(fname, 256);

        if (!readTwoPolynomialsFromFile(fname, P1, P2)) {
            cout << "Помилка читання файлу\n";
            return 1;
        }
    }

    // друк
    cout << "\nP1(x) = "; P1.print();
    cout << "P2(x) = "; P2.print();

    // конвертація у формат worms.h
    Poly A = toPoly(P1);
    Poly B = toPoly(P2);

    // меню
    while (true) {
        cout << "\n1 - додавання\n"
                "2 - віднімання\n"
                "3 - множення\n"
                "4 - ділення в стовпчик\n"
                "5 - значення P1(x0)\n"
                "6 - похідна P1\n"
                "7 - P1^k\n0 - вихід\n"
                "Ваш вибір: ";

        int op;
        cin >> op;

        if (op == 0) break;

        switch (op) {
            case 1: {
                Poly R = add(A, B);
                print_poly(R); cout << "\n";
                clear_poly(R);
                break;
            }
            case 2: {
                Poly R = sub(A, B);
                print_poly(R); cout << "\n";
                clear_poly(R);
                break;
            }
            case 3: {
                Poly R = mul(A, B);
                print_poly(R); cout << "\n";
                clear_poly(R);
                break;
            }
            case 4: {
                if (is_zero(B)) { cout << "Ділення в стовпчик неможливе\n"; break; }
                Poly Q = make_empty_poly(), R = make_empty_poly();
                div(A, B, Q, R);
                cout << "Q(x) = "; print_poly(Q);
                cout << "\nR(x) = "; print_poly(R); cout << "\n";
                clear_poly(Q); clear_poly(R);
                break;
            }
            case 5: {
                double x;
                cout << "x0 = "; cin >> x;
                cout << "P1(" << x << ") = " << value(A, x) << "\n";
                break;
            }
            case 6: {
                Poly D = derivative(A);
                print_poly(D); cout << "\n";
                clear_poly(D);
                break;
            }
            case 7: {
                int k;
                cout << "k = "; cin >> k;
                Poly R = power(A, k);
                print_poly(R); cout << "\n";
                clear_poly(R);
                break;
            }
            default:
                cout << "Невірна команда\n";
        }
    }

    // очищення
    clear_poly(A);
    clear_poly(B);
    P1.clear();
    P2.clear();

    return 0;
}
