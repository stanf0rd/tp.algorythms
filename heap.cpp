/*  4_3. Тупики.
 *
 *  На вокзале есть некоторое количество тупиков, куда прибывают электрички.
 *  Этот вокзал является их конечной станцией.
 *  Дано расписание движения электричек, в котором для каждой электрички
 *  указано время ее прибытия, а также время отправления в следующий рейс.
 *  Электрички в расписании упорядочены по времени прибытия.
 *  Когда электричка прибывает, ее ставят в свободный тупик с минимальным номером.
 *
 *  При этом если электричка из какого-то тупика отправилась в момент времени X,
 *  то электричку, которая прибывает в момент времени X, в этот тупик ставить нельзя,
 *  а электричку, прибывающую в момент X+1 — можно.
 *
 *  В данный момент на вокзале достаточное количество тупиков для работы по расписанию.
 *  Напишите программу, которая по данному расписанию определяет, 
 *  какое минимальное количество тупиков требуется для работы вокзала.
 *
 *  Максимальное время: 50мс, память: 5Мб.
*/

#define DEFAULT_SIZE 8
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <utility>

using namespace std;

template <typename T>
class Heap {
    public:
        Heap();
        Heap(T *arr, int count);
        ~Heap();
        void insert(T value);
        T extractMin();
        const T& getMin() const;
        int getCount() const;
        bool isEmpty() const;
    private:
        T *arr;
        int size;
        int elemCount;
        void siftUp();
        void siftDown(int parent = 0);
        void incSize();
        void decSize();
};

template <typename T>
Heap<T>::Heap() :
    arr(new T [DEFAULT_SIZE]),
    size(DEFAULT_SIZE),
    elemCount(0) {
}

template <typename T>
Heap<T>::Heap(T *arr, int count) :
    arr(arr),
    size(count),
    elemCount(count) {
    for (int i = elemCount/2; i >= 0; i--) {
        siftDown(i);
    }
}

template <typename T>
Heap<T>::~Heap() {
    delete [] arr;
}

template <typename T>
void Heap<T>::incSize() {
    T *newArr = new T[size*2];
    memcpy(newArr, arr, sizeof(T)*size);
    size *= 2;
    delete [] arr;
    arr = newArr;
}

template <typename T>
void Heap<T>::decSize() {
    size /= 2;
    T *newArr = new T[size];
    memcpy(newArr, arr, sizeof(T)*size);
    delete [] arr;
    arr = newArr;
}

template <typename T>
bool Heap<T>::isEmpty() const {
    if (!elemCount) return 1;
    else return 0;
}

template <typename T>
void Heap<T>::siftUp() {
    int n = elemCount - 1,
        parent = (n - 1)/2;

    while (arr[parent] > arr[n]) {
        swap(arr[parent], arr[n]);
        n = parent;
        parent = (n - 1)/2;
    }
}

template <typename T>
void Heap<T>::siftDown(int parent) {
    if (elemCount == 1) return;
    do {
        int leftCh = 2*parent + 1,
            rightCh = leftCh + 1;

        // проверяем, не выходим ли за границы массива
        if (rightCh == elemCount) {
            if (arr[leftCh] < arr[parent]) swap(arr[leftCh], arr[parent]);
            return;
        } else if (leftCh >= elemCount) {
            break;
        }

        if ((arr[rightCh] < arr[leftCh]) &&
            (arr[rightCh] < arr[parent])) {
            swap(arr[rightCh], arr[parent]);
            parent = rightCh;
        } else if (arr[leftCh] < arr[parent]) {
            swap(arr[leftCh], arr[parent]);
            parent = leftCh;
        } else {
            break;
        }

    } while(1);
}

template <typename T>
void Heap<T>::insert(T value) {
    if (elemCount == size) incSize();
    arr[elemCount] = value;
    if (++elemCount <= 1) return;
    siftUp();
}

template <typename T>
T Heap<T>::extractMin() {
    if (isEmpty())
        throw length_error("Unable to extract minimal from empty heap");
    T value = arr[0];
    arr[0] = arr[elemCount - 1];
    elemCount--;
    siftDown();
    if (elemCount < size/3) decSize();
    return value;
}

template <typename T>
const T& Heap<T>::getMin() const {
    if (isEmpty())
        throw length_error("Unable to get minimal from empty heap");
    return arr[0];
}

template <typename T>
int Heap<T>::getCount() const {
    return elemCount;
}

struct train {
    int arrive;
    int depart;
};

int countEnds(train *shedule, int trainCount) {
    Heap <int> heap;
    int maxEnds = 0;
    for (int i = 0; i < trainCount; i++) {
        while (!heap.isEmpty()) {
            if (heap.getMin() < shedule[i].arrive) heap.extractMin();
            else break;
        }
        heap.insert(shedule[i].depart);
        if (heap.getCount() > maxEnds) maxEnds = heap.getCount();
    }
    return maxEnds;
}

int main() {
    int trainCount = 0;
    cin >> trainCount;
    train *shedule = new train[trainCount];
    for (int i = 0; i < trainCount; i++) {
        int arrive = 0, depart = 0;
        cin >> arrive;
        cin >> depart;
        shedule[i] = {arrive, depart};
    }
    int ends = countEnds(shedule, trainCount);
    cout << ends << endl;
    delete [] shedule;
    return 0;
}