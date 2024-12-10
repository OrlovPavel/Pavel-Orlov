#include <iostream>
#include <cmath>
#include <vector>
#include <set>

#define E 0.000001
#define Pi 3.14159265

bool isEqual(double a, double b){
    return fabs(a - b) < E;
}
bool isLess(double a, double b){
    if(!isEqual(a, b)){
        if(a < b)
            return true;
        else
            return false;
    }
    return false;
}

struct Point {
    double x = 0;
    double y = 0;
    Point() = default;
    Point(double x, double y = 0) : x(x), y(y) {}
    bool operator==(const Point&) const;
    bool operator!=(const Point&) const;

    double getLength() const;
    Point& operator+=(const Point&);
    Point& operator*=(double);
    Point operator-() const;
    Point& operator-=(const Point&);
    void rotate(double rad);
};

double Point::getLength() const{
    return sqrt(x * x + y * y);
}

bool Point::operator==(const Point & p) const {
    return isEqual(p.x, x) && isEqual(p.y, y);
}

bool Point::operator!=(const Point & p) const {
    return !(*this == p);
}

Point& Point::operator+=(const Point& p){
    x += p.x;
    y += p.y;
    return *this;
}

Point Point::operator-() const{
    Point copy = *this;
    copy.x = -x;
    copy.y = -y;
    return copy;
}

Point& Point::operator-=(const Point& p){
    *this += -p;
    return *this;
}

Point& Point::operator*=(double a){
    x *= a;
    y *= a;
    return *this;
}

Point operator*(double a, const Point& p){
    Point copy = p;
    copy *= a;
    return copy;
}

Point operator+(const Point& p1, const Point& p2){
    Point copy = p1;
    copy += p2;
    return copy;
}

Point operator-(const Point& p1, const Point& p2){
    Point copy = p1;
    copy -= p2;
    return copy;
}

double operator*(const Point& p1, const Point& p2){ //скалярное
    return p1.x * p2.x + p1.y * p2.y;
}

