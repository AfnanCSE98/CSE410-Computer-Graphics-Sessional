#include<bits/stdc++.h>
using namespace std;

class Vector
{

public:

    double x,y,z;

    Vector(){

        x=0;
        y=0;
        z=0;
    }

    Vector(double a , double b , double c){

        x=a;
        y=b;
        z=c;
    }

    Vector operator+(Vector rhv){

        Vector res;

        res.x = x + rhv.x;
        res.y = y + rhv.y;
        res.z = z + rhv.z;

        return res;
    }

    Vector operator-(Vector rhv){

        Vector res;

        res.x = x - rhv.x;
        res.y = y - rhv.y;
        res.z = z - rhv.z;

        return res;
    }

    Vector operator-() const {

        Vector res;

        res.x = -x;
        res.y = -y;
        res.z = -z;

        return res;
    }

    Vector operator/(double scalar){

        Vector res;

        if (scalar != 0) {
            res.x = x / scalar;
            res.y = y / scalar;
            res.z = z / scalar;
        }

        return res;
    }

    

    Vector operator*(double scalar){

        Vector res;

        res.x = x * scalar;
        res.y = y * scalar;
        res.z = z * scalar;

        return res;
    }

    double dot(Vector &rhv){

        return x * rhv.x + y * rhv.y + z * rhv.z;
    }


    Vector operator*(Vector rhv){

        Vector res;

        res.x = y * rhv.z - z * rhv.y;
        res.y = z * rhv.x - x * rhv.z;
        res.z = x * rhv.y - y * rhv.x;

        return res;

    }


    Vector normalize(){

        double w = sqrt(x*x + y*y + z*z);
        (*this) = (*this)/w;
        return (*this) ;
    }

    friend istream &operator>>(std::istream &is, Vector &v) {

        is >> v.x >> v.y >> v.z;
        return is;
    }


    friend ostream &operator<<(ostream &os, const Vector &v) {

        os << v.x << " " << v.y << " " << v.z << endl;
        return os;
    }

    Vector I() {
        return Vector(1,0,0);
    }

    Vector J() {
        return {0, 1, 0};
    }

    Vector K() {
        return {0, 0, 1};
    }


};