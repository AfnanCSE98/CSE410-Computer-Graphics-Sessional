#include<bits/stdc++.h>
#include "vector.h"
#include "Color.h"
#include <random>
using namespace std;

class Triangle{
    Vector p1,p2,p3;
    vector<Vector>peaks;
    Color color;
public:
    Triangle(Vector a1 , Vector a2 , Vector a3){
        p1 = a1;
        p2 = a2;
        p3 = a3;

        peaks.push_back(a1);
        peaks.push_back(a2);
        peaks.push_back(a3);

        color = Color((int)getRandomFloat(0, 255), (int)getRandomFloat(0, 255), (int)getRandomFloat(0, 255));
    }

    Vector get_p1(){
        return p1;
    }

    Vector get_p2(){
        return p2;
    }

    Vector get_p3(){
        return p3;
    }

    Color get_color(){
        return color;
    }

    double getMax_X(){
        double d = max(max(p1.x , p2.x) , p3.x);
        return d;
    }

    double getMax_Y(){
        double d = max(max(p1.y , p2.y) , p3.y);
        return d;
    }

    double getMin_X(){
        double d = min(min(p1.x , p2.x) , p3.x);
        return d;
    }

    double getMin_Y(){
        double d = min(min(p1.y , p2.y) , p3.y);
        return d;
    }

    double getRandomFloat(float a , float b){
        random_device rd; // obtain a random number from hardware
        mt19937 gen(rd()); // seed the generator
        uniform_int_distribution<> distr(a,b); // define the range
        return (int)distr(gen);
    }


};