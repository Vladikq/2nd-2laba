#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Класс для хранения массива строк с динамическим размером
class StringArray {
private:
    string* data;
    size_t capacity;
    size_t size;
    
    void resize() {
        capacity = capacity == 0 ? 4 : capacity * 2;
        string* newData = new string[capacity];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
    
public:
    StringArray() : data(nullptr), capacity(0), size(0) {}
    
    ~StringArray() {
        delete[] data;
    }
    
    void add(const string& value) {
        if (size == capacity) {
            resize();
        }
        data[size++] = value;
    }
    
    bool remove(const string& value) {
        for (size_t i = 0; i < size; i++) {
            if (data[i] == value) {
                // Сдвигаем элементы
                for (size_t j = i; j < size - 1; j++) {
                    data[j] = data[j + 1];
                }
                size--;
                return true;
            }
        }
        return false;
    }
    
    bool contains(const string& value) const {
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
    
    string* begin() { return data; }
    string* end() { return data + size; }
    const string* begin() const { return data; }
    const string* end() const { return data + size; }
    
    // Сортировка элементов
    void sort() {
        std::sort(data, data + size);
    }
    
    // Удаление дубликатов (предполагается, что массив отсортирован)
    void unique() {
        if (size == 0) return;
        
        size_t j = 0;
        for (size_t i = 1; i < size; i++) {
            if (data[i] != data[j]) {
                data[++j] = data[i];
            }
        }
        size = j + 1;
    }
    
    void clear() {
        size = 0;
    }
};

// Класс собственного множества
class FastSet {
private:
    StringArray elements;
    
public:
    void add(const string& value) {
        if (!contains(value)) {
            elements.add(value);
        }
    }
    
    void remove(const string& value) {
        elements.remove(value);
    }
    
    bool contains(const string& value) const {
        return elements.contains(value);
    }
    
    string toString() const {
        string result;
        for (size_t i = 0; i < elements.getSize(); i++) {
            result += elements.begin()[i] + " ";
        }
        return result;
    }
    
    void clear() {
        elements.clear();
    }
    
    size_t size() const {
        return elements.getSize();
    }
    
    // Операции с множествами
    FastSet unionWith(const FastSet& other) const {
        FastSet result;
        
        // Добавляем все элементы из текущего множества
        for (size_t i = 0; i < elements.getSize(); i++) {
            result.add(elements.begin()[i]);
        }
        
        // Добавляем элементы из другого множества
        for (size_t i = 0; i < other.elements.getSize(); i++) {
            result.add(other.elements.begin()[i]);
        }
        
        return result;
    }
    
    FastSet intersectWith(const FastSet& other) const {
        FastSet result;
        
        for (size_t i = 0; i < elements.getSize(); i++) {
            if (other.contains(elements.begin()[i])) {
                result.add(elements.begin()[i]);
            }
        }
        
        return result;
    }
    
    FastSet differenceWith(const FastSet& other) const {
        FastSet result;
        
        for (size_t i = 0; i < elements.getSize(); i++) {
            if (!other.contains(elements.begin()[i])) {
                result.add(elements.begin()[i]);
            }
        }
        
        return result;
    }
    
    bool isSubsetOf(const FastSet& other) const {
        for (size_t i = 0; i < elements.getSize(); i++) {
            if (!other.contains(elements.begin()[i])) {
                return false;
            }
        }
        return true;
    }
    
    // Чтение элементов в массив
    void getAllElements(string* buffer, size_t maxSize) const {
        size_t count = std::min(elements.getSize(), maxSize);
        for (size_t i = 0; i < count; i++) {
            buffer[i] = elements.begin()[i];
        }
    }
};

// Чтение множества из файла
FastSet readSetFromFile(const string& filename, const string& setName) {
    FastSet set;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Не удалось открыть файл: " << filename << endl;
        return set;
    }
    
    string line;
    bool foundSet = false;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        ss >> name;
        
        if (name == setName) {
            foundSet = true;
            string value;
            while (ss >> value) {
                set.add(value);
            }
            break;
        }
    }
    
    if (!foundSet) {
        cout << "Множество '" << setName << "' не найдено в файле. Создано пустое множество." << endl;
    }
    
    file.close();
    return set;
}

// Запись множества в файл
void writeSetToFile(const string& filename, const string& setName, const FastSet& set) {
    // Читаем все строки из файла
    ifstream inFile(filename);
    string content;
    string line;
    bool foundSet = false;
    
    while (getline(inFile, line)) {
        stringstream ss(line);
        string name;
        ss >> name;
        
        if (name == setName) {
            // Заменяем строку обновленным множеством
            content += setName + " " + set.toString() + "\n";
            foundSet = true;
        } else {
            content += line + "\n";
        }
    }
    inFile.close();
    
    // Если множество не найдено, добавляем его в конец
    if (!foundSet) {
        content += setName + " " + set.toString() + "\n";
    }
    
    // Перезаписываем файл
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Не удалось открыть файл для записи: " << filename << endl;
        return;
    }
    
    outFile << content;
    outFile.close();
}

