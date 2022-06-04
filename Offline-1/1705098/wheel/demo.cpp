//
//  main.cpp
//  Wheel
//
//  Created by Shanjinur Islam on 5/29/19.
//  Copyright © 2019 Shanjinur Islam. All rights reserved.
//

#define GL_SILENCE_DEPRECATION
#include <iostream>
#include "glut.h"
#include <cmath>

double cameraHeight = 80.0;
double cameraAngle  = 1.0;
double radius ;
double rotateAngle = 0;
double distance = 0 ;

class Point{
public:
    double x,y,z ;
    
    Point(){
        x = 0 ;
        y = 0 ;
        z = 0 ;
    }
    Point(double x,double y,double z){
        this->x = x ;
        this->y = y ;
        this->z = z ;
    }
    Point(const Point &p){
        this->x = p.x ;
        this->y = p.y ;
        this->z = p.z ;
    }
} pos ;

void drawAxes(){
    glPushMatrix();
    glBegin(GL_LINES);{
        glColor3f(1.0, 0, 0);
        glVertex3f( 100,0,0);
        glVertex3f(-100,0,0);
        
        glColor3f(0, 1.0, 0);
        glVertex3f(0,-100,0);
        glVertex3f(0, 100,0);
        
        glColor3f(0, 0, 1.0);
        glVertex3f(0,0, 100);
        glVertex3f(0,0,-100);
    }glEnd();
    glPopMatrix() ;
}

void drawGrid()
{
    int i;
    
        glColor3f(0.6, 0.6, 0.6);    //grey
        glBegin(GL_LINES);{
            for(i=-8;i<=8;i++){
                
                if(i==0)
                    continue;    //SKIP the MAIN axes
                
                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);
                
                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }glEnd();
}

void wheel(double radius,int segments){
    glPushMatrix() ;
    glTranslatef(0,0,+radius);
    glTranslatef(pos.x,pos.y,pos.z) ;
    glRotatef(rotateAngle,0,0,1) ;
    glRotatef(distance*(360.0/(2*3.1416*radius)),0,1,0) ;
    glRotatef(90, 1,0,0) ;
    
    Point p[segments+2] ;
    Point d[4] ;
    double angle ;
    double h = radius / 2 ;
    for(int i=0;i<=segments;i++){
        angle = ((1.0*i)/(1.0*segments))*acos(-1.0)*2 ;
        if(i%8==0){
            d[i/8].x = radius*cos(angle) ;
            d[i/8].y = radius*sin(angle) ;
            d[i/8].z = h/4 ;
        }
        p[i].x = radius*cos(angle) ;
        p[i].y = radius*sin(angle) ;
        p[i].z = h/2 ;
    }
    
    glPushMatrix() ;
    
    
    double shade = 0.0 ;
    for(int i=0;i<segments;i++){
        
        if(i<segments/2)  shade=((double)i/(double)segments);
        else  shade=(1-(double)i/(double)segments);
        
        glColor3f(shade,shade,shade);
        
        glBegin(GL_QUADS) ;
        glVertex3f(p[i].x,p[i].y,p[i].z) ;
        glVertex3f(p[i+1].x,p[i+1].y,p[i+1].z) ;
        glVertex3f(p[i+1].x,p[i+1].y,-p[i+1].z) ;
        glVertex3f(p[i].x,p[i].y,-p[i].z) ;
        
        glEnd() ;
    }

    for(int i=0;i<2;i++){
        shade=((double)i/(double)2);
        
        glColor3f(shade,shade,shade);
        glBegin(GL_QUADS) ;
        glVertex3f(d[i].x,d[i].y,d[i].z) ;
        glVertex3f(d[i+2].x,d[i+2].y,d[i+2].z) ;
        glVertex3f(d[i+2].x,d[i+2].y,-d[i+2].z) ;
        glVertex3f(d[i].x,d[i].y,-d[i].z) ;
        glEnd();
    }
    glPopMatrix() ;
    
    glPopMatrix() ;
}



void init(){
    radius = 15 ;
    pos = Point(0,0,0) ;
    glClearColor(0,0,0,0) ;
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity() ;
    gluPerspective(80,1,1,1000.0) ;
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glClearColor(0,0,0,0) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity() ;
    gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,        0,0,0,        0,0,1);
    
    glMatrixMode(GL_MODELVIEW) ;
    
    glPushMatrix();
    drawAxes() ;
    drawGrid() ;
    wheel(radius,31) ;
    glPopMatrix();
    
    glutSwapBuffers() ;
}
void animate(){
    glutPostRedisplay() ;
}
void keyboardListener(unsigned char key,int x,int y){
    switch (key) {
        case 'w':
            pos.x += 2*cos(rotateAngle*acos(-1.0)/180.0) ;
            pos.y += 2*sin(rotateAngle*acos(-1.0)/180.0) ;
            distance += 2 ;
            break;
        case 's':
            pos.x -= 2*cos(rotateAngle*acos(-1.0)/180.0) ;
            pos.y -= 2*sin(rotateAngle*acos(-1.0)/180.0) ;
            distance -= 2 ;
            break;
            break;
        case 'a':
            rotateAngle += 5 ;
            break;
        case 'd':
            rotateAngle -= 5 ;
            break;
        default:
            break;
    }
}
void specialKeyListener(int key,int x,int y){
    switch (key) {
        case GLUT_KEY_DOWN:        //down arrow key
            cameraHeight -= 3.0;
            break;
        case GLUT_KEY_UP:        // up arrow key
            cameraHeight += 3.0;
            break;
            
        case GLUT_KEY_RIGHT:
            cameraAngle += 0.03;
            break;
        case GLUT_KEY_LEFT:
            cameraAngle -= 0.03;
            break;
        default:
            break;
    }
}


int main(int argc, char **argv)
{
    glutInit(&argc,argv);
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