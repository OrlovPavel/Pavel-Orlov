#include <iostream>
#include <cmath>
#include <vector>

struct Point {
    int x = 0;
    int y = 0;
    int len_square();
};

int operator^(const Point& p1, const Point& p2) {
    return p1.x * p2.y - p1.y * p2.x;
}

int Point::len_square() {
    return x * x + y * y;
}

Point operator-(const Point& p1, const Point& p2) {
    Point res = p1;
    res.x -= p2.x;
    res.y -= p2.y;
    return res;
}

std::istream& operator>>(std::istream& in, Point& p) {
    in >> p.x >> p.y;
    return in;
}

bool point_on_segment(const Point& p, const Point& a, const Point& b) {
    return ((p - a)^(p - b)) == 0 && p.x >= std::min(a.x, b.x) && p.x <= std::max(a.x, b.x)
        && p.y >= std::min(a.y, b.y) && p.y <= std::max(a.y, b.y);
}

bool contains_point(const std::vector<Point>& polygon, const Point& point) {
    Point a, b;
    int intersections = 0;
    for(int i = 0; i < polygon.size(); ++i) {
        a = polygon[i];
        b = polygon[(i + 1) % polygon.size()];
        if(point_on_segment(point, a, b)){
            return true;
        }
        if(a.y == b.y || point.y <= std::min(a.y, b.y) || point.y > std::max(a.y, b.y)) {
            continue;
        }
        if(a.y > b.y)
            std::swap(a, b);
        int orient = ((a - point)^(b - point));
        if(orient > 0) {
            ++intersections;
        }
    }
    return intersections % 2 == 1;
}

int main() {
    int n;
    std::cin >> n;
    Point p;
    std::cin >> p;
    std::vector<Point> polygon(n);
    for(int i = 0; i < n; ++i) {
        std::cin >> polygon[i];
    }
    if(contains_point(polygon, p)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}
