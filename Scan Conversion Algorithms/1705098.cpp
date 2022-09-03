#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glut.h>
#include "bitmap_image.hpp"
#include "Color.h"

using namespace std;

int window_height, window_width;
vector<vector<Color>> pixelBuffer;

#define PI acos(-1)

int round_(double d)
{
    return floor(d + 0.5);
}

void plot(int x, int y, double intensity)
{
    int tmp = y;
    y = x;
    x = window_height - tmp;

    pixelBuffer[x][y].set_r(intensity);
    pixelBuffer[x][y].set_g(intensity);
    pixelBuffer[x][y].set_b(intensity);
}

double filter(double distance)
{
    return 1 - pow(distance * 2 / 3, 2);
}

void intensifyPixelGupta(int x, int y, double distance)
{
    int intensity = filter(round_(abs(distance)));

    int tmp = y;
    y = x;
    x = window_height - tmp;

    pixelBuffer[x][y].set_r(intensity);
    pixelBuffer[x][y].set_g(intensity);
    pixelBuffer[x][y].set_b(intensity);
}

void intensifyPixelUA(int xp, int yp, double m, double c)
{
    int pixels_inside = 0;
    int total_pixels = 9;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            double x = xp + (1.0 / 6.0) + i * (1.0 / 3.0);
            double y = yp + (1.0 / 6.0) + j * (1.0 / 3.0);

            double y_ = m * x + c;
            double y_up = y_ + 0.5 / cos(atan(m));
            double y_down = y_ - 0.5 / cos(atan(m));
            if (y >= y_down && y <= y_up)
            {
                pixels_inside++;
            }
        }
    }

    double dA = ((1.0 * pixels_inside) / 9.0);
    double alpha = 1 - dA;

    double intensity = alpha * 255.0;

    int tmp = yp;
    yp = xp;
    xp = window_height - tmp;

    pixelBuffer[xp][yp].set_r(intensity);
    pixelBuffer[xp][yp].set_g(intensity);
    pixelBuffer[xp][yp].set_b(intensity);
}

void load_buffers()
{
    for (int i = 0; i < window_width; i++)
    {
        vector<Color> rowP;
        for (int j = 0; j < window_height; j++)
        {
            Color c(255, 255, 255);
            rowP.push_back(c);
        }
        pixelBuffer.push_back(rowP);
    }
    // cout<<"pixellbuffer sz: "<<pixelBuffer.size()<<endl;
}

void generate_image(string img_name)
{
    cout << "generating image" << endl;

    bitmap_image image(window_width, window_height);
    for (int i = 0; i < window_width; i++)
    {
        for (int j = 0; j < window_height; j++)
        {
            auto color = pixelBuffer.at(i).at(j);
            image.set_pixel(j, i, color.get_r(), color.get_g(), color.get_b());
        }
    }
    image.save_image(img_name + ".bmp");

    // clear pixelBuffer
    for(int i=0;i<pixelBuffer.size();i++){
        pixelBuffer[i].clear();
    }
    pixelBuffer.clear();
    load_buffers();
}

void MidpointLineAlgo(int pntX1, int pntY1, int pntX2, int pntY2)
{
    int dx = (pntX2 - pntX1),
        dy = (pntY2 - pntY1);
    int x = pntX1;
    int y = pntY1;

    if(dy<=dx){
        int d = 2 * dy - dx;
        int incrE = 2 * dy;
        int incrNE = 2 * (dy - dx);

        plot(x, y, 0.0);

        while (x < pntX2)
        {
            if (d <= 0)
            {
                d += incrE;
            }
            else
            {
                d += incrNE;
                y++;
            }
            x++;
            plot(x, y, 0.0);
        }
    }
    else{
        int d = 2 * dx - dy;
        int incrE = 2 * dx;
        int incrNE = 2 * (dx - dy);

        plot(x, y, 0.0);
        while (y < pntY2)
        {
            if (d <= 0)
            {
                d += incrE;
            }
            else
            {
                d += incrNE;
                x++;
            }
            y++;
            plot(x, y, 0.0);
        }
    }
    
}

