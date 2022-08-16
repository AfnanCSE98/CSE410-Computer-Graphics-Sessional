#ifndef OFFLINE_1705098_HEADERS_H
#define OFFLINE_1705098_HEADERS_H

#endif 
#include <GL/glut.h>
#include "1705098_bitmap_image.h"
#include <bits/stdc++.h>
using namespace std;

//configs

class Object;
class Light;
class Point;
#define PI acos(-1)

const int INF = 1e4;
const double EPS = 1e-7;
const char* scene_file = "scene.txt";
const char* output_file = "1705098_out.bmp";
const double RGB = 255;

static int REFRACTION_ON, TEXTURE_ON;
static bitmap_image texture_image;
static int reflection_level, pixels, total_objects, total_lights;


static vector<Object*>objects;
static vector<Light>lights;
//------------------------------------------------------------------------------point----------------------------------------------------------------------------------------------
const int M = 3;
class Point {
public:
    
    double pt[M];

    Point() {
        memset(pt, 0 , sizeof pt);
    }
    Point(double x, double y, double z) {
        pt[0] = x; pt[1] = y; pt[2] = z;
    }
    static double dot(Point v1, Point v2) {
        double ret = 0;
        for(int i = 0 ; i < M ; i++ ) {
            ret += v1[i] * v2[i];
        }
        return ret;
    }
    static Point cross(Point v1, Point v2) {
        Point ret;
        ret[0] = v1[1] * v2[2] - v1[2] * v2[1];
        ret[1] = -v1[0] * v2[2] + v1[2] * v2[0];
        ret[2] = v1[0] * v2[1] - v1[1] * v2[0];
        return ret;
    }
    void normalize() {
        double down = 0;
        for(double x : pt) down += x*x;
        down = sqrt(down);
        for(double &x: pt) x /= down;
    }
    //operator overloads
    inline double& operator[] (int id) {
        return pt[id];
    }
    friend ostream& operator<<(ostream& os, Point& v) {
        os<<" ( ";
        for(int i = 0 ; i < M ; i++ ) {
            os << v[i];
            if(i < M - 1) os << ", ";
        }
        os<<" ) ";
        return os;
    }
    friend istream& operator>>(istream& is, Point& v) {
        for(int i = 0 ; i < M; i++ ) is >> v[i] ;
        return is;
    }
    bool equals( Point p) {
        for(int i = 0 ; i < M ; i++) {
            if(p[i] != (*this)[i]) return false;
        }
        return true;
    }
    Point operator* (double val) {
        Point ret = *this;
        for(double &x: ret.pt) x *= val;
        return ret;
    }
    Point operator/ (double val) {
        Point ret = *this;
        for(double &x: ret.pt) x /= val;
        return ret;
    }
    Point operator+ (Point v) {
        Point ret;
        for(int i = 0 ; i < M ; i++) ret[i] = (*this)[i] + v[i];
        return ret;
    }
    Point operator- (Point v) {
        Point ret;
        for(int i = 0 ; i < M ; i++) ret[i] = (*this)[i] - v[i];
        return ret;
    }
    static double distance(Point p1, Point p2) {
        double ret = 0;
        for(int i = 0  ; i < M ; i++) ret += (p1[i] - p2[i]) * (p1[i] - p2[i]);
        return sqrt(ret);
    }

};

static Point eye, u, r, l;
//------------------------------------------------------ray----------------------------------------------------------------
class Ray {
public:
    Point start;
    Point dir;
    Ray(){

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
        for(double &x: rgb) x = min(1.0, x);
    }
    bool equals(Color p) {
        for(int i = 0 ; i < 3 ; i++) {
            if(p[i] != (*this)[i]) return false;
        }
        return true;
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
    void draw() {
        glPointSize(5);
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_POINTS);
        glVertex3f(light_pos[0], light_pos[1], light_pos[2]);
        glEnd();
    }
};


//------------------------------------------------------Object----------------------------------------------------------------
class Object{
public:
    Color object_color;
    double coEfficients[4]{} ;// reflection coefficients
    int shine{}; // exponent term of specular component

