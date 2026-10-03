/*
Practica 6: Texturizado
Cubo por codigo + cubo importado de Blender.
*/

#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"
#include <filesystem>
#include <fstream>
#include "HolocronGeometria.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshListColor;
std::vector<MeshModel*> meshListModel;
std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture holocronTexture;

Model Dado_M;
Model Holocron_M;
Model Avion_M;

Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

static const char* vShader = "shaders/shader_texture.vert";
static const char* fShader = "shaders/shader_texture.frag";

void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        // XYZ                  UV           Normal
        -1.0f, -1.0f, -0.6f,   0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,   0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,   1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,   0.5f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f,
         10.0f, 0.0f, -10.0f, 10.0f,  0.0f, 0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,  0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
         10.0f, 0.0f,  10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f
    };

    unsigned int vegetacionIndices[] = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };

    GLfloat vegetacionVertices[] = {
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

         0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.0f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    MeshModel* obj1 = new MeshModel();
    obj1->CreateMeshModel(vertices, indices, 32, 12);
    meshListModel.push_back(obj1);

    MeshModel* obj2 = new MeshModel();
    obj2->CreateMeshModel(vertices, indices, 32, 12);
    meshListModel.push_back(obj2);

    MeshModel* obj3 = new MeshModel();
    obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
    meshListModel.push_back(obj3);

    MeshModel* obj4 = new MeshModel();
    obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
    meshListModel.push_back(obj4);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void CrearDado()
{
    // Atlas: 3 columnas y 2 filas.
    const GLfloat U1 = 1.0f / 3.0f;
    const GLfloat U2 = 2.0f / 3.0f;

    // Cada vertice: X, Y, Z, U, V, NX, NY, NZ.
    GLfloat vertices[] = {
        // Frente +Z: Rebelde, arriba izquierda.
        -0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, U1,   0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, U1,   1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        // Derecha +X: Imperio, arriba centro.
         0.5f, -0.5f,  0.5f, U1, 0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, U2, 0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, U2, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, U1, 1.0f, 1.0f, 0.0f, 0.0f,

         // Atras -Z: Rebelde con puno, arriba derecha.
          0.5f, -0.5f, -0.5f, U2,   0.5f, 0.0f, 0.0f, -1.0f,
         -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, -1.0f,
         -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
          0.5f,  0.5f, -0.5f, U2,   1.0f, 0.0f, 0.0f, -1.0f,

          // Izquierda -X: Rebelde con estrellas, abajo izquierda.
          -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
          -0.5f, -0.5f,  0.5f, U1,   0.0f, -1.0f, 0.0f, 0.0f,
          -0.5f,  0.5f,  0.5f, U1,   0.5f, -1.0f, 0.0f, 0.0f,
          -0.5f,  0.5f, -0.5f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f,

          // Abajo -Y: Calavera mandaloriana, abajo centro.
          -0.5f, -0.5f, -0.5f, U1, 0.0f, 0.0f, -1.0f, 0.0f,
           0.5f, -0.5f, -0.5f, U2, 0.0f, 0.0f, -1.0f, 0.0f,
           0.5f, -0.5f,  0.5f, U2, 0.5f, 0.0f, -1.0f, 0.0f,
          -0.5f, -0.5f,  0.5f, U1, 0.5f, 0.0f, -1.0f, 0.0f,

          // Arriba +Y: Mandalorian y Grogu, abajo derecha.
          -0.5f, 0.5f,  0.5f, U2,   0.0f, 0.0f, 1.0f, 0.0f,
           0.5f, 0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
           0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,
          -0.5f, 0.5f, -0.5f, U2,   0.5f, 0.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    MeshModel* dado = new MeshModel();
    dado->CreateMeshModel(vertices, indices, 192, 36);
    meshListModel.push_back(dado);
}

void CrearHolocron() {
    std::vector<GLfloat> datos;
    std::vector<unsigned int> indices;
    for (size_t t=0;t<std::size(triangulos);t+=3) {
        auto p=posiciones[triangulos[t]], q=posiciones[triangulos[t+1]], r=posiciones[triangulos[t+2]];
        glm::vec3 n=glm::normalize(glm::cross(q-p,r-p));
        unsigned int face=caras[t/3];
        // Base planar y limites propios de la cara, compartidos entre sus triangulos.
        glm::vec3 normal(0);
        for(size_t k=0;k<std::size(triangulos);k+=3) if(caras[k/3]==face) {
            auto a=posiciones[triangulos[k]], b=posiciones[triangulos[k+1]], c=posiciones[triangulos[k+2]];
            normal=glm::normalize(glm::cross(b-a,c-a)); break;
        }
        glm::vec3 ref=std::abs(normal.y)<.9f?glm::vec3(0,1,0):glm::vec3(0,0,1);
        auto right=glm::normalize(glm::cross(ref,normal));
        auto up=glm::normalize(glm::cross(normal,right));
        float x0=1e9f,x1=-1e9f,y0=1e9f,y1=-1e9f;
        for(size_t k=0;k<std::size(triangulos);++k) if(caras[k/3]==face) {
            auto v=posiciones[triangulos[k]];
            float x=glm::dot(v,right),y=glm::dot(v,up);
            x0=std::min(x0,x);x1=std::max(x1,x);y0=std::min(y0,y);y1=std::max(y1,y);
        }
        if(t==0 || caras[t/3]!=caras[(t-3)/3]) printf("Cara %u: region propia del atlas.\n",face+1);
        for(int j=0;j<3;++j) {
            auto v=posiciones[triangulos[t+j]];
            float u=(glm::dot(v,right)-x0)/(x1-x0);
            float w=(glm::dot(v,up)-y0)/(y1-y0);
            u=(face%4+.03f+.94f*u)/4;
            w=(3-face/4+.03f+.94f*w)/4;
            datos.insert(datos.end(),{v.x,v.y,v.z,u,w,n.x,n.y,n.z});
            indices.push_back(static_cast<unsigned int>(indices.size()));
        }
    }
    MeshModel* holocron = new MeshModel();
    holocron->CreateMeshModel(datos.data(),indices.data(),static_cast<unsigned int>(datos.size()),static_cast<unsigned int>(indices.size()));
    meshListModel.push_back(holocron);
    printf("Ejercicio 1: 14 caras texturizadas, %zu vertices con UV y normales; %zu triangulos.\n",indices.size(),indices.size()/3);
}
int main(int argc, char** argv)
{
    auto dir = std::filesystem::absolute(argv[0]).parent_path();
    for (int i = 0; i < 3; ++i) {
        if (std::filesystem::exists(dir / "Textures")) { std::filesystem::current_path(dir); break; }
        dir = dir.parent_path();
    }
    bool evidencia = argc > 1 && std::string(argv[1]) == "--evidencia";
    int frames = 0;
    mainWindow = Window(1366, 768);
    if (mainWindow.Initialise() != 0) return 1;

    CreateObjects();
    CrearDado();
    CrearHolocron();
    CreateShaders();

    // Camara mirando inicialmente hacia los dos cubos.
    camera = Camera(
        glm::vec3(-2.0f, 7.0f, 21.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        -9.0f,
        6.0f,
        0.12f
    );

    plainTexture = Texture("Textures/plain.png");
    plainTexture.LoadTextureA();

    pisoTexture = Texture("Textures/piso.tga");
    pisoTexture.LoadTextureA();

    dadoTexture = Texture("Textures/starwars_atlas.png");
    dadoTexture.LoadTextureA();

    holocronTexture = Texture("Textures/P06-320324187-atlas.png");
    holocronTexture.LoadTextureA();

    Holocron_M.LoadModel("Models/P06-320324187.obj");
    // Ejercicio 3: modelo sencillo del ZIP, con UV de ojos, cara y alas.
    Avion_M.LoadModel("Models/P06-320324187-avion.obj");
    Dado_M.LoadModel(
        "Models/Cubo_StarWars.obj"
    );

    std::vector<std::string> skyboxFaces;
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

    skybox = Skybox(skyboxFaces);

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (GLfloat)mainWindow.getBufferWidth()
        / mainWindow.getBufferHeight(),
        0.1f,
        1000.0f
    );

    glm::mat4 model(1.0f);
    glm::vec3 color(1.0f, 1.0f, 1.0f);

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = (GLfloat)glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime = std::min(deltaTime, 0.05f);
        lastTime = now;

        glfwPollEvents();

        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(
            mainWindow.getXChange(),
            mainWindow.getYChange()
        );

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.DrawSkybox(
            camera.calculateViewMatrix(),
            projection
        );

        shaderList[0].UseShader();

        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        glUniformMatrix4fv(
            uniformProjection, 1, GL_FALSE,
            glm::value_ptr(projection)
        );

        glUniformMatrix4fv(
            uniformView, 1, GL_FALSE,
            glm::value_ptr(camera.calculateViewMatrix())
        );

        // Piso.
        color = glm::vec3(1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        model = glm::mat4(1.0f);
        model = glm::translate(
            model, glm::vec3(0.0f, -2.0f, 0.0f)
        );
        model = glm::scale(
            model, glm::vec3(30.0f, 1.0f, 30.0f)
        );

        glUniformMatrix4fv(
            uniformModel, 1, GL_FALSE,
            glm::value_ptr(model)
        );

        pisoTexture.UseTexture();
        meshListModel[2]->RenderMeshModel();

        // Ejercicio 1: cubo creado por codigo.
        // Lado 1; se coloca a la izquierda.
        color = glm::vec3(1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        model = glm::mat4(1.0f);
        model = glm::translate(
            model, glm::vec3(-1.5f, 4.5f, -2.0f)
        );

        glUniformMatrix4fv(
            uniformModel, 1, GL_FALSE,
            glm::value_ptr(model)
        );

        dadoTexture.UseTexture();
        meshListModel[4]->RenderMeshModel();

        // Ejercicio 2: cubo importado de Blender.
        // El cubo predeterminado tiene lado 2.
        // Escala 0.5 para que tenga lado 1.
        model = glm::mat4(1.0f);
        model = glm::translate(
            model, glm::vec3(1.5f, 4.5f, -2.0f)
        );
        model = glm::scale(model, glm::vec3(0.5f));

        glUniformMatrix4fv(
            uniformModel, 1, GL_FALSE,
            glm::value_ptr(model)
        );

        glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
        Dado_M.RenderModel();

        // Holocron de la base del profesor.
        color = glm::vec3(1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        model = glm::mat4(1.0f);
        model = glm::translate(
            model, glm::vec3(-8.5f, 4.5f, 0.0f)
        );
        model = glm::scale(model, glm::vec3(0.5f));

        glUniformMatrix4fv(
            uniformModel, 1, GL_FALSE,
            glm::value_ptr(model)
        );

        holocronTexture.UseTexture();
        meshListModel[5]->RenderMeshModel();

        // Holocron importado de la base del profesor.
        color = glm::vec3(1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        model = glm::mat4(1.0f);
        model = glm::translate(
            model, glm::vec3(-4.5f, 2.5f, 0.0f)
        );
        model = glm::scale(model, glm::vec3(0.5f));

        glUniformMatrix4fv(
            uniformModel, 1, GL_FALSE,
            glm::value_ptr(model)
        );

        Holocron_M.RenderModel();
        // Avion importado: ojos en vidrio, nariz y sonrisa al frente, logos en alas.
        model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 2.5f, -2.0f));
        model = glm::scale(model, glm::vec3(0.8f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
        Avion_M.RenderModel();

        if (evidencia && ++frames == 3) {
            std::vector<unsigned char> rgb(1366 * 768 * 3);
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, 1366, 768, GL_RGB, GL_UNSIGNED_BYTE, rgb.data());
            std::ofstream out("Evidencias/P06-320324187-escena.ppm", std::ios::binary);
            out << "P6\n1366 768\n255\n";
            for(int y=767; y>=0; --y) out.write(reinterpret_cast<char*>(rgb.data()+y*1366*3),1366*3);
            printf("Escena: dos dados, dos holocrones, avion texturizado, piso y skybox. Error GL: %u\n", glGetError());
            break;
        }
        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}




