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
            cout<<s<<endl;
        }
    }

    cin>>scene.numLightSources;
    for (int i = 0; i < scene.numLightSources; i++) {
        Light l;
        cin>>l;
        scene.lights.push_back(l);
        cout<<l<<endl;
    }
   
    Object * floor = new Floor(TILE_WIDTH, camera.farPlane);

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
    cout<<"init done"<<endl;
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
    cout<<"display done"<<endl;
    glutIdleFunc(animate);
    cout<<"idle done"<<endl;

   // registerSpecialKeysListener(specialKeyListener);
   // registerKeyboardListener(keyboardListener);

    glutMainLoop();
    cout<<"main loop done"<<endl;

    return 0;
}
