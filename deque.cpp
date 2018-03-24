/*
 *  3_2. Реализовать дек с динамическим зацикленным буфером.
 *
 *  Формат входных данных.
 *  В первой строке количество команд n. n ≤ 1000000.
 *  Каждая команда задаётся как 2 целых числа: a b.
 *  a = 1 - push front
 *  a = 2 - pop front
 *  a = 3 - push back
 *  a = 4 - pop back
 *  Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
 *  Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
 *  Если дана команда pop*, то число b - ожидаемое значение. 
 *  Если команда pop вызвана для пустой структуры данных, то ожидается “-1”. 
 *
 *  Требуется напечатать YES - если все ожидаемые значения совпали. 
 *  Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
 *
*/


#define ARR_SIZE_DIFF 100
#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std;

template <typename T>
class Deque {
    public:
        Deque();
        ~Deque();
        void pushFront(T value);
        void pushBack(T value);
        T popFront();
        T popBack();
        bool isEmpty();
        // void printAll();
    private:
        int head;
        int tail;
        int size;
        int elemCount;
        T *arr;
        void incrSize();
        void decrSize();
};

template <typename T>
Deque<T>::Deque() :
    head(-1),
    tail(-1),
    size(0),
    elemCount(0) {
    arr = new T[ARR_SIZE_DIFF];
    size = ARR_SIZE_DIFF;
}

template <typename T>
Deque<T>::~Deque() {
    delete [] arr;
}

template <typename T>
void Deque<T>::incrSize() {
    T *newArr = new T[size + ARR_SIZE_DIFF];
    if (
        ((tail == 0) && (head == size - 1)) ||
        ((head == 0) && (tail == size - 1))
    ) {
        memcpy(newArr, arr, sizeof(T) * size);
    } else if (tail == head + 1) {
        memcpy(newArr, arr, sizeof(T) * (head + 1));
        memcpy(&newArr[tail + ARR_SIZE_DIFF], &arr[tail], sizeof(T) * (size - tail));
        tail += ARR_SIZE_DIFF;
    }
    delete [] arr;
    arr = newArr;
    size += ARR_SIZE_DIFF;
}

template <typename T>
void Deque<T>::decrSize() {
    T *newArr = new T[size - ARR_SIZE_DIFF];
    if (head < tail) {
        memcpy(newArr, arr, sizeof(T) * (head + 1));
        memcpy(&newArr[tail - ARR_SIZE_DIFF], &arr[tail], sizeof(T) * (size - tail));
        tail -= ARR_SIZE_DIFF;
    } else if (tail >= ARR_SIZE_DIFF) {
        memcpy(newArr, arr + ARR_SIZE_DIFF, sizeof(T) * (size - ARR_SIZE_DIFF));
        tail -= ARR_SIZE_DIFF;
        head -= ARR_SIZE_DIFF;
    } else if (size - head >= ARR_SIZE_DIFF) {
        memcpy(newArr, arr, sizeof(T) * (size - ARR_SIZE_DIFF));
    } else {
        delete [] newArr;
        return;
    }
    delete [] arr;
    arr = newArr;
    size -= ARR_SIZE_DIFF;
}

template <typename T>
void Deque<T>::pushFront(T value) {
    if (isEmpty()) {
        head = tail = 0;
    } else if (head == size - 1) {
        if (tail == 0) {
            incrSize();
            head++;
        } else {
            head = 0;
        }
    } else if (head + 1 == tail) {
        incrSize();
        head++;
    } else {
        head++;
    }
    arr[head] = value;
    elemCount++;
}

template <typename T>
void Deque<T>::pushBack(T value) {
    if (isEmpty()) {
        head = tail = 0;
    } else if (tail == 0) {
        if (head == size - 1) incrSize();
        tail = size - 1;
    } else if (tail == head + 1) {
        incrSize();
        tail--;
    } else {
        tail--;
    }
    arr[tail] = value;
    elemCount++;
}

template <typename T>
T Deque<T>::popFront() {
    if (isEmpty())
        throw length_error("Unable to pop from empty deque");
    T value = arr[head];
    if (head == tail) head = tail = -1;
    else if (head == 0) head = size - 1;
    else head--;
    elemCount--;
    if ((size - elemCount) == (2 * ARR_SIZE_DIFF)) decrSize();
    return value;
}

template <typename T>
T Deque<T>::popBack() {
    if (isEmpty())
        throw length_error("Unable to pop from empty deque");
    T value = arr[tail];
    if (head == tail) head = tail = -1;
    else if (tail == size - 1) tail = 0;
    else tail++;
    elemCount--;
    if ((size - elemCount) == (2 * ARR_SIZE_DIFF)) decrSize();
    return value;
}

template <typename T>
bool Deque<T>::isEmpty() {
    return (head == -1) && (tail == -1);
}

/* template <typename T>
void Deque<T>::printAll() {
    if (head >= tail) {
        for (int i = tail; i <= head; i++) {
            cout << arr[i] << " ";
        }
    } else {
        for (int i = tail; i < size; i++)
            cout << arr[i] << " ";
        for (int i = 0; i <= head; i++)
            cout << arr[i] << " ";
    }

    cout << endl;
} */

bool testDeque(Deque<int> &deque, short func, int val) {
    int value = 0;
    switch (func) {
    case 1:
        deque.pushFront(val);
        break;
    case 2:
        try {
            value = deque.popFront();
        } catch (length_error) {
            value = -1;
        }
        if (value != val) return 0;
        break;
    case 3:
        deque.pushBack(val);
        break;
    case 4:
        try {
            value = deque.popBack();
        } catch (length_error) {
            value = -1;
        }
        if (value != val) return 0;
        break;
        }
        // deque.printAll();
    return 1;
}

int main() {
    Deque <int> deque;

    int opCount = 0;
    short func = 0;
    int val = 0;

    cin >> opCount;

    for (int i = 0; i < opCount; i++) {
        cin >> func >> val;
        if (!testDeque(deque, func, val)) { 
            cout << "NO" << endl;
            return 0;
        }
    }

    cout << "YES" << endl;
    return 0;
}