/*
 *  1.2 Площадь n-угольника
 * 
 *  Вычислить площадь выпуклого n-угольника, заданного 
 *  координатами своих вершин. 
 *
 *  Вначале вводится количество вершин,
 *  затем последовательно целочисленные координаты всех вершин 
 *  в порядке обхода по часовой стрелке.
 *
 *  n < 1000, координаты < 10000.
*/

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

float count3angleArea(int x1, int y1, int x2, int y2, int x3, int y3) {
    float a = static_cast<float>((x1-x3)*(y2-y3));
    float b = static_cast<float>((x2-x3)*(y1-y3));
    return abs(a - b) / 2;
}

float countArea(int *xArr, int *yArr, int dotCount) {
    assert(dotCount > 0);
    if (dotCount < 3) return 0;
    int x1 = xArr[0], y1 = yArr[0],
        x2 = xArr[1], y2 = yArr[1],
        x3 = xArr[2], y3 = yArr[2];

    float area = 0;
    for (int i = 3; i <= dotCount; i++) {
        area += count3angleArea(x1, y1, x2, y2, x3, y3);
        x2 = x3;
        y2 = y3;
        x3 = xArr[i];
        y3 = yArr[i];
    }

    return area;
}

int main() {
    int angleCount = 0;
    cin >> angleCount;

    int *xArr = new int[angleCount];
    int *yArr = new int[angleCount];

    for (int i = 0; i < angleCount; i++) {
        cin >> xArr[i] >> yArr[i];
    }

    float area = countArea(xArr, yArr, angleCount);
    cout << area << endl;

    delete [] xArr;
    delete [] yArr;
    return 0;
}