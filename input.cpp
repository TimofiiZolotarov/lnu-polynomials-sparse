#include "input.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
using namespace std;

// Розміри внутрішніх буферів
const int MAX_INPUT_SIZE = 256;  
const int MAX_LINE_SIZE = 512;   
const int MAX_TERM_SIZE = 100;   
const int MAX_NUMBER_SIZE = 64;  


// Створює порожній список
Polynomial::Polynomial() {
    head = nullptr;
}

// Додає моном у список, пропускаючи нульовий коефіцієнт
void Polynomial::addMonom(Monom m) {
    if (m.coef == 0.0) return;
    Node* newNode = new Node{m, nullptr};

    // Якщо список порожній — додаємо перший елемент
    if (!head) {
        head = newNode;
        return;
    }

    // Вставка на початок, якщо степінь більший за наявний
    if (m.power > head->data.power) {
        newNode->next = head;
        head = newNode;
        return;
    }

    // Якщо степені збігаються — додаємо коефіцієнти
    if (m.power == head->data.power) {
        head->data.coef += m.coef;
        delete newNode;

        // Видаляємо моном, якщо коефіцієнт став нульовим
        if (head->data.coef == 0.0) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        return;
    }

    // Пошук місця для вставки, підтримуючи спадний порядок степенів
    Node* cur = head;
    while (cur->next && cur->next->data.power >= m.power) {
        // Якщо знайшли моном з тим самим степенем
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

    // Вставка нового монома
    newNode->next = cur->next;
    cur->next = newNode;
}

// Виводить поліном у звичайному вигляді
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

        // Виведення знака
        if (!first) {
            if (c >= 0) cout << " + ";
            else { cout << " - "; c = -c; }
        } else {
            if (c < 0) { cout << "-"; c = -c; }
            first = false;
        }

        // Форматування залежно від степеня
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

// Очищає список і звільняє памʼять
void Polynomial::clear() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    head = nullptr;
}


// Видаляє пробіли з рядка
void removeSpaces(char* s) {
    int j = 0;
    for (int i = 0; s[i] != '\0'; ++i) {
        if (!isspace((unsigned char)s[i]))
            s[j++] = s[i];
    }
    s[j] = '\0';
}

// Зчитує число з початку терма
double readNumber(const char* term, int& len) {
    int i = 0;
    char buf[MAX_NUMBER_SIZE];
    int  bpos = 0;
    bool hasDigit = false;
    bool hasDot   = false;

    // Можливий знак числа
    if (term[i] == '+' || term[i] == '-') {
        buf[bpos++] = term[i];
        i++;
    }

    // Зчитування цифр і крапки
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

// Розбирає один множник (коефіцієнт і степінь)
Monom parseFactor(const char* s, int& consumed) {
    Monom m;
    m.coef  = 1.0;
    m.power = 0;

    int i      = 0;
    int lenNum = 0;

    // Читаємо коефіцієнт, якщо він є
    if (isdigit((unsigned char)s[i]) || s[i] == '.' ||
        s[i] == '+' || s[i] == '-') {
        double num = readNumber(s + i, lenNum);
        if (lenNum > 0) {
            m.coef = num;
            i += lenNum;
        }
    }

    // Читаємо x і його степінь
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

// Розбирає один терм із можливими множеннями та діленням
Monom parseTerm(const char* term) {
    Monom result;
    result.coef  = 0;
    result.power = 0;

    int i = 0;

    // Читаємо знак
    int termSign = 1;
    if (term[i] == '+') i++;
    else if (term[i] == '-') { termSign = -1; i++; }

    // Перший множник
    int   consumed = 0;
    Monom fac      = parseFactor(term + i, consumed);
    double accCoef  = fac.coef;
    int    accPower = fac.power;
    i += consumed;

    // Подальші множення або ділення
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


// Розбирає текстовий запис одного полінома
static Polynomial parsePolynomialFromCStr(const char* original) {
    char input[MAX_INPUT_SIZE];

    // Копіюємо рядок у локальний буфер
    std::strncpy(input, original, MAX_INPUT_SIZE - 1);
    input[MAX_INPUT_SIZE - 1] = '\0';

    // Прибираємо пробіли
    removeSpaces(input);

    // Додаємо знак на початку, якщо його немає
    char expr[MAX_INPUT_SIZE + 4];
    if (input[0] != '+' && input[0] != '-') {
        expr[0] = '+';
        std::strcpy(expr + 1, input);
    } else {
        std::strcpy(expr, input);
    }

    Polynomial poly;
    int  i    = 0;
    int  tpos = 0;
    char term[MAX_TERM_SIZE];

    // Розбиваємо вираз на терми
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

    // Обробка останнього терма
    term[tpos] = '\0';
    Monom last = parseTerm(term);
    poly.addMonom(last);

    return poly;
}


// Ділить рядок на два вирази за комою
static bool splitByComma(const char* line, char* part1, char* part2, int maxSize) {
    int commaPos = -1;
    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == ',') {
            commaPos = i;
            break;
        }
    }

    if (commaPos == -1)
        return false;

    // Копіюємо перший вираз
    int i;
    for (i = 0; i < commaPos && i < maxSize - 1; ++i) {
        part1[i] = line[i];
    }
    part1[i] = '\0';

    // Копіюємо другий вираз
    int j = 0;
    for (i = commaPos + 1; line[i] != '\0' && j < maxSize - 1; ++i, ++j) {
        part2[j] = line[i];
    }
    part2[j] = '\0';

    return true;
}


// Зчитує один поліном із консолі
Polynomial inputPolynomial() {
    char input[MAX_INPUT_SIZE];
    cout << "Введіть поліном (наприклад: 3x^2 - 2x + 1):\n";
    cin.getline(input, MAX_INPUT_SIZE);

    return parsePolynomialFromCStr(input);
}


// Зчитує два поліноми з файлу, записані в одному рядку через кому
bool readTwoPolynomialsFromFile(const char* fileName,
                                Polynomial& P1,
                                Polynomial& P2)
{
    ifstream fin(fileName);
    if (!fin)
        return false;

    char line[MAX_LINE_SIZE];
    if (!fin.getline(line, MAX_LINE_SIZE))
        return false;

    char part1[MAX_INPUT_SIZE];
    char part2[MAX_INPUT_SIZE];

    if (!splitByComma(line, part1, part2, MAX_INPUT_SIZE))
        return false;

    P1 = parsePolynomialFromCStr(part1);
    P2 = parsePolynomialFromCStr(part2);

    return true;
}


// Зчитує два поліноми з консолі, введені в одному рядку через кому
bool readTwoPolynomialsFromCin(Polynomial& P1,
                               Polynomial& P2)
{
    char line[MAX_LINE_SIZE];

    cout << "Введіть два поліноми в одному рядку, розділені комою:\n";

    cin.getline(line, MAX_LINE_SIZE);

    char part1[MAX_INPUT_SIZE];
    char part2[MAX_INPUT_SIZE];

    if (!splitByComma(line, part1, part2, MAX_INPUT_SIZE))
        return false;

    P1 = parsePolynomialFromCStr(part1);
    P2 = parsePolynomialFromCStr(part2);

    return true;
}
