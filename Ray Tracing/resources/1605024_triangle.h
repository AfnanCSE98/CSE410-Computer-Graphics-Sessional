//
// Created by mahir on 6/30/21.
//

#ifndef RAY_TRACING_1605024_TRIANGLE_H
#define RAY_TRACING_1605024_TRIANGLE_H
#include <utility>

#include "1605024_point.h"
#include "1605024_object.h"

class Triangle: public Object {
    Point A, B, C;
    Point normal;
public:
    Triangle(Point &a, Point &b, Point &c): A(move(a)), B(move(b)), C(move(c)) {
        normal =  Point::cross( B - A, C - A);
        normal.normalize();
    }
    void draw() override {
        glBegin(GL_TRIANGLES);
        glColor3f(object_color[0], object_color[1], object_color[2]);
        glVertex3f(A[0], A[1], A[2]);
        glVertex3f(B[0], B[1], B[2]);
        glVertex3f(C[0], C[1], C[2]);
        glEnd();
    }
    Point get_normal(Point pt) override {
        return normal;
    }

    static double get_determinant(double mat[3][3]) {
        double ret = 0;
        ret += mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
        ret -= mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]);
        ret += mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
        return ret;
    }

    double get_intersecting_t(Ray &ray) override {

        double mat1[3][3] = {
                { A[0] - B[0],  A[0] - C[0], ray.dir[0] },
                { A[1] - B[1],  A[1] - C[1], ray.dir[1] },
                { A[2] - B[2],  A[2] - C[2], ray.dir[2] },
        };
        double D = get_determinant(mat1);
        if(D == 0) return -1;

        double mat2[3][3] = {
                { A[0] - ray.start[0] , A[0] - C[0] , ray.dir[0] },
                { A[1] - ray.start[1] , A[1] - C[1] , ray.dir[1] },
                { A[2] - ray.start[2] , A[2] - C[2] , ray.dir[2] },
        };
        double beta = get_determinant(mat2) / D;
        if(beta < 0 || beta > 1) return -1;

        double mat3[3][3] = {
                { A[0] - B[0] , A[0] - ray.start[0] , ray.dir[0] },
                { A[1] - B[1] , A[1] - ray.start[1] , ray.dir[1] },
                { A[2] - B[2] , A[2] - ray.start[2] , ray.dir[2] },
        };
        double gamma = get_determinant(mat3) / D;
        if(gamma < 0 || gamma > 1) return -1;
        if(beta + gamma > 1) return -1;

        double mat4[3][3] = {
                { A[0] - B[0] , A[0] - C[0] , A[0] - ray.start[0] },
                { A[1] - B[1] , A[1] - C[1] , A[1] - ray.start[1] },
                { A[2] - B[2] , A[2] - C[2] , A[2] - ray.start[2] },
        };

        double t = get_determinant(mat4) / D;
        if(t < 0) return -1;
        return t;
    }
    Color get_object_color(Point &p) override {
        return object_color;
    }
};

#endif //RAY_TRACING_1605024_TRIANGLE_H
