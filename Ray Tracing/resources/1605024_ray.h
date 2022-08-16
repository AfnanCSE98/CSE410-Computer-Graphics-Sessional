//
// Created by mahir on 6/28/21.
//

#ifndef RAY_TRACING_1605024_RAY_H
#define RAY_TRACING_1605024_RAY_H

class Ray {
public:
    Point start;
    Point dir;
    Ray() = default;
    Ray(const Point &from, Point direction) : start(std::move(from)), dir(std::move(direction)) {
        dir.normalize();
    }
};

#endif //RAY_TRACING_1605024_RAY_H
