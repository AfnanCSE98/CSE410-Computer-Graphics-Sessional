#ifndef RAY_TRACING_1605024_OBJECT_H
#define RAY_TRACING_1605024_OBJECT_H

#include "1605024_config.h"
#include "1605024_point.h"
#include "1605024_ray.h"
#include "1605024_color.h"
#include "1605024_light.h"

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

#endif //RAY_TRACING_1605024_OBJECT_H


