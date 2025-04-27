#include <bits/stdc++.h>
using namespace std;
// ---- ---- ---- ---- ---- ---- ---- ----
template <typename T>
class List {
    T *data = nullptr;
    int size = 0;
public:
    List &operator=(const List &other) {
        size = other.size;
        T *newData = new T[size];
        for(int i = 0; i < size; ++i) {
            newData[i] = other.data[i];
        }
        delete[] data;
        data = newData;
        return *this;
    }
    void PushBack(T obj) {
        T *newData = new T[size + 1];
        for(int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        newData[size++] = obj;
        delete[] data;
        data = newData;
    }
    T& operator[](int index) {
        if(index < 0 || index >= size)
            throw out_of_range("Index out of bounds");
        return data[index];
    }
    void RemoveAt(int index) {
        T *newData = new T[size - 1];
        for(int i = 0; i < index; ++i) {
            newData[i] = data[i];
        }
        for(int i = index; i < size - 1; ++i) {
            newData[i] = data[i + 1];
        }
        delete[] data;
        data = newData;
        --size;
    }
    int Size() {
        return size;
    }
    ~List() {
        delete[] data;
    }
};
// ---- ---- ---- ---- ---- ---- ---- ----
class Point {
    public:
        double x, y;
    };
    typedef Point Vector;
// ---- ---- ---- ---- ---- ---- ---- ----
Vector Minus(Vector A, Vector B) {
    return Vector {A.x - B.x, A.y - B.y};
}

int Orientation(Point A, Point B, Point C) {
    Vector AB = Minus(B, A);
    Vector BC = Minus(C, B);
    double crossProduct = AB.x * BC.y - BC.x * AB.y;
    if(crossProduct == 0) return 0;
    else if(crossProduct < 0) return -1; // Clock wise
    else return 1;  // CounterClock wise
}
// ---- ---- ---- ---- ---- ---- ---- ----
int MinXPosition(Point points[], int n) {
    int res = 0;
    for(int i = 1; i < n; ++i) {
        if(points[i].x < points[res].x) {
            res = i;
        }
    }
    return res;
}
// ---- ---- ---- ---- ---- ---- ---- ----
List<Point> JarvisMarch(Point points[], int n) {
    List<Point> convexHull;
    if (n < 3) return convexHull;
    int first = MinXPosition(points, n);
    int inHull = first;
    int next = (inHull + 1) % n;
    while(next != first) {
        convexHull.PushBack(points[inHull]);
        next = (inHull + 1) % n;
        for(int i = 0; i < n; ++i) {
            if(Orientation(points[inHull], points[i], points[next]) < 0) {
                next = i;
            }
        }
        inHull = next;
    }
    return convexHull;
}
// ---- ---- ---- ---- ---- ---- ---- ----
void Print(List<Point> &list) {
    for(int i = 0; i < list.Size(); ++i) {
        cout << list[i].x << ", " << list[i].y;
        if(i != list.Size() - 1) cout << ", ";
        else cout << endl;
    }
}
// ---- ---- ---- ---- ---- ---- ---- ----
int main() {
    // Time complexity: O(m * n), n is number of points, m is number of hull points
    // Worst case: O(n ^ 2)
    // Point points[] = {-5, 2, -1, 3, -1, 8, 9, 5, 6, 2, 9, 4, 12, 1, 9, -5, 5, -8, 5, -10, 1, -9, 2, -3, -1, -7, -6, -8, -7, -4};
    // Point points[] = {1,25,14,1,25,0,17,12,18,6,14,7,3,9,23,0,8,9,6,16,21,19,2,16,17,12,14,12,11,18,18,12,14,22,9,2,4,7,4,13,21,5,1,7,9,1,7,12,2,21,24,6,16,21,5,19,18,11,20,18};
    Point points[] = {0,45,7,9,43,5,38,44,31,33,12,12,38,24,30,26,7,34,48,36,22,11,28,39,46,2,8,18,25,48,44,5,2,21,4,44,47,28,6,40,23,45,13,34,28,37,44,25,46,20,49,29,50,36,15,15,50,0,16,23,22,44,49,39,43,44,38,20,46,34,29,30,37,29,8,30,9,22,31,39,18,19,47,14,6,48,23,31,33,4,32,5,3,23,11,12,3,7,41,38,47,11,20,12,27,43,39,50,24,24,12,4,30,19,19,1,19,34,30,8};
    int n = sizeof(points)/sizeof(points[0]);
    List<Point> convexHull = JarvisMarch(points, n);
    Print(convexHull);
    return 0;
}