//
//  main.cpp
//  Offline1
//
//  Created by Shanjinur Islam on 5/23/19.
//  Copyright © 2019 Shanjinur Islam. All rights reserved.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include "glut.h"
#include <cmath>

double angle, increment;
double cubeSide, sphereRadius;

class Point
{
public:
    double x, y, z;

    Point()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point(const Point &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }
    void print()
    {
        printf("(%lf,%lf,%lf)\n\n", x, y, z);
    }
} pos, u, l, r, corner[8];

Point generateCrossProduct(Point a, Point b)
{
    Point p;
    p.x = a.y * b.z - a.z * b.y;
    p.y = a.z * b.x - a.x * b.z;
    p.z = a.x * b.y - a.y * b.x;

    return p;
}

void drawAxes()
{
    glPushMatrix();
    glBegin(GL_LINES);
    {
        glColor3f(1.0, 0, 0);
        glVertex3f(100, 0, 0);
        glVertex3f(-100, 0, 0);

        glColor3f(0, 1.0, 0);
        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);

        glColor3f(0, 0, 1.0);
        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, -100);
    }
    glEnd();
    glPopMatrix();
}

void init()
{
    corner[0] = Point(1, 1, 1);
    corner[1] = Point(-1, 1, 1);
    corner[2] = Point(-1, -1, 1);
    corner[3] = Point(1, -1, 1);

    pos = Point(100, 100, 0);
    u = Point(0, 0, 1); //z axis is up vector
    l = Point(-1 / (sqrt(2)), -1 / sqrt(2), 0);
    r = Point(-1 / (sqrt(2)), 1 / sqrt(2), 0);
    angle = acos(-1.0) / 30; //3 degree angle change
    increment = 5;
    cubeSide = 50;
    sphereRadius = 10;

    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1, 1, 1000.0);
}

void sphereComponent(double radius, int slices, int stacks)
{
    Point p[stacks + 1][slices + 1];
    double h, r, angle1, angle2;
    for (int i = 0; i <= stacks; i++)
    {
        angle1 = ((double)i / (double)stacks) * (acos(-1.0) / 2);
        h = radius * sin(angle1);
        r = radius * cos(angle1);
        for (int j = 0; j <= slices; j++)
        {
            angle2 = ((double)j / (double)slices) * acos(-1.0) / 2;
            p[i][j].x = r * cos(angle2);
            p[i][j].y = r * sin(angle2);
            p[i][j].z = h  ;
        }
    }
    for (int i = 0; i < stacks; i++)
    {
        glColor3f(1, 0, 0);
        for (int j = 0; j < slices; j++)
        {
            glBegin(GL_QUADS);
            {
                glVertex3f(p[i][j].x, p[i][j].y, p[i][j].z);
                glVertex3f(p[i][j + 1].x, p[i][j + 1].y, p[i][j + 1].z);
                glVertex3f(p[i + 1][j + 1].x, p[i + 1][j + 1].y, p[i + 1][j + 1].z);
                glVertex3f(p[i + 1][j].x, p[i + 1][j].y, p[i + 1][j].z);
            }
            glEnd();
        }
    }
    glPopMatrix();
}

void cylinderComponent(double radius, double height, int segments)
{
    glPushMatrix();
    Point p[segments + 1];
    double angle;
    for (int i = 0; i <= segments; i++)
    {
        angle = ((1.0 * i) / (1.0 * segments)) * acos(-1.0) * 0.5;
        p[i].x = radius * cos(angle);
        p[i].y = radius * sin(angle);
        p[i].z = height;
    }

    for (int i = 0; i < segments; i++)
    {

        glBegin(GL_LINES);

        glVertex3f(p[i].x, p[i].y, p[i].z);
        glVertex3f(p[i + 1].x, p[i + 1].y, p[i + 1].z);
        glVertex3f(p[i].x, p[i].y, -p[i].z);
        glVertex3f(p[i + 1].x, p[i + 1].y, -p[i + 1].z);

        glEnd();

        glBegin(GL_QUADS);
        glVertex3f(p[i].x, p[i].y, p[i].z);
        glVertex3f(p[i + 1].x, p[i + 1].y, p[i + 1].z);
        glVertex3f(p[i + 1].x, p[i + 1].y, -p[i + 1].z);
        glVertex3f(p[i].x, p[i].y, -p[i].z);

        glEnd();
    }

    glPopMatrix();
}

void CubeSide(double a)
{
    a = a / 2;
    glBegin(GL_QUADS);
    glVertex3f(a, a, 0);
    glVertex3f(a, -a, 0);
    glVertex3f(-a, -a, 0);
    glVertex3f(-a, a, 0);
    glEnd();
}

