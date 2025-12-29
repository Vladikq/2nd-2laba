#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

// Структура множества
struct Set {
private:
    // Структура узла для хранения данных
    struct Node {
        int data;
        Node* next;
        Node(int value) : data(value), next(nullptr) {}
    };
    
    Node* head;
    int count;

public:
    Set() : head(nullptr), count(0) {}
    
    ~Set() {
        clear();
    }
    
    // Оператор присваивания
    Set& operator=(const Set& other) {
        // Проверяем, что это не самоприсваивание
        if (this != &other) {
            // Очищаем текущее множество
            clear();
            // Создаем указатель для обхода исходного множества
            Node* current = other.head;
            // Проходим по всем элементам исходного множества
            while (current != nullptr) {
                // Вставляем каждый элемент в текущее множество
                insert(current->data);
                // Переходим к следующему элементу
                current = current->next;
            }
        }
        return *this;
    }
    
    // Добавление элемента в множество
    bool insert(int value) {
        // Проверяем, существует ли элемент уже в множестве
        if (contains(value)) {
            return false;
        }
        
        // Создаем новый узел с заданным значением
        Node* newNode = new Node(value);
        // Устанавливаем указатель нового узла на текущую голову
        newNode->next = head;
        // Обновляем голову списка на новый узел
        head = newNode;
        count++;
        return true;
    }
    
    // Удаление элемента из множества
    bool erase(int value) {
        // Создаем указатель для текущего узла, начинаем с головы
        Node* current = head;
        // Создаем указатель для предыдущего узла
        Node* prev = nullptr;
        
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Проверяем, совпадает ли значение текущего узла с искомым
            if (current->data == value) {
                // Если удаляем первый элемент
                if (prev == nullptr) {
                    // Обновляем голову списка на следующий элемент
                    head = current->next;
                } else {
                    // Иначе связываем предыдущий узел со следующим
                    prev->next = current->next;
                }
                // Освобождаем память удаляемого узла
                delete current;
                // Уменьшаем счетчик элементов
                count--;
                // Возвращаем true - удаление успешно
                return true;
            }
            // Переходим к следующему узлу
            prev = current;
            current = current->next;
        }
        return false;
    }
    
    // Проверка наличия элемента в множестве
    bool contains(int value) const {
        // Создаем указатель для текущего узла, начинаем с головы
        Node* current = head;
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Проверяем, совпадает ли значение текущего узла с искомым
            if (current->data == value) {
                // Если нашли, возвращаем true
                return true;
            }
            // Переходим к следующему узлу
            current = current->next;
        }
        // Если не нашли, возвращаем false
        return false;
    }
    
    // Получение размера множества
    int size() const {
        // Возвращаем количество элементов
        return count;
    }
    
    // Очистка множества
    void clear() {
        // Создаем указатель для текущего узла, начинаем с головы
        Node* current = head;
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Сохраняем ссылку на текущий узел
            Node* temp = current;
            // Переходим к следующему узлу
            current = current->next;
            // Освобождаем память текущего узла
            delete temp;
        }
        // Устанавливаем голову в nullptr
        head = nullptr;
        // Сбрасываем счетчик элементов
        count = 0;
    }
    
    // Получение суммы элементов множества
    int sum() const {
        // Инициализируем переменную для суммы
        int total = 0;
        // Создаем указатель для текущего узла, начинаем с головы
        Node* current = head;
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Добавляем значение текущего узла к сумме
            total += current->data;
            // Переходим к следующему узлу
            current = current->next;
        }
        return total;
    }
    
    // Получение всех элементов множества
    void getElements(vector<int>& elements) const {
        // Очищаем переданный вектор
        elements.clear();
        // Создаем указатель для текущего узла, начинаем с головы
        Node* current = head;
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Добавляем значение текущего узла в вектор
            elements.push_back(current->data);
            // Переходим к следующему узлу
            current = current->next;
        }
    }
    
    // Вывод множества
    void print() const {
        // Выводим открывающую фигурную скобку
        cout << "{";
        // Создаем указатель для текущего узла, начинаем с головы
        Node* current = head;
        // Флаг для определения первого элемента
        bool first = true;
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Если это не первый элемент, выводим запятую
            if (!first) {
                cout << ", ";
            }
            // Выводим значение текущего узла
            cout << current->data;
            // Устанавливаем флаг в false после первого элемента
            first = false;
            // Переходим к следующему узлу
            current = current->next;
        }
        cout << "}";
    }
};

