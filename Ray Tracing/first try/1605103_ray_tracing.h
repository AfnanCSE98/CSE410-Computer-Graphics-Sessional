//
// Created by ashiq on 7/2/21.
//

#ifndef OFFLINE3_1605103_RAY_TRACING_H
#define OFFLINE3_1605103_RAY_TRACING_H

#endif //OFFLINE3_1605103_RAY_TRACING_H

#define MIN_OBSTACLE_DIST (-50)
#define MAX_OBSTACLE_DIST INT32_MAX

#include "1705098_objects.h"
#include "bitmap_image.h"
#include <vector>

class Ray {
public:
    Vector start, dir;
    Color color;
    double t;
    bool testDisableMirrorReflection;

    Ray() {
        t = -MIN_OBSTACLE_DIST;
    }

    Ray(const Vector &startVector, const Vector &direction) {
        start = startVector;
        dir = direction;
        t = -MIN_OBSTACLE_DIST;
        testDisableMirrorReflection = false;
    }
};

class RayTracingExecutor {
public:

    Scene & scene;

    RayTracingExecutor(Scene &sc) : scene(sc) {}

    Ray intersect(Ray ray, int recursionLevel, Object * object) const{
        Ray result;

        result.t = getIntersectionParameterT(object, ray);

        Vector intersectionPoint = ray.start + ray.dir * result.t;
        result.color = object->getColor(intersectionPoint) * object->getAmbient();

        if (result.t < 0) return result;
        if (recursionLevel < 1 && ray.testDisableMirrorReflection) return result;

        Color finalColor = phongLighting(object, ray, intersectionPoint, recursionLevel);
        result.color = finalColor;

        return result;
    }

    Color phongLighting(Object *object, Ray ray, Vector intersectionPoint, int reflectionLevel) const {
        Color resultColor = object->getColor(intersectionPoint) * object->getAmbient();

        Vector normalAtIntersectionPoint = object->getNormal(intersectionPoint);

        double dotValue = normalAtIntersectionPoint.dot(ray.dir);
        if (dotValue > 0) {
            normalAtIntersectionPoint = normalAtIntersectionPoint * (-1);
        }

        for (auto &light : scene.lights) {
            Vector lightRayDirection = light.position - intersectionPoint;
            double lightToObjectDistance = lightRayDirection.abs();
            lightRayDirection = lightRayDirection.normalize();

            Vector lightRayStart = intersectionPoint + lightRayDirection;
            Ray lightRayToObject(lightRayStart, lightRayDirection);

            bool isOccluded = false;

            for (int i=0; i < scene.numObjects; i++) {
                lightRayToObject.testDisableMirrorReflection= true;
                Ray interceptResult = intersect(lightRayToObject, 0, scene.objects[i]);
                if (isIntersect(interceptResult, lightToObjectDistance)) {
                    isOccluded = true;
                    break;
                }
            }

            if (!isOccluded) {
                double lambertValue = std::max(lightRayDirection.dot(normalAtIntersectionPoint), 0.0);
                Vector R = normalAtIntersectionPoint * 2.0 * lightRayDirection.dot(normalAtIntersectionPoint) - lightRayDirection;
                double phongValue = std::max(std::abs(ray.dir.dot(R)), 0.0);

                Color diffuseComponent = (light.color * lambertValue * object->getDiffuse()) * object->getColor(intersectionPoint);
                Color specularComponent = light.color * object->getColor(intersectionPoint) *  object->getSpecular() * pow(phongValue, object->getShine());

                resultColor = resultColor + diffuseComponent + specularComponent;
            }
        }
        if (reflectionLevel > 0) {
            Vector reflectionRayDirection = ray.dir.reflection(normalAtIntersectionPoint);

            Vector reflectedRayStart = intersectionPoint + reflectionRayDirection;
            Ray reflectedRay(reflectedRayStart, reflectionRayDirection);

            int closestObstacleIdx = getClosestObstacleIdx(reflectedRay);

            if (closestObstacleIdx != MIN_OBSTACLE_DIST) {
                Ray nextLevel = intersect(reflectedRay, reflectionLevel - 1, scene.objects[closestObstacleIdx]);
                Color reflectionComponent = nextLevel.color * object->getReflection();
                resultColor = resultColor + reflectionComponent;
            }
        }
        return resultColor;
    }

