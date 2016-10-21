#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H


#include "Primitives.h"
#include "KDTree.h"

class Scene {
    std::vector<Primitive *> objects;
    std::vector<LightSource> lightSources;
    KDTree objectsTree;

public:
    ~Scene();
    void addObject(Primitive *primitive);
    void addLightSource(const LightSource &lightSource);
    void buildScene();
    bool findRayIntersection(Point rayStart, Vector rayDirection, Intersection &nearestIntersection);

    const std::vector<LightSource> &getLightSources() const { return lightSources; }
};


#endif //RAYTRACING_SCENE_H
