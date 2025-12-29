#include <iostream>
#include <vector>
#include <string>

using namespace std;


class OpenAddressingHashTable {
private:
    vector<pair<char, int>> table;  // Вектор для хранения пар (символ, позиция в строке)
    vector<bool> occupied;         // Флаги занятости ячеек таблицы
    int capacity;                  // Общий размер таблицы
    int size;                      // Текущее количество элементов в таблице

    // Хеш-функция с линейным пробированием
    int hash(char key, int attempt) {
        return (static_cast<int>(key) + attempt) % capacity; // Преобразуем символ в число и применяем пробирование
    }

public:
    // Конструктор с параметром по умолчанию
    OpenAddressingHashTable(int cap = 256) : capacity(cap), size(0) {
        table.resize(capacity);           // Инициализируем вектор пар заданного размера
        occupied.resize(capacity, false); // Инициализируем все ячейки как пустые
    }

    // Метод для вставки пары (символ, позиция) в таблицу
    void insert(char key, int value) {
        // Проверяем, не заполнена ли таблица
        if (size >= capacity) {
            return; // Если таблица заполнена, выходим
        }

        // Пробуем найти свободную ячейку
        for (int attempt = 0; attempt < capacity; attempt++) {
            int index = hash(key, attempt); // Вычисляем индекс с учетом номера попытки
            
            // Если ячейка свободна
            if (!occupied[index]) {
                table[index] = {key, value}; // Записываем пару ключ-значение
                occupied[index] = true;      // Помечаем ячейку как занятую
                size++;                      // Увеличиваем счетчик элементов
                return;                      // Выходим из функции
            }
            
            // Если ключ уже существует в таблице
            if (occupied[index] && table[index].first == key) {
                table[index].second = value; // Обновляем значение (позицию символа)
                return;                      // Выходим из функции
            }
        }
    }

    // Метод для поиска символа в таблице
    bool search(char key, int& value) {
        // Пробуем найти символ в таблице
        for (int attempt = 0; attempt < capacity; attempt++) {
            int index = hash(key, attempt); // Вычисляем индекс с учетом номера попытки
            
            // Если нашли нужный символ
            if (occupied[index] && table[index].first == key) {
                value = table[index].second; // Записываем найденную позицию
                return true;                 
            }
            
            // Если наткнулись на пустую ячейку
            if (!occupied[index]) {
                break; // Прерываем поиск (элемента нет)
            }
        }
        return false;
    }

    // Метод для удаления символа из таблицы
    bool remove(char key) {
        // Пробуем найти символ для удаления
        for (int attempt = 0; attempt < capacity; attempt++) {
            int index = hash(key, attempt); // Вычисляем индекс с учетом номера попытки
            
            // Если нашли нужный символ
            if (occupied[index] && table[index].first == key) {
                occupied[index] = false; // Помечаем ячейку как свободную
                size--;                  // Уменьшаем счетчик элементов
                return true;             // Возвращаем true - удаление успешно
            }
            
            // Если наткнулись на пустую ячейку
            if (!occupied[index]) {
                break; // Прерываем поиск
            }
        }
        return false; 
    }

    // Метод для очистки таблицы
    void clear() {
        // Проходим по всем ячейкам таблицы
        for (int i = 0; i < capacity; i++) {
            occupied[i] = false; // Помечаем все ячейки как пустые
        }
        size = 0; // Сбрасываем счетчик элементов
    }
};

// Функция для нахождения максимальной длины подстроки с уникальными символами
int lengthOfLongestSubstring(string s) {
    int n = s.length(); // Получаем длину входной строки
    if (n == 0) return 0; // Если строка пустая, возвращаем 0
    
    OpenAddressingHashTable charMap(256); // Создаем хеш-таблицу для символов ASCII
    int maxLength = 0; // Переменная для хранения максимальной длины
    int left = 0;      // Левая граница текущей подстроки
    
    // Проходим по строке правой границей
    for (int right = 0; right < n; right++) {
        char currentChar = s[right]; // Текущий символ
        int lastPosition;            // Переменная для хранения последней позиции символа
        
        // Проверяем, встречался ли символ уже в текущей подстроке
        if (charMap.search(currentChar, lastPosition) && lastPosition >= left) {
            // Если символ повторяется и находится в текущей подстроке
            left = lastPosition + 1; // Сдвигаем левую границу за повторяющийся символ
        }
        
        // Обновляем позицию текущего символа в таблице
        charMap.insert(currentChar, right);
        
        // Вычисляем длину текущей подстроки и обновляем максимум
        maxLength = max(maxLength, right - left + 1);
    }
    
    return maxLength; // Возвращаем найденную максимальную длину
}


int main() {
    string input; 
    cout << "Введите строку: "; 
    getline(cin, input);       
    
    
    int result = lengthOfLongestSubstring(input);
    
   
    cout << "Максимальная длина подстроки с уникальными символами: " << result << endl;
    
    return 0; 
}
