#ifndef OFFLINE3_1705098_OBJECTS_H
#define OFFLINE3_1705098_OBJECTS_H

#endif 

#include <bits/stdc++.h>
#include "1705098_color.h"
//#include "1705098_vector.h"
#include "1705098_opengl_util.h"

#define pi 3.1416

using namespace std;

class Object {
    vector<double> coEfficients;
    int shine;
public:
    string name;
    Vector reference_point;
    Color color;

    Object() {
        reference_point = {0, 0, 0};
        shine = 0;
        coEfficients.push_back(0);
        coEfficients.push_back(0);
        coEfficients.push_back(0);
        coEfficients.push_back(0);
        name = "";
        color = Color(0, 0, 0);
    }

    double getAmbient() {
        return coEfficients[0];
    }

    void setAmbient(double val) {
        coEfficients[0] = val;
    }

    double getDiffuse() {
        return coEfficients[1];
    }

    void setDiffuse(double val) {
        coEfficients[1] = val;
    }

    double getSpecular() {
        return coEfficients[2];
    }

    void setSpecular(double val) {
        coEfficients[2] = val;
    }

    double getReflection() {
        return coEfficients[3];
    }

    void setReflection(double val) {
        coEfficients[3] = val;
    }

    int getShine() {
        return shine;
    }

    void setShine(int s) {
        this->shine = s;
    }

    virtual void draw() const = 0;

    virtual Vector getNormal(Vector intersectionPoint) const = 0;

    virtual Color getColor(Vector IntersectingPoint) const = 0;
};

class Sphere : public Object {
public:
    double radius;

    Sphere() {
        radius = 0;
        name = "sphere";
    }

    Sphere(Vector center, Color color, double radius, double ambient, double diffuse, double specular, double reflection, int shine) {
        name = "sphere";
        reference_point = center;
        this->radius = radius;
        this->color = color;

        setAmbient(ambient);
        setDiffuse(diffuse);
        setSpecular(specular);
        setReflection(reflection);
        setShine(shine);

    }

