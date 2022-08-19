//
// Created by mahir on 6/29/21.
//

#ifndef RAY_TRACING_1605024_COLOR_H
#define RAY_TRACING_1605024_COLOR_H

class Color {
public:
    double rgb[3];
    Color() {
        memset(rgb, 0, sizeof rgb);
    }
    Color(double r, double g, double b) : Color() {
        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
    }
    void clear() {
        memset(rgb, 0, sizeof rgb);
    }
    void clamp() {
        for(double &x: rgb) x = min(1.0, x);
    }
    bool equals(Color p) {
        for(int i = 0 ; i < 3 ; i++) {
            if(p[i] != (*this)[i]) return false;
        }
        return true;
    }
    double& operator[] (int id) {
        return rgb[id];
    }
    Color operator* (const double val) {
        Color ret = *this;
        for(int i = 0 ; i < 3 ; i++) ret[i] = rgb[i] * val;
        return ret;
    }
    Color operator/ (const double val) {
        Color ret = *this;
        for(int i = 0 ; i < 3 ; i++) ret[i] = rgb[i] / val;
        return ret;
    }
    Color operator* (Color col) {
        Color ret = *this;
        for(int i = 0 ; i < 3 ; i++) ret[i] = rgb[i] * col[i];
        return ret;
    }
    Color operator+= (Color col) {
        for(int i = 0 ; i < 3 ; i++) rgb[i] += col[i];
        return *this;
    }
    friend istream& operator>>(istream& is, Color& color) {
        for(int i = 0 ; i <3; i++ ) is >> color[i] ;
        return is;
    }
    friend ostream& operator<<(ostream& os, Color& c) {
        os<<" ( "<<c[0]<<" , "<<c[1]<<" , "<<c[2]<<" )";
        return os;
    }
};

#endif //RAY_TRACING_1605024_COLOR_H
