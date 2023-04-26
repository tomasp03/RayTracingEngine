#pragma once
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "Shapes/AllShapes.h"
#include "Files.h"
#include "ToString.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <string>


static struct Skybox
{
    std::string file = "";
    float* image = nullptr;
    int x = 0;
    int y = 0;
    int chanels = 4;
};

struct Scene
{
    enum ObjectType
    {
        NONE = -1, SPHERE = 0, PLANE = 1
    };

    Scene(const std::string& sceneFile, const std::string& skyboxFile);
    std::vector<Material> materials;
    Files file;
    glm::ivec2 dimensions;
    void Load(std::string path);
    void Save(std::string name);


    //For now Planes are in the beginning, Spheres at the end
    std::vector<Shape*> shapes;
    int SphereCount = 0;
    int PlaneCount = 0;
    std::string folder = "assets/scenes";

    Skybox skybox;

    void LoadSkybox(std::string skyboxFile);

private:

    std::string TypeOfObject[2] = {
        "#SPHERE",
        "#PLANE"
    };

    Sphere* sphere;
    Plane* plane;

    void LoadSphere(std::string& line, Sphere* sphere, Material& mat);
    void LoadPlane(std::string& line, Plane* plane, Material& mat);
};
