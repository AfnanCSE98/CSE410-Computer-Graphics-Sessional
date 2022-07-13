#include<bits/stdc++.h>
using namespace std;

class Color
{
private:
    int r,g,b;
public:
    Color(/* args */){
        r = g = b = 0;
    }

    Color(int red , int green , int blue){
        r = red;
        g = green;
        b = blue;
    }

    int get_r(){
        return r;
    }

    int get_g(){
        return g;
    }

    int get_b(){
        return b;
    }
    
};

