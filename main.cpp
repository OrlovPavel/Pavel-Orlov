#include <iostream>

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

Point operator+(const Point& p1, const Point& p2) {
    Point res = p1;
    res += p2;
    return res;
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

std::vector<Point> input_polygon() {
    int n;
    std::cin >> n;
    std::vector<Point> polygon(n);
    for(int i = 0; i < n; ++i) {
        std::cin >> polygon[i];
    }
    return polygon;
}

int find_rightest(const std::vector<Point>& poly) {
    int ans = 0;
    for(int i = 1; i < poly.size(); ++i) {
        if(poly[i].x < poly[ans].x) {
            ans = i;
        } else {
            if(poly[i].x == poly[ans].x && poly[i].y < poly[ans].y) {
                ans = i;
            }
        }
    }
    return ans;
}

std::vector<Point> Minkowski_add(const std::vector<Point>& poly1, const std::vector<Point>& poly2) {
    std::vector<Point> ans;
    int n = poly1.size();
    int m = poly2.size();
    int start1 = find_rightest(poly1);
    int start2 = find_rightest(poly2);
    Point v1, v2;
    for(int i = start1, j = start2; i < start1 + n || j < start2 + m;) {
        ans.push_back(poly1[i % n] + poly2[j % m]);
        v1 = poly1[(i + 1) % n] - poly1[i % n];
        v2 = poly2[(j + 1) % m] - poly2[j % m];
        long long cross = v1^v2;
        if(cross > 0) {
            ++i;
        } else {
            if (cross < 0) {
                ++j;
            } else {
                ++i; ++j;
            }
        }
    }
    return ans;
}

bool is_inside(const std::vector<Point>& poly, Point& vector) {
    if((vector^poly[1]) > 0 || (vector^poly[poly.size() - 1]) < 0)
        return false;
    int l = 0;
    int r = poly.size();
    while(l + 1 < r) {
        int mid = (l + r) / 2;
        if((vector^poly[mid]) <= 0) {
            l = mid;
        }
        else {
            r = mid;
        }
    }
    if(l == poly.size() - 1) {
        return poly[l].len_square() >= vector.len_square();
    }
    Point v1 = vector - poly[l];
    Point v2 = poly[r] - poly[l];
    return (v1^v2) <= 0;
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::vector<Point> poly1 = input_polygon();
    std::vector<Point> poly2 = input_polygon();
    std::vector<Point> poly3 = input_polygon();
    std::vector<Point> sum0 = Minkowski_add(poly1, poly2);
    std::vector<Point> sum = Minkowski_add(sum0, poly3);
    Point start = sum[0];
    for(int i = 0; i < sum.size(); ++i) {
        sum[i] -= start;
    }
    int q;
    Point point;
    std::cin >> q;
    for(int i = 0; i < q; ++i) {
        std::cin >> point;
        point.x *= 3; point.y *= 3;
        point -= start;
        if(is_inside(sum, point)) {
            std::cout << "YES\n";
        } else{
            std::cout << "NO\n";
        }
    }
    return 0;
}
