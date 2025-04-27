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
double DotProduct(Vector A, Vector B) {
    return A.x * B.x + A.y * B.y;
}
double RadianDegree(Vector A, Vector B) {
    double aLength = sqrt(DotProduct(A, A));
    double bLength = sqrt(DotProduct(B, B));
    return acos(DotProduct(A, B) / (aLength * bLength));
}

template <typename T>
void Swap(T &A, T &B) {
    T C = A;
    A = B;
    B = C;
}
// ---- ---- ---- ---- ---- ---- ---- ----
const Vector xAxis {1};
// cmpType: 0
bool CompareXAxis(Point A, Point B) {
    if(A.x == B.x) return A.y < B.y;
    return A.x < B.x;
}
// cmpType: 1
bool CompareRadian(Point A, Point B, Point O) {
    double ARad = RadianDegree(Minus(A, O), xAxis);
    double BRad = RadianDegree(Minus(B, O), xAxis);
    return ARad < BRad;
}
void Merge(List<Point> &points, int l, int mid, int r, int cmpType = 0) {
    Point result[r - l + 1];
    int k = 0, i = l, j = mid + 1;
    while(i <= mid && j <= r) {
        if ((cmpType == 0 && CompareXAxis(points[i], points[j])) 
            || (cmpType == 1 && CompareRadian(points[i], points[j], points[0]))) {
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

void MergeSort(List<Point> &points, int l, int r, int cmpType = 0) {
    if(l < r) {
        int mid = (l + r) / 2;
        MergeSort(points, l, mid, cmpType);
        MergeSort(points, mid + 1, r, cmpType);
        Merge(points, l, mid, r, cmpType);
    }
}
// ---- ---- ---- ---- ---- ---- ---- ----
List<Point> Conquer(List<Point> &points) {
    List<Point> result;
    for(int i = 0; i < points.Size(); ++i) {
        for(int j = i + 1; j < points.Size(); ++j) {
            int leftSide = 0, rightSide = 0;
            for(int k = 0; k < points.Size(); ++k) {
                int direc = Orientation(points[i], points[j], points[k]);
                if(direc < 0) {
                    ++rightSide;
                }
                else if(direc > 0) {
                    ++leftSide;
                }
            }
            if(leftSide == 0 || rightSide == 0) {
                bool isExist = false;
                for(int t = 0; t < result.Size(); ++t) {
                    if(result[t].x == points[i].x && result[t].y == points[i].y) {
                        isExist = true;
                        break;
                    }
                }
                if(!isExist) result.PushBack(points[i]);
                isExist = false;
                for(int t = 0; t < result.Size(); ++t) {
                    if(result[t].x == points[j].x && result[t].y == points[j].y) {
                        isExist = true;
                        break;
                    }
                }
                if(!isExist) result.PushBack(points[j]);
            }
        }
    }
    int minPos = 0;
    for(int i = 1; i < result.Size(); ++i) {
        if(result[i].y < result[minPos].y) {
            minPos = i;
        }
    }
    Swap(result[0], result[minPos]);
    MergeSort(result, 1, result.Size() - 1, 1);
    return result;
}
List<Point> MergeDivide(List<Point> &leftPoints, List<Point> &rightPoints) {
    int n1 = leftPoints.Size(), n2 = rightPoints.Size();
    int leftMost = 0, rightMost = 0;
    for(int i = 1; i < leftPoints.Size(); ++i) {
        if(leftPoints[i].x > leftPoints[rightMost].x) {
            rightMost = i;
        }
    }
    for(int i = 1; i < rightPoints.Size(); ++i) {
        if(rightPoints[i].x < rightPoints[leftMost].x) {
            leftMost = i;
        }
    }
    int upperLeft, upperRight, lowerLeft, lowerRight;
    int p1 = rightMost, p2 = leftMost;
    int next1, next2;
    bool done = false;
    while(!done) {
        done = true;
        next1 = (p1 + 1) % n1;
        next2 = (((p2 - 1) % n2) + n2) % n2;
        while(Orientation(rightPoints[p2], leftPoints[p1], leftPoints[next1]) <= 0) {
            p1 = next1;
            next1 = (p1 + 1) % n1;
            done = false;
        }
        while(Orientation(leftPoints[p1], rightPoints[p2], rightPoints[next2]) >= 0) {
            p2 = next2;
            next2 = (((p2 - 1) % n2) + n2) % n2;
            done = false;
        }
    }
    upperLeft = p1; upperRight = p2;
    p1 = rightMost, p2 = leftMost;
    done = false;
    while(!done) {
        done = true;
        next1 = (((p1 - 1) % n1) + n1) % n1;
        next2 = (p2 + 1) % n2;
        while(Orientation(rightPoints[p2], leftPoints[p1], leftPoints[next1]) >= 0) {
            p1 = next1;
            next1 = (((p1 - 1) % n1) + n1) % n1;
            done = false;
        }
        while(Orientation(leftPoints[p1], rightPoints[p2], rightPoints[next2]) <= 0) {
            p2 = next2;
            next2 = (p2 + 1) % n2;
            done = false;
        }
    }
    lowerLeft = p1; lowerRight = p2;
    List<Point> result;
    int i = upperLeft;
    result.PushBack(leftPoints[upperLeft]);
    while(i != lowerLeft) {
        i = (i + 1) % n1;
        result.PushBack(leftPoints[i]);
    }
    i = lowerRight;
    result.PushBack(rightPoints[lowerRight]);
    while(i != upperRight) {
        i = (i + 1) % n2;
        result.PushBack(rightPoints[i]);
    }
    return result;
}
List<Point> Divide(List<Point> &points) {
    int n = points.Size();
    if(n < 6) {
        return Conquer(points);
    }
    else {
        List<Point> leftPoints, rightPoints;
        for(int i = 0; i < n / 2; ++i) {
            leftPoints.PushBack(points[i]);
        }
        for(int i = n / 2; i < n; ++i) {
            rightPoints.PushBack(points[i]);
        }
        leftPoints = Divide(leftPoints);
        rightPoints = Divide(rightPoints);
        return MergeDivide(leftPoints, rightPoints);
    }
}
List<Point> DivideAndConquer(Point points[], int n) {
    List<Point> convexHull;
    if(n < 3) return convexHull;
    for(int i = 0; i < n; ++i) {
        convexHull.PushBack(points[i]);
    }
    MergeSort(convexHull, 0, convexHull.Size() - 1, 0);
    convexHull = Divide(convexHull);
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
    List<Point> convexHull = DivideAndConquer(points, n);
    Print(convexHull);
    return 0;
}