#ifndef OFFLINE_1705098_HEADERS_H
#define OFFLINE_1705098_HEADERS_H

#endif
#include <GL/glut.h>
#include "1705098_bitmap_image.h"
#include <bits/stdc++.h>
using namespace std;


class Object;
class Light;
class Point;

#define PI acos(-1)
const int INF = 1e4;
const double EPS = 1e-7;
const double RGB = 255;


const int cam_pos_x = 100;
const int cam_pos_y = 100;
const int cam_pos_z = 50;

static int REFRACTION_ON;
static int reflection_level, pixels, total_objects, total_lights;

static vector<Object*>objects;
static vector<Light>lights;

const int floor_width = 1000;
const int tile_width = 20;

//------------------------------------------------------------------------------point----------------------------------------------------------------------------------------------

class Point {
public:

    double x,y,z;

    Point(){}

    Point(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void normalize() {
        double down = x*x + y*y + z*z;
        down = sqrt(down);
        x /= down;
        y /= down;
        z /= down;
    }

    friend ostream& operator<<(ostream& os, Point& v) {
        os<<" ( ";
        os<<v.x<<" "<<v.y<<" "<<v.z;
        os<<" ) ";
        return os;
    }

    friend istream& operator>>(istream& is, Point& v) {
        is >> v.x ;
        is >> v.y ;
        is >> v.z ;
        return is;
    }


    Point mul(double val) {
        Point ret = *this;
        ret.x *= val;
        ret.y *= val;
        ret.z *= val;
        return ret;
    }

    Point div(double val) {
        Point ret = *this;
        ret.x /= val;
        ret.y /= val;
        ret.z /= val;
        return ret;
    }

    Point add(Point v) {
        Point ret;
        ret.x = (*this).x + v.x;
        ret.y = (*this).y + v.y;
        ret.z = (*this).z + v.z;
        return ret;
    }

    Point sub(Point v) {
        Point ret;
        ret.x = (*this).x - v.x;
        ret.y = (*this).y - v.y;
        ret.z = (*this).z - v.z;
        return ret;
    }

};

//------------------------------------------------------ray----------------------------------------------------------------
class Ray {
public:
    Point start;
    Point dir;
    Ray(){

    }
    //setter getter for start and dir
    void setStart(Point start) {
        this->start = start;
    }

    void setDir(Point dir) {
        this->dir = dir;
    }

    Point getStart() {
        return start;
    }

    Point getDir() {
        return dir;
    }

    Ray(const Point &from, Point direction) {
        start = from;
        dir = direction;
        dir.normalize();
    }
};

//------------------------------------------------------color----------------------------------------------------------------
class Color {
public:
    double rgb[3];
    Color() {
        memset(rgb, 0, sizeof rgb);
    }

    Color(double r, double g, double b) : Color() {
        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
    }

    void clear() {
        memset(rgb, 0, sizeof rgb);
    }

    void clamp() {
        for(double &x: rgb) {
            if(x > 1) x = 1;
            else if(x < 0) x = 0;
        }
    }

    double& operator[] (int id) {
        return rgb[id];
    }

    Color operator* (const double val) {
        Color ret = *this;
        for(int i = 0 ; i < 3 ; i++) ret[i] = rgb[i] * val;
        return ret;
    }

    Color operator/ (const double val) {
        Color ret = *this;
        for(int i = 0 ; i < 3 ; i++) ret[i] = rgb[i] / val;
        return ret;
    }

    Color operator* (Color col) {
        Color ret = *this;
        for(int i = 0 ; i < 3 ; i++) ret[i] = rgb[i] * col[i];
        return ret;
    }

    Color operator+= (Color col) {
        for(int i = 0 ; i < 3 ; i++) rgb[i] += col[i];
        return *this;
    }

    friend istream& operator>>(istream& is, Color& color) {
        for(int i = 0 ; i <3; i++ ) is >> color[i] ;
        return is;
    }

    friend ostream& operator<<(ostream& os, Color& c) {
        os<<" ( "<<c[0]<<" , "<<c[1]<<" , "<<c[2]<<" )";
        return os;
    }

};

//------------------------------------------------------light----------------------------------------------------------------
class Light  {
public:
    Point light_pos;
    Color color;
    bool is_spotlight;
    Point dir;
    double cutoff_angle;

