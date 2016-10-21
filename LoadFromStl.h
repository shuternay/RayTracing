#ifndef RAYTRACING_LOADFROMSTL_H
#define RAYTRACING_LOADFROMSTL_H

#include <fstream>

#include "Scene.h"

class LoadFromStl {
public:
    static void load(const std::string &filePath, const Material &material, Scene &scene);
};


#endif //RAYTRACING_LOADFROMSTL_H
