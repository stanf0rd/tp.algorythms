/*
 *  2.4 Поиск близнеца
 * 
 *  Дан отсортированный массив различных целых чисел 
 *  A[0..n-1] и массив целых чисел B[0..m-1].
 *  Для каждого элемента массива B[i] найдите 
 *  минимальный индекс элемента массива A[k],
 *  ближайшего по значению к B[i].
 *
 *  n ≤ 110000, m ≤ 1000.
*/
#define ERROR -1

#include <iostream>

using namespace std;

int findTwinIndx(int *, int, int, int);

int main() {
    int n, m;
    cin >> n;
    int *A = new int[n];
    for (int i = 0; i < n; i++) cin >> A[i];

    cin >> m;
    int *B = new int[m];
    for (int i = 0; i < m; i++) cin >> B[i];

    for (int i = 0; i < m; i++) {
        int result = findTwinIndx(A, 0, n-1, B[i]);
        if (result == ERROR) break;
        cout << result << " ";
    }

    delete [] A;
    delete [] B;

    return 0;
}

int findTwinIndx(int *arr, int l, int r, int pivot) {
    if ((!arr) ||
        (l > r) ||
        (l < 0) ||
        (r < 0)
    ) return ERROR;

    if (l == r) return l;
    else if (l + 1 == r) {
    // здесь и далее: diff - разница между элементом и искомым числом
        int lDiff = abs(arr[l] - pivot);
        int rDiff = abs(arr[r] - pivot);
        if (lDiff <= rDiff) return l;
        else return r;
    }
    int size = r - l,
        middle = l + size/2,
        diff = arr[middle] - pivot,
        newIndx = 0, newDiff = 0;
    if (!diff) {
        return middle;
    } else if (diff > 0) {
        newIndx = findTwinIndx(arr, l, middle, pivot);
    } else {
        newIndx = findTwinIndx(arr, middle + 1, r, pivot);
    }
    newDiff = arr[newIndx] - pivot;
    if (abs(newDiff) == abs(diff)) {
        if (newIndx < middle) return newIndx;
    } else if (abs(newDiff) < abs(diff)) {
        return newIndx;
    }
    return middle;
}
