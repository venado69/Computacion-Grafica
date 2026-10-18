#pragma once
#include <memory>
#include "Window.h"
#include "Model.h"
#include "Shader_light.h"
#include "Material.h"
#include "Skybox.h"
class BaseE08 {
public:
    bool Initialise();
    void UpdateLights(Window&,PointLight*,unsigned int&,SpotLight*,unsigned int&,glm::vec3,glm::vec3);
    void DrawSkybox(glm::mat4 view,glm::mat4 projection);
    void Draw(Shader&,Window&);
    void Shutdown();
private:
    Model avion,holocron,rover[8];
    Texture dado{"Textures/starwars_atlas.png"},piso{"Textures/piso.tga"},blanco{"Textures/plain.png"};
    std::unique_ptr<Skybox> skybox;
    Material mate{.12f,24},metal{.35f,64},suelo{.10f,16};
    PointLight luzHolo{.35f,.75f,1,0,3.5f,0,0,0,1,.05f,.035f};
    PointLight luzPinza{0,1,0,0,4,0,0,0,1,.04f,.025f};
    SpotLight faros[6],linterna{1,1,1,0,2.5f,0,0,0,0,0,-1,1,.015f,.002f,22};
    glm::mat4 avionM{1},holoM{1},roverM{1},brazoM{1},model{1};
    glm::vec3 faroPos,faroDir,holoPos,pinzaPos,pivote{2.1f,6,-1.1f},punta{2.44f,12.44f,-.92f};
    const glm::vec3 colores[6]{{1,0,0},{1,1,0},{0,1,0},{0,1,1},{0,0,1},{1,0,1}};
    int index=0;
    void Matrix(Shader&,const glm::mat4&);
    void Emisor(Shader&,glm::vec3,glm::vec3);
};