    void draw() {
        glPointSize(5);
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_POINTS);
        glVertex3f(light_pos.x, light_pos.y, light_pos.z);
        glEnd();
    }

    //setter for dir,is_spotlight,cutoff_angle
    void set_spotlight(bool is_spotlight, Point dir, double cutoff_angle) {
        this->is_spotlight = is_spotlight;
        this->dir = dir;
        this->dir.normalize();
        this->cutoff_angle = cutoff_angle;
    }

    //getter for cutoff_angle
    double get_cutoff_angle() {
        return cutoff_angle;
    }

};

class util {
    public:

    static double distance(Point p1, Point p2) {
        double ret = 0;
        ret += (p1.x - p2.x) * (p1.x - p2.x);
        return sqrt(ret);
    }

    static double dot(Point v1, Point v2) {
        double ret = 0;
        ret += v1.x * v2.x;
        ret += v1.y * v2.y;
        ret += v1.z * v2.z;
        return ret;
    }

    //get angle betwen two point
    static double get_angle(Point p1, Point p2) {
        double ret = dot(p1, p2);
        return acos(ret);
    }

    static Point cross(Point v1, Point v2) {
        Point ret;
        ret.x = v1.y * v2.z - v1.z * v2.y;
        ret.y = -v1.x * v2.z + v1.z * v2.x;
        ret.z = v1.x * v2.y - v1.y * v2.x;
        return ret;
    }

    static Point get_reflection_vector(Point &L, Point &N) {
        Point ret = N.mul(2.0).mul(dot(L, N)).sub(L);
        ret.normalize();
        return ret;
    }

    /*
    https://stackoverflow.com/a/58676386/11837247
    */
    static Point get_refraction_vector(Point &I, Point &N, Point &intersecting_point) {
        double eta = 1.5;
        double N_dot_I = dot(N, I);
        double k = 1.0 - eta * eta * (1.0 - N_dot_I * N_dot_I);

        return I.mul(eta).sub(N.mul(N_dot_I * eta - sqrt(1 - k)));
    }

    //following 4 func are for triangle class
    static double get_determinant(double a[3][3]) {
        return a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
    }

    static double get_beta(Ray &ray , double D , Point A , Point C) {
         double mat[3][3] = {
                { A.x - ray.start.x , A.x - C.x , ray.dir.x },
                { A.y - ray.start.y , A.y - C.y , ray.dir.y },
                { A.z - ray.start.z , A.z - C.z , ray.dir.z },
        };
        double beta = get_determinant(mat) / D;
        return beta;
    }

    static double get_gamma(Ray &ray , double D , Point A , Point B) {
        double mat[3][3] = {
                { A.x - B.x , A.x - ray.start.x , ray.dir.x },
                { A.y - B.y , A.y - ray.start.y , ray.dir.y },
                { A.z - B.z , A.z - ray.start.z , ray.dir.z },
        };
        double gamma = get_determinant(mat) / D;
        return gamma;
    }

    static double get_t(Ray &ray , double D , Point A , Point B , Point C) {
        double mat[3][3] = {
                { A.x - B.x , A.x - C.x , A.x - ray.start.x },
                { A.y - B.y , A.y - C.y , A.y - ray.start.y },
                { A.z - B.z , A.z - C.z , A.z - ray.start.z },
        };

        double t = get_determinant(mat) / D;
        return t;
    }

    //for general surface
    static double getSol(double a, double b, double c, double &t1, double &t2){
        double d2 = b*b-4*a*c;
        if(d2 < 0) return -1;

        double d = sqrt(d2);
        t1 = (-b-d)/(2*a);
        t2 = (-b+d)/(2*a);

        if(t1 > 0) return t1;
        if(t2 > 0) return t2;

        return -1;
    }
};


//------------------------------------------------------Object----------------------------------------------------------------
class Object{
public:
    Color object_color;
    double ambient_coEfficient;
    double diffuse_coEfficient;
    double specular_coEfficient;
    double reflection_coEfficient;
    int shine;

    virtual void draw() = 0;
    virtual Point get_normal(Point pt) = 0;
    virtual double get_intersecting_t(Ray &ray) = 0;
    virtual Color get_object_color(Point &p) = 0;

    virtual bool is_inside(Point &pt) {
        return false;
    }

    //setter for ambient_coEfficient, diffuse_coEfficient, specular_coEfficient, reflection_coEfficient, shine
    void set_ambient_coEfficient(double val) {
        ambient_coEfficient = val;
    }