void CubeSphere(double side, double radius, int segments)
{
    glPushMatrix();
    double a = (side / 2) - radius ;

    glPushMatrix() ;
    
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(a * corner[i].x, a * corner[i].y, a * corner[i].z);
        glRotatef(90 * i, 0, 0, 1);
        sphereComponent(radius, segments, segments);
        glPopMatrix();
    }
    
    glPopMatrix();
    
    glPushMatrix() ;
    
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotated(180,1, 0, 0) ;
        glTranslatef(a * corner[i].x, a * corner[i].y, a * corner[i].z);
        glRotatef(90 * i, 0, 0, 1);
        sphereComponent(radius, segments, segments);
        glPopMatrix();
    }
    
    glPopMatrix() ;

    glPushMatrix() ;
    
    //
    for (int q = 0; q < 4; q++)
    {
        glPushMatrix();
        glColor3f(0, 1, 0);
        glTranslatef(a * corner[q].x, a * corner[q].y, 0);
        glRotatef(90 * q, 0, 0, 1);
        cylinderComponent(radius, a, segments);
        glPopMatrix();
    }
    
    glPopMatrix() ;
    
    glPushMatrix() ;
    
    for (int q = 0; q < 4; q++)
    {
        glPushMatrix();
        glColor3f(0, 1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(a * corner[q].x, a * corner[q].y, 0);
        glRotatef(90 * q, 0, 0, 1);
        cylinderComponent(radius, a, segments);
        glPopMatrix();
    }
    
    glPopMatrix() ;
    
    glPushMatrix() ;
    
    for (int q = 0; q < 4; q++)
    {
        glPushMatrix();
        glColor3f(0, 1, 0);
        
        glRotatef(90, 0, 1, 0);
        glTranslatef(a * corner[q].x, a * corner[q].y, 0);
        glRotatef(90 * q, 0, 0, 1);
        cylinderComponent(radius, a, segments);
        glPopMatrix();
    }
    
    glPopMatrix() ;
    
    
    glPushMatrix();
    glColor3f(1, 1, 1);
    double s = side - (2*radius) ;
    
    glPushMatrix() ;
    //upper side
    glTranslatef(0, 0, side/2);
    CubeSide(s);
    
    //lower side
    glTranslatef(0, 0, -side);
    CubeSide(s);
    glPopMatrix();

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef(90 * i, 0, 0, 1);
        glTranslatef(side/2, 0, 0);
        glRotatef(90, 0, 1, 0);
        CubeSide(s);
        glPopMatrix();
    }
    

    glPopMatrix();

    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    drawAxes();
    CubeSphere(cubeSide, sphereRadius, 20);
    glPopMatrix();

    glutSwapBuffers();
}
void animate()
{
    glutPostRedisplay();
}
void keyboardListener(unsigned char key, int x, int y)
{
    /*
     U
     |
     |
     |
     L ______ R
     
     1. Rotating will not change the UP vector but will change the LOOK Vector and RIGHT Vector.
     2. Looking up/down not change the RIGHT vector but will change the LOOK Vector and RIGHT Vector.
     3. Tilt not change the LOOK vector but will change the UP Vector and RIGHT Vector.
     */

    switch (key)
    {

    case 'w':
        if (sphereRadius < cubeSide / 2)
            sphereRadius++;
        break;
    case 's':
        if (sphereRadius > 0)
            sphereRadius--;
        break;
    case '1':
        r.x = r.x * cos(-1.0 * angle) + l.x * sin(-1.0 * angle);
        r.y = r.y * cos(-1.0 * angle) + l.y * sin(-1.0 * angle);
        r.z = r.z * cos(-1.0 * angle) + l.z * sin(-1.0 * angle);
        l = generateCrossProduct(u, r);
        break;
    case '2':
        r.x = r.x * cos(angle) + l.x * sin(angle);
        r.y = r.y * cos(angle) + l.y * sin(angle);
        r.z = r.z * cos(angle) + l.z * sin(angle);
        l = generateCrossProduct(u, r);
        break;
    case '3':
        l.x = l.x * cos(angle) + u.x * sin(angle);
        l.y = l.y * cos(angle) + u.y * sin(angle);
        l.z = l.z * cos(angle) + u.z * sin(angle);
        u = generateCrossProduct(r, l);
        break;
    case '4':
        l.x = l.x * cos(-1.0 * angle) + u.x * sin(-1.0 * angle);
        l.y = l.y * cos(-1.0 * angle) + u.y * sin(-1.0 * angle);
        l.z = l.z * cos(-1.0 * angle) + u.z * sin(-1.0 * angle);
        u = generateCrossProduct(r, l);
        break;
    case '5':
        u.x = u.x * cos(angle) + r.x * sin(angle);
        u.y = u.y * cos(angle) + r.y * sin(angle);
        u.z = u.z * cos(angle) + r.z * sin(angle);
        r = generateCrossProduct(l, u);
        break;
    case '6':
        u.x = u.x * cos(-1.0 * angle) + r.x * sin(-1.0 * angle);
        u.y = u.y * cos(-1.0 * angle) + r.y * sin(-1.0 * angle);
        u.z = u.z * cos(-1.0 * angle) + r.z * sin(-1.0 * angle);
        r = generateCrossProduct(l, u);
        break;

    default:
        break;
    }
}
void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        pos.x += l.x * increment; //as l.x is negative incrementing value linearly will change position of eye
        pos.y += l.y * increment; //as l.x is negative incrementing value linearly will change position of eye
        pos.z += l.z * increment;
        break;
    case GLUT_KEY_DOWN:
        pos.x -= l.x * increment;
        pos.y -= l.y * increment;
        pos.z -= l.z * increment;
        break;

    case GLUT_KEY_LEFT:
        pos.x -= r.x * increment; //as right vector is perpendicular to up vector linear shifting with negative incrementing value linearly will change position of eye left side
        pos.y -= r.y * increment;
        pos.z -= r.z * increment;
        break;
    case GLUT_KEY_RIGHT:
        pos.x += r.x * increment; //as right vector is perpendicular to up vector linear shifting with positive incrementing value linearly will change position of eye right side
        pos.y += r.y * increment;
        pos.z += r.z * increment;
        break;

    case GLUT_KEY_PAGE_UP:
        pos.x += u.x * increment;
        pos.y += u.y * increment;
        pos.z += u.z * increment; // shifting of position of eye on in z axis
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.x -= u.x * increment;
        pos.y -= u.y * increment;
        pos.z -= u.z * increment; // shifting of position of eye on in z axis
        break;

    default:
        break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Offline 1");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();

    return 0;
}