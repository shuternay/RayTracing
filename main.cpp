#include <iostream>
#include <string>
#include <vector>

#include <QImage>
#include <QApplication>
#include <QLabel>
#include <chrono>

#include "Scene.h"
#include "Render.h"
#include "ConvertToQImage.h"
#include "LoadFromStl.h"
#include "LoadFromRt.h"


void draw(Scene &scene, const Screen &screen, int height, int width, const std::string name) {
#ifdef COUNT_STATS
    KDNode::_resetStats();
#endif
    std::chrono::steady_clock::time_point buildStart = std::chrono::steady_clock::now();
    scene.buildScene();
    std::chrono::steady_clock::time_point buildEnd = std::chrono::steady_clock::now();

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    Render render(&scene, screen, height, width);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    QImage image = ConvertToQImage(&render).getQImage();
    image.save(QString::fromStdString(name) + ".png");

    std::cerr << name << " build time: " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(buildEnd - buildStart).count() << "\n";
    std::cerr << name << " time: " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
#ifdef COUNT_STATS
    std::cerr << "build calls: " << KDNode::_getBuildCalls() << "\n";
    std::cerr << "find calls: " << KDNode::_getFindCalls() <<
    " (" << KDNode::_getFindCalls() * 1.0 / (height * width) << " per pix)" << "\n\n";
#endif
}


