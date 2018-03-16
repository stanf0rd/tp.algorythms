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
 * n < 1000, координаты < 10000.
*/

#include <iostream>
#include <cmath>

using namespace std;

float count3angleArea(int x1, int y1, int x2, int y2, int x3, int y3) {
    float a = static_cast<float>((x1-x3)*(y2-y3));
    float b = static_cast<float>((x2-x3)*(y1-y3));
    return abs(a - b) / 2;
}

int main() {
    float area = 0;
    int angleCount = 0,
        x1 = 0, y1 = 0,
        x2 = 0, y2 = 0,
        x3 = 0, y3 = 0;
    cin >> angleCount;
    cin >> x1 >> y1 >> x2 >> y2;

    // i == 2, потому что 2 точки мы уже считали
    for (int i = 2; i != angleCount; i++) {
        cin >> x3 >> y3;
        area += count3angleArea(x1, y1, x2, y2, x3, y3);
        x2 = x3;
        y2 = y3;
    }

    cout << area << endl;
    return 0;
}