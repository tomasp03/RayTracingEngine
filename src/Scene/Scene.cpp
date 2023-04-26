#include "Scene.h"

static glm::vec3 LoadVec3(std::string line)
{
    int vecLoc = line.find("vec3");
    float x = std::stof(line.substr(vecLoc + 5, vecLoc + 12));
    float y = std::stof(line.substr(vecLoc + 15, vecLoc + 22));
    float z = std::stof(line.substr(vecLoc + 25, vecLoc + 32));

    return glm::vec3(x, y, z);
}

static glm::vec4 LoadVec4(std::string line)
{
    int vecLoc = line.find("vec4");
    float x = std::stof(line.substr(vecLoc + 5, vecLoc + 12));
    float y = std::stof(line.substr(vecLoc + 15, vecLoc + 22));
    float z = std::stof(line.substr(vecLoc + 25, vecLoc + 32));
    float w = std::stof(line.substr(vecLoc + 35, vecLoc + 42));

    return glm::vec4(x, y, z, w);
}

Scene::Scene(const std::string& sceneFile, const std::string& skyboxFile)
{
    Load(sceneFile);
    LoadSkybox(skyboxFile);
}

void Scene::LoadSkybox(std::string skyboxFile)
{
    delete[] skybox.image;

    skybox.file = skyboxFile;

    stbi_set_flip_vertically_on_load(true);
    skybox.image = stbi_loadf(skybox.file.c_str(), &skybox.x, &skybox.y, &skybox.chanels, 0);
}

void Scene::Load(std::string path)
{
    for (Shape* shape : shapes)
    {
        delete shape;
    }

    Shape::PlanePos.clear();
    Shape::SpherePos.clear();
    shapes.clear();

    std::ifstream stream;
    stream.open(path);

    if (!stream.is_open())
    {
        std::cout << "ERROR: Failed Loading FILE" << std::endl;
    }

    std::string line;
    Material mat;
    ObjectType currentObject = ObjectType::NONE;
    while (std::getline(stream, line))
    {

        if (line.find(TypeOfObject[ObjectType::SPHERE]) != std::string::npos)
        {
            currentObject = ObjectType::SPHERE;
            sphere = new Sphere();
            continue;
        }
        else if (line.find(TypeOfObject[ObjectType::PLANE]) != std::string::npos)
        {
            currentObject = ObjectType::PLANE;
            plane = new Plane();
            continue;
        }

        if (currentObject == ObjectType::SPHERE)
        {
            LoadSphere(line, sphere, mat);
        }
        else if (currentObject == ObjectType::PLANE)
        {
            LoadPlane(line, plane, mat);
        }
    }
    stream.close();
}

void Scene::LoadSphere(std::string& line, Sphere* sphere, Material& mat)
{
    std::string type = line.substr(0, line.find(" "));

    if (type == "Radius")
        sphere->radius(std::stof(line.substr(line.find("=") + 2, line.find("\n"))));
    else if (type == "Center")
    {
        sphere->center(LoadVec3(line));
    }
    else if (type == "Albido")
    {
        mat.albido = LoadVec4(line);
    }
    else if (type == "Smoothness")
    {
        mat.smoothness = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
    }
    else if (type == "SpecularColor")
    {
        mat.specularColor = LoadVec4(line);
    }
    else if (type == "SpecularProb")
    {
        mat.specularProbability = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
    }
    else if (type == "EmissionStrengh")
    {
        mat.emissionStrength = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
    }
    else if (type == "EmissionColor")
    {
        mat.emissionColor = LoadVec4(line);
    }
    else
    {
        sphere->material(mat);
        Shape::SpherePos.push_back(shapes.size());
        shapes.push_back(sphere);
    }
}

