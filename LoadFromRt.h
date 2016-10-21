#ifndef RAYTRACING_LOADFROMRT_H
#define RAYTRACING_LOADFROMRT_H


#include <map>
#include "Scene.h"
#include "Render.h"

class LoadFromRt {
    static Point readPoint(std::ifstream &fileStream);
    static Color readColor(std::ifstream &fileStream);
    static Screen readViewport(std::ifstream &fileStream);
    static std::map<std::string, Material> readMaterials(std::ifstream &fileStream);
    static void readLights(std::ifstream &fileStream, Scene &scene);
    static void readGeometry(std::ifstream &fileStream, Scene &scene, std::map<std::string, Material> &materials);
    static void readSphere(std::ifstream &fileStream, Scene &scene, std::map<std::string, Material> &materials);
    static void readTriangle(std::ifstream &fileStream, Scene &scene, std::map<std::string, Material> &materials);
    static void readQuadrangle(std::ifstream &fileStream, Scene &scene, std::map<std::string, Material> &materials);
public:
    static void load(const std::string &filePath, Scene &scene, Screen &screen);
};


#endif //RAYTRACING_LOADFROMRT_H
