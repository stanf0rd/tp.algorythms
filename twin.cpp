/*  2.4 Поиск близнеца
 * 
 *  Дан отсортированный массив различных целых чисел 
 *  A[0..n-1] и массив целых чисел B[0..m-1].
 *  Для каждого элемента массива B[i] найдите 
 *  минимальный индекс элемента массива A[k],
 *  ближайшего по значению к B[i].
 *
 *  n ≤ 110000, m ≤ 1000.
*/

#include <iostream>

using namespace std;

void findTwinIndx(int *A, int *B, int aSize, int bSize) {
    for (int i = 0; i < bSize; i++) {
        int pivot = B[i];
        int j = 1, left = 0, right = aSize - 1;
        while(1) {
            if (right - left <= 1) {
                if (abs(A[left] - pivot) <= abs(A[right] - pivot))
                    cout << left << " ";
                else cout << right << " ";
                break;
            }
            if (A[left + j] >= pivot) {
                right = left + j;
                left += j/2;
                j = 1;
            } else if (left + j*2 > right) {
                left += j;
                j = 1;
            } else {
                j *= 2;
            }
        }
    }
    cout << endl;
}

int main() {
    int aSize, bSize;
    cin >> aSize;
    int *A = new int[aSize];
    for (int i = 0; i < aSize; i++) cin >> A[i];

    cin >> bSize;
    int *B = new int[bSize];
    for (int i = 0; i < bSize; i++) cin >> B[i];

    findTwinIndx(A, B, aSize, bSize);

    delete [] A;
    delete [] B;

    return 0;
}
