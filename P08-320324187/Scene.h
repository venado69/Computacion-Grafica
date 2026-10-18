#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include "Window.h"
#include "Model.h"
#include "Shader_light.h"
#include "Texture.h"
#include "Material.h"
#include "BaseE08.h"
class Scene {
public:
    bool Initialise(const char* executable,bool capture);
    bool ShouldClose()const;
    void Frame();
    int Shutdown();
private:
    Window window;
    Camera camera;
    Model ship;
    BaseE08 baseE08;
    Shader shader;
    MeshModel flame;
    Texture flameTexture{"Textures/plasma-azul.png"};
    Material hull{.30f,64},ground{.12f,24};
    DirectionalLight ambient{1,.96f,.85f,.16f,.35f,-3,-6,-2};
    PointLight shipPoint{1,.72f,.30f,0,3.5f,0,0,0,1,.09f,.035f};
    PointLight enginePoints[2]{PointLight(.04f,.35f,1,0,6,0,0,0,1,.10f,.045f),PointLight(.04f,.35f,1,0,6,0,0,0,1,.10f,.045f)};
    SpotLight shipSpot{1,.85f,.55f,0,6,0,0,0,0,-1,0,1,.08f,.025f,28};
    PointLight activePoints[5];
    SpotLight activeSpots[3];
    unsigned int pointCount=0,spotCount=0;
    glm::mat4 projection{1},view{1},shipMatrix{1},model{1};
    glm::vec3 eye{18,14,28},shipPosition{0,4,-10},enginePosition[2],lightPosition,spotPosition,spotDirection;
    glm::vec3 nozzles[2]{glm::vec3(-.4f,.25f,-1.02f),glm::vec3(.4f,.25f,-1.02f)};
    GLint unlit=-1,plasma=-1,timeUniform=-1;
    double last=0,now=0,sceneTime=0;
    float dt=0;
    bool evidence=false,finished=false;
    int frame=0,index=0,result=0;
    GLenum error=GL_NO_ERROR;
    std::vector<unsigned char> pixels;
    std::ofstream screenshot,log;
    std::string screenshotName;
    void CreateMeshes();
    void UpdateLights();
    void DrawScene();
    void Capture();
};