    void set_diffuse_coEfficient(double val) {
        diffuse_coEfficient = val;
    }

    void set_specular_coEfficient(double val) {
        specular_coEfficient = val;
    }

    void set_reflection_coEfficient(double val) {
        reflection_coEfficient = val;
    }

    void set_shine(int val) {
        shine = val;
    }

    void perform_reflection(Point &L, Point &N, Point &intersection_point, Color &color, int level) {

        Object *nearest = this;
        Color reflected_color;
        Point R = util::get_reflection_vector(L, N);
        Ray reflecting_ray(intersection_point.add(R.mul(EPS)), R);//without adding R*EPS , the image doesnt become smooth

        double reflected_t_min = INF;

        //find the nearest object to which the ray is reflected
        for(Object *object: objects) {
            //put level = 0 since we are not performing reflection yet rather finding the nearest object to which the ray is reflected
            double t = object->intersect(reflecting_ray, reflected_color, 0);
            if(t > 0 && t < reflected_t_min) {
                reflected_t_min = t;
                nearest = object;
            }
        }

        //no object is found to which the ray is reflected : reflected_t_min = INF.So no reflection is performed.
        //perform reflection.before that , clear the reflected_color since it has been modified in the previous iteration of for loop.
        //append the reflected_color to original color.
        if(reflected_t_min != INF) {
            reflected_color.clear();
            nearest->intersect(reflecting_ray, reflected_color, level + 1);
            color += reflected_color * reflection_coEfficient;
        }
    }

    void perform_refraction(Point &L, Point &N, Point &intersection_point, Color &color, int level) {

        Object *nearest = this;
        Color refracted_color;
        Point R = util::get_refraction_vector(L, N, intersection_point);
        Ray refracting_ray(intersection_point.add(R.mul(EPS)), R);

        double refracted_t_min = INF;
        for(Object *object: objects) {
            double t = object->intersect(refracting_ray, refracted_color, 0);
            if(t > 0 && t < refracted_t_min) {
                refracted_t_min = t;
                nearest = object;
            }
        }
        if(refracted_t_min != INF) {
            refracted_color.clear();
            nearest->intersect(refracting_ray, refracted_color, level + 1);
            color += refracted_color * (reflection_coEfficient*0.5);
        }
    }

    virtual double intersect(Ray &ray, Color &color, int level) {
        double t_min = get_intersecting_t(ray);
        if(level == 0) return t_min;

        Point intersection_point = ray.start.add(ray.dir.mul(t_min));
        Color intersection_point_color = get_object_color(intersection_point);

        color = intersection_point_color * ambient_coEfficient;

        Point N = get_normal(intersection_point);

        //if( util::dot(ray.dir, N) > 0 ) N = N * -1;

        for(Light light: lights) {

            Ray light_to_point_ray(light.light_pos, (intersection_point.sub(light.light_pos)));

            //check for spotlight
            if(light.is_spotlight){
                double ang = util::get_angle(light_to_point_ray.dir, light.dir) + 3;
                double cut_ang = light.get_cutoff_angle() + 3;
                //cout<<ang<<" "<<cut_ang<<endl;
                if(ang < cut_ang) {
                    //cout<<ang<<" "<<cut_ang<<endl;
                    continue;
                }
            }

            double t_tgt = util::distance(intersection_point, light_to_point_ray.start) - EPS;

            Point L = light.light_pos.sub(intersection_point);
            Point R = util::get_reflection_vector(L, N);
            Point eye = Point(cam_pos_x , cam_pos_y , cam_pos_z);//default camera position
            Point V = eye.sub(intersection_point);

            L.normalize();
            V.normalize();

            double t_other_min = INF;
            for (Object *object: objects) {
                double t = object->intersect(light_to_point_ray, color, 0);
                if (t > 0 && t < t_other_min) {
                    t_other_min = t;
                }
            }

            if (t_tgt > t_other_min) continue;
            color += light.color * intersection_point_color * diffuse_coEfficient * max(0.0, util::dot(L, N));
            color += light.color * intersection_point_color * specular_coEfficient * max(0.0, pow(util::dot(R, V), shine));
        }

        if(level == reflection_level) return t_min;

        Point L = ray.dir.mul(-1.0);
        perform_reflection(L, N, intersection_point, color, level);
        if(REFRACTION_ON) perform_refraction(ray.dir, N, intersection_point, color, level);
        return t_min;
    }

};


