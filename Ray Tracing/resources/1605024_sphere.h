//
// Created by mahir on 6/28/21.
//

#ifndef RAY_TRACING_1605024_SPHERE_H
#define RAY_TRACING_1605024_SPHERE_H

#include "1605024_object.h"
#include "1605024_config.h"


class Sphere: public Object {
    Point center;
    double radius;
public:
    Sphere(Point  cen, double rad) : center(std::move(cen)) , radius(rad) {}

    void draw() override {
        Point points[80][80];

        int stacks = 70, slices = 70;
        //generate points
        for(int i = 0 ; i<= stacks ; i++) {
            double h = radius * sin(((double) i / (double) stacks ) * (PI / 2 ) ) ;
            double r = radius * cos(((double) i / (double) stacks ) * (PI / 2));
            for(int j = 0; j <= slices ; j++) {
                points[i][j][0] = r * cos( ( (double) j / (double) slices ) * 2 * PI);
                points[i][j][1] = r * sin( ( (double) j / (double) slices ) * 2 * PI);
                points[i][j][2] = h;
            }
        }
        //draw quads using generated points
        for(int i = 0 ; i < stacks ; i++) {
            glColor3f(object_color[0], object_color[1], object_color[2]);
            for (int j = 0; j < slices; j++) {

                glPushMatrix();
                {
                    glTranslatef(center[0], center[1], center[2]);
                    glBegin(GL_QUADS);
                    {
                        //upper hemisphere
                        glVertex3f(points[i][j][0], points[i][j][1], points[i][j][2]);
                        glVertex3f(points[i][j + 1][0], points[i][j + 1][1], points[i][j + 1][2]);
                        glVertex3f(points[i + 1][j + 1][0], points[i + 1][j + 1][1], points[i + 1][j + 1][2]);
                        glVertex3f(points[i + 1][j][0], points[i + 1][j][1], points[i + 1][j][2]);
                        //lower hemisphere
                        glVertex3f(points[i][j][0], points[i][j][1], -points[i][j][2]);
                        glVertex3f(points[i][j + 1][0], points[i][j + 1][1], -points[i][j + 1][2]);
                        glVertex3f(points[i + 1][j + 1][0], points[i + 1][j + 1][1], -points[i + 1][j + 1][2]);
                        glVertex3f(points[i + 1][j][0], points[i + 1][j][1], -points[i + 1][j][2]);
                    }
                    glEnd();
                }
                glPopMatrix();
            }
        }
    }

    /*
     * Reference: http://www.ambrsoft.com/TrigoCalc/Sphere/SpherLineIntersection_.htm
     */

    double get_intersecting_t(Ray &ray) override {
        Point pt = center - ray.start;
        double b = -2.0 * Point::dot(ray.dir, pt );
        double c = Point::dot(pt, pt) - radius * radius;
        double D = b * b - 4 * c;
        if(D < 0) return -1;

        D = sqrt(D);

        double ret = INF;
        double t1 = (-b + D) / 2.0;
        double t2 = (-b - D) / 2.0;
        if(t1 > 0) ret = min(ret, t1);
        if(t2 > 0) ret = min(ret, t2);
        if(ret == INF) ret = -1;
        return ret;
    }

    Point get_normal(Point intersection_point) override {
        Point normal = intersection_point - center;
        normal.normalize();
        return normal;
    }
    Color get_object_color(Point &p) override {
        return object_color;
    }
    bool is_inside(Point &p) override {
        double dist = Point::distance(p, center) - EPS;
        return dist <= radius;
    }
};

#endif //RAY_TRACING_1605024_SPHERE_H