void drawTriangle1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Triangle(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0), Material(Color(1, 0, 0), 0, 0)));
    scene.addLightSource(LightSource(Point(1, 1, 1), 1));

    draw(scene, Screen(Point(-4.0 / 3, -1, 1), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawTriangle2() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    LoadFromStl::load(std::string("../models/triangle.stl"), Material(Color(0.5, 0, 0), 0, 0), scene);
    scene.addLightSource(LightSource(Point(2, 2, 2), 10));

    draw(scene, Screen(Point(-4.0 / 3, -1, 5), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawPolygon1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    std::vector<Point> vertices = {
            Point(0, 1, 0),
            Point(-1, 0, 0),
            Point(1, -2, 0),
            Point(1, 0, 0)
    };
    scene.addObject(new Polygon(vertices, Material(Color(1, 0, 0), 0, 0)));
    scene.addLightSource(LightSource(Point(1, 1, 1), 1.5));

    draw(scene, Screen(Point(-4.0 / 3, -1.7, 3), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawSphere1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Sphere(Point(0, 0, 0), 0.7, Material(Color(0, 1, 0), 0, 0)));
    scene.addLightSource(LightSource(Point(1, 1, 1), 1));

    draw(scene, Screen(Point(-4.0 / 3, -1, 1), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawSphereAndTriangle1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Sphere(Point(0, 0, 0), 0.7, Material(Color(0, 1, 0), 0, 0)));
    scene.addObject(new Triangle(Point(0, 0, 0.4), Point(1, 0, 0.4), Point(0, 1, 0.4), Material(Color(1, 0, 0), 0, 0)));
    scene.addLightSource(LightSource(Point(-1, -1, 1.5), 1.3));
    scene.addLightSource(LightSource(Point(0.0, 0.0, 5), 3));

    draw(scene, Screen(Point(-4.0 / 3, -1, 1), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawGnomeWithBottle1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    LoadFromStl::load(std::string("../models/gnome.stl"), Material(Color(1, 0, 0), 0, 0), scene);
    scene.addLightSource(LightSource(Point(0, 5, 10), 60));
    scene.addLightSource(LightSource(Point(10, 5, 0), 60));

    draw(scene, Screen(Point(-10.0 / 3, -0.5, 10), Vector(20.0 / 3, 0, 0), Vector(0, 5, 0)), 600, 800, caseName);
}

void drawGnomeWithBottle2() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    LoadFromStl::load(std::string("../models/gnome.stl"), Material(Color(1, 0, 0), 0, 0), scene);
    scene.addLightSource(LightSource(Point(-10, 10, 0), 60));
    scene.addLightSource(LightSource(Point(10, 5, 0), 60));
    scene.addLightSource(LightSource(Point(-10, 10, 10), 60));

    draw(scene, Screen(Point(5, 0, 10.0 / 3), Vector(0, 0, -20.0 / 3), Vector(0, 5, 0)), 600, 800, caseName);
}

void drawUtahTeapot1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    LoadFromStl::load(std::string("../models/utah_teapot.stl"), Material(Color(1, 0, 0), 0, 0), scene);
    scene.addLightSource(LightSource(Point(0, 100, 0), 3000));
    scene.addLightSource(LightSource(Point(0, 0, 100), 3000));

    draw(scene, Screen(Point(-200.0 / 3, -50, 50), Vector(400.0 / 3, 0, 0), Vector(0, 100, 0)), 600, 800, caseName);
}

void drawUtahTeapot2() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    LoadFromStl::load(std::string("../models/utah_teapot.stl"), Material(Color(1, 0, 0), 0, 0), scene);
    scene.addLightSource(LightSource(Point(0, 100, 0), 3000));
    scene.addLightSource(LightSource(Point(0, 0, 100), 3000));

    draw(scene, Screen(Point(200.0 / 3, 50, -50), Vector(-400.0 / 3, 0, 0), Vector(0, 0, 100)), 600, 800, caseName);
}

void drawMirror1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Polygon({Point(-1, -1, 1), Point(0, -1, 0), Point(0, 1, 0), Point(-1, 1, 1)},
                                Material(Color(0, 0, 1), 0.5, 0)));
//    scene.addObject(new Polygon({Point(0, -10, -10), Point(1, -10, 10), Point(1, 10, 10), Point(0, 10, -10)},
//                                Color(1, 0, 0), 0.5));
    scene.addObject(new Polygon({Point(0, -10, 0), Point(1, -10, 10), Point(1, 10, 10), Point(0, 10, 0)},
                                Material(Color(1, 0, 0), 0.5, 0)));
    scene.addLightSource(LightSource(Point(0, 0, 100), 100000));

    draw(scene, Screen(Point(-4.0 / 3, -1, 3), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawMirror2() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Polygon({Point(-1, -1, 1), Point(0, -1, 0), Point(0, 1, 0), Point(-1, 1, 1)},
                                Material(Color(0, 1, 0), 0.5, 0)));
    scene.addObject(new Sphere(Point(-1, 0, 0), 1, Material(Color(0, 0, 1), 0.95, 0)));
    scene.addObject(new Polygon({Point(0, 0, 0), Point(1, 0, 10), Point(1, 10, 10), Point(0, 10, 0)},
                                Material(Color(1, 0, 0), 0.5, 0)));
    scene.addLightSource(LightSource(Point(0, 0, 20), 700));

    draw(scene, Screen(Point(-4.0 / 3, -1, 5), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawMirror3() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Sphere(Point(-2, 0, 0), 1.5, Material(Color(0, 0, 1), 0.8, 0)));
    scene.addObject(new Sphere(Point(2, 0, 0), 1.5, Material(Color(0, 0, 1), 0.8, 0)));
    scene.addLightSource(LightSource(Point(0, 0, 20), 500));

    draw(scene, Screen(Point(-4.0 / 3, -1, 10), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawGnomeWithBottleWithMirrors1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    LoadFromStl::load(std::string("../models/gnome.stl"), Material(Color(1, 0, 0), 0, 0), scene);
    scene.addObject(new Polygon({Point(-10, -2, -10), Point(-10, 7, -10), Point(-13, 7, 0), Point(-13, -2, 0)},
//                                Material(Color(1, 0, 0), 0.0, 0)));
                                Material(Color(1, 0, 0), 1, 0)));
    scene.addObject(new Polygon({Point(-13, -2, 0), Point(-13, 7, 0), Point(-10, 7, 10), Point(-10, -2, 10)},
//                                Material(Color(0, 0, 1), 0.0, 0)));
                                Material(Color(0, 0, 1), 0.7, 0)));
    scene.addLightSource(LightSource(Point(-10, 10, 0), 60));
    scene.addLightSource(LightSource(Point(10, 5, 0), 60));
    scene.addLightSource(LightSource(Point(-10, 10, 10), 60));

    draw(scene, Screen(Point(5, 0, 10.0 / 3), Vector(0, 0, -20.0 / 3), Vector(0, 5, 0)), 600, 800, caseName);
}


void drawGlass1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Sphere(Point(0, 0, -3), 1, Material(Color(0, 0, 1), 0, 0)));
    scene.addObject(new Polygon({Point(-5, 0, 0), Point(-5, -5, 0), Point(5, -5, 0), Point(5, 0, 0)},
                                Material(Color(0, 1, 0), 0, 0.5)));
    scene.addObject(new Polygon({Point(-5, 0, -1), Point(-5, -5, -1), Point(5, -5, -1), Point(5, 0, -1)},
                                Material(Color(0, 1, 0), 0, 0.5)));
    scene.addLightSource(LightSource(Point(0, 20, 20), 700));

    draw(scene, Screen(Point(-4.0 / 3, -1, 5), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawGlass2() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Sphere(Point(0, 1, -3), 1, Material(Color(0, 0, 1), 0, 0)));
    scene.addObject(new Sphere(Point(0, 0, -1), 1, Material(Color(0, 0, 1), 0, 0.5)));
    scene.addLightSource(LightSource(Point(0, 20, 20), 700));
//    scene.addLightSource(LightSource(Point(20, -20, 20), 700));

    draw(scene, Screen(Point(-4.0 / 3, -1, 5), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawInternalReflection1() {
    std::string caseName = __FUNCTION__;

    Scene scene;
    scene.addObject(new Polygon({Point(-1, 0, 30), Point(-1, 0, -10), Point(-1, -5, -10), Point(-1, -5, 30)},
                                Material(Color(0, 1, 0), 0, 0.01)));
    scene.addObject(new Polygon({Point(1, 0, 30), Point(1, -5, 30), Point(1, -5, -10), Point(1, 0, -10)},
                                Material(Color(0, 1, 0), 0, 0.01)));
    scene.addObject(new Polygon({Point(-1, 5, 30), Point(-1, 5, -10), Point(-1, 0, -10), Point(-1, 0, 30)},
                                Material(Color(0, 1, 0), 0, 0.0)));
    scene.addObject(new Polygon({Point(1, 5, 30), Point(1, 0, 30), Point(1, 0, -10), Point(1, 5, -10)},
                                Material(Color(0, 1, 0), 0, 0.0)));
    scene.addObject(new Sphere(Point(0, 0, -15), 1, Material(Color(0, 0, 1), 0, 0)));

    scene.addLightSource(LightSource(Point(0, 20, 20), 700));

    draw(scene, Screen(Point(-4.0 / 3, -1, 5), Vector(8.0 / 3, 0, 0), Vector(0, 2, 0)), 600, 800, caseName);
}

void drawRtFile(const std::string &name, const std::string &caseName) {
    Scene scene;
    Screen screen;
    LoadFromRt::load(std::string(name), scene, screen);

    draw(scene, screen, 768, 1024, caseName);
}

int main(int argc, char *argv[]) {
    drawTriangle1();
    drawTriangle2();
    drawPolygon1();
    drawSphere1();
    drawSphereAndTriangle1();
    drawGnomeWithBottle1();
    drawGnomeWithBottle2();
    drawUtahTeapot1();
    drawUtahTeapot2();
    drawMirror1();
    drawMirror2();
    drawMirror3();
    drawGnomeWithBottleWithMirrors1();
    drawGlass1();
    drawGlass2();
    drawInternalReflection1();

    drawRtFile("../models/coloringtest.rt", "rt_coloringtest");
    // drawRtFile("../models/gnome_pure.rt", "rt_gnome_pure");
    drawRtFile("../models/pool.rt", "rt_pool");
    drawRtFile("../models/quad.rt", "rt_quad");
    drawRtFile("../models/simple.rt", "rt_simple");
    // drawRtFile("../models/spheres_pure.rt", "rt_spheres_pure");

    return 0;
}
