//
// Created by mahir on 6/28/21.
//

#ifndef RAY_TRACING_1605024_FLOOR_H
#define RAY_TRACING_1605024_FLOOR_H

#include "1605024_object.h"


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


#endif //RAY_TRACING_1605024_FLOOR_H
