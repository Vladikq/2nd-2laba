#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Структура узла для хранения данных
struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {} // Конструктор узла с значением
};

class MySet {
private:
    Node* head; // Указатель на начало списка
    int count;  // Количество элементов в множестве
    
    // Вспомогательная функция для поиска узла с определенным значением
    Node* findNode(int value) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

public:
    MySet() : head(nullptr), count(0) {} 
    
    ~MySet() { 
        clear(); 
    }
    
    // Добавление элемента в множество
    bool insert(int value) {
        if (contains(value)) { // Проверяем, есть ли элемент уже в множестве
            return false; // Элемент уже существует
        }
        
        Node* newNode = new Node(value); // Создаем новый узел
        newNode->next = head; // Новый узел указывает на текущую голову
        head = newNode; 
        count++; 
        return true; 
    }
    
    // Удаление элемента из множества
    bool erase(int value) {
        Node* current = head; // Начинаем с головы списка
        Node* prev = nullptr; // Указатель на предыдущий узел
        
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
        Node* current = head; // Начинаем с головы списка
        while (current != nullptr) { // Пока есть узлы
            Node* temp = current; // Сохраняем текущий узел
            current = current->next; // Переходим к следующему узлу
            delete temp; // Удаляем сохраненный узел
        }
        head = nullptr; // Обнуляем указатель на голову
        count = 0; 
    }
    
    // Получение суммы всех элементов множества
    int sum() const {
        int total = 0;
        Node* current = head;
        while (current != nullptr) {
            total += current->data;
            current = current->next;
        }
        return total;
    }
    
    // Получение всех элементов множества 
    void getElements(vector<int>& elements) const {
        elements.clear(); // Очищаем вектор элементов
        Node* current = head; // Начинаем с головы списка
        while (current != nullptr) { // Проходим по всему списку
            elements.push_back(current->data); // Добавляем элемент в вектор
            current = current->next; // Переходим к следующему узлу
        }
    }
    
    // Проверка на пустоту множества
    bool empty() const {
        return count == 0;
    }
    
    // Вывод всех элементов множества
    void print() const {
        if (empty()) {
            cout << "Множество пусто" << endl;
            return;
        }
        
        Node* current = head;
        cout << "Элементы множества: ";
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
    
    // Копирование множества
    MySet* clone() const {
        MySet* newSet = new MySet();
        vector<int> elements;
        getElements(elements);
        for (int elem : elements) {
            newSet->insert(elem);
        }
        return newSet;
    }
    
    // Операция объединения множеств
    MySet* unionWith(const MySet& other) const {
        MySet* result = clone(); // Копируем текущее множество
        
        vector<int> otherElements;
        other.getElements(otherElements);
        for (int elem : otherElements) {
            result->insert(elem); // insert сам проверит на дубликаты
        }
        
        return result;
    }
    
    // Операция пересечения множеств
    MySet* intersectWith(const MySet& other) const {
        MySet* result = new MySet();
        
        Node* current = head;
        while (current != nullptr) {
            if (other.contains(current->data)) {
                result->insert(current->data);
            }
            current = current->next;
        }
        
        return result;
    }
    
    // Операция разности множеств
    MySet* differenceWith(const MySet& other) const {
        MySet* result = new MySet();
        
        Node* current = head;
        while (current != nullptr) {
            if (!other.contains(current->data)) {
                result->insert(current->data);
            }
            current = current->next;
        }
        
        return result;
    }
    
    // Проверка на подмножество
    bool isSubsetOf(const MySet& other) const {
        Node* current = head;
        while (current != nullptr) {
            if (!other.contains(current->data)) {
                return false;
            }
            current = current->next;
        }
        return true;
    }
    
    // Проверка на равенство множеств
    bool equals(const MySet& other) const {
        if (count != other.count) {
            return false;
        }
        
        Node* current = head;
        while (current != nullptr) {
            if (!other.contains(current->data)) {
                return false;
            }
            current = current->next;
        }
        return true;
    }
    
    // Поиск максимального элемента
    int max() const {
        if (empty()) {
            throw runtime_error("Множество пусто");
        }
        
        int maxVal = head->data;
        Node* current = head->next;
        while (current != nullptr) {
            if (current->data > maxVal) {
                maxVal = current->data;
            }
            current = current->next;
        }
        return maxVal;
    }
    
    // Поиск минимального элемента
    int min() const {
        if (empty()) {
            throw runtime_error("Множество пусто");
        }
        
        int minVal = head->data;
        Node* current = head->next;
        while (current != nullptr) {
            if (current->data < minVal) {
                minVal = current->data;
            }
            current = current->next;
        }
        return minVal;
    }
};

// Функция для сохранения множества в файл
void saveSetToFile(const MySet& mySet, const string& filePath) {
    ofstream outFile(filePath); 
    vector<int> elements; // Вектор для хранения элементов
    mySet.getElements(elements); // Получаем все элементы множества
    
    for (int x : elements) { 
        outFile << x << " "; 
    }
    outFile.close(); 
}

// Функция для загрузки множества из файла
MySet loadSetFromFile(const string& filePath) {
    MySet mySet;
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        return mySet; // Возвращаем пустое множество, если файл не существует
    }
    
