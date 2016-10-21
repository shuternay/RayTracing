#include "Scene.h"

Scene::~Scene() {
    for (auto object: objects) {
        delete object;
    }
}

void Scene::addObject(Primitive *primitive) {
    objects.push_back(primitive);
}

void Scene::addLightSource(const LightSource &lightSource) {
    lightSources.push_back(lightSource);
}

void Scene::buildScene() {
    objectsTree.buildTree(objects);
}

bool Scene::findRayIntersection(Point rayStart, Vector rayDirection, Intersection &nearestIntersection) {
    return objectsTree.findRayIntersection(rayStart, rayDirection, nearestIntersection);
}










