/*  1_2. Хеш-таблица.
 *
 *  Реализуйте структуру данных типа “множество строк”
 *  на основе динамической хеш-таблицы с открытой адресацией.
 *  Хранимые строки непустые и состоят из строчных латинских букв.
 *  Хеш-функция строки должна быть реализована с помощью
 *  вычисления значения многочлена методом Горнера.
 *
 *  Начальный размер таблицы должен быть равным 8-ми.
 *  Перехеширование выполняйте при добавлении элементов в случае,
 *  когда коэффициент заполнения таблицы достигает 3/4.
 *
 *  Структура данных должна поддерживать операции
 *  добавления строки в множество,
 *  удаления строки из множества
 *  и проверки принадлежности данной строки множеству.
 *
 *
 *  Для разрешения коллизий используйте двойное хеширование.
 *
 *  Максимальное время: 0.2 с, память: 15Мб.
*/

#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>

#define DEFAULT_SIZE 8

using std::string;
using std::vector;
using std::function;
using std::cout;
using std::endl;

template <typename T>
class HashTable {
public:
    HashTable(int(const T, const int), int(const T, const int), T, T);
    // no data on heap - no destructor
    bool add(const T&);
    bool has(const T&) const;
    bool remove(const T&);
private:
    vector<T> table;
    function<int(const T, const int)> hash;
    function<int(const T, const int)> hashTwo;
    void incSize();
    void decSize();
    int tableSize;
    int elemCount;
    float alpha;
    const float maxAlpha;  // 3/4
    const float minAlpha;  // 1/3
    T nil, del;  // reserved values for empty cells
};

template <typename T>
HashTable<T>::HashTable(
    int func(const T, const int),
    int funcTwo(const T, const int),
    T _nil, T _del)
    :   hash(func),
        hashTwo(funcTwo),
        tableSize(DEFAULT_SIZE),
        elemCount(0), alpha(0),
        maxAlpha( float(3)/float(4) ),
        minAlpha( float(1)/float(3) ),
        nil(_nil), del(_del) {
    table.resize(tableSize, nil);
}

template <typename T>
void HashTable<T>::incSize() {
    tableSize *= 2;
    vector<T> newTable(tableSize, nil);
    for (auto it : table) {
        if (it == nil || it == del) continue;
        int index = hash(it, tableSize);
        while (newTable[index] != nil) {
            index += hashTwo(it, tableSize);
            index %= tableSize;
        }
        newTable[index] = it;
    }
    table = newTable;
    alpha = (float)elemCount/(float)tableSize;
}

template <typename T>
void HashTable<T>::decSize() {
    tableSize /= 2;
    vector<T> newTable(tableSize, nil);
    for (auto it : table) {
        if (it == nil || it == del) continue;
        int index = hash(it, tableSize);
        while (newTable[index] != nil) {
            index += hashTwo(it, tableSize);
            index %= tableSize;
        }
        newTable[index] = it;
    }
    table = newTable;
    alpha = (float)elemCount/(float)tableSize;
}

template <typename T>
bool HashTable<T>::add(const T& value) {
    if (alpha > maxAlpha) incSize();
    if (alpha < minAlpha && tableSize > DEFAULT_SIZE) decSize();

    if (value == nil || (value == del))
        throw std::invalid_argument("Reserved value catched");

    int findInd = hash(value, tableSize),
        putInd = -1,
        indexInc = 0;
    if (table[findInd] == nil) {
        indexInc = hashTwo(value, tableSize);
        findInd += indexInc;
    }
    while (table[findInd] != nil) {
        if (table[findInd] == value) return 0;
        if (table[findInd] == del) putInd = findInd;
        findInd += indexInc;
        findInd %= tableSize;
    }
    if (putInd == -1) putInd = findInd;
    table[putInd] = value;
    elemCount++;
    alpha = (float)elemCount/(float)tableSize;
    return 1;
}

template <typename T>
bool HashTable<T>::has(const T& value) const {
    int findInd = hash(value, tableSize),
        attempts = 0;
    while (table[findInd] != nil && attempts != tableSize) {
        if (table[findInd] == value) return 1;
        findInd += hashTwo(value, tableSize);
        findInd %= tableSize;
        attempts++;
    }
    return 0;
}

template <typename T>
bool HashTable<T>::remove(const T& value) {
    int findInd = hash(value, tableSize);
    while (table[findInd] != nil) {
        if (table[findInd] == value) {
            table[findInd] = del;
            elemCount--;
            return 1;
        }
        findInd += hashTwo(value, tableSize);
        findInd %= tableSize;
    }
    return 0;
}

int stringHash(const string key, const int tableSize) {
    uint hash = 0, a = 13;
    for (auto it : key) {
        hash *= a;
        hash += it;
        hash %= tableSize;
    } return hash;
}

int stringHashTwo(const string key, const int tableSize) {
    uint hash = 0, a = 59;
    for (auto it : key) {
        hash *= a;
        hash += it;
        hash %= tableSize;
    } return (hash * 2 + 1) % tableSize;
}

struct action {
    string operation;
    string value;
    action(string _o, string _v) : operation(_o), value(_v) {}
};

vector<string> proceed(vector<action> input) {
    HashTable<string> table(stringHash, stringHashTwo, "NIL", "DEL");
    vector<string> result;
    for (auto it : input) {
        int res = -1;
        if (it.operation == "+") res = table.add(it.value);
        else if (it.operation == "-") res = table.remove(it.value);
        else if (it.operation == "?") res = (table.has(it.value));

        if (res == 1) result.push_back("OK");
        else if (!res) result.push_back("FAIL");
    }
    return result;
}

int main() {
    vector<action> input;
    while(!std::cin.eof()) {
        string operation, value;
        std::cin >> operation >> value;
        action newAct(operation, value);
        input.push_back(newAct);
    }

    auto result = proceed(input);
    for (auto it : result) cout << it << endl;

    return 0;
}