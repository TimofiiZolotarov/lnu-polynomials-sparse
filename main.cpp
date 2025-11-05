#include "utils.h"
using namespace std;

int main() {
    // створюємо порожній поліном
    Poly P = make_empty_poly();

    // пробуємо "вставити" щось (поки що функція пуста, але вона є)
    insert_mono(P, 5.0, 2); // 5x^2
    insert_mono(P, 3.0, 0); // +3

    cout << "Polynomial(x) = ";
    print_poly(P);
    cout << endl;

    double res = value(P, 2.0);
    cout << "Result: " << res << endl;

    // обов'язково очищаємо
    clear_poly(P);

    return 0;
}
