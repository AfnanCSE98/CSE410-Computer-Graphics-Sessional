#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include "Triangle.h"
#include <iostream>
#include <fstream>

using namespace std;

class zbuffer{
    ofstream osZBuffer;
    double screenWidth, screenHeight;
    double xLimit, yLimit;
    double zRear, zFront;
    double dx, dy;

    vector<Triangle> triangles;
    vector<vector<double>> zBuffer;
    vector<vector<Color>> pixelBuffer;

public:

    zbuffer(const char* input_file_config , const char* input_file_stage3 ){

        read_config_file(input_file_config);

        osZBuffer.open("Outputs/z-buffer.txt");
        osZBuffer << setprecision(7) << fixed;

        load_buffers();
        readStage3File(input_file_stage3);
    }

    void readStage3File(const char* input_file_stage3) {
        FILE *fp = freopen(input_file_stage3, "r", stdin);
        if (fp == nullptr) {
            cout << "Error: Input File Not Found" << endl;
            return;
        }

        int i = 0;

        while (true) {
            Vector p1, p2, p3;
            cin >> p1;
            if (cin.eof()) break;
            cin >> p2;
            if (cin.eof()) break;
            cin >> p3;
            triangles.push_back(Triangle(p1, p2, p3));
            if (cin.eof()) {break;}
        
        }
        fclose(fp);

        cout<<"Triangles Found: "<<triangles.size()<<std::endl;

    }

    void read_config_file(const char* input_file_config){
        FILE *fp = freopen(input_file_config, "r", stdin);

        if (fp == nullptr) {
            cout << "Error: Input File Not Found" << endl;
            return;
        }

        cin >> screenWidth >> screenHeight;
        cin >> xLimit;
        cin >> yLimit;

        xLimit = -xLimit;
        yLimit = -yLimit;

        dx = 2*xLimit/ screenWidth;
        dy = 2*yLimit / screenHeight;

        cin >> zFront >> zRear;

        cin.clear();

        fclose(fp);

        cout<<"SW:"<<screenWidth<<" SH:"<<screenHeight<<" xLim:"<<xLimit<<" yLim:"<<yLimit<<" zFront:"<<zFront<<" zRear:"<<zRear<<std::endl;
  
    }

    void load_buffers(){
        vector<double> rowZ((int)screenWidth, zRear);

        // initialize zbuffer with the vaue of zRear
        for (int i = 0; i < screenHeight; i++) {
            zBuffer.push_back(rowZ);
        }

        for (int i = 0; i < screenHeight; i++) {
            vector<Color> rowP;
            for (int j = 0; j < screenWidth; j++) {
                rowP.push_back(Color(0,0,0));
            }
            pixelBuffer.push_back(rowP);
        }

        cout<<"zbuffer sz "<<zBuffer.size()<<endl;
        cout<<"pixellbuffer sz: "<<pixelBuffer.size()<<endl;
        

    }

    vector<pair<Vector, Vector>> getIntersectingEdges(double yScanline, Triangle & t) {
        vector<pair<Vector, Vector>> result;
        // p1 p2
        if((t.get_p1().y >= yScanline && t.get_p2().y <= yScanline)) {
            result.push_back(pair<Vector, Vector>(t.get_p1(), t.get_p2()));
        }
        else if(t.get_p1().y <= yScanline && t.get_p2().y >= yScanline) {
            result.push_back(pair<Vector, Vector>(t.get_p2(), t.get_p1()));
        }

        //p2 p3
        if (t.get_p2().y >= yScanline && t.get_p3().y <= yScanline) {
            result.push_back(pair<Vector, Vector>(t.get_p2(), t.get_p3()));
        }
        else if ((t.get_p2().y <= yScanline && t.get_p3().y >= yScanline)) {
            result.push_back(pair<Vector, Vector>(t.get_p3(), t.get_p2()));
        }

        //p1 p3
        if ((t.get_p1().y >= yScanline && t.get_p3().y <= yScanline)) {
            result.push_back(pair<Vector, Vector>(t.get_p1(), t.get_p3()));
        }
        else if (t.get_p1().y <= yScanline && t.get_p3().y >= yScanline) {
            result.push_back(pair<Vector, Vector>(t.get_p3(), t.get_p1()));
        }

        return result;
    }

    void load_z_x(pair<Vector, Vector> e1 , pair<Vector, Vector> e2 ,double yScanline
                    , double& za,double& zb,double& xa,double& xb){
            
        double m1 = (yScanline - e1.first.y) / (e1.second.y - e1.first.y);
        za = e1.first.z + m1 * (e1.second.z - e1.first.z);

        double m2 = (yScanline - e2.first.y) / (e2.second.y - e2.first.y);
        zb = e2.first.z + m2 * (e2.second.z - e2.first.z);

        xa = e1.first.x + m1 * (e1.second.x - e1.first.x);
        xb = e2.first.x + m2 * (e2.second.x - e2.first.x);
    }

    void apply_procedure(){
        int topScanLine,bottomScanLine;

        for(Triangle t : triangles){
            topScanLine = round((yLimit - min(t.getMax_Y(), yLimit)) / dy);
            bottomScanLine  = round((yLimit - max(t.getMin_Y(), -yLimit)) / dy);

            double yScanline = min(t.getMax_Y(), yLimit);
            for(int row = topScanLine; row<bottomScanLine ; row++){

                auto edges = getIntersectingEdges(yScanline, t);

                if (edges.size() != 2) {
                    yScanline -= dy;
                    continue;
                }
                auto e1 = edges.at(0);
                auto e2 = edges.at(1);

                double za,zb,xa,xb=9;;
                load_z_x(e1 , e2 , yScanline , za , zb , xa , xb);
                //cout<<"xb = ";
                
                
                // keeping smaller valu in xa and za
                if (xb < xa) {
                    double temp = xa;
                    xa = xb;
                    xb = temp;

                    temp = za;
                    za = zb;
                    zb = temp;
                }

                //clipping
                xa = max(xa, -xLimit);
                xb = min(xb, xLimit);

                int leftIntersectingCol = round((xLimit + xa) / dx);
                int rightIntersectingCol = round((xLimit + xb) / dx);

                double m3 = (zb - za) / (xb - xa);
                double inc = dx * m3;
                double zp = za;
                for (int col = leftIntersectingCol; col < rightIntersectingCol; col++) {
                    if (zp > zFront && zp < zBuffer.at(row).at(col)) {
                        //setZBufferValue
                        zBuffer.at(row).at(col) = zp;
                        //setPixelBufferVal
                        pixelBuffer.at(row).at(col) = t.get_color();

                    }
                    zp = zp + inc;
                }
                yScanline -= dy;

            }
         }
    }

    void generate_output(){
        //zbuffer.txt
        for (auto & r: zBuffer) {
            for (auto & data: r) {
                if (data < zRear) {
                    osZBuffer<<data<<"    ";
                }
            }
            osZBuffer<<'\n';
        }

        //bitmap img
        bitmap_image image((int)screenWidth, (int)screenHeight);

        for(int i=0;i<screenWidth;i++){
            for(int j=0;j<screenHeight;j++){
                auto color = pixelBuffer.at(i).at(j);
                image.set_pixel(j,i, color.get_r(), color.get_g(), color.get_b());
            }
        }
        image.save_image("Outputs/out.bmp");
    }
};