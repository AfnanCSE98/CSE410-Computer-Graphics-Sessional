//
// Created by mahir on 6/28/21.
//

#ifndef RAY_TRACING_1605024_CONFIG_H
#define RAY_TRACING_1605024_CONFIG_H

class Object;
class Light;
class Point;


#define PI acos(-1)

const int INF = 1e4;
const double EPS = 1e-7;
const char* scene_file = "scene.txt";
const char* output_file = "1605024_out.bmp";
const string texture_file = "1605024_texture_file.bmp";
const double RGB = 255;

static int REFRACTION_ON, TEXTURE_ON;
static bitmap_image texture_image;
static int reflection_level, pixels, total_objects, total_lights;
static Point eye, u, r, l;

static vector<Object*>objects;
static vector<Light>lights;


#endif //RAY_TRACING_1605024_CONFIG_H
