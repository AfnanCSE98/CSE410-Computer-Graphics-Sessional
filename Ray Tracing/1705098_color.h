#include<bits/stdc++.h>
using namespace std;

class Color
{

public:
    double r,g,b;

    Color(/* args */){
        r = g = b = 0;
    }

    Color(double red , double green , double blue){
        r = red;
        g = green;
        b = blue;
    }

    Color operator*(const double &coefficient) const{
        Color result;
        result.r = r * coefficient;
        result.g = g * coefficient;
        result.b = b * coefficient;
        return result;
    }

    Color operator+(const Color &color) const{
        Color result;
        result.r = r + color.r;
        result.g = g + color.g;
        result.b = b + color.b;
        return result;
    }

    Color operator*(const Color &color) const{
        Color result;
        result.r = r * color.r;
        result.g = g * color.g;
        result.b = b * color.b;
        return result;
    }

    friend ostream &operator<<(ostream &os, const Color &t) {
        os <<"("<< t.r << ", " << t.g << ", " << t.b << ")" << endl;
        return os;
    }

    friend istream &operator>>(istream &is, Color &v) {
        is >> v.r >> v.g >> v.b;
        return is;
    }

    double get_r(){
        return r;
    }

    double get_g(){
        return g;
    }

    double get_b(){
        return b;
    }
    
};

