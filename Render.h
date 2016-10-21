#ifndef RAYTRACING_RENDER_H
#define RAYTRACING_RENDER_H

#include <vector>

#include "Material.h"
#include "Scene.h"

struct Screen {
    Screen() = default;

    Screen(Point camera, Point leftBottomCorner, Vector leftToRight, Vector bottomToTop) :
            camera(camera), leftBottomCorner(leftBottomCorner), leftToRight(leftToRight), bottomToTop(bottomToTop) { }

    Screen(Point leftBottomCorner, Vector leftToRight, Vector bottomToTop) :
            leftBottomCorner(leftBottomCorner), leftToRight(leftToRight), bottomToTop(bottomToTop) {
        camera = leftBottomCorner + leftToRight * 0.5 + bottomToTop * 0.5 +
                 (leftToRight * bottomToTop).normalise(1.5 * leftToRight.length());
    }

    Point camera;
    Point leftBottomCorner;
    Vector leftToRight;
    Vector bottomToTop;
};

class Render {
    std::vector<std::vector<Color>> colorMap;
    Scene *scene;
    Screen screen;
    int height;
    int width;

    const static int maxDepth = 15;
    constexpr static double minIntensity = 0.1;

    void calculatePixels();

    bool isVisible(Point rayStart, Vector rayDirection, Primitive *object) const;

    double countRayIntensity(const Intersection &intersection) const;

    Color traceRay(Point &rayStart, Vector rayDirection, double intensity, int depth) const;

public:
    Render(Scene *scene, Screen screen, int height, int width);

    int getHeight() const { return colorMap.size(); }

    int getWidth() const { return colorMap[0].size(); }

    Color getPixel(int y, int x) { return colorMap[y][x]; }
};


#endif //RAYTRACING_RENDER_H
