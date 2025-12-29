#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
using namespace std;

class NodeT {
public:
    int data;        // Значение узла
    NodeT* left;     // Указатель на левое поддерево
    NodeT* right;    // Указатель на правое поддерево

    NodeT(int value) : data(value), left(nullptr), right(nullptr) {} // Конструктор узла
};

class FullBinaryTree {
private:
    NodeT* root;    // Корень дерева

public:
    FullBinaryTree() : root(nullptr) {}    
    ~FullBinaryTree() { clear(root); }     

    void insert(int value) { root = _insert(root, value); }  // Вставка нового узла
    void printTree(NodeT* node, int depth);                  
    void print();                                           
    void printZigZag();                                     // Обход змейкой

private:
    NodeT* _insert(NodeT* node, int value);  // Рекурсивная вставка
    void clear(NodeT* node);                 // Рекурсивное удаление дерева
};

// Рекурсивная вставка узла
NodeT* FullBinaryTree::_insert(NodeT* node, int value) {
    if (!node) return new NodeT(value);           // Если пусто, создаём новый узел
    if (value < node->data) node->left = _insert(node->left, value);   // Идём в левое поддерево
    else node->right = _insert(node->right, value);                     // Идём в правое поддерево
    return node;                                  // Возвращаем текущий узел
}

// Рекурсивное удаление всех узлов
void FullBinaryTree::clear(NodeT* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}


void FullBinaryTree::printTree(NodeT* node, int depth) {
    if (!node) return;
    printTree(node->right, depth + 1);                   // Сначала правое поддерево
    cout << setw(4 * depth) << " " << node->data << endl; // Вывод с отступом по уровню
    printTree(node->left, depth + 1);                   // Затем левое поддерево
}

void FullBinaryTree::print() { printTree(root, 0); }    // Вызов печати с корня

// Обход дерева змейкой
void FullBinaryTree::printZigZag() {
    if (!root) return;

    queue<NodeT*> q;              // Очередь для BFS
    q.push(root);
    bool leftToRight = true;      // Направление текущего уровня

    cout << "чтение змейкой дерева:\n";

    while (!q.empty()) {
        int levelSize = q.size();             // Количество узлов на текущем уровне
        vector<int> levelNodes(levelSize);    // Вектор для хранения узлов уровня

        for (int i = 0; i < levelSize; i++) {
            NodeT* node = q.front();
            q.pop();

            int index = leftToRight ? i : (levelSize - 1 - i);  // Определяем индекс для вставки
            levelNodes[index] = node->data;

            if (node->left) q.push(node->left);    // Добавляем детей в очередь
            if (node->right) q.push(node->right);
        }

        for (int val : levelNodes) cout << val << " ";  // Вывод текущего уровня
        leftToRight = !leftToRight;                     // Меняем направление на следующем уровне
    }
    cout << endl;
}


int main() {
    FullBinaryTree tree;
    int n, value;

    cout << "Сколько узлов будет в дереве? ";
    cin >> n;

    cout << "Введите " << n << " значений узлов:\n";
    for (int i = 0; i < n; ++i) {
        cin >> value;
        tree.insert(value);      // Вставка узла
    }

    cout << "\nДерево:\n";
    tree.print();                

    cout << "\nОбход змейкой:\n";
    tree.printZigZag();          // Обход змейкой

    return 0;
}
