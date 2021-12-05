#include <iostream>
#include <algorithm>
#include <vector>

struct Point {
    long long x = 0;
    long long y = 0;
    long long len_square() const;
    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);
};

long long operator^(const Point& p1, const Point& p2) {
    return p1.x * p2.y - p1.y * p2.x;
}

long long Point::len_square() const {
    return x * x + y * y;
}

Point& Point::operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Point& Point::operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Point operator-(const Point& p1, const Point& p2) {
    Point res = p1;
    res -= p2;
    return res;
}

std::istream& operator>>(std::istream& in, Point& p) {
    in >> p.x >> p.y;
    return in;
}


bool comp(Point& p1, Point& p2) {
    long long orient = p1 ^ p2;
    return orient != 0? (orient > 0) : (p1.len_square() < p2.len_square());
}

void sort_by_angle(std::vector<Point>& points, Point start) {
    for(auto & point : points) {
        point -= start;
    }
    std::sort(points.begin(), points.end(), comp);
    for(auto& point: points) {
        point += start;
    }
}

std::vector<Point> convex_hull(std::vector<Point>& points, Point& start) {
    std::vector<Point> hull;
    sort_by_angle(points, start);
    Point a, b;
    long long orient;
    for(auto& point: points) {
        while(hull.size() > 2) {
            a = hull[hull.size() - 1];
            b = hull[hull.size() - 2];
            orient = (point - a)^(a - b);
            if(orient >= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        if(hull.size() == 2) {
            a = hull[1];
            b = hull[0];
            orient = (point - a)^(a - b);
            if(orient == 0)
                hull.pop_back();
        }
        hull.push_back(point);
    }
    return hull;
};


int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n;
    std::cin >> n;
    std::vector<Point> points(n);
    Point start;
    for(int i = 0; i < n; ++i) {
        std::cin >> points[i];
        if(i == 0 || points[i].x < start.x) {
            start = points[i];
        } else if(points[i].x == start.x && points[i].y < start.y) {
            start = points[i];
        }
    }
    std::vector<Point> ans = convex_hull(points, start);
    std::cout << ans.size() << "\n";
    for(auto& point: ans) {
        std::cout << point.x << " " << point.y << "\n";
    }
    return 0;
}
