//
// Created by USER on 24-Jun-21.
//

#ifndef RAY_TRACING_1605024_POINT_H
#define RAY_TRACING_1605024_POINT_H

using namespace std;
const int M = 3;

class Point {
    double pt[M];
public:
    Point() {
        memset(pt, 0 , sizeof pt);
    }
    Point(double x, double y, double z) {
        pt[0] = x; pt[1] = y; pt[2] = z;
    }
    static double dot(Point v1, Point v2) {
        double ret = 0;
        for(int i = 0 ; i < M ; i++ ) {
            ret += v1[i] * v2[i];
        }
        return ret;
    }
    static Point cross(Point v1, Point v2) {
        Point ret;
        ret[0] = v1[1] * v2[2] - v1[2] * v2[1];
        ret[1] = -v1[0] * v2[2] + v1[2] * v2[0];
        ret[2] = v1[0] * v2[1] - v1[1] * v2[0];
        return ret;
    }
    void normalize() {
        double down = 0;
        for(double x : pt) down += x*x;
        down = sqrt(down);
        for(double &x: pt) x /= down;
    }
    //operator overloads
    inline double& operator[] (int id) {
        return pt[id];
    }
    friend ostream& operator<<(ostream& os, Point& v) {
        os<<" ( ";
        for(int i = 0 ; i < M ; i++ ) {
            os << v[i];
            if(i < M - 1) os << ", ";
        }
        os<<" ) ";
        return os;
    }
    friend istream& operator>>(istream& is, Point& v) {
        for(int i = 0 ; i < M; i++ ) is >> v[i] ;
        return is;
    }
    bool equals( Point p) {
        for(int i = 0 ; i < M ; i++) {
            if(p[i] != (*this)[i]) return false;
        }
        return true;
    }
    Point operator* (double val) {
        Point ret = *this;
        for(double &x: ret.pt) x *= val;
        return ret;
    }
    Point operator/ (double val) {
        Point ret = *this;
        for(double &x: ret.pt) x /= val;
        return ret;
    }
    Point operator+ (Point v) {
        Point ret;
        for(int i = 0 ; i < M ; i++) ret[i] = (*this)[i] + v[i];
        return ret;
    }
    Point operator- (Point v) {
        Point ret;
        for(int i = 0 ; i < M ; i++) ret[i] = (*this)[i] - v[i];
        return ret;
    }
    static double distance(Point p1, Point p2) {
        double ret = 0;
        for(int i = 0  ; i < M ; i++) ret += (p1[i] - p2[i]) * (p1[i] - p2[i]);
        return sqrt(ret);
    }
};


#endif //RAY_TRACING_1605024_POINT_H
