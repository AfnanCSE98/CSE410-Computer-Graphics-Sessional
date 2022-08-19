//
// Created by mahir on 7/1/21.
//

#ifndef RAY_TRACING_1605024_GENERAL_SURFACE_H
#define RAY_TRACING_1605024_GENERAL_SURFACE_H

class General_surface: public Object {
    double A, B, C, D, E, F, G, H, I, J;
    Point reference_point;
    double min_lim[3], max_lim[3];
public:
    General_surface(const vector<double>&input_coEffs,const Point& p, const vector<double>&range) {
        A = input_coEffs[0];
        B = input_coEffs[1];
        C = input_coEffs[2];
        D = input_coEffs[3];
        E = input_coEffs[4];
        F = input_coEffs[5];
        G = input_coEffs[6];
        H = input_coEffs[7];
        I = input_coEffs[8];
        J = input_coEffs[9];
        reference_point = p;

        for(int i = 0 ; i < 3 ; i++) {
            min_lim[i] = reference_point[i];
            max_lim[i] = reference_point[i] + range[i];
            if(min_lim[i] > max_lim[i]) swap(min_lim[i], max_lim[i]);
        }
    }
    void draw() override {}
    double get_intersecting_t(Ray &ray) override {
        double AA =
                A * ray.dir[0] * ray.dir[0] +
                B * ray.dir[1] * ray.dir[1] +
                C * ray.dir[2] * ray.dir[2] +
                D * ray.dir[0] * ray.dir[1] +
                E * ray.dir[0] * ray.dir[2] +
                F * ray.dir[1] * ray.dir[2];

        double BB =
                2 * A * ray.start[0] * ray.dir[0] +
                2 * B * ray.start[1] * ray.dir[1] +
                2 * C * ray.start[2] * ray.dir[2] +
                D * ( ray.dir[0] * ray.start[1] + ray.dir[1] * ray.start[0] ) +
                E * ( ray.dir[0] * ray.start[2] + ray.dir[2] * ray.start[0] ) +
                F * ( ray.dir[1] * ray.start[2] + ray.dir[2] * ray.start[1] ) +
                G * ray.dir[0] +
                H * ray.dir[1] +
                I * ray.dir[2];

        double CC =
                A * ray.start[0] * ray.start[0] +
                B * ray.start[1] * ray.start[1] +
                C * ray.start[2] * ray.start[2] +
                D * ray.start[0] * ray.start[1] +
                E * ray.start[0] * ray.start[2] +
                F * ray.start[1] * ray.start[2] +
                G * ray.start[0] +
                H * ray.start[1] +
                I * ray.start[2] + J;

        double DD = BB * BB - 4 * AA * CC;
        if(DD < 0) return -1;
        if( AA == 0) return - CC / BB;
        DD = sqrt(DD);

        double t_0 = (- BB - DD ) / (2.0 * AA);
        double t_1 = (- BB + DD ) / (2.0 * AA);


        for(int i = 0 ; i < 3  ; i++) {
            if(min_lim[i] == max_lim[i]) continue;
            if(t_0 > 0) {
                Point intersecting_point_0 = ray.start + ray.dir * t_0;
                if (intersecting_point_0[i] < min_lim[i] || intersecting_point_0[i] > max_lim[i]) t_0 = -1;
            }
            if(t_1 > 0) {
                Point intersecting_point_1 = ray.start + ray.dir * t_1;
                if (intersecting_point_1[i] < min_lim[i] || intersecting_point_1[i] > max_lim[i]) t_1 = -1;
            }
        }
        double t = INF;
        if(t_0 > 0) t = min(t, t_0);
        if(t_1 > 0) t = min(t, t_1);
        if(t == INF) t = -1;
        return t;
    }
    Point get_normal(Point pt) override {
        double x = 2 * A * pt[0] + D * pt[1] + E * pt[2] + G;
        double y = 2 * B * pt[1] + D * pt[0] + F * pt[2] + H;
        double z = 2 * C * pt[2] + E * pt[0] + F * pt[1] + I;
        Point normal = Point(x, y, z);
        normal.normalize();
        return normal;
    }
    Color get_object_color(Point &p) override {
        return object_color;
    }
};

#endif //RAY_TRACING_1605024_GENERAL_SURFACE_H
