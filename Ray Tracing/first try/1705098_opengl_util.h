//
// Created by ashiq on 3/14/21.
//

#ifndef OFFLINE1_UTIL_H
#define OFFLINE1_UTIL_H

#endif //OFFLINE1_UTIL_H

#include<cmath>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include "1705098_vector.h"

#define pi 3.1416

void drawAxes(int len) {


    glBegin(GL_LINES);

    {
        // x-axis
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(len, 0, 0);
        glVertex3f(-len, 0, 0);

        // y-axis
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0, -len, 0);
        glVertex3f(0, len, 0);

        // z-axis
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0, 0, len);
        glVertex3f(0, 0, -len);
    }

    glEnd();


}


void drawGrid(int lines, int len, int spacing) {

    int i;

    glBegin(GL_LINES);

    {
        for (i = -lines; i <= lines; i++) {

            if (i == 0)
                continue;    //SKIP the MAIN axes

            //lines parallel to Y-axis
            glVertex3f(i * spacing, -len, 0);
            glVertex3f(i * spacing, len, 0);

            //lines parallel to X-axis
            glVertex3f(-len, i * spacing, 0);
            glVertex3f(len, i * spacing, 0);
        }
    }

    glEnd();


}

void drawRectXY(Vector tl, Vector tr, Vector bl, Vector br) {

    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);

    {
        glVertex3f(tl.x, tl.y, tl.z);
        glVertex3f(tr.x, tr.y, tr.z);

        glVertex3f(bl.x, bl.y, bl.z);
        glVertex3f(br.x, br.y, br.z);

        glVertex3f(tl.x, tl.y, tl.z);
        glVertex3f(bl.x, bl.y, bl.z);

        glVertex3f(tr.x, tr.y, tr.z);
        glVertex3f(br.x, br.y, br.z);
    }


    glEnd();
}

void drawSquare(double a) {

    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);

    {
        glVertex3f(a, a, 2);
        glVertex3f(a, -a, 2);
        glVertex3f(-a, -a, 2);
        glVertex3f(-a, a, 2);
    }

    glEnd();
}


void drawCircleXY(double radius, int segments, Vector center) {

    int i;
    Vector points[100];

    //generate points
    for (i = 0; i <= segments; i++) {
        points[i].x = radius * cos(((double) i / (double) segments) * 2 * pi) + center.x;
        points[i].y = radius * sin(((double) i / (double) segments) * 2 * pi) + center.y;
        points[i].z = center.z;
    }

    //draw segments using generated points
    for (i = 0; i < segments; i++) {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x, points[i].y, points[i].z);
            glVertex3f(points[i + 1].x, points[i + 1].y, points[i].z);
        }
        glEnd();
    }
}


void drawCone(double radius, double height, int segments) {

    int i;
    double shade;
    Vector points[100];

    //generate points
    for (i = 0; i <= segments; i++) {
        points[i].x = radius * cos(((double) i / (double) segments) * 2 * pi);
        points[i].y = radius * sin(((double) i / (double) segments) * 2 * pi);
    }

    //draw triangles using generated points
    for (i = 0; i < segments; i++) {
        //create shading effect
        if (i < segments / 2)shade = 2 * (double) i / (double) segments;
        else shade = 2 * (1.0 - (double) i / (double) segments);

        glBegin(GL_TRIANGLES);

        {
            glVertex3f(0, 0, height);
            glVertex3f(points[i].x, points[i].y, 0);
            glVertex3f(points[i + 1].x, points[i + 1].y, 0);
        }

        glEnd();
    }
}


void drawSphere(double radius, int slices, int stacks, Vector center) {

    Vector points[stacks + 1][slices + 1];
    int i, j;
    double h, r;

    //generate points
    for (i = 0; i <= stacks; i++) {
        h = radius * sin(((double) i / (double) stacks) * (pi / 2));
        r = radius * cos(((double) i / (double) stacks) * (pi / 2));
        for (j = 0; j <= slices; j++) {
            double angle = ((double) j / (double) slices) * 2 * pi;
            points[i][j].x = r * cos(angle) + center.x;
            points[i][j].y = r * sin(angle) + center.y;
            points[i][j].z = h ;
        }
    }

    int color = 0;
    //draw quads using generated points
    for (i = 0; i < stacks; i++) {
        // glColor3f((double) i / (double) stacks, (double) i / (double) stacks, (double) i / (double) stacks);
        for (j = 0; j < slices; j++) {
            glBegin(GL_QUADS);

            {
                glColor3f(color, color, color);
                color = 1 - color;
                //upper hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, center. z + points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, center. z + points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, center. z + points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, center. z + points[i + 1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, center. z - points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, center. z - points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, center. z - points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, center. z - points[i + 1][j].z);
            }

            glEnd();
        }
    }
}

void clearDisplay() {
    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);    //color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void registerKeyboardListener(void (*listener)(unsigned char, int, int)) {
    glutKeyboardFunc(listener);
}

void registerMouseListener(void (*listener)(int, int, int, int)) {
    glutMouseFunc(listener);
}

void registerSpecialKeysListener(void (*listener)(int, int, int)) {
    glutSpecialFunc(listener);
}








