#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

// Класс для хранения массива целых чисел с динамическим размером
class IntArray {
private:
    int* data;
    size_t capacity;
    size_t size;
    
    void resize() {
        capacity = capacity == 0 ? 4 : capacity * 2;
        int* newData = new int[capacity];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
    
public:
    IntArray() : data(nullptr), capacity(0), size(0) {}
    
    ~IntArray() {
        delete[] data;
    }
    
    // Копирующий конструктор
    IntArray(const IntArray& other) : data(nullptr), capacity(0), size(0) {
        capacity = other.capacity;
        size = other.size;
        data = new int[capacity];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    
    // Оператор присваивания
    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new int[capacity];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    void add(int value) {
        if (size == capacity) {
            resize();
        }
        data[size++] = value;
    }
    
    bool remove(int value) {
        for (size_t i = 0; i < size; i++) {
            if (data[i] == value) {
                for (size_t j = i; j < size - 1; j++) {
                    data[j] = data[j + 1];
                }
                size--;
                return true;
            }
        }
        return false;
    }
    
    bool contains(int value) const {
        for (size_t i = 0; i < size; i++) {
            if (data[i] == value) {
                return true;
            }
        }
        return false;
    }
    
    size_t getSize() const {
        return size;
    }
    
    int getAt(size_t index) const {
        if (index < size) {
            return data[index];
        }
        throw out_of_range("Index out of bounds");
    }
    
    void clear() {
        size = 0;
    }
    
    // Итераторы
    int* begin() { return data; }
    int* end() { return data + size; }
    const int* begin() const { return data; }
    const int* end() const { return data + size; }
    
    // Сортировка элементов
    void sort() {
        std::sort(data, data + size);
    }
};

// Класс собственного множества целых чисел
class FastSet {
private:
    IntArray elements;
    
public:
    void add(int value) {
        if (!contains(value)) {
            elements.add(value);
        }
    }
    
    void remove(int value) {
        elements.remove(value);
    }
    
    bool contains(int value) const {
        return elements.contains(value);
    }
    
    void clear() {
        elements.clear();
    }
    
    size_t size() const {
        return elements.getSize();
    }
    
    int sum() const {
        int total = 0;
        for (size_t i = 0; i < elements.getSize(); i++) {
            total += elements.getAt(i);
        }
        return total;
    }
    
    void getElements(vector<int>& result) const {
        result.clear();
        result.reserve(elements.getSize());
        for (size_t i = 0; i < elements.getSize(); i++) {
            result.push_back(elements.getAt(i));
        }
    }
    
    vector<int> getElements() const {
        vector<int> result;
        getElements(result);
        return result;
    }
    
    void print() const {
        cout << "{";
        bool first = true;
        for (size_t i = 0; i < elements.getSize(); i++) {
            if (!first) {
                cout << ", ";
            }
            cout << elements.getAt(i);
            first = false;
        }
        cout << "}";
    }
    
    // Операции с множествами
    FastSet unionWith(const FastSet& other) const {
        FastSet result;
        
        // Добавляем все элементы из текущего множества
        for (size_t i = 0; i < elements.getSize(); i++) {
            result.add(elements.getAt(i));
        }
        
        // Добавляем элементы из другого множества
        for (size_t i = 0; i < other.elements.getSize(); i++) {
            result.add(other.elements.getAt(i));
        }
        
        return result;
    }
    
    FastSet intersectWith(const FastSet& other) const {
        FastSet result;
        
        for (size_t i = 0; i < elements.getSize(); i++) {
            int value = elements.getAt(i);
            if (other.contains(value)) {
                result.add(value);
            }
        }
        
        return result;
    }
    
    FastSet differenceWith(const FastSet& other) const {
        FastSet result;
        
        for (size_t i = 0; i < elements.getSize(); i++) {
            int value = elements.getAt(i);
            if (!other.contains(value)) {
                result.add(value);
            }
        }
        
        return result;
    }
    
    bool isSubsetOf(const FastSet& other) const {
        for (size_t i = 0; i < elements.getSize(); i++) {
            if (!other.contains(elements.getAt(i))) {
                return false;
            }
        }
        return true;
    }
};

// Вспомогательная структура для хранения результата
struct PartitionResult {
    FastSet set1;
    FastSet set2;
    int difference;
    
    void print() const {
        cout << "Подмножество 1: ";
        set1.print();
        cout << " (сумма = " << set1.sum() << ")" << endl;
        
        cout << "Подмножество 2: ";
        set2.print();
        cout << " (сумма = " << set2.sum() << ")" << endl;
        
        cout << "Разница сумм: " << difference << endl;
    }
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
    
    // Добавляем текущий элемент в первое множество
    currentSet1.push_back(current);
    findMinDifference(elements, index + 1, currentSet1, currentSet2, 
                     bestSet1, bestSet2, minDiff, sum1 + current, sum2);
    currentSet1.pop_back();
    
    // Добавляем текущий элемент во второе множество
    currentSet2.push_back(current);
    findMinDifference(elements, index + 1, currentSet1, currentSet2, 
                     bestSet1, bestSet2, minDiff, sum1, sum2 + current);
    currentSet2.pop_back();
}

// Оптимизированная версия с отсечением
void findMinDifferenceOptimized(const vector<int>& elements, int index, 
                               vector<int>& currentSet1, vector<int>& currentSet2,
                               vector<int>& bestSet1, vector<int>& bestSet2, 
                               int& minDiff, int sum1, int sum2,
                               int totalSum, vector<int>& suffixSums) {
    // Базовый случай: все элементы распределены
    if (index == elements.size()) {
        int diff = abs(sum1 - sum2);
        if (diff < minDiff) {
            minDiff = diff;
            bestSet1 = currentSet1;
            bestSet2 = currentSet2;
        }
        return;
    }
    
    // Отсечение: если текущая разница + минимальная возможная разница оставшихся элементов
    // больше чем лучшая найденная разница, прекращаем поиск
    int remainingSum = totalSum - sum1 - sum2;
    int currentDiff = abs(sum1 - sum2);
    
    // Минимальная возможная разница при идеальном распределении оставшихся элементов
    if (currentDiff - remainingSum > minDiff) {
        return;
    }
    
    int current = elements[index];
    
    // Пробуем добавить в первое множество
    currentSet1.push_back(current);
    findMinDifferenceOptimized(elements, index + 1, currentSet1, currentSet2,
                              bestSet1, bestSet2, minDiff, sum1 + current, sum2,
                              totalSum, suffixSums);
    currentSet1.pop_back();
    
    // Пробуем добавить во второе множество
    currentSet2.push_back(current);
    findMinDifferenceOptimized(elements, index + 1, currentSet1, currentSet2,
                              bestSet1, bestSet2, minDiff, sum1, sum2 + current,
                              totalSum, suffixSums);
    currentSet2.pop_back();
}

// Функция для разделения множества на два подмножества с минимальной разницей сумм
PartitionResult partitionSet(const FastSet& originalSet) {
    // Получаем все элементы из исходного множества
    vector<int> elements = originalSet.getElements();
    
    // Если множество пустое
    if (elements.empty()) {
        PartitionResult result;
        result.difference = 0;
        return result;
    }
    
    // Если только один элемент
    if (elements.size() == 1) {
        PartitionResult result;
        result.set1.add(elements[0]);
        result.difference = elements[0];
        return result;
    }
    
    // Сортируем элементы по убыванию для лучшего отсечения
    sort(elements.rbegin(), elements.rend());
    
    vector<int> currentSet1, currentSet2;
    vector<int> bestSet1, bestSet2;
    int minDiff = INT_MAX;
    
    // Вычисляем общую сумму для оптимизации
    int totalSum = 0;
    for (int num : elements) {
        totalSum += num;
    }
    
    // Вычисляем суммы суффиксов для отсечения
    vector<int> suffixSums(elements.size() + 1, 0);
    for (int i = elements.size() - 1; i >= 0; i--) {
        suffixSums[i] = suffixSums[i + 1] + elements[i];
    }
    
    // Вызываем оптимизированную рекурсивную функцию
    findMinDifferenceOptimized(elements, 0, currentSet1, currentSet2,
                              bestSet1, bestSet2, minDiff, 0, 0,
                              totalSum, suffixSums);
    
    // Создаем структуру для результата
    PartitionResult result;
    
    // Заполняем первое подмножество лучшими элементами
    for (int num : bestSet1) {
        result.set1.add(num);
    }
    
    // Заполняем второе подмножество лучшими элементами
    for (int num : bestSet2) {
        result.set2.add(num);
    }
    
    // Если какие-то элементы остались (должно быть все распределено)
    // Добавляем оставшиеся во второе множество
    FastSet allAssigned = result.set1.unionWith(result.set2);
    for (int num : elements) {
        if (!allAssigned.contains(num)) {
            result.set2.add(num);
        }
    }
    
    result.difference = minDiff;
    return result;
}

// Альтернативный жадный алгоритм (быстрее, но не всегда оптимальный)
PartitionResult partitionSetGreedy(const FastSet& originalSet) {
    vector<int> elements = originalSet.getElements();
    
    // Сортируем по убыванию
    sort(elements.rbegin(), elements.rend());
    
    PartitionResult result;
    int sum1 = 0, sum2 = 0;
    
    // Жадное распределение
    for (int num : elements) {
        if (sum1 <= sum2) {
            result.set1.add(num);
            sum1 += num;
        } else {
            result.set2.add(num);
            sum2 += num;
        }
    }
    
    result.difference = abs(sum1 - sum2);
    return result;
}

// Функция для ввода множества
FastSet inputSet() {
    FastSet set;
    int n;
    
    cout << "Введите количество элементов в множестве: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Множество будет пустым." << endl;
        return set;
    }
    
    cout << "Введите " << n << " натуральных чисел:" << endl;
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        if (num > 0) {
            set.add(num);
        } else {
            cout << "Число " << num << " пропущено (только натуральные числа)." << endl;
        }
    }
    