//------------------------------------------------------Sphere----------------------------------------------------------------
class Sphere: public Object {
    Point center;
    double radius;
public:
    Sphere(Point  cen, double rad) {
        center = cen;
        radius = rad;
    }

    void draw() override {
        Point points[80][80];

        int stacks = 70, slices = 70;
        //generate points
        for(int i = 0 ; i<= stacks ; i++) {
            double h = radius * sin(((double) i / (double) stacks ) * (PI / 2 ) ) ;
            double r = radius * cos(((double) i / (double) stacks ) * (PI / 2));
            for(int j = 0; j <= slices ; j++) {
                points[i][j].x = r * cos( ( (double) j / (double) slices ) * 2 * PI);
                points[i][j].y = r * sin( ( (double) j / (double) slices ) * 2 * PI);
                points[i][j].z = h;
            }
        }
        //draw quads using generated points
        for(int i = 0 ; i < stacks ; i++) {
            glColor3f(object_color[0], object_color[1], object_color[2]);
            for (int j = 0; j < slices; j++) {

                glPushMatrix();
                {
                    glTranslatef(center.x, center.y, center.z);
                    glBegin(GL_QUADS);
                    {
                        //upper hemisphere
                        glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                        glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                        //lower hemisphere
                        glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                        glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                    }
                    glEnd();
                }
                glPopMatrix();
            }
        }
    }

    double get_intersecting_t(Ray &ray) override {
        Point pt = center.sub(ray.start);
        double b = -2.0 * util::dot(ray.dir, pt );
        double c = util::dot(pt, pt) - radius * radius;
        double D = b * b - 4 * c;
        if(D < 0) return -1;

        D = sqrt(D);

        double ret = INF;
        double t1 = (-b + D) * 0.5;
        double t2 = (-b - D) * 0.5;
        if(t1 > 0) ret = min(ret, t1);
        if(t2 > 0) ret = min(ret, t2);
        if(ret == INF) ret = -1;
        return ret;
    }

    Point get_normal(Point intersection_point) override {
        Point normal = intersection_point.sub(center);
        normal.normalize();
        return normal;
    }

    Color get_object_color(Point &p) override {
        return object_color;
    }

    bool is_inside(Point &p) override {
        double dist = util::distance(p, center) - EPS;
        return dist <= radius;
    }
};

//------------------------------------------------------Triangle----------------------------------------------------------------

//create a class for solving 3 variable eqn using cramer's rule
class Equation {
    double a, b, c, d, e, f, g, h, i;
    public:
    Equation(double a, double b, double c, double d, double e, double f, double g, double h, double i) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        this->e = e;
        this->f = f;
        this->g = g;
        this->h = h;
        this->i = i;
    }

    double get_x() {
        return (-b * e * i + b * f * g + c * d * i - c * f * h - a * d * g + a * e * h) / (a * e * h - a * d * g + b * d * f - b * e * i + c * e * g - c * d * h);
    }
    
    double get_y() {
        return (a * f * i - a * e * g - b * d * i + b * e * h + c * d * g - c * f * h) / (a * e * h - a * d * g + b * d * f - b * e * i + c * e * g - c * d * h);
    }
    
    double get_z() {
        return (a * e * g - a * d * h - b * d * g + b * e * h + c * d * h - c * e * g) / (a * e * h - a * d * g + b * d * f - b * e * i + c * e * g - c * d * h);
    }
};

class Triangle: public Object {
    Point A, B, C;
public:
    
    Triangle(Point &a, Point &b, Point &c) {
        A = a;
        B = b;
        C = c;
    }

    void draw() override {
        glBegin(GL_TRIANGLES);
        glColor3f(object_color[0], object_color[1], object_color[2]);
        glVertex3f(A.x, A.y, A.z);
        glVertex3f(B.x, B.y, B.z);
        glVertex3f(C.x, C.y, C.z);
        glEnd();
    }

    Point get_normal(Point pt) override {
        Point normal =  util::cross( B.sub(A), C.sub(A));
        normal.normalize();
        return normal;
    }

