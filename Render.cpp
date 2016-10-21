#include "Render.h"

Render::Render(Scene *scene, Screen screen, int height, int width) : scene(scene),
                                                                     screen(screen), height(height),
                                                                     width(width) {
    calculatePixels();
}

void Render::calculatePixels() {
    colorMap = std::vector<std::vector<Color>>(height, std::vector<Color>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Point screenPoint = screen.leftBottomCorner +
                                screen.bottomToTop * (i * 1.0 / height) +
                                screen.leftToRight * (j * 1.0 / width);
            Vector rayDirection = screenPoint - screen.camera;
            colorMap[height - 1 - i][j] = traceRay(screen.camera, rayDirection, 1, 0);
        }
    }
}

bool Render::isVisible(Point rayStart, Vector rayDirection, Primitive *object) const {
    Intersection intersection;

    if (scene->findRayIntersection(rayStart, rayDirection, intersection)) {
        return intersection.primitive == object &&
               Point::doubleEqual(0, (intersection.point - (rayStart + rayDirection)).length());
    }
    return false;
}

double Render::countRayIntensity(const Intersection &intersection) const {
    double rayIntensity = 0;

    for (auto lightSource : scene->getLightSources()) {
        if (isVisible(lightSource.getPosition(), intersection.point - lightSource.getPosition(),
                      intersection.primitive)) {
            double incidence = intersection.surfaceNormal.normalise() %
                               (lightSource.getPosition() - intersection.point).normalise();

            if (incidence > 0) {
                rayIntensity += lightSource.getIntensity() * incidence /
                                (lightSource.getPosition() - intersection.point).squaredLength();
            }
        }
    }

    return rayIntensity;
}

Color Render::traceRay(Point &rayStart, Vector rayDirection, double intensity, int depth) const {
    if (depth > maxDepth || intensity < minIntensity) {
        return Color(0, 0, 0);
    }
    Intersection intersection;

    if (scene->findRayIntersection(rayStart, rayDirection, intersection)) {
        Color reflectedColor(0, 0, 0);
        if (intersection.isFrontFace && intersection.primitive->getMaterial().getReflect() > 1e-3) {
            Vector reflectedRayDirection = rayDirection.reflect(intersection.surfaceNormal);
            reflectedColor = traceRay(intersection.point, reflectedRayDirection,
                                      intensity * intersection.primitive->getMaterial().getReflect(), depth + 1);
        }

        if (Point::doubleEqual(intersection.primitive->getMaterial().getRefract(), 0)) {
            double rayIntensity = countRayIntensity(intersection);
            return intersection.color * rayIntensity * (1 - intersection.primitive->getMaterial().getReflect()) +
                   reflectedColor * intersection.primitive->getMaterial().getReflect();
        }
        else {
            Vector refractedRayDirection;
            if (intersection.isFrontFace) {
                refractedRayDirection = rayDirection.refract(intersection.surfaceNormal,
                                                             intersection.primitive->getMaterial().getRefract());
            }
            else {
                refractedRayDirection = rayDirection.refract(intersection.surfaceNormal,
                                                             1 / intersection.primitive->getMaterial().getRefract());
            }

            Color refractedColor = traceRay(intersection.point, refractedRayDirection,
                                            intensity * (1 - intersection.primitive->getMaterial().getReflect()),
                                            depth + 1);
            return refractedColor * (1 - intersection.primitive->getMaterial().getReflect()) +
                   reflectedColor * intersection.primitive->getMaterial().getReflect();
        }
    }
    else {
        return Color(0.05, 0.05, 0.05);
    }
}





