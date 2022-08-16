//
// Created by USER on 24-Jun-21.
//

#ifndef RAY_TRACING_1605024_LIGHT_H
#define RAY_TRACING_1605024_LIGHT_H

#include "1605024_point.h"
#include "1605024_color.h"

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


#endif //RAY_TRACING_1605024_LIGHT_H