    double get_intersecting_t(Ray &ray) override {

        double mat1[3][3] = {
                { A.x - B.x,  A.x - C.x, ray.dir.x },
                { A.y - B.y,  A.y - C.y, ray.dir.y },
                { A.z - B.z,  A.z - C.z, ray.dir.z },
        };
        double D = util::get_determinant(mat1);
        if(D == 0) return -1;

        double beta = util::get_beta(ray, D , A, C);
        if(beta < 0 || beta > 1) return -1;

        double gamma = util::get_gamma(ray, D , A, B);
        if(gamma < 0 || gamma > 1) return -1;
        if(beta + gamma > 1) return -1;

        double t = util::get_t(ray, D , A, B, C);
        if(t < 0) return -1;
        return t;
    }

    //get_object_color
    Color get_object_color(Point &p) override {
        return object_color;
    }

};

class General_Surface : public Object {
    public:
    double a, b, c, d, e, f, g, h, i, j;
    Point refPoint;
    double length, width, height;

    General_Surface(){}

    //constructor
    General_Surface(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, Point refPoint, double length, double width, double height) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        this->e = e;
        this->f = f;
        this->g = g;
        this->h = h;
        this->i = i;
        this->j = j;
        this->refPoint = refPoint;
        this->length = length;
        this->width = width;
        this->height = height;
    }

    void draw() override {}

    bool is_inside(Point &point) override {
        double lns[] = {length, width, height};
        
        for(int idx = 0; idx < 3; idx++) {
            if(fabs(lns[idx]) < EPS) continue;
            if(idx == 0){
                  if(point.x < refPoint.x) return false;
                  if(point.x > refPoint.x+length) return false;
            }
            else if(idx==1){
                  if(point.y < refPoint.y) return false;
                if(point.y > refPoint.y+width) return false;
            }
            else{
                if(point.z < refPoint.z) return false;
                 if(point.z > refPoint.z+height) return false;
            }
        }

        return true;
    }

    double get_intersecting_t(Ray &ray) override {
        double x0 = ray.start.x, y0 = ray.start.y, z0 = ray.start.z;
        double x1 = ray.dir.x, y1 = ray.dir.y, z1 = ray.dir.z;

        double c0 = a*x1*x1 + b*y1*y1 + c*z1*z1 + d*x1*y1 + e*x1*z1 + f*y1*z1;
        double c1 = 2*a*x0*x1 + 2*b*y0*y1 + 2*c*z0*z1 + d*(x0*y1 + x1*y0) + e*(x0*z1 + x1*z0);
        c1 += f*(y0*z1 + y1*z0) + g*x1 + h*y1 + i*z1;
        double c2 = a*x0*x0 + b*y0*y0 + c*z0*z0 + d*x0*y0 + e*x0*z0 + f*y0*z0 + g*x0 + h*y0 + i*z0 + j;

        double t1, t2;
        double t = util::getSol(c0, c1, c2, t1, t2);

        if(t < 0) return -1;

        //cout << t1 << " " << t2 << endl;

        if(t1 > 0)
        {
            //cout << "t1 paise :'(" << endl;
            Point kache = ray.start.add(ray.dir.mul(t1));
            if(is_inside(kache)) return t1;
        }

        if(t2 > 0)
        {
            //cout << "t2 paise :'(" << endl;
            Point dure = ray.start.add(ray.dir.mul(t2));
            if(is_inside(dure)) return t2;
        }

        return -1;
    }

    Point get_normal(Point intersection_point) override {
        double x = intersection_point.x, y = intersection_point.y, z = intersection_point.z;
        Point normal = Point (2*a*x + d*y + e*z + g, 2*b*y + d*x + f*z + h, 2*c*z + e*x + f*y + i);
        normal.normalize();
        return normal;
    }

    Color get_object_color(Point &p) override {
        return object_color;
    }

};


//------------------------------------------------------Floor----------------------------------------------------------------
class Floor: public Object {
    Point width_from_center;
    double width;
    double tile_width;
public:
    Floor(double floorWidth, double tileWidth) {
        width = floorWidth;
        width_from_center = Point(floorWidth / 2, floorWidth / 2, 0);
        tile_width = tileWidth;
        ambient_coEfficient = 0.1;
        diffuse_coEfficient = 0.4;
        specular_coEfficient = 0.1;
        reflection_coEfficient = 0.5;
        shine = 1;
    }