// Вспомогательная структура для хранения результата
struct PartitionResult {
    Set set1;
    Set set2;
    int difference;
};

// Рекурсивная функция для нахождения минимальной разницы
void findMinDifference(const vector<int>& elements, int index, 
                      vector<int>& currentSet1, vector<int>& currentSet2,
                      vector<int>& bestSet1, vector<int>& bestSet2, 
                      int& minDiff, int sum1, int sum2) {
    // Базовый случай: все элементы распределены
    if (index == elements.size()) {
        // Вычисляем абсолютную разницу сумм двух множеств
        int diff = abs(sum1 - sum2);
        // Если нашли разницу меньше текущей минимальной
        if (diff < minDiff) {
            // Обновляем минимальную разницу
            minDiff = diff;
            bestSet1 = currentSet1;
            bestSet2 = currentSet2;
        }
       
        return;
    }
    
    // Получаем текущий элемент по индексу
    int current = elements[index];
    // Добавляем текущий элемент в вектор первого множества
    currentSet1.push_back(current);
    // Рекурсивно вызываем функцию для следующего элемента с обновленными параметрами
    findMinDifference(elements, index + 1, currentSet1, currentSet2, bestSet1, bestSet2, minDiff, sum1 + current, sum2);
    // Убираем элемент из вектора (backtracking)
    currentSet1.pop_back();
    currentSet2.push_back(current);
    // Рекурсивно вызываем функцию для следующего элемента с обновленными параметрами
    findMinDifference(elements, index + 1, currentSet1, currentSet2, bestSet1, bestSet2, minDiff, sum1, sum2 + current);
    // Убираем элемент из вектора (backtracking)
    currentSet2.pop_back();
}

// Функция для разделения множества на два подмножества с минимальной разницей сумм
PartitionResult partitionSet(const Set& originalSet) {
    // Создаем вектор для хранения элементов исходного множества
    vector<int> elements;
    // Получаем все элементы из исходного множества
    originalSet.getElements(elements);
    
    vector<int> currentSet1, currentSet2;  // Создаем временные векторы для текущего распределения
    vector<int> bestSet1, bestSet2; // Создаем временные векторы для текущего распределения
    int minDiff = INT_MAX;  // Инициализируем минимальную разницу максимальным значением
    
    findMinDifference(elements, 0, currentSet1, currentSet2, bestSet1, bestSet2, minDiff, 0, 0); // Инициализируем минимальную разницу максимальным значением
    
    // Создаем структуру для результата
    PartitionResult result;
    // Заполняем первое подмножество лучшими элементами
    for (int num : bestSet1) {
        result.set1.insert(num);
    }
    // Заполняем второе подмножество лучшими элементами
    for (int num : bestSet2) {
        result.set2.insert(num);
    }
    result.difference = minDiff;    
    return result;
}

int main() {
    Set originalSet;
    int n;
    
    cout << "Введите количество элементов в множестве: ";
    cin >> n;
    
    cout << "Введите " << n << " натуральных чисел:" << endl;
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        originalSet.insert(num);
    }
    
    PartitionResult result = partitionSet(originalSet);
    
    cout << "\nИсходное множество: ";
    originalSet.print();
    cout << endl;
    
    cout << "Подмножество 1: ";
    result.set1.print();
    cout << " (сумма = " << result.set1.sum() << ")" << endl;
    
    cout << "Подмножество 2: ";
    result.set2.print();
    cout << " (сумма = " << result.set2.sum() << ")" << endl;
    
    cout << "Разница сумм: " << result.difference << endl;
    
    return 0;
}
