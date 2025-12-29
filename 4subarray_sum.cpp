#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;


class Array {
private:
    string *arr;              // Указатель на динамический массив строк
    size_t volume;            // Вместимость массива (объем)
    size_t size;              // Текущее количество элементов

public:
    Array() : volume(10), size(0) {         
        arr = new string[volume];           
    }

    ~Array() {                             
        delete[] arr;                      
    }

    void ShowArray() const {                // Метод вывода всех элементов массива
        for (size_t i = 0; i < size; ++i) { // Проходим по всем элементам
            cout << arr[i] << endl;         
        }
        cout << endl;                       
    }

    void addToEnd(string value) {           // Добавление элемента в конец массива
        if (size >= volume) {               // Проверка: достаточно ли памяти
            volume *= 2;                    
            string *newArr = new string[volume]; // Создаем новый массив увеличенного размера
            for (size_t i = 0; i < size; ++i)    
                newArr[i] = arr[i];
            delete[] arr;                   
            arr = newArr;                  
        }
        arr[size++] = value;                
    }

    void add(size_t index, string value) {  // Вставка элемента по индексу
        if (index >= size) return;          // Проверяем, чтобы индекс был корректным

        string *newArr = new string[size + 1]; // Новый массив на один элемент больше
        for (size_t i = 0; i < index; ++i)     
            newArr[i] = arr[i];

        newArr[index] = value;              // Вставляем новый элемент на нужное место

        for (size_t i = index; i < size; ++i)  // Сдвигаем оставшиеся элементы вправо
            newArr[i + 1] = arr[i];

        delete[] arr;                       
        arr = newArr;                       
        size++;                             
    }

    string getIndex(size_t index) {         // Получение элемента по индексу
        if (index >= size) throw out_of_range("Index out of range"); // Проверка выхода за границы
        return arr[index];     
    }

    void remove(size_t index) {  
        if (index >= size) return;          // Проверка корректности индекса

        string *newArr = new string[size - 1]; // Новый массив меньшего размера
        for (size_t i = 0; i < index; ++i)    
            newArr[i] = arr[i];
        for (size_t i = index + 1; i < size; ++i) // Копируем элементы после удаляемого
            newArr[i - 1] = arr[i];

        delete[] arr;                       
        arr = newArr;                       
        size--;                             
    }

    void replace(size_t index, string value) { // Замена элемента по индексу
        if (index >= size) return;          // Проверяем границы
        arr[index] = value;                 // Присваиваем новое значение
    }

    size_t getSize() const {                // Возврат текущего размера массива
        return size;                        
    }
};




string subarrayToString(const vector<int>& arr, int start, int end) {
    ostringstream oss;                      // Создаем поток для формирования строки
    oss << "[";                             
    for (int i = start; i <= end; ++i) {    
        oss << arr[i];                      // Добавляем число в поток
        if (i < end) oss << ", ";           
    }
    oss << "]";                             
    return oss.str();                       
}

// Поиск всех подмассивов с заданной суммой
void findSubarraysWithSum(const vector<int>& arr, int target, Array& result) {
    int n = arr.size();                     // Получаем длину исходного массива

    for (int i = 0; i < n; ++i) {           
        int sum = 0;                        // Переменная для накопления суммы
        for (int j = i; j < n; ++j) {       // Внутренний цикл: конец подмассива
            sum += arr[j];                  
            if (sum == target) {            
                result.addToEnd(subarrayToString(arr, i, j)); // Добавляем строку-подмассив в Array
            }
        }
    }
}


int main() {
    vector<int> numbers = {4, -7, 1, 5, -4, 0, -3, 2, 4, 1}; 
    int target = 5;                                          

    Array result;                                            
    findSubarraysWithSum(numbers, target, result);           

    cout << "Подмассивы с суммой " << target << ":" << endl; 
    result.ShowArray();                                      

    return 0;                                                
}
