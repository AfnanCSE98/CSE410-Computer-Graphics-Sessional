#include "matrix.h"
#include <fstream>
#include <stack>
#include <iomanip>
#include <string>

using namespace std;

class InputHandler {
private:
    ofstream osStage1, osStage2, osStage3;
    stack <Matrix> currentStack;
    stack <stack<Matrix>> history;
    Matrix VTM;
    Matrix PM;
    Matrix m;
    Vector v;
public:

    InputHandler(const char *input_scene_file = "scene.txt") {

        m = Matrix(4,4);
        v = Vector();

        FILE *fp = freopen(input_scene_file, "r", stdin);

        if (fp == nullptr) {
            cout << "Error: Input File Not Found" << endl;
            return;
        }

        osStage1.open("Outputs/stage1.txt");
        osStage2.open("Outputs/stage2.txt");
        osStage3.open("Outputs/stage3.txt");

        osStage1 << setprecision(7) << fixed; 
        osStage2 << setprecision(7) << fixed; 
        osStage3 << setprecision(7) << fixed;

        Vector eye, look, up;
        cin >> eye >> look >> up;
        double fovY, aspectRatio, near, far;
        cin >> fovY >> aspectRatio >> near >> far;
        
        VTM = m.viewTransformation(eye, look, up);
        PM = m.projection(near, far, fovY, aspectRatio);

        currentStack.push(m.identityMatrix(4, 4));
    }

    void processInput() {

        string instruction;
        while (true) {
            cin >> instruction;

            if (instruction == "triangle") {
                processTriangle();
            } else if (instruction == "translate") {
                processTranslate();
            } else if (instruction == "scale") {
                processScale();
            } else if (instruction == "rotate") {
                processRotate();
            } else if (instruction == "push") {
                history.push(currentStack);
            } else if (instruction == "pop") {
                currentStack = history.top();
                history.pop();
            } else if (instruction == "end") {
                cin.clear();
                break;
            }
        }

        osStage1.close();
        osStage2.close();
        osStage3.close();
    }

    void processTriangle() {
        Vector points[3];
        cin >> points[0];
        cin >> points[1];
        cin >> points[2];

        for (int i=0; i < 3; i++) {

            Vector model = transformPoint(points[i], currentStack.top());
            Vector view = transformPoint(model, VTM);
            Vector projection = transformPoint(view, PM);

            osStage1 << model; osStage2 << view; osStage3 << projection;
        }

        osStage1 << endl; osStage2 << endl; osStage3 << endl;
    }

    void processTranslate() {
        double dx, dy, dz;
        cin >> dx >> dy >> dz;

        Matrix t = m.translationMatrix(dx, dy, dz);

        currentStack.push(currentStack.top() * t);
    }

    void processScale() {
        double sx, sy, sz;
        cin >> sx >> sy >> sz;

        Matrix t = m.scalingMatrix(sx ,sy , sz);

        currentStack.push(currentStack.top() * t);
    }

    void processRotate() {
        double angle;
        Vector axis;
        cin >> angle >> axis;

        axis = axis.normalize();

        auto col1 = rodrigues(v.I(), axis, angle);
        auto col2 = rodrigues(v.J(), axis, angle);
        auto col3 = rodrigues(v.K(), axis, angle);

        Matrix r = m.rotationMatrix(col1, col2, col3);

        currentStack.push(currentStack.top() * r);
    }

    Vector transformPoint(Vector point, Matrix &T) {

        Matrix res = T * m.columnMatrix(point);
        return Vector(res.data[0][0], res.data[1][0], res.data[2][0]) / res.getW();
    }

    Vector rodrigues(Vector x, Vector a, double angle) {

        double cost = cos((angle)*PI/180.0);
        double sint = sin((angle)*PI/180.0);

        return x * cost  + a * ((1 - cost) * a.dot(x)) + (a * x) * sint;
    }
};