    void draw() override {
        bool white = false;
        for(double i = -width_from_center.x; i <= width_from_center.x ; i += tile_width) {
            for(double j = -width_from_center.y ; j <= width_from_center.y ; j += tile_width) {
                if(white) glColor3f(1,1,1);
                else glColor3f(0,0,0);
                glBegin(GL_QUADS); {
                    glVertex3f(i, j, 0 );
                    glVertex3f(i, j + tile_width, 0);
                    glVertex3f(i + tile_width, j + tile_width, 0 );
                    glVertex3f(i + tile_width, j , 0);
                } glEnd();
                white ^= 1;
            }
        }
    }

    Point get_normal(Point intersection_point) override {
        return Point(0, 0, 1);
    }

    double get_intersecting_t(Ray &ray) override {
        if(ray.dir.z == 0) return -1;

        double t = - ray.start.z / ray.dir.z;
        Point pt = ray.start.add(ray.dir.mul(t));
        if(t < 0) return -1;
        if(pt.x < -width_from_center.x || pt.x > width_from_center.x) return -1;
        if(pt.y < -width_from_center.y || pt.y > width_from_center.y) return -1;
        return t;
    }

    Color get_object_color(Point &p) override {
        int ro_id = (int) ( (p.x + width_from_center.x) / tile_width );
        int col_id = (int) ( (p.y + width_from_center.y) / tile_width );
        if( (ro_id + col_id) % 2 ) return Color(1, 1, 1);
        //cout<<"ro_id: "<<ro_id<<" col_id: "<<col_id<<endl;
        return Color(0,0,0);
    }
};


//------------------------------------------------------camera----------------------------------------------------------------
class Camera {
public:

    Point pos;
    Point u, r, l;
    int fovy;
    int aspectRatio;
    int nearPlane;
    int farPlane;
    double camera_speed;
    double rotate_angle;

    Camera() {

        pos = Point(cam_pos_x , cam_pos_y , cam_pos_z);
        r = Point(-1.0/sqrt(2.0), 1.0/sqrt(2.0), 0);
        l = Point(-1.0/sqrt(2.0), -1.0/sqrt(2.0), 0);
        u = Point(0, 0, 1);

        fovy = 90;
        aspectRatio = 1;
        nearPlane = 1;
        farPlane = 1000;
        camera_speed = 5;
        rotate_angle = camera_speed * PI / 180;
    }

    void look_left() {
        Point rr = r.mul(cos(-rotate_angle)).sub(l.mul(sin(-rotate_angle)));
        Point ll = r.mul(sin(-rotate_angle)).add(l.mul(cos(-rotate_angle)));
        r = rr;
        l = ll;
    }

    void look_right() {
        Point rr = r.mul(cos(rotate_angle)).sub(l.mul(sin(rotate_angle)));
        Point ll = r.mul(sin(rotate_angle)).add(l.mul(cos(rotate_angle)));
        r = rr;
        l = ll;
    }

    void look_up() {
        Point uu = u.mul(cos(-rotate_angle)).add(l.mul(sin(-rotate_angle)));
        Point ll = u.mul(sin(-rotate_angle)).mul(-1).add(l.mul(cos(-rotate_angle)));
        u = uu;
        l = ll;
    }

     void look_down() {
        Point uu = u.mul(cos(rotate_angle)).add(l.mul(sin(rotate_angle)));
        Point ll = u.mul(sin(rotate_angle)).mul(-1).add(l.mul(cos(rotate_angle)));
        u = uu;
        l = ll;
    }

    void tilt_clockwise() {
        Point uu = u.mul(cos(rotate_angle)).sub(r.mul(sin(rotate_angle)));
        Point rr = u.mul(sin(rotate_angle)).add(r.mul(cos(rotate_angle)));
        u = uu;
        r = rr;
    }

    void tilt_counter_clockwise() {
        Point uu = u.mul(cos(-rotate_angle)).sub(r.mul(sin(-rotate_angle)));
        Point rr = u.mul(sin(-rotate_angle)).add(r.mul(cos(-rotate_angle)));
        u = uu;
        r = rr;
    }

    void up_arrow(){
        pos = pos.add(l.mul(camera_speed));
    }

    void down_arrow(){
        pos = pos.sub(l.mul(camera_speed));
    }

    void right_arrow(){
        pos = pos.add(r.mul(camera_speed));
    }

    void left_arrow(){
        pos = pos.sub(r.mul(camera_speed));
    }

    void page_up(){
        pos = pos.add(u.mul(camera_speed));
    }

    void page_down(){
        pos = pos.sub(u.mul(camera_speed));
    }

};