    virtual void draw() = 0;
    virtual Point get_normal(Point pt) = 0;
    virtual double get_intersecting_t(Ray &ray) = 0;
    virtual Color get_object_color(Point &p) = 0;
    virtual bool is_inside(Point &pt) {
        return false;
    }
    static Point get_reflection_vector(Point &L, Point &N) {
        Point ret = N * 2 * Point::dot(L, N) - L;
        ret.normalize();
        return ret;
    }
    Point get_refraction_vector(Point &I, Point &N, Point &intersecting_point) {
        double eta = 1.5;
        eta = is_inside(intersecting_point) ? (1 / eta) : eta ;
        double N_dot_I = Point::dot(N, I);
        double k = 1.0 - eta * eta * (1.0 - N_dot_I * N_dot_I);
        if(k < 0) {
            I = I * -1;
            return get_reflection_vector(I, N);
        }
        return I * eta - N * (eta * N_dot_I + sqrt(k));
    }

    void perform_reflection(Point &L, Point &N, Point &intersection_point, Color &color, int level) {

        Object *nearest = this;
        Color reflected_color;
        Point R = get_reflection_vector(L, N);
        Ray reflecting_ray(intersection_point + R * EPS, R);

        double reflected_t_min = INF;
        for(Object *object: objects) {
            double t = object->intersect(reflecting_ray, reflected_color, 0);
            if(t > 0 && t < reflected_t_min) {
                reflected_t_min = t;
                nearest = object;
            }
        }
        if(reflected_t_min != INF) {
            reflected_color.clear();
            nearest->intersect(reflecting_ray, reflected_color, level + 1);
            color += reflected_color * coEfficients[3];
        }
    }
    void perform_refraction(Point &L, Point &N, Point &intersection_point, Color &color, int level) {

        Object *nearest = this;
        Color refracted_color;
        Point R = get_refraction_vector(L, N, intersection_point);
        Ray refracting_ray(intersection_point + R * EPS, R);

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
            color += refracted_color * (coEfficients[3]/2.0);
        }
    }

    virtual double intersect(Ray &ray, Color &color, int level) {

        double t_min = get_intersecting_t(ray);
        if(level == 0) return t_min;
        Point intersection_point = ray.start + ray.dir * t_min;
        Color intersection_point_color = get_object_color(intersection_point);

        color = intersection_point_color * coEfficients[0];
        Point N = get_normal(intersection_point);
        if( Point::dot(ray.dir, N) > 0 ) N = N * -1;

        for(Light light: lights) {

            Ray light_to_point_ray(light.light_pos, (intersection_point - light.light_pos));
            double t_tgt = Point::distance(intersection_point, light_to_point_ray.start) - EPS;

            Point L = light.light_pos - intersection_point;
            Point R = get_reflection_vector(L, N);
            Point V = eye - intersection_point;
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
            color += light.color * intersection_point_color * coEfficients[1] * max(0.0, Point::dot(L, N));
            color += light.color * intersection_point_color * coEfficients[2] * max(0.0, pow(Point::dot(R, V), shine));
        }
        if(level == reflection_level) return t_min;

        Point L = ray.dir * -1;
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
    Sphere(Point  cen, double rad) : center(std::move(cen)) , radius(rad) {}

    void draw() override {
        Point points[80][80];

        int stacks = 70, slices = 70;
        //generate points
        for(int i = 0 ; i<= stacks ; i++) {
            double h = radius * sin(((double) i / (double) stacks ) * (PI / 2 ) ) ;
            double r = radius * cos(((double) i / (double) stacks ) * (PI / 2));
            for(int j = 0; j <= slices ; j++) {
                points[i][j][0] = r * cos( ( (double) j / (double) slices ) * 2 * PI);
                points[i][j][1] = r * sin( ( (double) j / (double) slices ) * 2 * PI);
                points[i][j][2] = h;
            }
        }
        //draw quads using generated points
        for(int i = 0 ; i < stacks ; i++) {
            glColor3f(object_color[0], object_color[1], object_color[2]);
            for (int j = 0; j < slices; j++) {

                glPushMatrix();
                {
                    glTranslatef(center[0], center[1], center[2]);
                    glBegin(GL_QUADS);
                    {
                        //upper hemisphere
                        glVertex3f(points[i][j][0], points[i][j][1], points[i][j][2]);
                        glVertex3f(points[i][j + 1][0], points[i][j + 1][1], points[i][j + 1][2]);
                        glVertex3f(points[i + 1][j + 1][0], points[i + 1][j + 1][1], points[i + 1][j + 1][2]);
                        glVertex3f(points[i + 1][j][0], points[i + 1][j][1], points[i + 1][j][2]);
                        //lower hemisphere
                        glVertex3f(points[i][j][0], points[i][j][1], -points[i][j][2]);
                        glVertex3f(points[i][j + 1][0], points[i][j + 1][1], -points[i][j + 1][2]);
                        glVertex3f(points[i + 1][j + 1][0], points[i + 1][j + 1][1], -points[i + 1][j + 1][2]);
                        glVertex3f(points[i + 1][j][0], points[i + 1][j][1], -points[i + 1][j][2]);
                    }
                    glEnd();
                }
                glPopMatrix();
            }
        }
    }

    /*
     * Reference: http://www.ambrsoft.com/TrigoCalc/Sphere/SpherLineIntersection_.htm
     */

    double get_intersecting_t(Ray &ray) override {
        Point pt = center - ray.start;
        double b = -2.0 * Point::dot(ray.dir, pt );
        double c = Point::dot(pt, pt) - radius * radius;
        double D = b * b - 4 * c;
        if(D < 0) return -1;

        D = sqrt(D);

        double ret = INF;
        double t1 = (-b + D) / 2.0;
        double t2 = (-b - D) / 2.0;
        if(t1 > 0) ret = min(ret, t1);
        if(t2 > 0) ret = min(ret, t2);
        if(ret == INF) ret = -1;
        return ret;
    }

    Point get_normal(Point intersection_point) override {
        Point normal = intersection_point - center;
        normal.normalize();
        return normal;
    }
    Color get_object_color(Point &p) override {
        return object_color;
    }
    bool is_inside(Point &p) override {
        double dist = Point::distance(p, center) - EPS;
        return dist <= radius;
    }
};

