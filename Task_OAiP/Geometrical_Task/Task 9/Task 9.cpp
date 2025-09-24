#include <iostream>
using namespace std;

int orientation(int px, int py, int qx, int qy, int rx, int ry) {
    int val = (qy - py) * (rx - qx) - (qx - px) * (ry - qy);
    if (val == 0) return 0;
    if (val > 0) return 1;
    else return 2;

}

bool onSegment(int px, int py, int qx, int qy, int rx, int ry) {
    return (qx <= max(px, rx) && qx >= min(px, rx) &&
        qy <= max(py, ry) && qy >= min(py, ry));
}

bool doIntersect(int p1x, int p1y, int q1x, int q1y, int p2x, int p2y, int q2x, int q2y) {
    int o1 = orientation(p1x, p1y, q1x, q1y, p2x, p2y);
    int o2 = orientation(p1x, p1y, q1x, q1y, q2x, q2y);
    int o3 = orientation(p2x, p2y, q2x, q2y, p1x, p1y);
    int o4 = orientation(p2x, p2y, q2x, q2y, q1x, q1y);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && onSegment(p1x, p1y, p2x, p2y, q1x, q1y)) return true;
    if (o2 == 0 && onSegment(p1x, p1y, q2x, q2y, q1x, q1y)) return true;
    if (o3 == 0 && onSegment(p2x, p2y, p1x, p1y, q2x, q2y)) return true;
    if (o4 == 0 && onSegment(p2x, p2y, q1x, q1y, q2x, q2y)) return true;

    return false;
}

bool pointInPolygon(int vec[][2], int n, int px, int py) {
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        int x1 = vec[i][0], y1 = vec[i][1];
        int x2 = vec[(i + 1) % n][0], y2 = vec[(i + 1) % n][1];
        if (((y1 > py) != (y2 > py)) &&
            (px < (x2 - x1) * (py - y1) / (double)(y2 - y1) + x1))
            cnt++;
    }
    return cnt % 2 == 1;
}

int main() {
    int n, m;
    cout << "Введите количество вершин первого многоугольника: ";
    cin >> n;
    int vec1[10000][2]; 
    cout << "Введите вершины первого многоугольника:" << endl;
    for (int i = 0; i < n; i++) cin >> vec1[i][0] >> vec1[i][1];

    cout << "Введите количество вершин второго многоугольника: ";
    cin >> m;
    int vec2[10000][2];  
    cout << "Введите вершины второго многоугольника:" << endl;
    for (int i = 0; i < m; i++) cin >> vec2[i][0] >> vec2[i][1];

    bool per = false;

    // проверка пересечения рёбер
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (doIntersect(vec1[i][0], vec1[i][1], vec1[(i + 1) % n][0], vec1[(i + 1) % n][1],
                vec2[j][0], vec2[j][1], vec2[(j + 1) % m][0], vec2[(j + 1) % m][1])) {
                per = true;
            }
        }
    }

    // если рёбра не пересекаются, то проверка на то что один многоугольник находится внутри
    if (!per) {
        if (pointInPolygon(vec1, n, vec2[0][0], vec2[0][1]) ||
            pointInPolygon(vec2, m, vec1[0][0], vec1[0][1])) {
            per = true;
        }
    }

    if (per) {
        cout << "Многоугольники пересекаются";
    }
    else {
        cout << "Многоугольники не пересекаются";
    }

}
