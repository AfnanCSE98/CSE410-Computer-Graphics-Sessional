#include<bits/stdc++.h>
using namespace std;

class Color
{
private:
    double r,g,b;
public:
    Color(/* args */){
        r = g = b = 0;
    }

    Color(double red , double green , double blue){
        r = red;
        g = green;
        b = blue;
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
    //setters
    void set_r(double red){
        r = red;
    }
    void set_g(double green){
        g = green;
    }
    void set_b(double blue){
        b = blue;
    }
    
    
};