    int getClosestObstacleIdx(const Ray &ray) const {

        int closestObstacleObjectID = MIN_OBSTACLE_DIST;
        double minParamT = MAX_OBSTACLE_DIST;

        for (int objectID = 0; objectID < scene.numObjects; objectID++) {
            Object * obj = scene.objects[objectID];

            Ray intersectRay = intersect(ray, 0, obj);

            if (isIntersect(intersectRay, minParamT)) {
                closestObstacleObjectID = objectID;
                minParamT = intersectRay.t;
            }
        }
        return closestObstacleObjectID;
    }

    Color execute(const Ray &ray) const{
        int closestObstacleID = MIN_OBSTACLE_DIST;
        double minParamT = MAX_OBSTACLE_DIST;

        for (int objectID = 0; objectID < scene.numObjects; objectID++) {
            Ray intersectRay = intersect(ray, 0, scene.objects[objectID]);
            if (RayTracingExecutor::isIntersect(intersectRay, minParamT)) {
                closestObstacleID = objectID;
                minParamT = intersectRay.t;
            }
        }
        if (closestObstacleID != MIN_OBSTACLE_DIST) {
            Ray intersectRayRecursive = intersect(ray, scene.recursionLevels, scene.objects[closestObstacleID]);
            return intersectRayRecursive.color;
        }
        return {0, 0, 0};
    }

    /** utils **/

    static bool isIntersect(Ray & ray, double minValOfParamT){
        if (ray.t > 0 && ray.t < minValOfParamT) {
            return true;
        }
        return false;
    }

    static double getIntersectionParameterT(Object * object, Ray & ray) {
        if (object->name == "floor") {
            return -(ray.start.z / ray.dir.z);
        }
        else if (object->name == "sphere") {
            auto s = dynamic_cast<Sphere *>(object);
            double t, t1, t2, r;

            r = s->radius;
            Vector R0 = ray.start - s->reference_point;
            Vector Rd = ray.dir;

            double a = Rd.dot(Rd);
            double b = 2.0 * Rd.dot(R0);
            double c = R0.dot(R0) - r * r;

            double dVal = b * b - 4.0 * a * c;

            if (dVal < 0) {
                t = -1.0;
            } else {
                double determinant = sqrt(dVal);
                t1 = (-b - determinant) / (2.0 * a);
                t2 = (-b + determinant) / (2.0 * a);

                if (t1 >= 0 && t2 >= 0) {
                    t = std::min(t1, t2);
                } else {
                    t = std::max(t1, t2);
                }
            }

            return t;
        }
        return 0.0;
    }

};

class RayTracingCapturer {
    RayTracingExecutor rtExecutor;
public:
    Scene & scene;

    RayTracingCapturer(Scene &sc) : scene(sc), rtExecutor(sc) {}

    void capture(const std::string & filename, double windowWidth, double windowHeight, double imageWidth, double imageHeight, const Camera & camera) const {
        std::vector<std::vector<Color>> imagePixels;

        double planeDistance = (windowHeight / 2.0) / tan(camera.fovy / 2.0 * (pi / 180.0));

        Vector frameTopLeft = camera.pos + (camera.l * planeDistance) - (camera.r * (windowWidth / 2.0)) + (camera.u * (windowHeight / 2.0));

        double du = windowWidth / imageWidth;
        double dv = windowHeight / imageHeight;

        for (int i = 0; i < imageHeight; i++) {
            imagePixels.emplace_back();

            for (int j = 0; j < imageWidth; j++) {
                Vector currentTopLeft = frameTopLeft + camera.r * (j * du) - camera.u * (i * dv);

                Vector cameraToPixelDir = (currentTopLeft - camera.pos).normalize();
                Ray cameraToPixelRay(camera.pos, cameraToPixelDir);

                imagePixels[i].push_back(rtExecutor.execute(cameraToPixelRay));
            }
        }

        generateBitmap(filename, imageWidth, imageHeight, imagePixels);
    }

    static void generateBitmap(const std::string & filename, double imageWidth, double imageHeight, const std::vector<std::vector<Color>> &frame) {
        bitmap_image image((unsigned int)imageWidth, (unsigned int)imageHeight);
        for (int i = 0; i < imageHeight; i++) {
            for (int j = 0; j < imageWidth; j++) {

                double r = std::min(255.0, std::max(0.0, frame[i][j].r * 255));
                double g = std::min(255.0, std::max(0.0, frame[i][j].g * 255));
                double b = std::min(255.0, std::max(0.0, frame[i][j].b * 255));

                image.set_pixel(j, i, r, g, b);
            }
        }
        image.save_image(filename);
    }
};