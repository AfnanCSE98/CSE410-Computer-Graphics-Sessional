#include <bits/stdc++.h>
#include "1705098_objects.h"
#define TILE_WIDTH 20

Camera camera;
Scene scene;
//RayTracingCapturer rayTracingCapturer(scene);
int numCaptures = 0;

void display() {

    clearDisplay();

    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z, camera.pos.x + camera.l.x, camera.pos.y + camera.l.y, camera.pos.z + camera.l.z, camera.u.x, camera.u.y, camera.u.z);

    glMatrixMode(GL_MODELVIEW);

    drawAxes(9999);

    scene.draw();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate() {
    glutPostRedisplay();
}

void keyboardListener(unsigned char key, int x, int y) {

    switch (key) {

        case '1':
        //look left
            camera.l = camera.l.rotate(camera.u, 1.5);
            camera.r = camera.l * camera.u;
            //camera.print();
            break;
        case '2':
        //look right
            camera.l = camera.l.rotate(camera.u, -1.5);//neg angle means rotation anticlockwise
            camera.r = camera.l * camera.u;
            //camera.print();
            break;
        case '3':
        //look up
            camera.l = camera.l.rotate(camera.r, 1.5);
            camera.u = camera.r * camera.l;
            camera.print();
            break;
        case '4':
        //look down
            camera.l = camera.l.rotate(camera.r, -1.5);//neg angle means rotation anticlockwise
            camera.u = camera.r * camera.l;
            //camera.print();
            break;
        case '5':
        //tilt clockwise
            camera.r = camera.r.rotate(camera.l, -1.5);//neg angle means rotation anticlockwise
            camera.u = camera.r * camera.l;
            //camera.print();
            break;
        case '6':
        //tilt counter-clockwise
            camera.r = camera.r.rotate(camera.l, 1.5);
            camera.u = camera.r * camera.l;
            //camera.print();
            break;
        case '0':
            string f = "1705098_" + to_string(numCaptures++) + ".bmp";
            cout<<"CAPTURING: "<<f<<"\n";
            //rayTracingCapturer.capture(f, 500, 500, scene.pixels, scene.pixels, camera);
            cout<<"COMPLETE"<<endl;
            break;
    }
}

void specialKeyListener(int key, int x, int y) {

    switch (key) {

        case GLUT_KEY_UP:
        //move forward
            camera.pos = camera.pos + camera.l;
            //camera.print();
            break;

        case GLUT_KEY_DOWN:
        //move backward
            camera.pos = camera.pos - camera.l;
            //camera.print();
            break;

        case GLUT_KEY_RIGHT:
        //move right
            camera.pos = camera.pos + camera.r;
            //camera.print();
            break;

        case GLUT_KEY_LEFT:
        //move left
            camera.pos = camera.pos - camera.r;
            //camera.print();
            break;

        case GLUT_KEY_PAGE_UP:
        //move up
            camera.pos = camera.pos + camera.u;
            //camera.print();
            break;

        case GLUT_KEY_PAGE_DOWN:
        //move down
            camera.pos = camera.pos - camera.u;
            //camera.print();
            break;

        default:
            break;

    }

}


void loadData() {

    FILE *fp = freopen("scene.txt", "r", stdin);

    if (fp == nullptr) {
        cout << "Error: Input File Not Found" << endl;
        return;
    }

    cin>>scene.recursionLevels;
    cin>>scene.pixels;
    cin>>scene.numObjects;

    for (int i = 0; i < scene.numObjects; ++i) {
        string type;
        cin>>type;
        Object * obj;
        if (type == "sphere") {
            Sphere s;
            cin>>s;
            obj = new Sphere(s.reference_point, s.color, s.radius, s.getAmbient(), s.getDiffuse(), s.getSpecular(), s.getReflection(), s.getShine());
            scene.objects.push_back(obj);
            cout<<type<<endl;
        }

    }

    cin>>scene.numLightSources;
    for (int i = 0; i < scene.numLightSources; i++) {
        Light l;
        cin>>l;
        scene.lights.push_back(l);
        
    }
   
    Object * floor = new Floor(TILE_WIDTH, 60);

    floor->setAmbient(0.35);
    floor->setDiffuse(0.25);
    floor->setSpecular(0.15);
    floor->setReflection(0.35);
    floor->setShine(1);

    scene.numObjects += 1;
    scene.objects.push_back(floor);
  
}

void init() {
    clearDisplay();
    loadData();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.fovy, camera.aspectRatio, camera.nearPlane, camera.farPlane);
}


int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

    glutCreateWindow("Ray Tracing");

    init();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(animate);

    registerSpecialKeysListener(specialKeyListener);
    registerKeyboardListener(keyboardListener);

    glutMainLoop();

    return 0;
}
