#include <iostream>
#include <string>
#include <unordered_set>
#include <fstream>
#include <sstream>

using namespace std;

class FastSet {
private:
    unordered_set<string> data;

public:
    void add(const string& value) {
        data.insert(value);
    }

    void remove(const string& value) {
        data.erase(value);
    }

    bool contains(const string& value) {
        return data.find(value) != data.end();
    }

    string toString() const {  // ✅ ДОБАВЛЕНО const
        string result;
        for (const auto& item : data) {
            result += item + " ";
        }
        return result;
    }

    void clear() {
        data.clear();
    }
};

// Чтение множества из файла
FastSet readSetFromFile(const string& filename, const string& setName) {
    FastSet set;
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        ss >> name;
        
        if (name == setName) {
            string value;
            while (ss >> value) {
                set.add(value);
            }
            break;
        }
    }
    file.close();
    return set;
}

// Запись множества в файл
void writeSetToFile(const string& filename, const string& setName, const FastSet& set) {
    // Читаем все строки кроме нужного множества
    ifstream inFile(filename);
    string content, line;
    
    while (getline(inFile, line)) {
        stringstream ss(line);
        string name;
        ss >> name;
        
        if (name != setName) {
            content += line + "\n";
        }
    }
    inFile.close();

    // Добавляем обновленное множество
    content += setName + " " + set.toString() + "\n";  // ✅ ТЕПЕРЬ РАБОТАЕТ

    // Перезаписываем файл
    ofstream outFile(filename);
    outFile << content;
    outFile.close();
}

// Обработка команд
void processSetCommand(const string& command, const string& filename) {
    stringstream ss(command);
    string operation, setName, value;
    ss >> operation >> setName >> value;

    FastSet set = readSetFromFile(filename, setName);

    if (operation == "SETADD") {
        set.add(value);
        cout << "Элемент " << value << " добавлен в множество " << setName << endl;
    }
    else if (operation == "SETDEL") {
        set.remove(value);
        cout << "Элемент " << value << " удален из множества " << setName << endl;
    }
    else if (operation == "SET_AT") {
        bool exists = set.contains(value);
        cout << "Элемент " << value << (exists ? " присутствует" : " отсутствует") 
             << " в множестве " << setName << endl;
        return;
    }
    else {
        cout << "Неизвестная операция: " << operation << endl;
        return;
    }

    writeSetToFile(filename, setName, set);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Использование: " << argv[0] << " --file <filename> --query <command>" << endl;
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

    if (query.substr(0, 3) == "SET") {
        processSetCommand(query, filename);
    }
    else {
        cout << "Неизвестный тип запроса: " << query << endl;
    }

    return 0;
}