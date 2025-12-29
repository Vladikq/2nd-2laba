#include <iostream>
#include <vector>
#include <limits> 

using namespace std;

class OpenAddressingHashTable {
private:
    vector<pair<int, int>> table;  // Вектор для хранения пар ключ-значение
    vector<bool> occupied;         // Вектор флагов занятости ячеек
    int capacity;                  // Общий размер таблицы
    int size;                      // Текущее количество элементов

    // Хэш-функция с линейным пробированием
    int hash(int key, int attempt) {
        return (key + attempt) % capacity;
    }

public:
    // Конструктор с указанием размера таблицы
    OpenAddressingHashTable(int cap = 10) : capacity(cap), size(0) {
        table.resize(capacity);
        occupied.resize(capacity, false);
    }

    // ------------------ Вставка ------------------
    void insert(int key, int value) {
        for (int attempt = 0; attempt < capacity; attempt++) {
            int index = hash(key, attempt); // Вычисляем индекс с учетом попытки

            if (occupied[index] && table[index].first == key) { 
                // Если ключ уже существует — обновляем значение
                table[index].second = value;
                cout << "Элемент обновлен в ячейке " << index << endl;
                return;
            }

            if (!occupied[index]) { 
                // Если нашли свободное место — вставляем новый элемент
                table[index] = {key, value};
                occupied[index] = true;
                size++;
                cout << "Элемент помещен в ячейку " << index << " после " << attempt << " попыток" << endl;
                return;
            }
        }

        // Если все ячейки проверены и не нашлось свободной — таблица заполнена
        cout << "ОШИБКА: Таблица заполнена!" << endl;
    }

    // ------------------ Поиск ------------------
    bool search(int key, int& value) {
        for (int attempt = 0; attempt < capacity; attempt++) {
            int index = hash(key, attempt);

            if (occupied[index] && table[index].first == key) {
                // Найден нужный ключ
                value = table[index].second;
                cout << "Элемент найден в ячейке " << index << " после " << attempt << " попыток" << endl;
                return true;
            }

            if (!occupied[index]) {
                // Если наткнулись на пустую ячейку — элемента нет
                break;
            }
        }
        cout << "Элемент не найден!" << endl;
        return false;
    }

    // ------------------ Удаление ------------------
    bool remove(int key) {
        for (int attempt = 0; attempt < capacity; attempt++) {
            int index = hash(key, attempt);

            if (occupied[index] && table[index].first == key) {
                // Если нашли элемент — удаляем
                occupied[index] = false;
                size--;
                cout << "Элемент удален из ячейки " << index << endl;
                return true;
            }

            if (!occupied[index]) {
                break;
            }
        }
        cout << "Элемент для удаления не найден!" << endl;
        return false;
    }

    // ------------------ Печать таблицы ------------------
    void print() {
        for (int i = 0; i < capacity; i++) {
            cout << "Ячейка " << i << ": ";
            if (occupied[i]) {
                cout << "[" << table[i].first << ":" << table[i].second << "]";
            } else {
                cout << "[ПУСТО]";
            }
            cout << endl;
        }
        cout << "Всего элементов: " << size << "/" << capacity << endl;
    }
};


int main() {
    int capacity;
    cout << "Введите размер хэш-таблицы: ";
    cin >> capacity;

    OpenAddressingHashTable ht(capacity);

    while (true) {
        cout << "\n===== МЕНЮ =====" << endl;
        cout << "1. Добавить элемент" << endl;
        cout << "2. Найти элемент" << endl;
        cout << "3. Удалить элемент" << endl;
        cout << "4. Показать таблицу" << endl;
        cout << "5. Выход" << endl;
        cout << "Выберите действие: ";

        int choice;
        cin >> choice;

        // если ошибка ввода — очищаем поток
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка ввода! Попробуйте снова." << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                int key, value;
                cout << "Введите ключ: ";
                cin >> key;
                cout << "Введите значение: ";
                cin >> value;
                ht.insert(key, value);
                break;
            }
            case 2: {
                int key, value;
                cout << "Введите ключ для поиска: ";
                cin >> key;
                if (ht.search(key, value)) {
                    cout << "Найден элемент: ключ=" << key << ", значение=" << value << endl;
                }
                break;
            }
            case 3: {
                int key;
                cout << "Введите ключ для удаления: ";
                cin >> key;
                ht.remove(key);
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
