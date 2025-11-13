#include "input.h"
#include <iostream>
using namespace std;

// ================== Реалізація структур ==================

Polynomial::Polynomial() {
    head = nullptr;
}

void Polynomial::addMonom(Monom m) {
    if (m.coef == 0.0) return;
    Node* newNode = new Node{m, nullptr};

    // якщо список порожній
    if (!head) {
        head = newNode;
        return;
    }

    // якщо степінь більша за перший
    if (m.power > head->data.power) {
        newNode->next = head;
        head = newNode;
        return;
    }

    // якщо степінь та сама, що й у першого
    if (m.power == head->data.power) {
        head->data.coef += m.coef;
        delete newNode;
        if (head->data.coef == 0.0) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        return;
    }

    // шукаємо місце далі
    Node* cur = head;
    while (cur->next && cur->next->data.power >= m.power) {
        if (cur->next->data.power == m.power) {
            cur->next->data.coef += m.coef;
            delete newNode;
            if (cur->next->data.coef == 0.0) {
                Node* tmp = cur->next;
                cur->next = cur->next->next;
                delete tmp;
            }
            return;
        }
        cur = cur->next;
    }

    newNode->next = cur->next;
    cur->next = newNode;
}

void Polynomial::print() {
    if (!head) {
        cout << "0\n";
        return;
    }

    Node* cur = head;
    bool first = true;
    while (cur) {
        double c = cur->data.coef;
        int p = cur->data.power;

        if (!first) {
            if (c >= 0) cout << " + ";
            else {
                cout << " - ";
                c = -c;
            }
        } else {
            if (c < 0) {
                cout << "-";
                c = -c;
            }
            first = false;
        }

        if (p == 0) {
            cout << c;
        } else if (p == 1) {
            if (c == 1) cout << "x";
            else cout << c << "x";
        } else {
            if (c == 1) cout << "x^" << p;
            else cout << c << "x^" << p;
        }

        cur = cur->next;
    }
    cout << "\n";
}

void Polynomial::clear() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    head = nullptr;
}

// ================== Допоміжні функції ==================

void removeSpaces(char* s) {
    int j = 0;
    for (int i = 0; s[i] != '\0'; ++i) {
        if (!isspace((unsigned char)s[i]))
            s[j++] = s[i];
    }
    s[j] = '\0';
}

double readNumber(const char* term, int& len) {
    int i = 0;
    char buf[64];
    int bpos = 0;
    bool hasDigit = false;
    bool hasDot = false;

    // опційний знак
    if (term[i] == '+' || term[i] == '-') {
        buf[bpos++] = term[i];
        i++;
    }

    // цифри і, можливо, одна крапка
    while (isdigit((unsigned char)term[i]) || (term[i] == '.' && !hasDot)) {
        if (term[i] == '.') hasDot = true;
        else hasDigit = true;
        buf[bpos++] = term[i];
        i++;
    }

    buf[bpos] = '\0';
    len = i;

    if (!hasDigit) {
        len = 0;
        return 0.0;
    }

    return atof(buf);
}

Monom parseFactor(const char* s, int& consumed) {
    Monom m;
    m.coef = 1.0;
    m.power = 0;

    int i = 0;
    int lenNum = 0;

    // може починатися з числа (знаком або без)
    if (isdigit((unsigned char)s[i]) || s[i] == '.' || s[i] == '+' || s[i] == '-') {
        double num = readNumber(s + i, lenNum);
        if (lenNum > 0) {
            m.coef = num;
            i += lenNum;
        }
    }

    // змінна x і степінь
    if (s[i] == 'x') {
        i++;
        if (s[i] == '^') {
            i++;
            int power = 0;
            while (isdigit((unsigned char)s[i])) {
                power = power * 10 + (s[i] - '0');
                i++;
            }
            m.power = power;
        } else {
            m.power = 1;
        }
    }

    consumed = i;
    return m;
}

Monom parseTerm(const char* term) {
    Monom result;
    result.coef = 0;
    result.power = 0;

    int i = 0;

    int termSign = 1;
    if (term[i] == '+') {
        i++;
    } else if (term[i] == '-') {
        termSign = -1;
        i++;
    }

    int consumed = 0;
    Monom fac = parseFactor(term + i, consumed);
    double accCoef = fac.coef;
    int accPower = fac.power;
    i += consumed;

    // обробка * і /
    while (term[i] == '*' || term[i] == '/') {
        char op = term[i];
        i++;
        Monom fac2 = parseFactor(term + i, consumed);
        i += consumed;

        if (op == '*') {
            accCoef *= fac2.coef;
            accPower += fac2.power;
        } else {
            accCoef /= fac2.coef;
            accPower -= fac2.power;
        }
    }

    result.coef = termSign * accCoef;
    result.power = accPower;
    return result;
}

// ================== Зчитування полінома ==================

// 1) зчитування з будь-якого потоку (файл / cin / stringstream)
Polynomial inputPolynomialFromStream(std::istream& in) {
    char input[256];

    if (!in.getline(input, 256)) {
        // нічого не прочитали — повертаємо порожній поліном
        return Polynomial();
    }

    removeSpaces(input);

    // додаємо знак, якщо його нема
    char expr[260];
    if (input[0] != '+' && input[0] != '-') {
        expr[0] = '+';
        strcpy(expr + 1, input);
    } else {
        strcpy(expr, input);
    }

    Polynomial poly;
    int i = 0;
    int tpos = 0;
    char term[100];

    while (expr[i] != '\0') {
        if ((expr[i] == '+' || expr[i] == '-') && i != 0) {
            term[tpos] = '\0';
            Monom m = parseTerm(term);
            poly.addMonom(m);
            tpos = 0;
            term[tpos++] = expr[i];
        } else {
            term[tpos++] = expr[i];
        }
        i++;
    }

    // останній терм
    term[tpos] = '\0';
    Monom last = parseTerm(term);
    poly.addMonom(last);

    return poly;
}

// 2) зчитування з консолі — просто обгортка
Polynomial inputPolynomial() {
    return inputPolynomialFromStream(std::cin);
}