// Вывод информации о множестве
void displaySetInfo(const string& setName, const FastSet& set) {
    cout << "Множество '" << setName << "':" << endl;
    cout << "  Размер: " << set.size() << endl;
    cout << "  Элементы: ";
    
    if (set.size() == 0) {
        cout << "(пусто)";
    } else {
        cout << set.toString();
    }
    cout << endl;
}

// Обработка команд с множествами
void processSetCommand(const string& command, const string& filename) {
    stringstream ss(command);
    string operation, setName, value;
    ss >> operation >> setName;
    
    FastSet set = readSetFromFile(filename, setName);
    
    if (operation == "SETADD") {
        ss >> value;
        set.add(value);
        cout << "Элемент '" << value << "' добавлен в множество '" << setName << "'" << endl;
        writeSetToFile(filename, setName, set);
        displaySetInfo(setName, set);
    }
    else if (operation == "SETDEL") {
        ss >> value;
        bool removed = set.contains(value);
        set.remove(value);
        if (removed) {
            cout << "Элемент '" << value << "' удален из множества '" << setName << "'" << endl;
        } else {
            cout << "Элемент '" << value << "' не найден в множестве '" << setName << "'" << endl;
        }
        writeSetToFile(filename, setName, set);
        displaySetInfo(setName, set);
    }
    else if (operation == "SET_AT") {
        ss >> value;
        bool exists = set.contains(value);
        cout << "Элемент '" << value << "' " 
             << (exists ? "присутствует" : "отсутствует") 
             << " в множестве '" << setName << "'" << endl;
    }
    else if (operation == "SET_SHOW") {
        displaySetInfo(setName, set);
    }
    else if (operation == "SET_CLEAR") {
        set.clear();
        cout << "Множество '" << setName << "' очищено" << endl;
        writeSetToFile(filename, setName, set);
    }
    else if (operation == "SET_SIZE") {
        cout << "Размер множества '" << setName << "': " << set.size() << endl;
    }
    else {
        cout << "Неизвестная операция: " << operation << endl;
        cout << "Доступные операции: SETADD, SETDEL, SET_AT, SET_SHOW, SET_CLEAR, SET_SIZE" << endl;
    }
}

// Операции с двумя множествами
void processBinarySetCommand(const string& command, const string& filename) {
    stringstream ss(command);
    string operation, setName1, setName2, resultName;
    ss >> operation >> setName1 >> setName2 >> resultName;
    
    FastSet set1 = readSetFromFile(filename, setName1);
    FastSet set2 = readSetFromFile(filename, setName2);
    FastSet result;
    
    if (operation == "SET_UNION") {
        result = set1.unionWith(set2);
        cout << "Объединение множеств '" << setName1 << "' и '" << setName2 
             << "' сохранено в '" << resultName << "'" << endl;
    }
    else if (operation == "SET_INTERSECT") {
        result = set1.intersectWith(set2);
        cout << "Пересечение множеств '" << setName1 << "' и '" << setName2 
             << "' сохранено в '" << resultName << "'" << endl;
    }
    else if (operation == "SET_DIFFERENCE") {
        result = set1.differenceWith(set2);
        cout << "Разность множеств '" << setName1 << "' и '" << setName2 
             << "' сохранено в '" << resultName << "'" << endl;
    }
    else if (operation == "SET_SUBSET") {
        bool isSubset = set1.isSubsetOf(set2);
        cout << "Множество '" << setName1 << "' " 
             << (isSubset ? "является" : "не является") 
             << " подмножеством '" << setName2 << "'" << endl;
        return;
    }
    else {
        cout << "Неизвестная бинарная операция: " << operation << endl;
        cout << "Доступные операции: SET_UNION, SET_INTERSECT, SET_DIFFERENCE, SET_SUBSET" << endl;
        return;
    }
    
    writeSetToFile(filename, resultName, result);
    displaySetInfo(resultName, result);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Использование: " << argv[0] << " --file <filename> --query <command>" << endl;
        cout << "\nПримеры команд:" << endl;
        cout << "  SETADD myset apple" << endl;
        cout << "  SETDEL myset apple" << endl;
        cout << "  SET_AT myset apple" << endl;
        cout << "  SET_SHOW myset" << endl;
        cout << "  SET_CLEAR myset" << endl;
        cout << "  SET_SIZE myset" << endl;
        cout << "  SET_UNION set1 set2 result" << endl;
        cout << "  SET_INTERSECT set1 set2 result" << endl;
        cout << "  SET_DIFFERENCE set1 set2 result" << endl;
        cout << "  SET_SUBSET set1 set2" << endl;
        return 1;
    }

    string filename, query;

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--file") {
            filename = argv[++i];
        }
        else if (string(argv[i]) == "--query") {
            query = argv[++i];
        }
    }

    // Определяем тип операции
    stringstream ss(query);
    string operation;
    ss >> operation;
    
    if (operation.substr(0, 3) == "SET") {
        if (operation == "SET_UNION" || operation == "SET_INTERSECT" || 
            operation == "SET_DIFFERENCE" || operation == "SET_SUBSET") {
            processBinarySetCommand(query, filename);
        } else {
            processSetCommand(query, filename);
        }
    }
    else {
        cout << "Неизвестный тип запроса: " << query << endl;
    }

    return 0;
}