void gupta_sproull(int pntX1, int pntY1, int pntX2, int pntY2)
{
    int dx = (pntX2 - pntX1),
        dy = (pntY2 - pntY1);
    int x = pntX1;
    int y = pntY1;

    if(dy<=dx){
        int d = 2 * dy - dx;
        int incrE = 2 * dy;
        int incrNE = 2 * (dy - dx);

        int two_v_dx = 0;
        double inv_d = 1.0 / (2 * sqrt(dx * dx + dy * dy));
        double two_dx_inv_d = two_v_dx * inv_d;

        

        intensifyPixelGupta(x, y, 0.0);
        intensifyPixelGupta(x, y + 1, two_dx_inv_d); // neighbour

        intensifyPixelGupta(x, y - 1, two_dx_inv_d); // neighbour

        while (x < pntX2)
        {
            if (d <= 0)
            {
                two_v_dx = d + dx;
                d += incrE;
            }
            else
            {
                two_v_dx = d - dx;
                d += incrNE;
                y++;
            }
            x++;

            intensifyPixelGupta(x, y, 0.0);
            intensifyPixelGupta(x, y + 1, two_dx_inv_d - two_v_dx * inv_d); // neighbour
            intensifyPixelGupta(x, y - 1, two_dx_inv_d + two_v_dx * inv_d); // neighbour
        }
    }
    else{
        int d = 2 * dx - dy;
        int incrE = 2 * dx;
        int incrNE = 2 * (dx - dy);

        int two_v_dx = 0;
        double inv_d = 1.0 / (2 * sqrt(dx * dx + dy * dy));
        double two_dx_inv_d = two_v_dx * inv_d;

        

        intensifyPixelGupta(x, y, 0.0);
        intensifyPixelGupta(x+1, y, two_dx_inv_d); // neighbour

        intensifyPixelGupta(x-1, y , two_dx_inv_d); // neighbour

        while (y < pntY2)
        {
            if (d <= 0)
            {
                two_v_dx = d + dy;
                d += incrE;
            }
            else
            {
                two_v_dx = d - dy;
                d += incrNE;
                x++;
            }
            y++;

            intensifyPixelGupta(x, y, 0.0);
            intensifyPixelGupta(x+1, y, two_dx_inv_d - two_v_dx * inv_d); // neighbour
            intensifyPixelGupta(x-1, y, two_dx_inv_d + two_v_dx * inv_d); // neighbour
        }
    }
    
}

void unweightedAliasing(int pntX1, int pntY1, int pntX2, int pntY2)
{
    int dx = (pntX2 - pntX1),
        dy = (pntY2 - pntY1);

    int x = pntX1;
    int y = pntY1;

    if (dy <= dx)
    {
        int d = 2 * dy - dx;
        int incrE = 2 * dy;
        int incrNE = 2 * (dy - dx);

        double m = 1.0 * dy / dx;
        double c = pntY2 * 1.0 - pntX2 * m;

        intensifyPixelUA(x, y, m, c);
        intensifyPixelUA(x, y + 1, m, c); // neighbour
        intensifyPixelUA(x, y - 1, m, c); // neighbour

        while (x < pntX2)
        {
            if (d < 0)
            {
                d += incrE;
            }
            else
            {
                d += incrNE;
                y++;
            }
            x++;

            intensifyPixelUA(x, y, m, c);
            intensifyPixelUA(x, y + 1, m, c); // neighbour
            intensifyPixelUA(x, y - 1, m, c); // neighbour
        }
    }
    else
    {
        int d = 2 * dx - dy;
        int incrE = 2 * dx;
        int incrNE = 2 * (dx - dy);

        double m = 1.0 * dy / dx;
        double c = pntY2 * 1.0 - pntX2 * m;

        intensifyPixelUA(x, y, m, c);
        intensifyPixelUA(x + 1, y, m, c); // neighbour
        intensifyPixelUA(x - 1, y, m, c); // neighbour

        while (y < pntY2)
        {
            if (d < 0)
            {
                d += incrE;
            }
            else
            {
                d += incrNE;
                x++;
            }
            y++;

            intensifyPixelUA(x, y, m, c);
            intensifyPixelUA(x + 1, y, m, c); // neighbour
            intensifyPixelUA(x - 1, y, m, c); // neighbour
        }
    }
}

int main(int argc, char **argv)
{

    FILE *fp = freopen("input.txt", "r", stdin);
    if (fp == NULL)
    {
        cout << "file not found" << endl;
        return 0;
    }

    int pntX1, pntY1;
    int pntX2, pntY2;

    cin >> window_width >> window_height;

    load_buffers();

    int n;
    cin >> n;
    vector<vector<int>> points;
    for (int i = 0; i < n; i++)
    {
        cin >> pntX1 >> pntY1 >> pntX2 >> pntY2;
        vector<int> point;
        point.push_back(pntX1);
        point.push_back(pntY1);
        point.push_back(pntX2);
        point.push_back(pntY2);

        points.push_back(point);
    }
    //print points
    for (int i = 0; i < n; i++){
        cout << points[i][0] << " " << points[i][1] << " " << points[i][2] << " " << points[i][3] << endl;
    }
        

    for (int i = 0; i < n; i++)
    {
        pntX1 = points[i][0];
        pntY1 = points[i][1];
        pntX2 = points[i][2];
        pntY2 = points[i][3];

        MidpointLineAlgo(pntX1, pntY1, pntX2, pntY2);
    }
    generate_image("1_R");

    for (int i = 0; i < n; i++)
    {
        pntX1 = points[i][0];
        pntY1 = points[i][1];
        pntX2 = points[i][2];
        pntY2 = points[i][3];

        unweightedAliasing(pntX1, pntY1, pntX2, pntY2);
    }

    generate_image("2_RUA");

    for (int i = 0; i < n; i++)
    {
        pntX1 = points[i][0];
        pntY1 = points[i][1];
        pntX2 = points[i][2];
        pntY2 = points[i][3];

        gupta_sproull(pntX1, pntY1, pntX2, pntY2);
    }

    generate_image("3_RWA");

   

    return 0;

}