//------------------------------------------------------Triangle----------------------------------------------------------------
class Triangle: public Object {
    Point A, B, C;
    Point normal;
public:
    Triangle(Point &a, Point &b, Point &c): A(move(a)), B(move(b)), C(move(c)) {
        normal =  Point::cross( B - A, C - A);
        normal.normalize();
    }
    void draw() override {
        glBegin(GL_TRIANGLES);
        glColor3f(object_color[0], object_color[1], object_color[2]);
        glVertex3f(A[0], A[1], A[2]);
        glVertex3f(B[0], B[1], B[2]);
        glVertex3f(C[0], C[1], C[2]);
        glEnd();
    }
    Point get_normal(Point pt) override {
        return normal;
    }

    static double get_determinant(double mat[3][3]) {
        double ret = 0;
        ret += mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
        ret -= mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]);
        ret += mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
        return ret;
    }

    double get_intersecting_t(Ray &ray) override {

        double mat1[3][3] = {
                { A[0] - B[0],  A[0] - C[0], ray.dir[0] },
                { A[1] - B[1],  A[1] - C[1], ray.dir[1] },
                { A[2] - B[2],  A[2] - C[2], ray.dir[2] },
        };
        double D = get_determinant(mat1);
        if(D == 0) return -1;

        double mat2[3][3] = {
                { A[0] - ray.start[0] , A[0] - C[0] , ray.dir[0] },
                { A[1] - ray.start[1] , A[1] - C[1] , ray.dir[1] },
                { A[2] - ray.start[2] , A[2] - C[2] , ray.dir[2] },
        };
        double beta = get_determinant(mat2) / D;
        if(beta < 0 || beta > 1) return -1;

        double mat3[3][3] = {
                { A[0] - B[0] , A[0] - ray.start[0] , ray.dir[0] },
                { A[1] - B[1] , A[1] - ray.start[1] , ray.dir[1] },
                { A[2] - B[2] , A[2] - ray.start[2] , ray.dir[2] },
        };
        double gamma = get_determinant(mat3) / D;
        if(gamma < 0 || gamma > 1) return -1;
        if(beta + gamma > 1) return -1;

        double mat4[3][3] = {
                { A[0] - B[0] , A[0] - C[0] , A[0] - ray.start[0] },
                { A[1] - B[1] , A[1] - C[1] , A[1] - ray.start[1] },
                { A[2] - B[2] , A[2] - C[2] , A[2] - ray.start[2] },
        };

        double t = get_determinant(mat4) / D;
        if(t < 0) return -1;
        return t;
    }
    Color get_object_color(Point &p) override {
        return object_color;
    }
};


