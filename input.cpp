#include "input.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
using namespace std;

Polynomial::Polynomial() {
    head = nullptr;
}

void Polynomial::addMonom(Monom m) {
    if (m.coef == 0.0) return;
    Node* newNode = new Node{m, nullptr};

    if (!head) {
        head = newNode;
        return;
    }

    if (m.power > head->data.power) {
        newNode->next = head;
        head = newNode;
        return;
    }

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
        int    p = cur->data.power;

        if (!first) {
            if (c >= 0) cout << " + ";
            else { cout << " - "; c = -c; }
        } else {
            if (c < 0) { cout << "-"; c = -c; }
            first = false;
        }

        if (p == 0)
            cout << c;
        else if (p == 1)
            (c == 1) ? cout << "x" : cout << c << "x";
        else
            (c == 1) ? cout << "x^" << p : cout << c << "x^" << p;

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

// ================== Допоміжні функції парсера ==================

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
    int  bpos = 0;
    bool hasDigit = false;
    bool hasDot   = false;

    if (term[i] == '+' || term[i] == '-') {
        buf[bpos++] = term[i];
        i++;
    }

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
    m.coef  = 1.0;
    m.power = 0;

    int i      = 0;
    int lenNum = 0;

    if (isdigit((unsigned char)s[i]) || s[i] == '.' ||
        s[i] == '+' || s[i] == '-') {
        double num = readNumber(s + i, lenNum);
        if (lenNum > 0) {
            m.coef = num;
            i += lenNum;
        }
    }

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
    result.coef  = 0;
    result.power = 0;

    int i = 0;

    int termSign = 1;
    if (term[i] == '+') {
        i++;
    } else if (term[i] == '-') {
        termSign = -1;
        i++;
    }

    int   consumed = 0;
    Monom fac      = parseFactor(term + i, consumed);
    double accCoef  = fac.coef;
    int    accPower = fac.power;
    i += consumed;

    while (term[i] == '*' || term[i] == '/') {
        char op = term[i];
        i++;
        Monom fac2 = parseFactor(term + i, consumed);
        i += consumed;

        if (op == '*') {
            accCoef  *= fac2.coef;
            accPower += fac2.power;
        } else {
            accCoef  /= fac2.coef;
            accPower -= fac2.power;
        }
    }

    result.coef  = termSign * accCoef;
    result.power = accPower;
    return result;
}

// ===== Парсинг цілого полінома з C-рядка (один поліном) =====

static Polynomial parsePolynomialFromCStr(const char* original) {
    char input[256];

    std::strncpy(input, original, 255);
    input[255] = '\0';

    removeSpaces(input);

    char expr[260];
    if (input[0] != '+' && input[0] != '-') {
        expr[0] = '+';
        std::strcpy(expr + 1, input);
    } else {
        std::strcpy(expr, input);
    }

    Polynomial poly;
    int  i    = 0;
    int  tpos = 0;
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

    term[tpos] = '\0';
    Monom last = parseTerm(term);
    poly.addMonom(last);

    return poly;
}

// ===== 1) зчитати ОДИН поліном з КОНСОЛІ =====

Polynomial inputPolynomial() {
    char input[256];
    cout << "Введіть поліном (напр. 3x^2 - 2x + x*x/2 - 4):\n";
    cin.getline(input, 256);

    return parsePolynomialFromCStr(input);
}

// ===== 2) зчитати ДВА поліноми з ФАЙЛУ, в одному рядку через кому =====

bool readTwoPolynomialsFromFile(const char* fileName,
                                Polynomial& P1,
                                Polynomial& P2)
{
    ifstream fin(fileName);
    if (!fin) {
        return false;
    }

    char line[512];
    if (!fin.getline(line, 512)) {
        return false;
    }

    int commaPos = -1;
    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == ',') {
            commaPos = i;
            break;
        }
    }

    if (commaPos == -1) {
        return false; // немає коми
    }

    char part1[256];
    char part2[256];

    int i, j;

    // перший поліном (до коми)
    for (i = 0; i < commaPos && i < 255; ++i) {
        part1[i] = line[i];
    }
    part1[i] = '\0';

    // другий поліном (після коми)
    j = 0;
    for (i = commaPos + 1; line[i] != '\0' && j < 255; ++i, ++j) {
        part2[j] = line[i];
    }
    part2[j] = '\0';

    P1 = parsePolynomialFromCStr(part1);
    P2 = parsePolynomialFromCStr(part2);

    return true;
}

// ===== 3) зчитати ДВА поліноми з КЛАВІАТУРИ, в одному рядку через кому =====

bool readTwoPolynomialsFromCin(Polynomial& P1,
                               Polynomial& P2)
{
    char line[512];

    cout << "Введіть ДВА поліноми в одному рядку, розділені комою:\n";
    // приклад: 3x^2 - 2x + 1, -x^3 + 5x

    cin.getline(line, 512);

    int commaPos = -1;
    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == ',') {
            commaPos = i;
            break;
        }
    }

    if (commaPos == -1) {
        return false; // немає коми
    }

    char part1[256];
    char part2[256];

    int i, j;

    // перший поліном (до коми)
    for (i = 0; i < commaPos && i < 255; ++i) {
        part1[i] = line[i];
    }
    part1[i] = '\0';

    // другий поліном (після worm)
    j = 0;
    for (i = commaPos + 1; line[i] != '\0' && j < 255; ++i, ++j) {
        part2[j] = line[i];
    }
    part2[j] = '\0';

    P1 = parsePolynomialFromCStr(part1);
    P2 = parsePolynomialFromCStr(part2);

    return true;
}

















//wormyworm