double dist(const Point& p1, const Point& p2){
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

void Point::rotate(double rad){
    double x1 = cos(rad) * x - sin(rad) * y;
    double y1 = sin(rad) * x + cos(rad) * y;
    x = x1;
    y = y1;
}

double getCos(const Point& v1, const Point& v2){
    return (v1 * v2) / (v1.getLength() * v2.getLength());
}

double getSin(const Point& v1, const Point& v2){
    return (v1.x * v2.y - v1.y * v2.x) / (v1.getLength() * v2.getLength());
}

//******************************************************************************************************************

class Line{ // ax+by+c=0
private:
    double a = 0;
    double b = 0;
    double c = 0;
public:
    Line() = default;
    Line(const Point&, const Point&);
    Line(const Point&, double);
    bool operator==(const Line&) const;
    bool operator!=(const Line&) const;

    int getSide(const Point&) const;
    bool isParallel(const Line&) const;
    Point intersect(const Line&) const;
    Point getNormal() const;
    double dist(const Point&) const;
};

Line::Line(const Point& p1, const Point& p2){  //(y1 - y2)x + (x2 - x1)y + x1y2 - y1x2 = 0
    a = p1.y - p2.y;
    b = p2.x - p1.x;
    c = p1.x * p2.y - p1.y * p2.x;
}

Line::Line(const Point& p, double k){ // y = kx + c, k = a, b = -1
    b = -1;
    a = k;
    c = p.y - k * p.x;
}

bool Line::operator==(const Line& l) const{ // a1/a2=b1/b2=c1/c2 вырожденный сулчай 0=0 отдельно надо бы
    // Не в это семестре, но такое должно проверяться в конструкторе а не здесь
    return isEqual(a * l.b, l.a * b) && isEqual(b * l.c, l.b * c) && isEqual(a * l.c, l.a * c);
}

bool Line::operator!=(const Line& l) const{
    return !(*this == l);
}

int Line::getSide(const Point& p) const{ // возвращает полуплоскость, в которую попадает точка, 0 если на прямой
    int res = 0;
    if(isLess(0, a * p.x + b * p.y + c))
        res = 1;
    if(isLess(a * p.x + b * p.y + c, 0))
        res = -1;
    return res;
}

bool Line::isParallel(const Line& l) const{
    return isEqual(a * l.b, b * l.a);
}

Point Line::intersect(const Line& l) const{
    Point res;
    double d = a * l.b - b * l.a;
    res.x = (-c * l.b + b * l.c) / d;
    res.y = (-a * l.c + c * l.a) / d;
    return res;
}

Point Line::getNormal() const{
    Point res;
    res.x = a;
    res.y = b;
    return (1/res.getLength()) * res;
}

double Line::dist(const Point& p) const{
    return fabs(a * p.x + b * p.y + c)/sqrt(a * a + b * b);
}


//****************************************************************************************************************

class Shape{
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool operator==(const Shape&) const = 0;
    virtual bool operator!=(const Shape&) const = 0;
    virtual bool isCongruentTo(const Shape&) const = 0;
    virtual bool isSimilarTo(const Shape&) const = 0;
    virtual bool containsPoint(const Point&) const = 0;

    virtual void rotate(const Point&, double) = 0;
    virtual void reflex(const Point&) = 0;
    virtual void reflex(const Line&) = 0;
    virtual void scale(const Point&, double) = 0;
    virtual ~Shape() = 0;
};

Shape::~Shape(){}


//******************************************************************************************************************


class Polygon: public Shape{
protected:
    std::vector<Point> points;
    void setPoints(const std::vector<Point>&, bool);
public:
    Polygon() = default;
    Polygon(const std::vector<Point>&);
    Polygon(std::initializer_list<Point>);
    size_t verticesCount() const;
    std::vector<Point> getVertices();
    bool isConvex() const;

    double perimeter() const override;
    double area() const override;
    bool operator==(const Shape&) const override;
    bool operator!=(const Shape&) const override;
    bool isCongruentTo(const Shape&) const override;
    bool isSimilarTo(const Shape&) const override;
    bool compare(const Polygon*, bool) const;
    bool containsPoint(const Point&) const override;

    void rotate(const Point&, double) override;
    void reflex(const Point&) override;
    void reflex(const Line&) override;
    void scale(const Point&, double) override;
};

void Polygon::setPoints(const std::vector<Point>& list, bool initialization = false){
    size_t min_index = 0;
    for(size_t i = 1; i < list.size(); ++i) {
        if (isLess(list[i].x, list[min_index].x))
            min_index = i;
        else if(isEqual(list[i].x, list[min_index].x) && isLess(list[i].y, list[min_index].y))
            min_index = i;
    }
    for(size_t i = min_index; i < list.size() + min_index; ++i) { // циклическим сдвигом делаем точку с минимальным x y первой
        if(initialization)
            points.push_back(list[i % list.size()]);
        else
            points[i - min_index] = list[i % list.size()];
    }
    if(isLess(points[1].y, points[points.size() - 1].y)){ //обходим по часовой
        for(size_t i = 1; i < points.size() / 2 + 1; ++i)
            std::swap(points[i], points[points.size() - i]);
    }
}

Polygon::Polygon(const std::vector<Point>& list){
    setPoints(list, true);
}

Polygon::Polygon(const std::initializer_list<Point> list){
    std::vector<Point> list1(list);
    setPoints(list1, true);
}

size_t Polygon::verticesCount() const{
    return points.size();
}

std::vector<Point> Polygon::getVertices(){
    return points;
}

bool Polygon::isConvex() const{
    for(size_t i = 0; i < points.size(); ++i){
        Line l(points[i], points[(i + 1) % points.size()]); //проверка по определению
        int f = l.getSide(points[(i + 2) % points.size()]);
        for(size_t j = 0; j < points.size(); ++j){
            if(j != i && j != (i + 1) % points.size() && l.getSide(points[j]) != f) {
                return false;
                //std::cout << "f(" << i << "," << (i + 1) % points.size() << ")\n";
            }
        }
    }
    return true;
}

double Polygon::perimeter() const {
    double ans = 0;
    for(size_t i = 0; i < points.size(); ++i){
        ans += dist(points[i], points[(i + 1) % points.size()]);
    }
    return ans;
}

double Polygon::area() const {
    double ans = 0;
    for(size_t i = 0; i < points.size(); ++i){
        ans += points[i].x * points[(i + 1) % points.size()].y;
        ans -= points[(i + 1) % points.size()].x * points[i].y;
    }
    return 0.5 * fabs(ans);
}

bool Polygon::operator==(const Shape& shape) const{
    const Shape *s = &shape;
    const Polygon* another = dynamic_cast<const Polygon*>(s);
    if(!another)
        return false;
    if(points.size() != another->points.size() || points[0] != another->points[0])
        return false;
    for(size_t i = 1; i < points.size(); ++i){
        //std::cout << "(" << points[i].x << ", " << points[i].y << ") ";
        //std::cout << "(" << another->points[i].x << ", " << another->points[i].y << ")\n";
        if(points[i] != another->points[i]) {
            return false;
        }
    }
    return true;
}

bool Polygon::operator!=(const Shape& shape) const {
    return !(*this==shape);
}

bool Polygon::compare(const Polygon* another, bool exact) const {
    if(points.size() != another->points.size())
        return false;
    std::multiset<double> cos1; std::multiset<double> cos2;
    std::multiset<double> sin1; std::multiset<double> sin2;
    std::multiset<double> sides1; std::multiset<double> sides2;

    sides1.insert(dist(points[0], points[1]));
    sides2.insert(dist(another->points[0], another->points[1]));
    cos1.insert(getCos(points[points.size() - 1] - points[0], points[1] - points[0]));
    cos2.insert(getCos(another->points[points.size() - 1] - another->points[0], another->points[1] - another->points[0]));
    sin1.insert(getSin(points[points.size() - 1] - points[0], points[1] - points[0]));
    sin2.insert(getSin(another->points[points.size() - 1] - another->points[0], another->points[1] - another->points[0]));
    for(size_t i = 1; i < points.size(); ++i){
        sides1.insert(dist(points[i], points[(i + 1) % points.size()]));
        sides2.insert(dist(another->points[i], another->points[(i + 1) % points.size()]));
        cos1.insert(getCos(points[i - 1] - points[i], points[(i + 1) % points.size()] - points[i]));
        cos2.insert(getCos(another->points[i - 1] - another->points[i], another->points[(i + 1) % points.size()] - another->points[i]));
        sin1.insert(getSin(points[i - 1] - points[i], points[(i + 1) % points.size()] - points[i]));
        sin2.insert(getSin(another->points[i - 1] - another->points[i], another->points[(i + 1) % points.size()] - another->points[i]));
    }
    for(auto it1 = cos1.begin(), it2 = cos2.begin(); it1 != cos1.end(); ++it1, ++it2){
        if(!isEqual(*it1, *it2))
            return false;
    }
    for(auto it1 = sin1.begin(), it2 = sin2.begin(); it1 != sin1.end(); ++it1, ++it2){
        if(!isEqual(*it1, *it2))
            return false;
    }
    double coef = 1;
    if(!exact)
        coef = *sides1.begin() / *sides2.begin();
    for(auto it1 = sides1.begin(), it2 = sides2.begin(); it1 != sides1.begin(); ++it1, ++it2){
        if(!isEqual(coef, *it1 / *it2))
            return false;
    }
    return true;
}

bool Polygon::isCongruentTo(const Shape& shape) const{
    const Shape *s = &shape;
    const Polygon* another = dynamic_cast<const Polygon*>(s);
    if(!another)
        return false;
    return compare(another, true);
}

bool Polygon::isSimilarTo(const Shape& shape) const{
    const Shape *s = &shape;
    const Polygon* another = dynamic_cast<const Polygon*>(s);
    if(!another)
        return false;
    return compare(another, false);
}


bool Polygon::containsPoint(const Point& point) const{ //луч параллельный оси ox вправо, point.x - левая граница
    Line ray(point, 0); Point a, b, c;
    for(size_t i = 0; i < points.size(); ++i){
        a = points[i];
        b = points[(i + 1) % points.size()];
        Line l(a, b);
        if(l.getSide(point) == 0)
            if (isEqual(fabs(point.x - a.x) + fabs(point.x - b.x), fabs(a.x - b.x)) &&
                isEqual(fabs(point.y - a.y) + fabs(point.y - b.y), fabs(a.y - b.y)))
                return true;
    }
    int intersection_count = 0;
    for(size_t i = 0; i < points.size(); ++i){
        a = points[i];
        b = points[(i + 1) % points.size()]; // a---c---b
        Line l(a, b);
        if(!ray.isParallel(l)) {
            c = ray.intersect(l);
            if (isLess(point.x, c.x))
                if (isEqual(fabs(c.x - a.x) + fabs(c.x - b.x), fabs(a.x - b.x)) &&
                    isEqual(fabs(c.y - a.y) + fabs(c.y - b.y), fabs(a.y - b.y)) && c != a)
                    intersection_count++;
        }
    }
    return intersection_count % 2 == 1;
}

void Polygon::rotate(const Point& center, double deg){
    double rad = (deg * Pi) / 180;
    for(size_t i = 0; i < points.size(); ++i){
        points[i] -= center;
        points[i].rotate(rad);
        points[i] += center;
    }
    std::vector<Point> copy = points;
    setPoints(copy);
}

void Polygon::reflex(const Point& center){
    for(size_t i = 0; i < points.size(); ++i){
        points[i] = 2 * center - points[i];
    }
    std::vector<Point> copy = points;
    setPoints(copy);
}

void Polygon::reflex(const Line& line) {
    Point n = line.getNormal();
    double h;
    for(size_t i = 0; i < points.size(); ++i){
        h = line.dist(points[i]);
        if(line.getSide(points[i] + 2 * h * n) != line.getSide(points[i]))
            points[i] = points[i] + 2 * h * n;
        else
            points[i] = points[i] - 2 * h * n;
    }
    std::vector<Point> copy = points;
    setPoints(copy);
}

void Polygon::scale(const Point& center, double coef){
    for(size_t i = 0; i < points.size(); ++i){
        points[i] -= center;
        points[i] *= coef;
        points[i] += center;
    }
    std::vector<Point> copy = points;
    setPoints(copy);
}

//****************************************************************************************************************

class Rectangle: public Polygon{
public:
    Rectangle() = default;
    Rectangle(const Point&, const Point&, double);
    Point center() const;
    std::pair<Line, Line> diagonals() const;
};

Rectangle::Rectangle(const Point& p1, const Point& p2, double coef){
    double tg = coef;
    if(tg < 1)
        tg = 1 / tg;
    std::vector<Point> v(4); v[0] = p1; v[2] = p2;
    Point center = 0.5 * (v[0] + v[2]);
    v[2] -= center;
    v[2].rotate(2 * atan(tg));
    v[1] = center + v[2]; v[3] = center - v[2];
    v[2] = p2;
    points = v;
    setPoints(v);
}

Point Rectangle::center() const{
    return 0.5 * (points[0] + points[2]);
}

std::pair<Line, Line> Rectangle::diagonals() const{
    std::pair<Line, Line> res;
    Line l1(points[0], points[1]);
    Line l2(points[1], points[3]);
    res.first = l1; res.second = l2;
    return res;
}

//****************************************************************************************************************
class Ellipse: public Shape{
protected:
    double a;
    double b;// Хоть опиши кто это
    double c;
    Point F1;
    Point F2;
    void setOrder();
public:
    Ellipse() = default;
    Ellipse(const Point&, const Point&, double);
    std::pair<Point,Point> focuses() const;
    std::pair<Line, Line> directrices() const;
    double eccentricity() const;
    Point center() const;

    double perimeter() const override;
    double area() const override;
    bool operator==(const Shape&) const override;
    bool operator!=(const Shape&) const override;
    bool isCongruentTo(const Shape&) const override;
    bool isSimilarTo(const Shape&) const override;
    bool containsPoint(const Point&) const override;

    void rotate(const Point&, double) override;
    void reflex(const Point&) override;
    void reflex(const Line&) override;
    void scale(const Point&, double) override;
};

void Ellipse::setOrder() {
    if(isLess(F2.x, F1.x))
        std::swap(F1, F2);
    else
    if(isEqual(F1.x, F2.x) && isLess(F2.y, F1.y))
        std::swap(F1, F2);
}

Ellipse::Ellipse(const Point& f1, const Point& f2, double sum) {
    F1 = f1;
    F2 = f2;
    setOrder();
    a = sum / 2;
    c = dist(F1, F2) / 2;
    b = sqrt(a * a - c * c);
}

std::pair<Point,Point> Ellipse::focuses() const {
    std::pair<Point, Point> res;
    res.first = F1;
    res.second = F2;
    return res;
}

std::pair<Line, Line> Ellipse::directrices() const {
    Line o(F1, F2);
    Point n = o.getNormal();
    Line d1(((a * a) / (c * c)) * (F1 - center()), ((a * a) / (c * c)) * (F1 - center()) + n);
    Line d2(((a * a) / (c * c)) * (F2 - center()), ((a * a) / (c * c)) * (F2 - center()) + n);
    std::pair<Line, Line> res(d1,d2);
    return res;
}

double Ellipse::eccentricity() const {
    return c / a;
}

Point Ellipse::center() const {
    return 0.5 * (F1 + F2);
}

double Ellipse::perimeter() const {
    return Pi * (3 * (a + b) - sqrt((3 * a + b) * (a + 3 * b)));
}

double Ellipse::area() const {
    return Pi * a * b;
}

bool Ellipse::operator==(const Shape& shape) const {
    const Shape *s = &shape;
    const Ellipse* another = dynamic_cast<const Ellipse*>(s);
    if(!another)
        return false;
    if(F1 == another->F1 && F2 == another->F2 && isEqual(a, another->a))
        return true;
    return false;
}

bool Ellipse::operator!=(const Shape& shape) const {
    return !(*this == shape);
}

bool Ellipse::isCongruentTo(const Shape& shape) const {
    const Shape *s = &shape;
    const Ellipse* another = dynamic_cast<const Ellipse*>(s);
    if(!another)
        return false;
    return isEqual(a, another->a) && isEqual(c, another->c);
}

bool Ellipse::isSimilarTo(const Shape& shape) const {
    const Shape *s = &shape;
    const Ellipse* another = dynamic_cast<const Ellipse*>(s);
    if(!another)
        return false;
    return isEqual(eccentricity(), another->eccentricity());
}

bool Ellipse::containsPoint(const Point& p) const {
    return !isLess(2 * a, dist(F1, p) + dist(F2, p));
}

void Ellipse::rotate(const Point& center, double deg) {
    double rad = (deg * Pi) / 180;
    F1 -= center;   F2 -= center;
    F1.rotate(rad); F2.rotate(rad);
    F1 += center;   F2 += center;
    setOrder();
}

void Ellipse::reflex(const Point& center) {
    F1 = 2 * center - F1;
    F2 = 2 * center - F2;
    setOrder();
}

void Ellipse::reflex(const Line& line) {
    Point n = line.getNormal();
    double h = line.dist(F1);
    if(line.getSide(F1 + 2 * h * n) != line.getSide(F1))
        F1 = F1 + 2 * h * n;
    else
        F1 = F1 - 2 * h * n;
    h = line.dist(F2);
    if(line.getSide(F2 + 2 * h * n) != line.getSide(F2))
        F2 = F2 + 2 * h * n;
    else
        F2 = F2 - 2 * h * n;
    setOrder();
}

void Ellipse::scale(const Point& center, double coef) {
    F1 -= center; F2-= center;
    F1 *= coef; F2 *= coef;
    F1 += center; F1 += center;
    a *= coef;
    c *= coef;
    b *= coef;
    setOrder();
}

//****************************************************************************************************************

class Circle: public Ellipse{
public:
    Circle() = default;
    Circle(const Point&, double);
    double radius() const;

    double perimeter() const override;
};

Circle::Circle(const Point& center, double r) : Ellipse(center, center, 2 * r){
}

double Circle::radius() const{
    return a;
}
double Circle::perimeter() const {
    return 2 * Pi * a;
}

//****************************************************************************************************************

class Square: public Rectangle{
public:
    Square() = default;
    Square(const Point&, const Point&);

    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
};

Square::Square(const Point& p1, const Point& p2) : Rectangle(p1, p2, 1){
}

Circle Square::circumscribedCircle() const {
    Circle res(center(), dist(points[0], center()));
    return res;
}

Circle Square::inscribedCircle() const {
    Line l(points[0], points[1]);
    Circle res(center(), l.dist(center()));
    return res;
}

//*****************************************************************************************************************

class Triangle: public Polygon{
public:
    Triangle() = default;
    Triangle(std::initializer_list<Point>);
    Triangle(const Point&, const Point&, const Point&);

    Point centroid() const;
    Point orthocenter() const;
    Point bisectorCenter() const;
    Point circumscribedCenter() const;
    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
    Line EulerLine() const;
    Circle ninePointsCircle() const;
};

Triangle::Triangle(std::initializer_list<Point> list) : Polygon(list) {}

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3) {
    points.push_back(p1); points.push_back(p2); points.push_back(p3);
    std::vector<Point> copy = points;
    setPoints(copy);
}