    int num;
    while (inFile >> num) {
        mySet.insert(num);
    }
    inFile.close();
    return mySet;
}

// Функция добавления элементов в множество
void SETADD(MySet& mySet, const string& filePath) {
    cout << "Введите числа для добавления через пробел: ";
    string line; 

    // Считываем всю строку чисел сразу
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, line); // Читаем всю строку из cin

    stringstream ss(line); // Создаем поток из строки для разбиения на числа
    int num; // Переменная для хранения числа
    bool changed = false;  // Флаг, чтобы определить, нужно ли сохранять файл

    while (ss >> num) {    // Считываем числа по очереди из потока
        if (mySet.insert(num)) { // Пытаемся добавить число в множество
            cout << num << " добавлено\n"; 
            changed = true; // Устанавливаем флаг изменений
        } else {
            cout << num << " уже есть\n"; 
        }
    }

    if (changed) saveSetToFile(mySet, filePath); // Сохраняем только если были изменения
}

// Функция удаления элементов из множества
void SETDEL(MySet& mySet, const string& filePath) {
    cout << "Введите числа для удаления через пробел: ";
    string line; // Строка для ввода чисел

    // Очистка буфера ввода, чтобы getline считал корректно
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорируем оставшиеся символы в буфере

    getline(cin, line);       // Считываем строку с числами для удаления
    stringstream ss(line);    // Создаем поток из строки
    int num; // Переменная для хранения числа
    bool changed = false;     // Флаг, чтобы определить, нужно ли сохранять файл

    while (ss >> num) { // Считываем числа по очереди
        if (mySet.erase(num)) { // Пытаемся удалить число из множества
            cout << num << " удалено\n"; // Сообщение об успешном удалении
            changed = true; 
        } else {
            cout << num << " не найдено\n"; 
        }
    }

    if (changed) saveSetToFile(mySet, filePath); // Сохраняем только если были изменения
}

// Функция проверки наличия элемента в множестве
void SET_AT(const MySet& mySet) {
    cout << "Введите число для проверки: ";
    int num; // Переменная для хранения числа
    cin >> num;                 // Считываем одно число
    if (mySet.contains(num)) // Проверяем наличие числа в множестве
        cout << num << " присутствует\n"; 
    else
        cout << num << " отсутствует\n"; 
}

// Функция вывода размера множества
void SET_SIZE(const MySet& mySet) {
    cout << "Размер множества: " << mySet.size() << endl;
}

// Функция вывода всех элементов множества
void SET_PRINT(const MySet& mySet) {
    mySet.print();
}

// Функция очистки множества
void SET_CLEAR(MySet& mySet, const string& filePath) {
    mySet.clear();
    saveSetToFile(mySet, filePath);
    cout << "Множество очищено" << endl;
}

// Функция вывода суммы элементов множества
void SET_SUM(const MySet& mySet) {
    cout << "Сумма элементов: " << mySet.sum() << endl;
}

// Функция вывода максимального элемента
void SET_MAX(const MySet& mySet) {
    try {
        cout << "Максимальный элемент: " << mySet.max() << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}

// Функция вывода минимального элемента
void SET_MIN(const MySet& mySet) {
    try {
        cout << "Минимальный элемент: " << mySet.min() << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}

// Функция объединения множеств
void SET_UNION(MySet& mySet, const string& filePath) {
    cout << "Введите числа для второго множества через пробел: ";
    string line;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, line);
    
    MySet otherSet;
    stringstream ss(line);
    int num;
    while (ss >> num) {
        otherSet.insert(num);
    }
    
    MySet* unionSet = mySet.unionWith(otherSet);
    mySet = *unionSet;
    delete unionSet;
    
    saveSetToFile(mySet, filePath);
    cout << "Множества объединены" << endl;
}

// Функция пересечения множеств
void SET_INTERSECT(MySet& mySet, const string& filePath) {
    cout << "Введите числа для второго множества через пробел: ";
    string line;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, line);
    
    MySet otherSet;
    stringstream ss(line);
    int num;
    while (ss >> num) {
        otherSet.insert(num);
    }
    
    MySet* intersectSet = mySet.intersectWith(otherSet);
    mySet = *intersectSet;
    delete intersectSet;
    
    saveSetToFile(mySet, filePath);
    cout << "Найдено пересечение множеств" << endl;
}

