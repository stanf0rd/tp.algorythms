/*  5_3. Закраска прямой 1.
 *  
 *  Во всех задачах данного раздела 
 *  необходимо реализовать и использовать сортировку слиянием. 
 *  Общее время работы алгоритма O(n log n).
 *
 *  На числовой прямой окрасили N отрезков. 
 *  Известны координаты левого и правого концов каждого отрезка (Li и Ri). 
 *  Найти длину окрашенной части числовой прямой.
*/

#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

template <typename T, typename Compare>
void mergeSort(T *arr, T *buff,
    int left, int right, const Compare &compare) {

    int size = right - left;
    if (size == 0) return;

    int middle = left + size/2 + 1;
    mergeSort(arr, buff, left, middle - 1, compare);
    mergeSort(arr, buff, middle, right, compare);

    int i = left,
        j = middle,
        k = left;  // new array iterator
    while ((i != middle) && j != right + 1) {
        if (compare(arr[i], arr[j])) buff[k++] = arr[i++];
        else buff[k++] = arr[j++];
    }

    while (i != middle) buff[k++] = arr[i++];
    while (j != right + 1) buff[k++] = arr[j++];

    memcpy(&arr[left], &buff[left], (size + 1)*sizeof(T) );
}

struct Line {
    int left = 0;
    int right = 0;

    Line() = default;
    Line(int _left, int _right) : left(_left), right(_right) {}
};

bool compareLines(Line first, Line second) {
    if (first.left == second.left)
        return first.right >= second.right;
    else
        return first.left < second.left;
}

int countLength(Line *lines, int count) {
    assert(count > 0);
    Line *buff = new Line[count];
    mergeSort(lines, buff, 0, count - 1, compareLines);
    delete [] buff;

    int begin = 0, end = 0, length = 0;
    for (int i = 0; i < count; i++) {
        if (i == 0) {
            begin = lines[i].left;
            end = lines[i].right;
            continue;
        } else if (lines[i].left == begin || lines[i].right <= end) {
            continue;
        }

        if (lines[i].left > end) {
            length += end - begin;
            begin = lines[i].left;
        }
        end = lines[i].right;
    }

    length += end - begin;
    return length;
}

int main() {
    int count = 0;
    cin >> count;
    Line *lines = new Line[count];
    for (int i = 0; i < count; i++) {
        int left = 0, right = 0;
        cin >> left >> right;
        lines[i] = Line(left, right);
    }

    cout << countLength(lines, count) << endl;

    delete [] lines;
    return 0;
}