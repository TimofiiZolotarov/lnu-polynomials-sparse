#include "input.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
using namespace std;

// Константи для розмірів буферів
const int MAX_INPUT_SIZE = 256;   // Максимальний розмір одного полінома
const int MAX_LINE_SIZE = 512;    // Максимальний розмір рядка для двох поліномів
const int MAX_TERM_SIZE = 100;    // Максимальний розмір одного терма (члена)
const int MAX_NUMBER_SIZE = 64;   // Максимальний розмір числа

Polynomial::Polynomial() {
    head = nullptr;
}

void Polynomial::addMonom(Monom m) {
    if (m.coef == 0.0) return;
    Node* newNode = new Node{m, nullptr};

    // Якщо список порожній, новий моном стає головою
    if (!head) {
        head = newNode;
        return;
    }

    // Якщо степінь нового монома більша за голову - вставляємо на початок
    if (m.power > head->data.power) {
        newNode->next = head;
        head = newNode;
        return;
    }

    // Якщо степінь співпадає з головою - додаємо коефіцієнти
    if (m.power == head->data.power) {
        head->data.coef += m.coef;
        delete newNode;
        // Якщо після додавання коефіцієнт став 0 - видаляємо моном
        if (head->data.coef == 0.0) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        return;
    }

    // Шукаємо відповідне місце для вставки (підтримуємо впорядкування)
    Node* cur = head;
    while (cur->next && cur->next->data.power >= m.power) {
        // Якщо знайшли моном з таким самим степенем
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

    // Вставляємо новий моном у відповідне місце
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

        // Виводимо знак (для не першого члена)
        if (!first) {
            if (c >= 0) cout << " + ";
            else { cout << " - "; c = -c; }
        } else {
            if (c < 0) { cout << "-"; c = -c; }
            first = false;
        }

        // Вивід члена в залежності від степеня
        if (p == 0)  // Константа
            cout << c;
        else if (p == 1)  // Лінійний член
            (c == 1) ? cout << "x" : cout << c << "x";
        else  // Степеневий член
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

//  Допоміжні функції парсера


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
    char buf[MAX_NUMBER_SIZE];
    int  bpos = 0;
    bool hasDigit = false;
    bool hasDot   = false;

    // Обробляємо знак числа
    if (term[i] == '+' || term[i] == '-') {
        buf[bpos++] = term[i];
        i++;
    }

    // Зчитуємо цифри та десяткову крапку
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

    // Спробуємо зчитати коефіцієнт
    if (isdigit((unsigned char)s[i]) || s[i] == '.' ||
        s[i] == '+' || s[i] == '-') {
        double num = readNumber(s + i, lenNum);
        if (lenNum > 0) {
            m.coef = num;
            i += lenNum;
        }
    }

    // Обробляємо змінну x та її степінь
    if (s[i] == 'x') {
        i++;
        if (s[i] == '^') {  // Читаємо показник степеня
            i++;
            int power = 0;
            while (isdigit((unsigned char)s[i])) {
                power = power * 10 + (s[i] - '0');
                i++;
            }
            m.power = power;
        } else {  // x без степеня означає x^1
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

    // Визначаємо знак терма
    int termSign = 1;
    if (term[i] == '+') {
        i++;
    } else if (term[i] == '-') {
        termSign = -1;
        i++;
    }

    // Парсимо перший фактор
    int   consumed = 0;
    Monom fac      = parseFactor(term + i, consumed);
    double accCoef  = fac.coef;
    int    accPower = fac.power;
    i += consumed;

    // Обробляємо операції множення та ділення
    while (term[i] == '*' || term[i] == '/') {
        char op = term[i];
        i++;
        Monom fac2 = parseFactor(term + i, consumed);
        i += consumed;

        if (op == '*') {
            accCoef  *= fac2.coef;
            accPower += fac2.power;  // При множенні степені додаються
        } else {
            accCoef  /= fac2.coef;
            accPower -= fac2.power;  // При діленні степені віднімаються
        }
    }

    result.coef  = termSign * accCoef;
    result.power = accPower;
    return result;
}

// ===== Парсинг цілого полінома з C-рядка (один поліном) =====

static Polynomial parsePolynomialFromCStr(const char* original) {
    char input[MAX_INPUT_SIZE];

    // Копіюємо вхідний рядок у буфер з обмеженням розміру
    std::strncpy(input, original, MAX_INPUT_SIZE - 1);
    input[MAX_INPUT_SIZE - 1] = '\0';

    // Видаляємо всі пробіли
    removeSpaces(input);

    // Нормалізуємо вираз: додаємо '+' на початку, якщо немає знака
    char expr[MAX_INPUT_SIZE + 4]; // +4 для можливості додати '+' на початку
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

    // Розбиваємо вираз на терми за знаками + та -
    while (expr[i] != '\0') {
        // Знайшли початок нового терма
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

    // Обробляємо останній терм
    term[tpos] = '\0';
    Monom last = parseTerm(term);
    poly.addMonom(last);

    return poly;
}

static bool splitByComma(const char* line, char* part1, char* part2, int maxSize) {
    // Знаходимо позицію коми
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

    // Копіюємо перший поліном (до коми)
    int i;
    for (i = 0; i < commaPos && i < maxSize - 1; ++i) {
        part1[i] = line[i];
    }
    part1[i] = '\0';

    // Копіюємо другий поліном (після коми)
    int j = 0;
    for (i = commaPos + 1; line[i] != '\0' && j < maxSize - 1; ++i, ++j) {
        part2[j] = line[i];
    }
    part2[j] = '\0';

    return true;
}

// ===== 1) зчитати ОДИН поліном з КОНСОЛІ =====

Polynomial inputPolynomial() {
    char input[MAX_INPUT_SIZE];
    cout << "Введіть поліном (напр. 3x^2 - 2x + x*x/2 - 4):\n";
    cin.getline(input, MAX_INPUT_SIZE);

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

    char line[MAX_LINE_SIZE];
    if (!fin.getline(line, MAX_LINE_SIZE)) {
        return false;
    }

    char part1[MAX_INPUT_SIZE];
    char part2[MAX_INPUT_SIZE];

    if (!splitByComma(line, part1, part2, MAX_INPUT_SIZE)) {
        return false;
    }

    P1 = parsePolynomialFromCStr(part1);
    P2 = parsePolynomialFromCStr(part2);

    return true;
}

// ===== 3) зчитати ДВА поліноми з КЛАВІАТУРИ, в одному рядку через кому =====

bool readTwoPolynomialsFromCin(Polynomial& P1,
                               Polynomial& P2)
{
    char line[MAX_LINE_SIZE];

    cout << "Введіть ДВА поліноми в одному рядку, розділені комою:\n";
    // приклад: 3x^2 - 2x + 1, -x^3 + 5x

    cin.getline(line, MAX_LINE_SIZE);

    char part1[MAX_INPUT_SIZE];
    char part2[MAX_INPUT_SIZE];

    if (!splitByComma(line, part1, part2, MAX_INPUT_SIZE)) {
        return false;
    }

    P1 = parsePolynomialFromCStr(part1);
    P2 = parsePolynomialFromCStr(part2);

    return true;
}
