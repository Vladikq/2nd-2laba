#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Структура узла для хранения данных
struct SetNode {
    int data;
    SetNode* next;
    SetNode(int value) : data(value), next(nullptr) {} // Конструктор узла с значением
};

class Set {
private:
    SetNode* head; // Указатель на начало списка
    int count;  // Количество элементов в множестве
    
    // Вспомогательная функция для поиска узла с определенным значением
    SetNode* findNode(int value) const {
        SetNode* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

public:
    Set() : head(nullptr), count(0) {} 
    
    ~Set() { 
        clear(); 
    }
    
    // Добавление элемента в множество
    bool insert(int value) {
        if (contains(value)) { // Проверяем, есть ли элемент уже в множестве
            return false; // Элемент уже существует
        }
        
        SetNode* newNode = new SetNode(value); // Создаем новый узел
        newNode->next = head; // Новый узел указывает на текущую голову
        head = newNode; 
        count++; 
        return true; 
    }
    
    // Удаление элемента из множества
    bool erase(int value) {
        SetNode* current = head; // Начинаем с головы списка
        SetNode* prev = nullptr; // Указатель на предыдущий узел
        
        while (current != nullptr) { // Проходим по всему списку
            if (current->data == value) { // Если нашли нужный элемент
                if (prev == nullptr) { // Если это первый элемент
                    head = current->next; // Обновляем голову списка
                } else {
                    prev->next = current->next; // Пропускаем удаляемый узел
                }
                delete current; 
                count--; 
                return true; 
            }
            prev = current; // Сохраняем текущий узел как предыдущий
            current = current->next; // Переходим к следующему узлу
        }
        return false; 
    }
    
    // Проверка наличия элемента в множестве
    bool contains(int value) const {
        return findNode(value) != nullptr;
    }
    
    // Получение размера множества
    int size() const {
        return count; 
    }
    
    // Очистка множества
    void clear() {
        SetNode* current = head; // Начинаем с головы списка
        while (current != nullptr) { // Пока есть узлы
            SetNode* temp = current; // Сохраняем текущий узел
            current = current->next; // Переходим к следующему узлу
            delete temp; // Удаляем сохраненный узел
        }
        head = nullptr; // Обнуляем указатель на голову
        count = 0; 
    }
    
    // Получение суммы всех элементов множества
    int sum() const {
        int total = 0;
        SetNode* current = head;
        while (current != nullptr) {
            total += current->data;
            current = current->next;
        }
        return total;
    }
    
    // Получение всех элементов множества 
    void getElements(vector<int>& elements) const {
        elements.clear(); // Очищаем вектор элементов
        SetNode* current = head; // Начинаем с головы списка
        while (current != nullptr) { // Проходим по всему списку
            elements.push_back(current->data); // Добавляем элемент в вектор
            current = current->next; // Переходим к следующему узлу
        }
    }
    
    // Получение строкового представления множества
    string toString() const {
        string result;
        SetNode* current = head;
        while (current != nullptr) {
            result += to_string(current->data) + " ";
            current = current->next;
        }
        return result;
    }
    
    // Вывод всех элементов множества
    void print() const {
        if (count == 0) {
            cout << "Множество пусто" << endl;
            return;
        }
        
        SetNode* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};

// Вспомогательные функции для работы с файлами
string Futext(string& filenm, string& nameStruct) {
    string str, text;
    ifstream fin(filenm);
    
    if (!fin.is_open()) {
        return "";
    }
    
    while (getline(fin, str)) {
        stringstream ss(str);
        string tokens;
        getline(ss, tokens, ' ');
        if (tokens != nameStruct) {
            text += str + "\n";
        }
    }
    fin.close();
    return text;
}

void writefl(string& filenm, string& text) {
    ofstream fout(filenm);
    if (!fout.is_open()) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    fout << text;
    fout.close();
}

// Чтение множества из файла
Set loadSetFromFile(string& filenm, string& nameStruct) {
    Set mySet;
    ifstream inFile(filenm);
    
    if (!inFile.is_open()) {
        return mySet;
    }
    
    string str;
    while (getline(inFile, str)) {
        stringstream ss(str);
        string tokens;
        getline(ss, tokens, ' ');
        if (tokens == nameStruct) {
            while (getline(ss, tokens, ' ')) {
                if (!tokens.empty()) {
                    try {
                        mySet.insert(stoi(tokens));
                    } catch (...) {
                        // Пропускаем некорректные значения
                    }
                }
            }
        }
    }
    inFile.close();
    return mySet;
}

// Функция добавления элементов в множество
void SETADD(string& name, string& filenm, string& value) {
    string textfull = Futext(filenm, name);
    Set mySet = loadSetFromFile(filenm, name);
    
    try {
        int num = stoi(value);
        if (mySet.insert(num)) {
            string str = name + " " + mySet.toString();
            textfull += str;
            writefl(filenm, textfull);
            cout << "Элемент " << num << " добавлен в множество '" << name << "'" << endl;
        } else {
            cout << "Элемент " << num << " уже существует в множестве" << endl;
        }
    } catch (...) {
        cout << "Ошибка: некорректное значение" << endl;
    }
}

// Функция удаления элементов из множества
void SETDEL(string& name, string& filenm, string& value) {
    string textfull = Futext(filenm, name);
    Set mySet = loadSetFromFile(filenm, name);
    
    try {
        int num = stoi(value);
        if (mySet.erase(num)) {
            string str = name + " " + mySet.toString();
            textfull += str;
            writefl(filenm, textfull);
            cout << "Элемент " << num << " удален из множества '" << name << "'" << endl;
        } else {
            cout << "Элемент " << num << " не найден в множестве" << endl;
        }
    } catch (...) {
        cout << "Ошибка: некорректное значение" << endl;
    }
}

// Функция проверки наличия элемента в множестве
void SET_AT(string& name, string& filenm, string& value) {
    Set mySet = loadSetFromFile(filenm, name);
    
    try {
        int num = stoi(value);
        if (mySet.contains(num)) {
            cout << "True" << endl;
        } else {
            cout << "False" << endl;
        }
    } catch (...) {
        cout << "Ошибка: некорректное значение" << endl;
    }
}

// Функция вывода размера множества
void SET_SIZE(string& name, string& filenm) {
    Set mySet = loadSetFromFile(filenm, name);
    cout << mySet.size() << endl;
}

// Функция вывода всех элементов множества
void SET_PRINT(string& name, string& filenm) {
    Set mySet = loadSetFromFile(filenm, name);
    mySet.print();
}

// Функция очистки множества
void SET_CLEAR(string& name, string& filenm) {
    string textfull = Futext(filenm, name);
    writefl(filenm, textfull);
    cout << "Множество '" << name << "' очищено" << endl;
}

// Функция вывода суммы элементов множества
void SET_SUM(string& name, string& filenm) {
    Set mySet = loadSetFromFile(filenm, name);
    cout << mySet.sum() << endl;
}

// Функция создания пустого множества
void SET_CREATE(string& name, string& filenm) {
    string textfull = Futext(filenm, name);
    string str = name;
    textfull += str + "\n";
    writefl(filenm, textfull);
    cout << "Множество '" << name << "' создано" << endl;
}

// Функция объединения двух множеств
void SET_UNION(string& name1, string& name2, string& resultName, string& filenm) {
    string textfull = Futext(filenm, resultName);
    Set set1 = loadSetFromFile(filenm, name1);
    Set set2 = loadSetFromFile(filenm, name2);
    
    // Создаем объединение
    Set resultSet;
    
    // Добавляем элементы из первого множества
    vector<int> elements1;
    set1.getElements(elements1);
    for (int elem : elements1) {
        resultSet.insert(elem);
    }
    
    // Добавляем элементы из второго множества
    vector<int> elements2;
    set2.getElements(elements2);
    for (int elem : elements2) {
        resultSet.insert(elem);
    }
    
    string str = resultName + " " + resultSet.toString();
    textfull += str;
    writefl(filenm, textfull);
    cout << "Объединение множеств '" << name1 << "' и '" << name2 
         << "' сохранено в '" << resultName << "'" << endl;
}

// Функция пересечения двух множеств
void SET_INTERSECT(string& name1, string& name2, string& resultName, string& filenm) {
    string textfull = Futext(filenm, resultName);
    Set set1 = loadSetFromFile(filenm, name1);
    Set set2 = loadSetFromFile(filenm, name2);
    
    // Создаем пересечение
    Set resultSet;
    
    // Находим общие элементы
    vector<int> elements1;
    set1.getElements(elements1);
    for (int elem : elements1) {
        if (set2.contains(elem)) {
            resultSet.insert(elem);
        }
    }
    
    string str = resultName + " " + resultSet.toString();
    textfull += str;
    writefl(filenm, textfull);
    cout << "Пересечение множеств '" << name1 << "' и '" << name2 
         << "' сохранено в '" << resultName << "'" << endl;
}

// Функция разности двух множеств
void SET_DIFFERENCE(string& name1, string& name2, string& resultName, string& filenm) {
    string textfull = Futext(filenm, resultName);
    Set set1 = loadSetFromFile(filenm, name1);
    Set set2 = loadSetFromFile(filenm, name2);
    
    // Создаем разность
    Set resultSet;
    
    // Находим элементы, которые есть в первом, но нет во втором
    vector<int> elements1;
    set1.getElements(elements1);
    for (int elem : elements1) {
        if (!set2.contains(elem)) {
            resultSet.insert(elem);
        }
    }
    
    string str = resultName + " " + resultSet.toString();
    textfull += str;
    writefl(filenm, textfull);
    cout << "Разность множеств '" << name1 << "' и '" << name2 
         << "' сохранена в '" << resultName << "'" << endl;
}

// Обработка команд для множеств
void setMenu(string& command, string& filenm) {
    string name, name2, resultName, value;
    
    if (command.substr(0, 7) == "SETADD ") {
        string cons = command.substr(7);
        stringstream stream(cons);
        stream >> name >> value;
        SETADD(name, filenm, value);
    } 
    else if (command.substr(0, 7) == "SETDEL ") {
        string cons = command.substr(7);
        stringstream stream(cons);
        stream >> name >> value;
        SETDEL(name, filenm, value);
    } 
    else if (command.substr(0, 7) == "SET_AT ") {
        string cons = command.substr(7);
        stringstream stream(cons);
        stream >> name >> value;
        SET_AT(name, filenm, value);
    } 
    else if (command.substr(0, 8) == "SETSIZE ") {
        string cons = command.substr(8);
        stringstream stream(cons);
        stream >> name;
        SET_SIZE(name, filenm);
    } 
    else if (command.substr(0, 8) == "SETPRINT ") {
        string cons = command.substr(8);
        stringstream stream(cons);
        stream >> name;
        SET_PRINT(name, filenm);
    } 
    else if (command.substr(0, 9) == "SETCLEAR ") {
        string cons = command.substr(9);
        stringstream stream(cons);
        stream >> name;
        SET_CLEAR(name, filenm);
    } 
    else if (command.substr(0, 7) == "SETSUM ") {
        string cons = command.substr(7);
        stringstream stream(cons);
        stream >> name;
        SET_SUM(name, filenm);
    } 
    else if (command.substr(0, 9) == "SETCREATE ") {
        string cons = command.substr(9);
        stringstream stream(cons);
        stream >> name;
        SET_CREATE(name, filenm);
    } 
    else if (command.substr(0, 8) == "SETUNION ") {
        string cons = command.substr(8);
        stringstream stream(cons);
        stream >> name >> name2 >> resultName;
        SET_UNION(name, name2, resultName, filenm);
    } 
    else if (command.substr(0, 11) == "SETINTERSECT ") {
        string cons = command.substr(11);
        stringstream stream(cons);
        stream >> name >> name2 >> resultName;
        SET_INTERSECT(name, name2, resultName, filenm);
    } 
    else if (command.substr(0, 12) == "SETDIFFERENCE ") {
        string cons = command.substr(12);
        stringstream stream(cons);
        stream >> name >> name2 >> resultName;
        SET_DIFFERENCE(name, name2, resultName, filenm);
    } 
    else {
        cout << "Ошибка. Неизвестная команда для множества: " << command << endl;
        cout << "Доступные команды: SETADD, SETDEL, SET_AT, SETSIZE, SETPRINT, SETCLEAR, SETSUM, SETCREATE, SETUNION, SETINTERSECT, SETDIFFERENCE" << endl;
    }
}

void printUsage(char* programName) {
    cout << "Использование: " << programName << " --file <filename> --query 'command'" << endl;
    cout << "Пример: " << programName << " --file data.txt --query 'SETADD myset 10'" << endl;
    cout << "Пример: " << programName << " --file data.txt --query 'SETUNION set1 set2 result'" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printUsage(argv[0]);
        return 1;
    }
    
    string filename;
    string query;
    
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--file") {
            if (++i < argc) {
                filename = argv[i];
            } else {
                printUsage(argv[0]);
                return 1;
            }
        } else if (string(argv[i]) == "--query") {
            if (++i < argc) {
                query = argv[i];
            } else {
                printUsage(argv[0]);
                return 1;
            }
        }
    }
    
    if (query.empty()) {
        cout << "Ошибка. Не указана команда." << endl;
        return 1;
    }
    
    // Определяем тип операции по первой букве
    if (query.substr(0, 3) == "SET") {
        setMenu(query, filename);
    } else {
        cout << "Ошибка. Неизвестный тип команды." << endl;
        return 1;
    }
    
    return 0;
}
