#define INC_BUFF_SIZE 4
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
        T *arr;
        void resize();
};

template <typename T>
Deque<T>::Deque() : 
    head(-1),
    tail(-1),
    size(0) {
    arr = new T[INC_BUFF_SIZE];
    size = INC_BUFF_SIZE;
}

template <typename T>
Deque<T>::~Deque() {
    delete [] arr;
}

template <typename T>
void Deque<T>::resize() {
    T *newArr = new T[size + INC_BUFF_SIZE];
    if (
        ((tail == 0) && (head == size - 1)) ||
        ((head == 0) && (tail == size - 1))
    ) {
        memcpy(newArr, arr, sizeof(T) * size);
    } else if (tail == head + 1) {
        memcpy(newArr, arr, sizeof(T) * (head + 1));
        memcpy(&newArr[tail + INC_BUFF_SIZE], &arr[tail], sizeof(T) * (size - tail));
        tail += INC_BUFF_SIZE;
    }
    delete [] arr;
    arr = newArr;
    size += INC_BUFF_SIZE;
}

template <typename T>
void Deque<T>::pushFront(T value) {
    // cout << "Pushing Front!" << " head = " << head << " tail = " << tail << endl;
    if (isEmpty()) {
        head = tail = 0;
    } else if (head == size - 1) {
        if (tail == 0) {
            resize();
            // cout << "head = " << head << endl;
            head++;
        } else {
            head = 0;
        }
    } else if (head + 1 == tail) {
        resize();
        head++;
    } else {
        head++;
    }
    arr[head] = value;
}

template <typename T>
void Deque<T>::pushBack(T value) {
    if (isEmpty()) {
        head = tail = 0;
    } else if (tail == 0) {
        if (head == size - 1) resize();
        tail = size - 1;
    } else if (tail == head + 1) {
        resize();
        tail--;
    } else {
        tail--;
    }
    arr[tail] = value;
}

template <typename T>
T Deque<T>::popFront() {
    if (isEmpty())
        throw length_error("Unable to pop from empty deque");
    T value = arr[head];
    if (head == tail) head = tail = -1;
    else if (head == 0) head = size - 1;
    else head--;
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

bool testDeque(int *funcArr, int *valArr, int opCount) {
    Deque <int> deque;
    int value = 0;
    for (int i = 0; i < opCount; i++) {
        switch (funcArr[i]) {
        case 1:
            // cout << "pushing front: " << valArr[i] << endl;
            deque.pushFront(valArr[i]);
            break;
        case 2:
            try {
                value = deque.popFront();
            } catch (length_error) {
                value = -1;
            }
            if (value != valArr[i]) return 0;
            break;
        case 3:
            // cout << "pushing back: " << valArr[i] << endl;            
            deque.pushBack(valArr[i]);
            break;
        case 4:
            try {
                value = deque.popBack();
            } catch (length_error) {
                value = -1;
            }
            // cout << "value = " << value << endl;
            if (value != valArr[i]) return 0;
            break;
        }
        // deque.printAll();
    }
    return 1;
}

int main() {
    int opCount = 0;

    cin >> opCount;

    int *funcArr = new int[opCount];
    int *valArr = new int[opCount];

    for (int i = 0; i < opCount; i++) {
        cin >> funcArr[i];
        cin >> valArr[i];
    }

    if (testDeque(funcArr, valArr, opCount)) { 
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    delete [] funcArr;
    delete [] valArr;

    return 0;
}