// Функция разности множеств
void SET_DIFFERENCE(MySet& mySet, const string& filePath) {
    cout << "Введите числа для второго множества через пробел: ";
    string line;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, line);
    
    MySet otherSet;
    stringstream ss(line);
    int num;
    while (ss >> num) {
        otherSet.insert(num);
    }
    
    MySet* diffSet = mySet.differenceWith(otherSet);
    mySet = *diffSet;
    delete diffSet;
    
    saveSetToFile(mySet, filePath);
    cout << "Выполнена разность множеств" << endl;
}

// Функция проверки на подмножество
void SET_SUBSET(const MySet& mySet) {
    cout << "Введите числа для проверки на подмножество через пробел: ";
    string line;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, line);
    
    MySet otherSet;
    stringstream ss(line);
    int num;
    while (ss >> num) {
        otherSet.insert(num);
    }
    
    if (mySet.isSubsetOf(otherSet)) {
        cout << "Текущее множество является подмножеством введенного" << endl;
    } else {
        cout << "Текущее множество НЕ является подмножеством введенного" << endl;
    }
}

void printHelp() {
    cout << "Доступные команды:" << endl;
    cout << "  SETADD      - добавить элементы в множество" << endl;
    cout << "  SETDEL      - удалить элементы из множества" << endl;
    cout << "  SET_AT      - проверить наличие элемента" << endl;
    cout << "  SET_SIZE    - показать размер множества" << endl;
    cout << "  SET_PRINT   - вывести все элементы" << endl;
    cout << "  SET_CLEAR   - очистить множество" << endl;
    cout << "  SET_SUM     - вывести сумму элементов" << endl;
    cout << "  SET_MAX     - найти максимальный элемент" << endl;
    cout << "  SET_MIN     - найти минимальный элемент" << endl;
    cout << "  SET_UNION   - объединить с другим множеством" << endl;
    cout << "  SET_INTERSECT - найти пересечение с другим множеством" << endl;
    cout << "  SET_DIFFERENCE - найти разность с другим множеством" << endl;
    cout << "  SET_SUBSET  - проверить на подмножество" << endl;
}

int main(int argc, char* argv[]) {
    // Проверка количества аргументов командной строки
    if (argc != 5) { 
        cerr << "Использование: ./program --file <файл> --query <операция>\n"; 
        cerr << "Пример: ./program --file data.txt --query SETADD\n";
        cerr << "Для справки: ./program --file data.txt --query HELP\n";
        return 1; 
    }

    string filePath = argv[2]; 
    string query = argv[4];    

    // Загружаем множество из файла
    MySet mySet = loadSetFromFile(filePath);

    // Вызов соответствующей функции в зависимости от запроса
    if (query == "SETADD") SETADD(mySet, filePath);
    else if (query == "SETDEL") SETDEL(mySet, filePath); 
    else if (query == "SET_AT") SET_AT(mySet); 
    else if (query == "SET_SIZE") SET_SIZE(mySet);
    else if (query == "SET_PRINT") SET_PRINT(mySet);
    else if (query == "SET_CLEAR") SET_CLEAR(mySet, filePath);
    else if (query == "SET_SUM") SET_SUM(mySet);
    else if (query == "SET_MAX") SET_MAX(mySet);
    else if (query == "SET_MIN") SET_MIN(mySet);
    else if (query == "SET_UNION") SET_UNION(mySet, filePath);
    else if (query == "SET_INTERSECT") SET_INTERSECT(mySet, filePath);
    else if (query == "SET_DIFFERENCE") SET_DIFFERENCE(mySet, filePath);
    else if (query == "SET_SUBSET") SET_SUBSET(mySet);
    else if (query == "HELP") printHelp();
    else cerr << "Неизвестная операция. Введите HELP для справки.\n"; 

    return 0; 
}
