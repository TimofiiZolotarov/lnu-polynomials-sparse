#include <iostream>
#include <fstream>
#include "utils.h"
#include "input.h"
using namespace std;

int main() {
    cout << "\nЗвідки зчитувати поліноми?\n";
    cout << "1 - з клавіатури\n";
    cout << "2 - з файлу\n";
    cout << "Ваш вибір: ";
    int mode;
    cin >> mode;
    cin.ignore(); // щоб прибрати \n після числа

    Polynomial userPoly1;
    Polynomial userPoly2;

    if (mode == 1) {
        // ---------- з КОНСОЛІ ----------
        cout << "\nВведіть перший поліном:\n";
        userPoly1 = inputPolynomial();   // читає з cin

        cout << "\nВведіть другий поліном:\n";
        userPoly2 = inputPolynomial();   // читає з cin
    } else {
        // ---------- з ФАЙЛУ ----------
        cout << "Введіть назву файла (наприклад polys.txt): ";
        string fname;
        getline(cin, fname);

        ifstream fin(fname);
        if (!fin) {
            cout << "Не вдалося відкрити файл!\n";
            return 1;
        }

        // читаємо два рядки = два поліноми
        userPoly1 = inputPolynomialFromStream(fin);
        userPoly2 = inputPolynomialFromStream(fin);
    }

    cout << "\nПерший поліном:\n";
    userPoly1.print();
    cout << "Другий поліном:\n";
    userPoly2.print();

    // ====== 3. Конвертуємо обидва у формат utils.h ======
    Poly A = make_empty_poly();
    Node* cur = userPoly1.head;
    while (cur) {
        insert_mono(A, cur->data.coef, cur->data.power);
        cur = cur->next;
    }

    Poly Bp = make_empty_poly();
    cur = userPoly2.head;
    while (cur) {
        insert_mono(Bp, cur->data.coef, cur->data.power);
        cur = cur->next;
    }

    cout << "\nУ форматі utils.h:\n";
    cout << "A(x) = "; print_poly(A); cout << "\n";
    cout << "B(x) = "; print_poly(Bp); cout << "\n";

    // ====== 4. Операції ======
    Poly AB_sum = add(A, Bp);
    cout << "A + B = "; print_poly(AB_sum); cout << "\n";

    Poly AB_sub = sub(A, Bp);
    cout << "A - B = "; print_poly(AB_sub); cout << "\n";

    Poly AB_mul = mul(A, Bp);
    cout << "A * B = "; print_poly(AB_mul); cout << "\n";

    cout << "Введіть n для A^n: ";
    int n;
    cin >> n;
    Poly A_pow = power(A, n);
    cout << "A^" << n << " = "; print_poly(A_pow); cout << "\n";

    // ====== 5. Прибираємо пам'ять ======
    clear_poly(A);
    clear_poly(Bp);
    clear_poly(AB_sum);
    clear_poly(AB_sub);
    clear_poly(AB_mul);
    clear_poly(A_pow);

    userPoly1.clear();
    userPoly2.clear();

    return 0;
}
