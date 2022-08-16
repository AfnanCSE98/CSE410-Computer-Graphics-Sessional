//
// Created by ashiq on 3/15/21.
//

#ifndef _1705098_VECTOR_H
#define _1705098_VECTOR_H
#endif

#include <fstream>
#include <cmath>
#include <iostream>
#include <bits/stdc++.h>

#define rad(t) ((t) * pi / 180.0)
#define pi (2*acos(0.0))

class Vector {
public:
    double x, y, z;

    Vector() {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector operator+(const Vector &rhs) const {

        Vector res;

        res.x = x + rhs.x;
        res.y = y + rhs.y;
        res.z = z + rhs.z;

        return res;
    }

    Vector operator-(const Vector &rhs) const {

        Vector res;

        res.x = x - rhs.x;
        res.y = y - rhs.y;
        res.z = z - rhs.z;

        return res;
    }

    Vector operator-() const {

        Vector res;

        res.x = -x;
        res.y = -y;
        res.z = -z;

        return res;
    }

    Vector operator/(double scalar) const {

        Vector res;

        if (scalar != 0) {
            res.x = x / scalar;
            res.y = y / scalar;
            res.z = z / scalar;
        }

        return res;
    }

    Vector operator*(const Vector &rhs) const {

        Vector res;

        res.x = y * rhs.z - z * rhs.y;
        res.y = z * rhs.x - x * rhs.z;
        res.z = x * rhs.y - y * rhs.x;

        return res;

    }

    Vector operator*(double scalar) const {

        Vector res;

        res.x = x * scalar;
        res.y = y * scalar;
        res.z = z * scalar;

        return res;
    }

    double getDistance(const Vector &rhs) const {

        double dX = x - rhs.x;
        double dY = y - rhs.y;
        double dZ = z - rhs.z;

        return sqrt(dX * dX + dY * dY + dZ * dZ);

    }

    double abs() const{
        return sqrt(x * x + y * y + z * z);
    }

    double dot(const Vector &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    Vector normalize() const {

        Vector res;

        auto val = sqrt(x * x + y * y + z * z);

        res.x = x / val;
        res.y = y / val;
        res.z = z / val;

        return res;
    }

    Vector rotate(const Vector &axis, double angleDegrees) const {
        Vector crossProduct = axis * (*this);

        Vector r1 = (*this) * cos(angleDegrees * pi / 180);
        Vector r2 = crossProduct * sin(angleDegrees * pi / 180.0);

        return r1 + r2;
    }

    Vector reflection(Vector normal) const{
        Vector reflected = *this - normal * (2.0 * this->dot(normal));
        reflected = reflected.normalize();
        return reflected;
    }

    friend istream &operator>>(istream &is, Vector &v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }


    friend ostream &operator<<(ostream &os, const Vector &v) {
        os << v.x << " " << v.y << " " << v.z << endl;
        return os;
    }

    void print() const {
        cout << "( " << x << "," << y << "," << z << " )" << endl;
    }

    static Vector I() {
        return {1, 0, 0};
    }

    static Vector J() {
        return {0, 1, 0};
    }

    static Vector K() {
        return {0, 0, 1};
    }
};