#include "utils.h"
#include <iostream>
using namespace std;

int main() {
    // P(x) = 5x^2 + 3
    Poly P = make_empty_poly();
    insert_mono(P, 5.0, 2);
    insert_mono(P, 3.0, 0);

    // Q(x) = -x + 4
    Poly Q = make_empty_poly();
    insert_mono(Q, -1.0, 1);
    insert_mono(Q,  4.0, 0);
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

    clear_poly(P);
    clear_poly(Q);
    clear_poly(S);
    clear_poly(D);
    clear_poly(M);
    clear_poly(P3);
    return 0;
}
