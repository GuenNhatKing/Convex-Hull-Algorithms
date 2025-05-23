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
void Merge(Point points[], int l, int mid, int r) {
    Point result[r - l + 1];
    int k = 0, i = l, j = mid + 1;
    while(i <= mid && j <= r) {
        bool cmp;
        if(points[i].x == points[j].x) cmp = points[i].y < points[j].y;
        else cmp = points[i].x < points[j].x;
        if(cmp) {
            result[k++] = points[i++];
        }
        else {
            result[k++] = points[j++];
        }
    }
    while(i <= mid) {
        result[k++] = points[i++];
    }
    while(j <= r) {
        result[k++] = points[j++];
    }
    i = 0;
    while(i < k) {
        points[i + l] = result[i];
        ++i;
    }
}

void MergeSort(Point points[], int l, int r) {
    if(l < r) {
        int mid = (l + r) / 2;
        MergeSort(points, l, mid);
        MergeSort(points, mid + 1, r);
        Merge(points, l, mid, r);
    }
}
// ---- ---- ---- ---- ---- ---- ---- ----
void RemoveMidPoints(List<Point> &convexHull) {
    for(int i = 0; i < convexHull.Size(); ++i) {
        int n = convexHull.Size();
        int prev = ((i - 1) % n + n) % n;
        int next = ((i + 1) % n + n) % n;
        if(Orientation(convexHull[prev], convexHull[i], convexHull[next]) == 0) {
            convexHull.RemoveAt(i);
            --i;
        }
    }
}
// ---- ---- ---- ---- ---- ---- ---- ----
List<Point> MonotoneChain(Point points[], int n) {
    List<Point> convexHull;
    if(n < 3) return convexHull;
    MergeSort(points, 0, n - 1);
    convexHull.PushBack(points[0]);
    convexHull.PushBack(points[1]);
    for(int i = 2; i < n; ++i) {
        convexHull.PushBack(points[i]);
        while(convexHull.Size() > 2 && Orientation(convexHull[convexHull.Size() - 3], convexHull[convexHull.Size() - 2], convexHull[convexHull.Size() - 1]) >= 0) {
            convexHull.RemoveAt(convexHull.Size() - 2);
        }
    }
    int t = convexHull.Size() - 1;
    convexHull.PushBack(points[n - 2]);
    for(int i = n - 3; i >= 0; --i) {
        convexHull.PushBack(points[i]);
        while(convexHull.Size() > t + 2 && Orientation(convexHull[convexHull.Size() - 3], convexHull[convexHull.Size() - 2], convexHull[convexHull.Size() - 1]) >= 0) {
            convexHull.RemoveAt(convexHull.Size() - 2);
        }
    }
    convexHull.RemoveAt(convexHull.Size() - 1);
    RemoveMidPoints(convexHull);
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
    // Time complexity: O(nlogn)
    // Point points[] = {-5, 2, -1, 3, -1, 8, 9, 5, 6, 2, 9, 4, 12, 1, 9, -5, 5, -8, 5, -10, 1, -9, 2, -3, -1, -7, -6, -8, -7, -4};
    // Point points[] = {1,25,14,1,25,0,17,12,18,6,14,7,3,9,23,0,8,9,6,16,21,19,2,16,17,12,14,12,11,18,18,12,14,22,9,2,4,7,4,13,21,5,1,7,9,1,7,12,2,21,24,6,16,21,5,19,18,11,20,18};
    Point points[] = {0,45,7,9,43,5,38,44,31,33,12,12,38,24,30,26,7,34,48,36,22,11,28,39,46,2,8,18,25,48,44,5,2,21,4,44,47,28,6,40,23,45,13,34,28,37,44,25,46,20,49,29,50,36,15,15,50,0,16,23,22,44,49,39,43,44,38,20,46,34,29,30,37,29,8,30,9,22,31,39,18,19,47,14,6,48,23,31,33,4,32,5,3,23,11,12,3,7,41,38,47,11,20,12,27,43,39,50,24,24,12,4,30,19,19,1,19,34,30,8};
    int n = sizeof(points)/sizeof(points[0]);
    List<Point> convexHull = MonotoneChain(points, n);
    Print(convexHull);
    return 0;
}