    return set;
}

int main() {
    cout << "=== РАЗБИЕНИЕ МНОЖЕСТВА НА ДВА ПОДМНОЖЕСТВА ===" << endl;
    cout << "Цель: минимизировать разницу сумм элементов в подмножествах" << endl << endl;
    
    FastSet originalSet = inputSet();
    
    if (originalSet.size() == 0) {
        cout << "Множество пустое. Программа завершена." << endl;
        return 0;
    }
    
    cout << "\nИсходное множество: ";
    originalSet.print();
    cout << endl;
    cout << "Сумма всех элементов: " << originalSet.sum() << endl;
    cout << "Количество элементов: " << originalSet.size() << endl;
    
    cout << "\nВыберите алгоритм:" << endl;
    cout << "1. Точный алгоритм (рекурсивный поиск)" << endl;
    cout << "2. Жадный алгоритм (быстрый, но не всегда оптимальный)" << endl;
    cout << "Ваш выбор (1 или 2): ";
    
    int choice;
    cin >> choice;
    
    PartitionResult result;
    
    if (choice == 1) {
        cout << "\nИспользуется точный алгоритм..." << endl;
        
        // Предупреждение для больших множеств
        if (originalSet.size() > 20) {
            cout << "Внимание: Для " << originalSet.size() << " элементов точный алгоритм" << endl;
            cout << "может работать очень долго (2^" << originalSet.size() << " вариантов)." << endl;
            cout << "Рекомендуется использовать жадный алгоритм для n > 20." << endl;
            cout << "Продолжить? (y/n): ";
            char answer;
            cin >> answer;
            if (answer != 'y' && answer != 'Y') {
                cout << "Используем жадный алгоритм вместо точного." << endl;
                result = partitionSetGreedy(originalSet);
            } else {
                result = partitionSet(originalSet);
            }
        } else {
            result = partitionSet(originalSet);
        }
    } else {
        cout << "\nИспользуется жадный алгоритм..." << endl;
        result = partitionSetGreedy(originalSet);
    }
    
    cout << "\n=== РЕЗУЛЬТАТ РАЗБИЕНИЯ ===" << endl;
    result.print();
    
    // Дополнительная информация
    cout << "\n=== АНАЛИЗ РЕЗУЛЬТАТА ===" << endl;
    cout << "Идеальная разница (если бы можно было разделить поровну): 0" << endl;
    cout << "Достигнутая разница: " << result.difference << endl;
    
    double totalSum = originalSet.sum();
    double relativeError = (result.difference * 100.0) / totalSum;
    cout << "Относительная погрешность: " << relativeError << "%" << endl;
    
    // Проверка, что все элементы распределены
    FastSet unionSets = result.set1.unionWith(result.set2);
    if (unionSets.size() == originalSet.size() && 
        unionSets.isSubsetOf(originalSet) && 
        originalSet.isSubsetOf(unionSets)) {
        cout << "✓ Все элементы корректно распределены" << endl;
    } else {
        cout << "✗ Ошибка: не все элементы распределены!" << endl;
    }
    
    return 0;
}
