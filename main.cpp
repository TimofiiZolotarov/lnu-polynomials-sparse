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

void print_menu() {
    cout << "\n============================================================\n";
    cout << "                 ОПЕРАЦІЇ З ПОЛІНОМАМИ\n";
    cout << "============================================================\n";
    cout << " 1)  Додавання            :   P1(x) + P2(x)\n";
    cout << " 2)  Віднімання           :   P1(x) - P2(x)\n";
    cout << " 3)  Множення             :   P1(x) *  P2(x)\n";
    cout << " 4)  Ділення в стовпчик   :   P1(x) /  P2(x)\n";
    cout << " 5)  Обчислити P1(x0)\n";
    cout << " 6)  Похідна P1(x)\n";
    cout << " 7)  Піднесення P1(x) до степеня k\n";
    cout << "------------------------------------------------------------\n";
    cout << " 0)  ВИХІД\n";
    cout << "============================================================\n";
    cout << "Ваш вибір: ";
}

void flushInput() {
    cin.clear();
    while (cin.get() != '\n');
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
    flushInput();

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

    // конвертація у формат utils.h
    Poly A = toPoly(P1);
    Poly B = toPoly(P2);

    // меню
    while (true) {
        print_menu();

        int op;
        if (!(cin >> op)) {
            flushInput();
            cout << "Будь ласка, введіть число від 0 до 7.\n";
            continue;
        }
        flushInput();

        if (op == 0) {
            cout << "Вихід...\n";
            break;
        }

        cout << "------------------------------------------------------------\n";

        switch (op) {
            case 1: {
                cout << "P1(x) + P2(x) = ";
                Poly R = add(A, B);
                print_poly(R); cout << "\n";
                clear_poly(R);
                break;
            }
            case 2: {
                cout << "P1(x) - P2(x) = ";
                Poly R = sub(A, B);
                print_poly(R); cout << "\n";
                clear_poly(R);
                break;
            }
            case 3: {
                cout << "P1(x) * P2(x) = ";
                Poly R = mul(A, B);
                print_poly(R); cout << "\n";
                clear_poly(R);
                break;
            }
            case 4: {
                if (is_zero(B)) {
                    cout << "Ділення неможливе: P2(x) = 0\n";
                    break;
                }
                Poly Q = make_empty_poly(), R = make_empty_poly();
                div(A, B, Q, R);
                cout << "Q(x) = "; print_poly(Q);
                cout << "\nR(x) = "; print_poly(R); cout << "\n";
                clear_poly(Q);
                clear_poly(R);
                break;
            }
            case 5: {
                double x;
                cout << "x0 = ";
                cin >> x;
                flushInput();

                cout << "P1(" << x << ") = " << value(A, x) << "\n";
                break;
            }
            case 6: {
                cout << "Похідна P1(x): ";
                Poly D = derivative(A);
                print_poly(D); cout << "\n";
                clear_poly(D);
                break;
            }
            case 7: {
                int k;
                cout << "k = ";
                cin >> k;
                flushInput();

                if (k < 0) {
                    cout << "k має бути >= 0\n";
                    break;
                }

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
