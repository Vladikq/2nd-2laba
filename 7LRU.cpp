#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

// Узел двусвязного списка для LRU-кэша
struct Node {
    string key;
    string value;
    Node* prev;
    Node* next;

    Node(string k, string v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    int capacity;
    Node* head; // Самый свежий элемент
    Node* tail; // Самый старый элемент
    unordered_map<string, Node*> hash; // Быстрый доступ по ключу

    // Удаление узла из двусвязного списка
    void removeNode(Node* node) {
        if (node->prev != nullptr)
            node->prev->next = node->next;
        else
            head = node->next;

        if (node->next != nullptr)
            node->next->prev = node->prev;
        else
            tail = node->prev;
    }

    // Добавление узла в начало списка (обновление LRU)
    void addToFront(Node* node) {
        node->next = head;
        node->prev = nullptr;

        if (head != nullptr)
            head->prev = node;
        head = node;

        if (tail == nullptr)
            tail = head;
    }

    // Перемещение существующего узла в начало списка
    void moveToFront(Node* node) {
        removeNode(node);
        addToFront(node);
    }

public:
    LRUCache(int cap) {
        capacity = cap;
        head = nullptr;
        tail = nullptr;
    }

    string get(string key) {
        if (hash.find(key) != hash.end()) {
            Node* node = hash[key];
            moveToFront(node);
            return node->value;
        }
        return "NOT FOUND";
    }

    void set(string key, string value) {
        if (hash.find(key) != hash.end()) {
            Node* node = hash[key];
            node->value = value;
            moveToFront(node);
        } else {
            if ((int)hash.size() == capacity) {
                // Удаляем старейший элемент
                hash.erase(tail->key);
                removeNode(tail);
            }

            Node* newNode = new Node(key, value);
            addToFront(newNode);
            hash[key] = newNode;
        }
    }

    // Вывод содержимого кэша
    void printCache() {
        cout << "Текущий кэш: ";
        Node* current = head;
        while (current != nullptr) {
            cout << "[" << current->key << ":" << current->value << "] ";
            current = current->next;
        }
        cout << endl;
    }

    // Освобождение памяти
    ~LRUCache() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
};

int main() {
    int capacity, Q;

    cout << "Введите емкость кэша (cap): ";
    cin >> capacity;
    cout << "Введите количество запросов (Q): ";
    cin >> Q;

    LRUCache cache(capacity);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nВведите запросы (SET key value или GET key):" << endl;

    for (int i = 0; i < Q; i++) {
        string query;
        cout << "Запрос " << (i + 1) << ": ";
        getline(cin, query);

        stringstream ss(query);
        string command;
        ss >> command;

        if (command == "SET") {
            string key, value;
            if (ss >> key >> value) {
                cache.set(key, value);
                cout << "Выполнено: SET " << key << " " << value << endl;
                cache.printCache();
            } else {
                cout << "Ошибка: неверный формат SET. Используйте: SET key value" << endl;
            }
        } else if (command == "GET") {
            string key;
            if (ss >> key) {
                string result = cache.get(key);
                cout << "Результат GET " << key << ": " << result << endl;
                cache.printCache();
            } else {
                cout << "Ошибка: неверный формат GET. Используйте: GET key" << endl;
            }
        } else {
            cout << "Ошибка: неизвестная команда. Используйте SET или GET" << endl;
        }

        cout << "---" << endl;
    }

    return 0;
}
