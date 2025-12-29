#include <iostream>      
#include <string>        
#include <stdexcept>     
using namespace std;     


class Stack {
private:
    string* data;        // Массив для хранения элементов стека
    size_t capacity;     // Максимальный размер стека
    int head;            // Индекс верхнего элемента стека (-1, если стек пуст)

public:
    Stack(size_t size) : capacity(size), head(-1) { // Конструктор с заданным размером
        data = new string[capacity];               // Выделяем память под элементы
    }

    Stack() : capacity(30), head(-1) {             // Конструктор по умолчанию (размер 30)
        data = new string[capacity];               // Выделяем память
    }

    ~Stack() { delete[] data; }                    

    void push(string value) {                      
        if (head == (int)capacity - 1) {          // Проверка переполнения
            throw overflow_error("Stack is full"); 
        }
        data[++head] = value;                      // Записываем элемент и увеличиваем head
    }

    string pop() {                                 // Удаление и возврат верхнего элемента
        if (head == -1) {                          // Проверка пустоты
            throw underflow_error("Stack is empty"); // Исключение при пустом стеке
        }
        return data[head--];                       // Возврат верхнего элемента и уменьшение head
    }

    string peek() {                                // Просмотр верхнего элемента без удаления
        if (head == -1) {                          // Проверка пустоты
            throw underflow_error("Stack is empty"); // Исключение
        }
        return data[head];                         // Возврат верхнего элемента
    }

    bool isEmpty() { return head == -1; }         

    size_t size() { return head + 1; }            // Количество элементов в стеке
};


// Функция для определения приоритета операторов
int priority(char op) {
    switch (op) {
        case '!': return 3;          // NOT — наивысший приоритет
        case '&': return 2;          // AND
        case '|':
        case '^': return 1;          // OR и XOR
        default: return 0;           // Для всех остальных символов
    }
}


int applyOp(char op, int a, int b = 0) {    // Применяет оператор к значениям
    switch (op) {
        case '!': return !a;         // Логическое NOT (унарный)
        case '&': return a & b;      // Логическое AND
        case '|': return a | b;      // Логическое OR
        case '^': return a ^ b;      // Логическое XOR
        default: throw invalid_argument("Неизвестный оператор"); // Исключение для неизвестного оператора
    }
}

// Основная функция вычисления выражения
int evaluate(string expr) {
    Stack stackValues;  
    Stack stackOps;     

    for (size_t i = 0; i < expr.length(); i++) { // Проходим по каждому символу
        char c = expr[i];
        if (c == ' ') continue;                  // Пропускаем пробелы

        if (c == '0' || c == '1') {             
            stackValues.push(string(1, c));     // Добавляем в стек значений
        }
        else if (c == '(') {                     
            stackOps.push(string(1, c));        // Добавляем в стек операторов
        }
        else if (c == ')') {                     
            while (!stackOps.isEmpty() && stackOps.peek() != "(") { // Обработка до '('
                char op = stackOps.pop()[0];     // Берём оператор из стека
                if (op == '!') {                 // Унарный оператор
                    int val = stoi(stackValues.pop()); // Берём одно значение
                    stackValues.push(to_string(applyOp(op, val))); // Применяем оператор
                } else {                         // Бинарный оператор
                    int b = stoi(stackValues.pop());
                    int a = stoi(stackValues.pop());
                    stackValues.push(to_string(applyOp(op, a, b)));
                }
            }
            if (!stackOps.isEmpty()) stackOps.pop(); // Удаляем '('
        }
        else if (c == '!' || c == '&' || c == '|' || c == '^') { // Если символ — оператор
            while (!stackOps.isEmpty() && priority(stackOps.peek()[0]) >= priority(c)) { // Пока приоритет в стеке >= текущего
                char op = stackOps.pop()[0];
                if (op == '!') {                  // Унарный
                    int val = stoi(stackValues.pop());
                    stackValues.push(to_string(applyOp(op, val)));
                } else {                           // Бинарный
                    int b = stoi(stackValues.pop());
                    int a = stoi(stackValues.pop());
                    stackValues.push(to_string(applyOp(op, a, b)));
                }
            }
            stackOps.push(string(1, c));          // Добавляем текущий оператор в стек
        }
    }

    while (!stackOps.isEmpty()) {               // Обработка оставшихся операторов
        char op = stackOps.pop()[0];
        if (op == '!') {
            int val = stoi(stackValues.pop());
            stackValues.push(to_string(applyOp(op, val)));
        } else {
            int b = stoi(stackValues.pop());
            int a = stoi(stackValues.pop());
            stackValues.push(to_string(applyOp(op, a, b)));
        }
    }

    return stoi(stackValues.pop());             // Возврат результата
}


int main() {
    setlocale(LC_ALL, "Russian");              
    string expr;                               
    cout << "Введите логическое выражение: ";
    getline(cin, expr);                         

    try {
        int result = evaluate(expr);            
        cout << "Результат: " << result << endl; 
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;  
    }

    return 0;
}