Point Triangle::centroid() const {
    Line l1(points[0], 0.5 * (points[1] + points[2]));
    Line l2(points[1], 0.5 * (points[0] + points[2]));
    return l1.intersect(l2);
}

Point Triangle::orthocenter() const {
    Line a(points[0], points[1]);
    Line b(points[0], points[2]);
    Line l1(points[2], a.getNormal() + points[2]);
    Line l2(points[1], b.getNormal() + points[1]);
    return l1.intersect(l2);
}

Point Triangle::bisectorCenter() const {
    double coef = dist(points[0], points[1]) / (dist(points[0], points[1]) + dist(points[0], points[2]));
    Point p = points[1] + coef * (points[2] - points[1]);
    Line l1(points[0], p);
    coef = dist(points[0], points[1]) / (dist(points[0], points[1]) + dist(points[1], points[2]));
    p = points[0] + coef * (points[2] - points[0]);
    Line l2(points[1], p);
    return l1.intersect(l2);
}

Point Triangle::circumscribedCenter() const {
    Line a(points[0], points[1]);
    Line b(points[0], points[2]);
    Point center1 = 0.5 * (points[0] + points[1]); Point center2 = 0.5 * (points[0] + points[2]);
    Line l1(center1, center1 + a.getNormal());
    Line l2(center2, center2 + b.getNormal());
    return l1.intersect(l2);
}

Circle Triangle::circumscribedCircle() const {
    Point center = circumscribedCenter();
    Circle res(center, dist(center, points[0]));
    return res;
}

Circle Triangle::inscribedCircle() const {
    Point center = bisectorCenter();
    Line a(points[0], points[1]);
    Circle res(center, a.dist(center));
    return res;
}

Line Triangle::EulerLine() const { // в правильном треугольнике прямая неопределена
    Line res(orthocenter(), centroid());
    return res;
}

Circle Triangle::ninePointsCircle() const {
    Triangle t(0.5 * (points[0] + points[1]), 0.5 * (points[0] + points[2]), 0.5 * (points[1] + points[2]));
    return t.circumscribedCircle();
}