void Scene::LoadPlane(std::string& line, Plane* plane, Material& mat)
{
    std::string type = line.substr(0, line.find(" "));

    if (type == "Point")
    {
        plane->point(LoadVec3(line));
    }
    else if (type == "Normal")
    {
        plane->normal(LoadVec3(line));
    }
    else if (type == "Albido")
    {
        mat.albido = LoadVec4(line);
    }
    else if (type == "Smoothness")
    {
        mat.smoothness = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
    }
    else if (type == "SpecularColor")
    {
        mat.specularColor = LoadVec4(line);
    }
    else if (type == "SpecularProb")
    {
        mat.specularProbability = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
    }
    else if (type == "EmissionStrengh")
    {
        mat.emissionStrength = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
    }
    else if (type == "EmissionColor")
    {
        mat.emissionColor = LoadVec4(line);
    }
    else
    {
        plane->material(mat);
        Shape::PlanePos.push_back(shapes.size());
        shapes.push_back(plane);
    }
}

void Scene::Save(std::string name)
{
    std::ofstream stream;
    std::string file = folder + "/" + name;
    stream.open(file);
    if (!stream.is_open())
    {
        std::cout << "ERROR: Failed Saving FILE" << std::endl;
        return;
    }

    for (int i = 0; i < Shape::SpherePos.size(); i++)
    {
        Shape* shape = shapes[Shape::SpherePos[i]];
        Sphere sphere = *static_cast<Sphere*>(shape);
        std::string number = std::to_string(i);
        std::string radius = "Radius " + number;
        std::string center = "Center " + number;
        std::string albido = "Albido " + number;
        std::string smoothness = "Smoothness " + number;
        std::string specularColor = "SpecularColor " + number;
        std::string specularProb = "SpecularProb " + number;
        std::string EmissionStrengh = "EmissionStrengh " + number;
        std::string EmissionColor = "EmissionColor " + number;

        stream << TypeOfObject[ObjectType::SPHERE] << '\n';
        stream << radius << " = " << sphere.radius() << "\n";
        stream << center << " = " << converter::ToString(sphere.center()) << "\n";
        stream << albido << " = " << converter::ToString(sphere.material().albido) << "\n";
        stream << smoothness << " = " << sphere.material().smoothness << "\n";
        stream << specularColor << " = " << converter::ToString(sphere.material().specularColor) << "\n";
        stream << specularProb << " = " << sphere.material().specularProbability << "\n";
        stream << EmissionStrengh << " = " << sphere.material().emissionStrength << "\n";
        stream << EmissionColor << " = " << converter::ToString(sphere.material().emissionColor) << "\n";
        stream << "\n";
    }

    for (int i = 0; i < Shape::PlanePos.size(); i++)
    {
        Shape* shape = shapes[Shape::PlanePos[i]];
        Plane plane = *static_cast<Plane*>(shape);
        std::string number = std::to_string(i);
        std::string point = "Point " + number;
        std::string normal = "Normal " + number;
        std::string albido = "Albido " + number;
        std::string smoothness = "Smoothness " + number;
        std::string specularColor = "SpecularColor " + number;
        std::string specularProb = "SpecularProb " + number;
        std::string EmissionStrengh = "EmissionStrengh " + number;
        std::string EmissionColor = "EmissionColor " + number;

        stream << TypeOfObject[ObjectType::PLANE] << '\n';
        stream << point << " = " << converter::ToString(plane.point()) << "\n";
        stream << normal << " = " << converter::ToString(plane.normal()) << "\n";
        stream << albido << " = " << converter::ToString(plane.material().albido) << "\n";
        stream << smoothness << " = " << plane.material().smoothness << "\n";
        stream << specularColor << " = " << converter::ToString(plane.material().specularColor) << "\n";
        stream << specularProb << " = " << plane.material().specularProbability << "\n";
        stream << EmissionStrengh << " = " << plane.material().emissionStrength << "\n";
        stream << EmissionColor << " = " << converter::ToString(plane.material().emissionColor) << "\n";
        stream << "\n";
    }

    stream.close();
}