//------------------------------------------------------Floor----------------------------------------------------------------
class Floor: public Object {
    Point width_from_center;
    double tile_width;
public:
    Floor(double floorWidth, double tileWidth) {
        width_from_center = Point(floorWidth / 2, floorWidth / 2, 0);
        tile_width = tileWidth;
        coEfficients[0] = 0.1;
        coEfficients[1] = 0.4;
        coEfficients[2] = 0.1;
        coEfficients[3] = 0.5;
        shine = 1;
    }
    void draw() override {
        bool white = false;
        for(double i = -width_from_center[0]; i <= width_from_center[0] ; i += tile_width) {
            for(double j = -width_from_center[1] ; j <= width_from_center[1] ; j += tile_width) {
                if(white) glColor3f(1,1,1);
                else glColor3f(0,0,0);
                glBegin(GL_QUADS); {
                    glVertex3f(i, j, width_from_center[2] );
                    glVertex3f(i, j + tile_width, width_from_center[2] );
                    glVertex3f(i + tile_width, j + tile_width, width_from_center[2] );
                    glVertex3f(i + tile_width, j , width_from_center[2] );
                } glEnd();
                white ^= 1;
            }
        }
    }
    Point get_normal(Point intersection_point) override {
        return Point(0, 0, 1);
    }
    double get_intersecting_t(Ray &ray) override {
        if(ray.dir[2] == 0) return -1;

        double t = - ray.start[2] / ray.dir[2];
        Point pt = ray.start + ray.dir * t;
        if(t < 0) return -1;
        if(pt[0] < -width_from_center[0] || pt[0] > width_from_center[0]) return -1;
        if(pt[1] < -width_from_center[1] || pt[1] > width_from_center[1]) return -1;
        return t;
    }
    Color get_object_color(Point &p) override {
        int ro_id = (int) ( (p[0] + width_from_center[0]) / tile_width );
        int col_id = (int) ( (p[1] + width_from_center[1]) / tile_width );
        if( (ro_id + col_id) % 2 ) return Color(1, 1, 1);

        if(!TEXTURE_ON) return Color(0,0,0);

        double xx = (p[0] + width_from_center[0]) - ro_id * tile_width;
        double yy = (p[1] + width_from_center[1]) - col_id * tile_width;

        int texture_ro_id = (int) ( (texture_image.height() * xx) / tile_width );
        int texture_col_id = (int) ( (texture_image.width() * yy) / tile_width );

        unsigned char r, g, b;
        texture_image.get_pixel(texture_col_id, texture_ro_id, r, g, b);
        Color texture(r,g,b);
        return texture / RGB;
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

        pos = Point(100, 100, 50);
        r = Point(-1.0/sqrt(2.0), 1.0/sqrt(2.0), 0);
        l = Point(-1.0/sqrt(2.0), -1.0/sqrt(2.0), 0);
        u = Point(0, 0, 1);

        fovy = 90;
        aspectRatio = 1;
        nearPlane = 1;
        farPlane = 1000;
        camera_speed = 2;
        rotate_angle = camera_speed * PI / 180;
    }

    void look_left() {
        Point rr = r * cos(-rotate_angle) - l * sin(-rotate_angle);
        Point ll = r * sin(-rotate_angle) + l * cos(-rotate_angle);
        r = rr;
        l = ll;
    }

    void look_right() {
        Point rr = r * cos(rotate_angle) - l * sin(rotate_angle);
        Point ll = r * sin(rotate_angle) + l * cos(rotate_angle);
        r = rr;
        l = ll;
    }

    void look_up() {
        Point uu = u * cos(-rotate_angle) + l * sin(-rotate_angle);
        Point ll = u * sin(-rotate_angle)*-1 + l * cos(-rotate_angle);
        u = uu;
        l = ll;
    }

     void look_down() {
        Point uu = u * cos(rotate_angle) + l * sin(rotate_angle);
        Point ll = u * sin(rotate_angle)*-1 + l * cos(rotate_angle);
        u = uu;
        l = ll;
    }

    void tilt_clockwise() {
        Point uu = u * cos(rotate_angle) - r * sin(rotate_angle);
        Point rr = u * sin(rotate_angle) + r * cos(rotate_angle);
        u = uu;
        r = rr;
    }

    void tilt_counter_clockwise() {
        Point uu = u * cos(-rotate_angle) - r * sin(-rotate_angle);
        Point rr = u * sin(-rotate_angle) + r * cos(-rotate_angle);
        u = uu;
        r = rr;
    }

    void up_arrow(){
        pos = pos + l * camera_speed;
    }

    void down_arrow(){
        pos = pos - l * camera_speed;
    }

    void right_arrow(){
        pos = pos + r * camera_speed;
    }

    void left_arrow(){
        pos = pos - r * camera_speed;
    }

    void page_up(){
        pos = pos + u * camera_speed;
    }

    void page_down(){
        pos = pos - u * camera_speed;
    }

};