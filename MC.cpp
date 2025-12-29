#include <iostream>
#include <vector>

using namespace std;

// Узел связанного списка для хранения пар ключ-значение
struct Node {
    int key;      // Ключ элемента 
    int value;    // Значение элемента  
    Node* next;   // Указатель на следующий узел в списке
    
    // Конструктор узла
    Node(int k, int v) : key(k), value(v), next(nullptr) {}
};

// Структура связанного списка
struct LinkedList {
    Node* head;  // Указатель на первый узел списка
    
    LinkedList() : head(nullptr) {}
    
    ~LinkedList() {
        clear();
    }
    
    // Добавление элемента в конец списка
    void push_back(int key, int value) {
        // Создаем новый узел
        Node* newNode = new Node(key, value);
        
        // Если список пустой, новый узел становится головой
        if (head == nullptr) {
            head = newNode;
        } else {
            // Ищем последний узел в списке
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            // Добавляем новый узел в конец
            current->next = newNode;
        }
    }
    
    // Поиск элемента по ключу
    bool search(int key, int& value) {
        Node* current = head;
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Если нашли нужный ключ
            if (current->key == key) {
                value = current->value;  // Записываем значение
                return true;             // Возвращаем успех
            }
            current = current->next;     // Переходим к следующему узлу
        }
        return false;  // Ключ не найден
    }
    
    // Обновление значения по ключу (если ключ существует)
    bool update(int key, int value) {
        Node* current = head;
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Если нашли нужный ключ
            if (current->key == key) {
                current->value = value;  // Обновляем значение
                return true;             // Возвращаем успех
            }
            current = current->next;     // Переходим к следующему узлу
        }
        return false;  // Ключ не найден
    }
    
    // Удаление элемента по ключу
    bool remove(int key) {
        // Если список пустой
        if (head == nullptr) {
            return false;
        }
        
        // Если удаляемый элемент - голова списка
        if (head->key == key) {
            Node* temp = head;    // Сохраняем указатель на удаляемый узел
            head = head->next;    // Перемещаем голову на следующий узел
            delete temp;          // Освобождаем память
            return true;          // Возвращаем успех
        }
        
        // Поиск элемента в середине или конце списка
        Node* current = head;
        while (current->next != nullptr) {
            // Если следующий узел - тот, который нужно удалить
            if (current->next->key == key) {
                Node* temp = current->next;           // Сохраняем указатель на удаляемый узел
                current->next = current->next->next;  // Пропускаем удаляемый узел
                delete temp;                          // Освобождаем память
                return true;                          // Возвращаем успех
            }
            current = current->next;  // Переходим к следующему узлу
        }
        
        return false;  // Ключ не найден
    }
    
    // Проверка на пустоту
    bool empty() const {
        return head == nullptr;  // true если список пустой
    }
    
    // Очистка списка
    void clear() {
        // Удаляем все узлы по одному
        while (head != nullptr) {
            Node* temp = head;    // Сохраняем указатель на текущий узел
            head = head->next;    // Перемещаем голову на следующий узел
            delete temp;          // Освобождаем память
        }
    }
};

class ChainingHashTable {
private:
    vector<LinkedList> table;  // Вектор корзин (связных списков)
    int capacity;              // Общий размер таблицы (количество корзин)
    int size;                  // Текущее количество элементов в таблице

    // Хэш-функция - вычисляет индекс корзины для ключа
    int hash(int key) {
        return key % capacity;  // Простая модульная хэш-функция
    }

public:
    // Конструктор хэш-таблицы
    ChainingHashTable(int cap = 10) : capacity(cap), size(0) {
        table.resize(capacity); // Инициализируем вектор заданного размера пустыми списками
    }

    // Вставка элемента в хэш-таблицу
    void insert(int key, int value) {
        int index = hash(key); // Вычисляем индекс корзины для данного ключа
        
        // Проверяем, есть ли уже такой ключ в корзине
        if (table[index].update(key, value)) {
            return; // Если ключ найден и значение обновлено, выходим
        }
        
        // Добавляем новую пару ключ-значение в конец списка корзины
        table[index].push_back(key, value);
        size++; // Увеличиваем счетчик элементов
    }

    // Поиск элемента в хэш-таблице
    bool search(int key, int& value) {
        int index = hash(key); // Вычисляем индекс корзины для данного ключа
        return table[index].search(key, value); // Ищем ключ в списке корзины
    }

    // Удаление элемента из хэш-таблицы
    bool remove(int key) {
        int index = hash(key); // Вычисляем индекс корзины для данного ключа
        
        if (table[index].remove(key)) {
            size--; // Уменьшаем счетчик элементов
            return true; // Возвращаем true - удаление успешно
        }
        return false; // Возвращаем false - элемент не найден
    }

    // Вывод содержимого хэш-таблицы
    void print() {
        // Проходим по всем корзинам таблицы
        for (int i = 0; i < capacity; i++) {
            cout << "Корзина " << i << ": "; // Выводим номер корзины
            if (table[i].empty()) {
                cout << "[ПУСТО]"; // Если корзина пустая
            } else {
                // Проходим по всем элементам в корзине
                Node* current = table[i].head;
                while (current != nullptr) {
                    cout << "[" << current->key << ":" << current->value << "] ";
                    current = current->next;
                }
            }
            cout << endl; // Переход на новую строку после каждой корзины
        }
        cout << "Всего элементов: " << size << endl; // Выводим общее количество элементов
    }
};
int main() {
    int capacity;
    cout << "Введите размер хэш-таблицы (количество корзин): ";
    cin >> capacity;
    
    ChainingHashTable ht(capacity);
    
    while (true) {
        cout << "1. Добавить элемент" << endl;
        cout << "2. Найти элемент" << endl;
        cout << "3. Удалить элемент" << endl;
        cout << "4. Показать таблицу" << endl;
        cout << "5. Выход" << endl;
        cout << "Выберите действие: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                int key, value;
                cout << "Введите ключ: ";
                cin >> key;
                cout << "Введите значение: ";
                cin >> value;
                ht.insert(key, value);
                cout << "Элемент добавлен!" << endl;
                break;
            }
            case 2: {
                int key, value;
                cout << "Введите ключ для поиска: ";
                cin >> key;
                if (ht.search(key, value)) {
                    cout << "Найден элемент: ключ=" << key << ", значение=" << value << endl;
                } else {
                    cout << "Элемент с ключом " << key << " не найден!" << endl;
                }
                break;
            }
            case 3: {
                int key;
                cout << "Введите ключ для удаления: ";
                cin >> key;
                if (ht.remove(key)) {
                    cout << "Элемент удален!" << endl;
                } else {
                    cout << "Элемент с ключом " << key << " не найден!" << endl;
                }
                break;
            }
            case 4: {
                ht.print();
                break;
            }
            case 5: {
                cout << "Выход из программы..." << endl;
                return 0;
            }
            default: {
                cout << "Неверный выбор! Попробуйте снова." << endl;
                break;
            }
        }
    }
    
    return 0;
}