    void draw() const override {
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glColor3f(color.r, color.g, color.b);

        struct Vector points[200][200];
        int i, j;
        double h, r;
        int slices = 100, stacks = 100;
        for (i = 0; i <= stacks; i++) {
            h = radius * sin(((double) i / (double) stacks) * (pi / 2));
            r = radius * cos(((double) i / (double) stacks) * (pi / 2));
            for (j = 0; j <= slices; j++) {
                points[i][j].x = r * cos(((double) j / (double) slices) * 2 * pi);
                points[i][j].y = r * sin(((double) j / (double) slices) * 2 * pi);
                points[i][j].z = h;
            }
        }

        for (i = 0; i < stacks; i++) {
            for (j = 0; j < slices; j++) {
                glBegin(GL_QUADS);
                {
                    glVertex3f((GLfloat)points[i][j].x, (GLfloat)points[i][j].y, (GLfloat)points[i][j].z);
                    glVertex3f((GLfloat)points[i][j + 1].x, (GLfloat)points[i][j + 1].y, (GLfloat)points[i][j + 1].z);
                    glVertex3f((GLfloat)points[i + 1][j + 1].x, (GLfloat)points[i + 1][j + 1].y, (GLfloat)points[i + 1][j + 1].z);
                    glVertex3f((GLfloat)points[i + 1][j].x, (GLfloat)points[i + 1][j].y, (GLfloat)points[i + 1][j].z);

                    glVertex3f((GLfloat)points[i][j].x, (GLfloat)points[i][j].y, (GLfloat)-points[i][j].z);
                    glVertex3f((GLfloat)points[i][j + 1].x, (GLfloat)points[i][j + 1].y, (GLfloat)-points[i][j + 1].z);
                    glVertex3f((GLfloat)points[i + 1][j + 1].x, (GLfloat)points[i + 1][j + 1].y, (GLfloat)-points[i + 1][j + 1].z);
                    glVertex3f((GLfloat)points[i + 1][j].x, (GLfloat)points[i + 1][j].y, (GLfloat)-points[i + 1][j].z);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }

    friend istream &operator>>(istream &is, Sphere &v) {
        is >> v.reference_point;
        is >> v.radius;
        is >> v.color;

        double c1, c2, c3, c4;
        int c5;

        is >> c1;
        v.setAmbient(c1);

        is >> c2;
        v.setDiffuse(c2);

        is >> c3;
        v.setSpecular(c3);

        is >> c4;
        v.setReflection(c4);

        is >> c5;
        v.setShine(c5);

        return is;
    }

    friend ostream &operator<<(ostream &os, const Sphere &v) {
        os <<"Sphere P:"<<v.reference_point;
        os <<"Sphere C:"<<v.color<<endl;
        return os;
    }

    Vector getNormal(Vector intersectionPoint) const override {
        Vector ret;
        ret = intersectionPoint - reference_point;
        ret = ret.normalize();
        return ret;
    }

    Color getColor(Vector IntersectingPoint) const override {
        return color;
    }

};



class Camera {
public:

    Vector pos;
    Vector u, r, l;
    int fovy;
    int aspectRatio;
    int nearPlane;
    int farPlane;

    Camera() {

        pos = Vector();
        pos.x = 0;
        pos.y = 0;
        pos.z = 200;

        l.x = -0.152173;
        l.y =  0.138297;
        l.z =  -0.950123;

        //r.x = -1 / sqrt(2.0);
        r.x = 0.4712;
        r.y = 0.51856;
        r.z = 0;
        //r.y = 1 / sqrt(2.0);
        //r.z = 0;

        u = l * r;

        fovy = 90;
        aspectRatio = 1;
        nearPlane = 1;
        farPlane = 1000;
    }

    void print() const {
        cout<<"Pos:";
        pos.print();
        cout<<"l:";
        l.print();
        cout<<"r:";
        r.print();
        cout<<"u:";
        u.print();
    }

};

class Light {
public:
    Vector position;
    Color color;

    friend ostream &operator<<(ostream &os, const Light &v) {
        os <<"Light P:"<<v.position;
        os <<"Light C:"<<v.color<<endl;
        return os;
    }

    friend istream &operator>>(istream &is, Light &v) {
        is >> v.position;
        is >> v.color;
        return is;
    }

    void draw() const{
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
        {
            glTranslatef(position.x, position.y, position.z);
            glutSolidSphere(0.5, 7, 7);
        }
        glPopMatrix();
    }

};


class Scene {
public:
    int numObjects;
    int numLightSources;
    int recursionLevels;
    int pixels;

    vector<Object *> objects;
    vector<Light> lights;

    void draw() const {
        for (int i=0; i < numObjects; i++) {
            objects[i]->draw();
        }
        for (const Light & light : lights) {
            light.draw();
        }
    }
};


class Floor : public Object {
public:
    int tileWidth;
    int width;

    Floor(int tw, int w) {
        name = "floor";
        tileWidth = tw;
        width = w;
    }

    void draw() const override {

        double color = 0;
        int leftLim = -5 * width;
        int rightLimit = 5 * width;
        glPushMatrix();
        for (int i = leftLim; i < rightLimit; i += tileWidth) {
            for (int j = leftLim; j < rightLimit; j += tileWidth) {
                color = 1 - color;
                glColor3f(color, color, color);

                glBegin(GL_QUADS);
                {
                    glVertex2f(i, j);

                    glVertex2f(i + tileWidth, j);

                    glVertex2f(i + tileWidth, j + tileWidth);

                    glVertex2f(i, j + tileWidth);

                }
                glEnd();
            }
            color = 1 - color;
        }
        glPopMatrix();
    }

    Vector getNormal(Vector intersectionPoint) const override {
        return {0, 0, 1};
    }

    Color getColor(Vector intersectPoint) const override {
        auto dx = static_cast<int>((static_cast<double>(intersectPoint.x + width / 2.0)) / tileWidth);
        auto dy = static_cast<int>((static_cast<double>(intersectPoint.y + width / 2.0)) / tileWidth);

        int remI = dx % 2;
        int remJ = dy % 2;

        if (remI < 0) {
            remI *= -1;
        }
        if (remJ < 0) {
            remJ *= -1;
        }

        if ((remI == 0 && remJ == 0) || (remI == 1 && remJ == 1)) {
            return {1, 1, 1};
        } else {
            return {0, 0, 0};
        }
    }

};
