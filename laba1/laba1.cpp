#include <iostream>
#include <vector>
#include "Line.h"
#include "Circle.h"
#include "SparseMatrix.h"
using namespace std;

double distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

vector <Point> intersection(Line l1, Line l2, bool show = false) {
    vector <Point> points;
    if (l1.k == l2.k) {
        if (l1.b == l2.b)
            if (show) cout << "Lines are the same\n";
        else
            if (show) cout << "No intersections\n";
        return points;
    }
    double x = (l1.b - l2.b) / (l2.k - l1.k);
    double y = l1.k * x + l1.b;
    if(show) cout << "Intersection : (" << x << ", " << y << ")\n";
    points.push_back(Point(x, y));
    return points;
}

vector <Point> intersection(Line l, Circle c, bool show = false) {
    vector <Point> points;
    double A = l.k * l.k + 1;
    double B = 2 * l.k * (l.b - c.center.y) - 2 * c.center.x;
    double C = c.center.x * c.center.x + (l.b - c.center.y) * (l.b - c.center.y) - c.radius * c.radius;
    double D = B * B - 4 * A * C;
    if (D < 0)
        if (show) cout << "No intersections\n";
    if (D == 0) {
        double x = -1 * B / (2 * A);
        double y = l.k * x + l.b;
        if (show) cout << "Intersection : (" << x << ", " << y << ")\n";
        points.push_back(Point(x, y));
    }
    if (D > 0) {
        if (show) cout << "Two intersections: (";
        double x = (-1 * B + sqrt(D)) / (2 * A);
        double y = l.k * x + l.b;
        if (show) cout << x << ", " << y << ") and (";
        points.push_back(Point(x, y));
        x = (-1 * B - sqrt(D)) / (2 * A);
        y = l.k * x + l.b;
        if (show) cout << x << ", " << y << ")\n";
        points.push_back(Point(x, y));
    }
    return points;
}

vector <Point> intersection(Circle c1, Circle c2, bool show = false) {
    vector <Point> points;
    double x1 = c1.center.x, y1 = c1.center.y, r1 = c1.radius, x2 = c2.center.x, y2 = c2.center.y, r2 = c2.radius;
    if (x1 == x2 && y1 == y2) {
        if (r1 == r2)
            if (show) cout << "Circles are the same\n";
        else
            if (show) cout << "No intersections\n";
        return points;
    }
    if (y1 == y2) {
        double x = (r2 * r2 - r1 * r1 + x1 * x1 - x2 * x2) / (2 * (x1 - x2));
        double D = r1 * r1 - (x - x1) * (x - x1);
        if (D < 0)
            if (show) cout << "No intersectinos\n";
        if (D == 0) {
            if (show) cout << "Intersection : (" << y1 << ", " << x << ")\n";
            points.push_back(Point(y1, x));
        }
        if (D > 0) {
            if (show) cout << "Two intersections: (";
            double y = y1 + sqrt(D);
            if (show) cout << x << ", " << y << ") and (";
            points.push_back(Point(x, y));
            y = y1 - sqrt(D);
            if (show) cout << x << ", " << y << ")\n";
            points.push_back(Point(x, y));
        }
        return points;
    }
    double k = (x2 - x1) / (y1 - y2);
    double b = (r2 * r2 - r1 * r1 + y1 * y1 - y2 * y2 + x1 * x1 - x2 * x2) / (2 * (y1 - y2));
    return intersection(Line(k, b), c1, true);
}

Point pointInversion(double x, double y, Circle c) {
    if (x == c.center.x && y == c.center.y) {
        cout << "Infinitely distant point\n";
        return Point(c.center.x, c.center.y);
    }
    double X = c.center.x + c.radius * c.radius * (x - c.center.x) / ((x - c.center.x) * (x - c.center.x) + (y - c.center.y) * (y - c.center.y));
    double Y = c.center.y + c.radius * c.radius * (y - c.center.y) / ((x - c.center.x) * (x - c.center.x) + (y - c.center.y) * (y - c.center.y));
    cout << "Inversed point : (" << X << ", " << Y << ")\n";
    return Point(X, Y);
}

void LineInversion(Line l, Circle c) {
    if (c.center.y == l.k * c.center.x + l.b) {
        cout << "Inversed line : " << "y = " << l.k << "*x + " << l.b << '\n';
        return;
    }
    double k = -1 / l.k;
    double b = c.center.y - c.center.x * k;
    Point q = intersection(Line(k, b), l).front();
    Point p = pointInversion(q.x, q.y, c);
    double x = (p.x + c.center.x) / 2, y = (p.y + c.center.y) / 2;
    double r = distance(c.center, p) / 2;
    cout << "Inversed line is circle with center (" << x << ", " << y << ") and radius " << r << '\n';
}

Line LineSymmetry(Line l, Line s) {
    Point p = intersection(l, s).front();
    double k = -1 / s.k, b = (s.k * (p.x - 1) + s.b) - k * (p.x - 1);
    Point x = intersection(l, Line(k, b)).front(), y = Point((2 * p.x - 2) - x.x, 2 * (s.k * (p.x - 1) + s.b) - x.y);
    Line res = Line(p, y);
    cout << "Symmetrycal line : " << "y = " << res.k << "*x + " << res.b << '\n';
    return res;
}

Circle CircleSymmetry(Circle c, Line s) {
    Point p = intersection(s, Line(-1 / s.k, c.center.y - (-1 * c.center.x / s.k))).front();
    Point z = Point(2 * p.x - c.center.x, 2 * p.y - c.center.y);
    Circle res = Circle(z, c.radius);
    cout << "Symmetrycal circle has center (" << z.x << ", " << z.y << ") and radius " << res.radius << '\n';
    return res;
}

double getAngle(Line l1, Line l2, bool show = false) {
    double angle = fabs(atan(l1.k) - atan(l2.k)) * 180 / acos(-1);
    cout << "Angle is : " << angle << " degrees\n";
    return angle;
}

int main()
{
    //intersection(Line(1, 0), Line (-1, 1), true);
    //intersection(Line(1, 10), Circle(0, 0, 1), true);
    //intersection(Circle(0, 0, 2), Circle(3, 1, 2), true);
    //pointInversion(0, 0, Circle(3, 1, 2));
    //LineInversion(Line(1, 1), Circle(0, 0, 1));
    //LineSymmetry(Line(2, 0), Line(1, 0));
    //CircleSymmetry(Circle(3, 1, 2), Line(1, 1));
    //getAngle(Line(2, 0), Line(1, 0));
    //getAngle(Line(0.5, 0), Line(1, 0));

    vector <pair< pair<int, int>, int> > a = {
        {{1, 2}, 1},
        {{2, 1}, 2},
        {{3, 2}, 3} };
    SparseMatrix<int> A(3, 3, a);

    vector <pair< pair<int, int>, int> > b = {
        {{1, 1}, 2},
        {{2, 1}, 3},
        {{3, 3}, 1} };
    SparseMatrix<int> B(3, 3, b);

    //A.add(B).print();
    //A.transpose().print();
    A.multiply(B).print();
}
