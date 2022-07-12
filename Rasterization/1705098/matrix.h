#include<bits/stdc++.h>
#include "vector.h"
#define PI 3.1416

using namespace std;

class Matrix{

public:
    vector<vector<double>>data;
    
    Matrix() = default;

   // Matrix(Matrix m) {
   //     this->data = m.data;
   // }

    Matrix(int r, int c) {
        vector<double>row(c, 0);
        for (int i = 0; i < r; i++) {
            data.push_back(row);
        }
    }

    double getW() {
        return data[data.size() - 1][data[0].size() - 1];
    }

    void setW(double val) {
        data[data.size() - 1][data[0].size() - 1] = val;
    }


    Matrix zeroMatrix(int r , int c){
        Matrix m(r,c);
        return m;
    }

    Matrix operator*(const Matrix &rhs){
        int r1 = (int)(*this).data.size();
        int r2 = (int)rhs.data.size();

        int c1 = (int)(*this).data[0].size();
        int c2 = (int)rhs.data[0].size();

        if (c1 != r2) {
            cout << "Invalid Matrix Multiplication: " << r1 << "x" << r2 << c1 << "x" << c2 << endl;
            return zeroMatrix(r1, c1);
        }

        Matrix result = zeroMatrix(r1, c2);

        double d;
        for (int i = 0; i < r1; i++) {
            for (int j = 0; j < c2; j++) {
                for (int k = 0; k < c1; k++) {
                    d = (*this).data[i][k] * rhs.data[k][j];
                    result.data[i][j] = result.data[i][j] + d;
                }
            }
        }

        return result;
    }

    Matrix identityMatrix(int r ,int c){
        Matrix m(r,c);
        for(int i=0 ; i<r ; i++){
            m.data[i][i] = 1;
        }

        return m;
    }

    Matrix columnMatrix(Vector &v){
        
        Matrix m(4,1);
        
        m.data[0][0] = v.x;
        m.data[1][0] = v.y;
        m.data[2][0] = v.z;
        m.data[3][0] = 1;
        
        return m;
    }

    Matrix translationMatrix(double x , double y , double z){

        Matrix m = identityMatrix(4,4);
        
        m.data[0][3] = x;
        m.data[1][3] = y;
        m.data[2][3] = z;
        
        return m;
    }

    Matrix scalingMatrix(double x , double y , double z){

        Matrix m(4,4);
        
        m.data[0][0] = x;
        m.data[1][1] = y;
        m.data[2][2] = z;

        m.setW(1);
        
        return m;
    }

    void printMatrix(){
        for(int i=0;i<data.size() ; i++)
        {
            for(int j = 0; j<data[0].size() ; j++){
                cout<<data[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    Matrix rotationMatrix(Vector col0, Vector col1, Vector col2) {

        Matrix m(4, 4);

        vector<Vector> columns;
        columns.push_back(col0);
        columns.push_back(col1);
        columns.push_back(col2);

        for (int col = 0; col < 3; col++) {
            m.data[0][col] = columns[col].x;
            m.data[1][col] = columns[col].y;
            m.data[2][col] = columns[col].z;
        }

        m.setW(1);
        
        return m;
    }

    Matrix viewTransformation(Vector &eye, Vector &look, Vector &up) {

        Vector l = (look - eye).normalize();
        Vector r = (l * up).normalize();
        Vector u = (r * l).normalize();

        Matrix R = rotationMatrix(Vector(r.x, u.x, -l.x), Vector(r.y, u.y, -l.y), Vector(r.z, u.z, -l.z));

        Vector negatedEye = -eye;
        Matrix T = translationMatrix(negatedEye.x, negatedEye.y, negatedEye.z);

        return R*T;
    }

    static Matrix projection(double near, double far, double fovY, double aspectRatio) {
        Matrix res(4, 4);
        double fovX = fovY * aspectRatio;

        res.data[0][0] = 1 / tan((fovX / 2)*PI/180.0);
        res.data[1][1] = 1 / tan((fovY / 2)*PI/180.0);
        res.data[2][2] = -(far + near) / (far - near);
        res.data[2][3] = -(2 * far * near) / (far - near);
        res.data[3][2] = -1;

        return res;
    }

};