/*
Pr�ctica 5: Optimizaci�n y Carga de Modelos
*/

#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshListColor;
std::vector<MeshModel*> meshListModel;
std::vector<Shader> shaderList;

Camera camera;


// ======================================================
// MODELOS
// ======================================================

Model Cuerpo_M;
Model Brazo_M;

Model PataDI_M;
Model PataCI_M;
Model PataTI_M;

Model PataDD_M;
Model PataCD_M;
Model PataTD_M;

// Holocron: un modelo para el cuerpo y uno por esquina.
Model HolocronCuerpo_M;
Model HolocronEsquina_M[8];

// Coordenadas del centro en los OBJ exportados desde Blender.
const glm::vec3 centroHolocron(-47.987f, 5.376f, 0.651f);
const glm::vec3 centrosEsquinas[8] = {
    {-50.606f, 7.936f, 3.332f},
    {-45.409f, 7.956f, 3.332f},
    {-45.372f, 8.085f, -1.873f},
    {-50.601f, 8.075f, -1.892f},
    {-50.596f, 2.794f, -2.024f},
    {-45.375f, 2.818f, -2.018f},
    {-45.389f, 2.681f, 3.174f},
    {-50.611f, 2.684f, 3.207f}
};

float anguloEsquina[8] = {};
float objetivoEsquina[8] = {};
bool teclaEsquinaAnterior[8] = {};
const int teclasEsquinas[8] = {
    GLFW_KEY_H, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L,
    GLFW_KEY_N, GLFW_KEY_M, GLFW_KEY_B, GLFW_KEY_V
};


// Satellite parts share the coordinates of the four exported OBJ files.
Model SateliteCuerpo_M;
Model PanelIzquierdo_M;
Model PanelDerecho_M;
Model PalaSuperior_M;

glm::vec3 posicionSatelite(-12.5f, 0.4f, -1.5f);
float anguloSatelite[3] = {};
float objetivoSatelite[3] = {};
bool teclaSateliteAnterior[3] = {};
const int teclasSatelite[3] = { GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C };
// In OBJ coordinates Blender's X remains X; Blender's Y becomes -Z.
const glm::vec3 pivotesSatelite[3] = {
    {-0.2385f, 0.0f, -0.1625f},  // Left panel at its support
    { 0.2645f, 0.0f, -0.0138f},  // Right panel at its support
    { 0.0640f, 0.1310f, -0.2720f} // Upper paddle at its joint
};
const glm::vec3 ejesSatelite[3] = {
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -1.0f}
};

// ======================================================
// SKYBOX
// ======================================================

Skybox skybox;


// ======================================================
// TIEMPO
// ======================================================

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


// ======================================================
// ANGULOS
// ======================================================

float angPataDI = 0.0f;
float angPataCI = 0.0f;
float angPataTI = 0.0f;

float angPataDD = 0.0f;
float angPataCD = 0.0f;
float angPataTD = 0.0f;

float angBrazo = 0.0f;


// ======================================================
// OBJETIVOS DE ROTACION
// ======================================================

float targetPataDI = 0.0f;
float targetPataCI = 0.0f;
float targetPataTI = 0.0f;

float targetPataDD = 0.0f;
float targetPataCD = 0.0f;
float targetPataTD = 0.0f;

float targetBrazo = 0.0f;


// ======================================================
// ESTADOS
// ======================================================

bool estadoPataDI = false;
bool estadoPataCI = false;
bool estadoPataTI = false;

bool estadoPataDD = false;
bool estadoPataCD = false;
bool estadoPataTD = false;

bool estadoBrazo = false;


// ======================================================
// TECLAS ANTERIORES
// ======================================================

bool tecla1Anterior = false;
bool tecla2Anterior = false;
bool tecla3Anterior = false;
bool tecla4Anterior = false;
bool tecla5Anterior = false;
bool tecla6Anterior = false;
bool tecla7Anterior = false;


// ======================================================
// SHADERS
// ======================================================

static const char* vShader = "shaders/shader_m.vert";
static const char* fShader = "shaders/shader_m.frag";


// ======================================================
// CREAR OBJETOS
// ======================================================

void CreateObjects()
{
    unsigned int indices[] =
    {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] =
    {
        -1.0f, -1.0f, -0.6f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] =
    {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] =
    {
        -10.0f, 0.0f, -10.0f,  0.0f,  0.0f,  0.0f, -1.0f, 0.0f,
         10.0f, 0.0f, -10.0f, 10.0f,  0.0f,  0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,  0.0f, 10.0f,  0.0f, -1.0f, 0.0f,
         10.0f, 0.0f,  10.0f, 10.0f, 10.0f,  0.0f, -1.0f, 0.0f
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
}


// ======================================================
// SHADERS
// ======================================================

void CreateShaders()
{
    Shader* shader1 = new Shader();

    shader1->CreateFromFiles(
        vShader,
        fShader
    );

    shaderList.push_back(*shader1);
}


// ======================================================
// MOVIMIENTO SUAVE
// ======================================================

void actualizarAngulo(
    float& angulo,
    float objetivo,
    float velocidad
)
{
    if (angulo < objetivo)
    {
        angulo += velocidad * deltaTime;

        if (angulo > objetivo)
            angulo = objetivo;
    }
    else if (angulo > objetivo)
    {
        angulo -= velocidad * deltaTime;

        if (angulo < objetivo)
            angulo = objetivo;
    }
}


// ======================================================
// MAIN
// ======================================================

int main()
{
    mainWindow = Window(1366, 768);
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();


    // ==================================================
    // CAMARA
    // 3.0f = velocidad WASD
    // 0.3f = sensibilidad del mouse
    // ==================================================

    camera = Camera(
        glm::vec3(0.0f, 0.5f, 7.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -60.0f,
        0.0f,
        3.0f,
        0.3f
    );


    // ==================================================
    // MODELOS
    // ==================================================

    Cuerpo_M = Model();
    Cuerpo_M.LoadModel("Models/Cuerpo.obj");

    Brazo_M = Model();
    Brazo_M.LoadModel("Models/Brazo.obj");

    HolocronCuerpo_M.LoadModel("Models/Holocron_Cuerpo.obj");
    const char* archivosEsquinas[8] = {
        "Models/Holocron_Esquina_1.obj", "Models/Holocron_Esquina_2.obj",
        "Models/Holocron_Esquina_3.obj", "Models/Holocron_Esquina_4.obj",
        "Models/Holocron_Esquina_5.obj", "Models/Holocron_Esquina_6.obj",
        "Models/Holocron_Esquina_7.obj", "Models/Holocron_Esquina_8.obj"
    };
    for (int i = 0; i < 8; ++i)
        HolocronEsquina_M[i].LoadModel(archivosEsquinas[i]);


    SateliteCuerpo_M.LoadModel("Models/Satelite_Cuerpo.obj");
    PanelIzquierdo_M.LoadModel("Models/Panel_Izquierdo.obj");
    PanelDerecho_M.LoadModel("Models/Panel_Derecho.obj");
    PalaSuperior_M.LoadModel("Models/Pala_Superior.obj");

    PataDI_M = Model();
    PataDI_M.LoadModel(
        "Models/Pata_Delantera_Izq.obj"
    );

    PataCI_M = Model();
    PataCI_M.LoadModel(
        "Models/Pata_Centro_Izq.obj"
    );

    PataTI_M = Model();
    PataTI_M.LoadModel(
        "Models/Pata_Trasera_Izq.obj"
    );


    PataDD_M = Model();
    PataDD_M.LoadModel(
        "Models/Pata_Delantera_Der.obj"
    );

    PataCD_M = Model();
    PataCD_M.LoadModel(
        "Models/Pata_Centro_Der.obj"
    );

    PataTD_M = Model();
    PataTD_M.LoadModel(
        "Models/Pata_Trasera_Der.obj"
    );


    // ==================================================
    // SKYBOX
    // ==================================================

    std::vector<std::string> skyboxFaces;

    skyboxFaces.push_back(
        "Textures/Skybox/cupertin-lake_rt.tga"
    );

    skyboxFaces.push_back(
        "Textures/Skybox/cupertin-lake_lf.tga"
    );

    skyboxFaces.push_back(
        "Textures/Skybox/cupertin-lake_dn.tga"
    );

    skyboxFaces.push_back(
        "Textures/Skybox/cupertin-lake_up.tga"
    );

    skyboxFaces.push_back(
        "Textures/Skybox/cupertin-lake_bk.tga"
    );

    skyboxFaces.push_back(
        "Textures/Skybox/cupertin-lake_ft.tga"
    );

    skybox = Skybox(skyboxFaces);


    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;


    glm::mat4 projection =
        glm::perspective(
            45.0f,
            (GLfloat)mainWindow.getBufferWidth() /
            mainWindow.getBufferHeight(),
            0.1f,
            1000.0f
        );


    glm::mat4 model(1.0f);
    glm::mat4 modelaux(1.0f);


    glm::vec3 color =
        glm::vec3(
            1.0f,
            1.0f,
            1.0f
        );


    // ==================================================
    // LOOP
    // ==================================================

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();

        deltaTime =
            now - lastTime;

        lastTime = now;


        // ==============================================
        // EVENTOS
        // ==============================================

        glfwPollEvents();


        camera.keyControl(
            mainWindow.getsKeys(),
            deltaTime
        );


        camera.mouseControl(
            mainWindow.getXChange(),
            mainWindow.getYChange()
        );


        // ==============================================
        // TECLAS
        // ==============================================

        bool tecla1 =
            mainWindow.getsKeys()[GLFW_KEY_1];

        bool tecla2 =
            mainWindow.getsKeys()[GLFW_KEY_2];

        bool tecla3 =
            mainWindow.getsKeys()[GLFW_KEY_3];

        bool tecla4 =
            mainWindow.getsKeys()[GLFW_KEY_4];

        bool tecla5 =
            mainWindow.getsKeys()[GLFW_KEY_5];

        bool tecla6 =
            mainWindow.getsKeys()[GLFW_KEY_6];

        bool tecla7 =
            mainWindow.getsKeys()[GLFW_KEY_7];


        // ==============================================
        // 1 - DELANTERA IZQUIERDA
        // ==============================================

        if (tecla1 && !tecla1Anterior)
        {
            if (!estadoPataDI)
            {
                targetPataDI = -45.0f;
                estadoPataDI = true;
            }
            else
            {
                targetPataDI = 45.0f;
                estadoPataDI = false;
            }
        }

        tecla1Anterior = tecla1;


        // ==============================================
        // 2 - CENTRO IZQUIERDA
        // ==============================================

        if (tecla2 && !tecla2Anterior)
        {
            if (!estadoPataCI)
            {
                targetPataCI = -45.0f;
                estadoPataCI = true;
            }
            else
            {
                targetPataCI = 45.0f;
                estadoPataCI = false;
            }
        }

        tecla2Anterior = tecla2;


        // ==============================================
        // 3 - TRASERA IZQUIERDA
        // ==============================================

        if (tecla3 && !tecla3Anterior)
        {
            if (!estadoPataTI)
            {
                targetPataTI = -45.0f;
                estadoPataTI = true;
            }
            else
            {
                targetPataTI = 45.0f;
                estadoPataTI = false;
            }
        }

        tecla3Anterior = tecla3;


        // ==============================================
        // 4 - DELANTERA DERECHA
        // ==============================================

        if (tecla4 && !tecla4Anterior)
        {
            if (!estadoPataDD)
            {
                targetPataDD = -45.0f;
                estadoPataDD = true;
            }
            else
            {
                targetPataDD = 45.0f;
                estadoPataDD = false;
            }
        }

        tecla4Anterior = tecla4;


        // ==============================================
        // 5 - CENTRO DERECHA
        // ==============================================

        if (tecla5 && !tecla5Anterior)
        {
            if (!estadoPataCD)
            {
                targetPataCD = -45.0f;
                estadoPataCD = true;
            }
            else
            {
                targetPataCD = 45.0f;
                estadoPataCD = false;
            }
        }

        tecla5Anterior = tecla5;


        // ==============================================
        // 6 - TRASERA DERECHA
        // ==============================================

        if (tecla6 && !tecla6Anterior)
        {
            if (!estadoPataTD)
            {
                targetPataTD = -45.0f;
                estadoPataTD = true;
            }
            else
            {
                targetPataTD = 45.0f;
                estadoPataTD = false;
            }
        }

        tecla6Anterior = tecla6;


        // ==============================================
        // 7 - BRAZO
        // ==============================================

        if (tecla7 && !tecla7Anterior)
        {
            if (!estadoBrazo)
            {
                targetBrazo = -45.0f;
                estadoBrazo = true;
            }
            else
            {
                targetBrazo = 45.0f;
                estadoBrazo = false;
            }
        }

        tecla7Anterior = tecla7;

        // Cada pulsaci�n alterna solamente una esquina entre 0 y 70 grados.
        for (int i = 0; i < 8; ++i)
        {
            bool pulsada = mainWindow.getsKeys()[teclasEsquinas[i]];
            if (pulsada && !teclaEsquinaAnterior[i])
                objetivoEsquina[i] = (objetivoEsquina[i] == 0.0f) ? 70.0f : 0.0f;
            teclaEsquinaAnterior[i] = pulsada;
        }
        

        // Continuous satellite translation; independent from camera WASD.
        const float pasoSatelite = 2.5f * deltaTime;
        if (mainWindow.getsKeys()[GLFW_KEY_LEFT]) posicionSatelite.x -= pasoSatelite;
        if (mainWindow.getsKeys()[GLFW_KEY_RIGHT]) posicionSatelite.x += pasoSatelite;
        if (mainWindow.getsKeys()[GLFW_KEY_Q]) posicionSatelite.y += pasoSatelite;
        if (mainWindow.getsKeys()[GLFW_KEY_E]) posicionSatelite.y -= pasoSatelite;
        if (mainWindow.getsKeys()[GLFW_KEY_UP]) posicionSatelite.z -= pasoSatelite;
        if (mainWindow.getsKeys()[GLFW_KEY_DOWN]) posicionSatelite.z += pasoSatelite;

        // A single press on Z/X/C opens or closes one satellite component.
        for (int i = 0; i < 3; ++i)
        {
            const bool pulsada = mainWindow.getsKeys()[teclasSatelite[i]];
            if (pulsada && !teclaSateliteAnterior[i])
                objetivoSatelite[i] = (objetivoSatelite[i] == 0.0f) ? 55.0f : 0.0f;
            teclaSateliteAnterior[i] = pulsada;
        }

        // ==============================================
        // MOVIMIENTO
        // ==============================================

        float velocidadMovimiento =
            50.0f;


        actualizarAngulo(
            angPataDI,
            targetPataDI,
            velocidadMovimiento
        );

        actualizarAngulo(
            angPataCI,
            targetPataCI,
            velocidadMovimiento
        );

        actualizarAngulo(
            angPataTI,
            targetPataTI,
            velocidadMovimiento
        );

        actualizarAngulo(
            angPataDD,
            targetPataDD,
            velocidadMovimiento
        );

        actualizarAngulo(
            angPataCD,
            targetPataCD,
            velocidadMovimiento
        );

        actualizarAngulo(
            angPataTD,
            targetPataTD,
            velocidadMovimiento
        );

        actualizarAngulo(
            angBrazo,
            targetBrazo,
            velocidadMovimiento
        );


        for (int i = 0; i < 8; ++i)
            actualizarAngulo(anguloEsquina[i], objetivoEsquina[i], 85.0f);

        for (int i = 0; i < 3; ++i)
            actualizarAngulo(anguloSatelite[i], objetivoSatelite[i], 85.0f);

        // ==============================================
        // LIMPIAR
        // ==============================================

        glClearColor(
            0.0f,
            0.0f,
            0.0f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );


        // ==============================================
        // SKYBOX
        // ==============================================

        skybox.DrawSkybox(
            camera.calculateViewMatrix(),
            projection
        );


        // ==============================================
        // SHADER
        // ==============================================

        shaderList[0].UseShader();


        uniformModel =
            shaderList[0].GetModelLocation();

        uniformProjection =
            shaderList[0].GetProjectionLocation();

        uniformView =
            shaderList[0].GetViewLocation();

        uniformColor =
            shaderList[0].getColorLocation();


        glUniformMatrix4fv(
            uniformProjection,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );


        glUniformMatrix4fv(
            uniformView,
            1,
            GL_FALSE,
            glm::value_ptr(
                camera.calculateViewMatrix()
            )
        );


        // ==============================================
        // PISO
        // ==============================================

        color =
            glm::vec3(
                0.5f,
                0.5f,
                0.5f
            );


        model =
            glm::mat4(1.0f);


        model =
            glm::translate(
                model,
                glm::vec3(
                    0.0f,
                    -2.0f,
                    0.0f
                )
            );


        model =
            glm::scale(
                model,
                glm::vec3(
                    30.0f,
                    1.0f,
                    30.0f
                )
            );


        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );


        glUniform3fv(
            uniformColor,
            1,
            glm::value_ptr(color)
        );


        meshListModel[2]->
            RenderMeshModel();


        // ==============================================
        // BASE ROVER
        // ==============================================

        model =
            glm::mat4(1.0f);


        model =
            glm::translate(
                model,
                glm::vec3(
                    0.0f,
                    -2.0f,
                    -1.5f
                )
            );


        // ==============================================
        // COLOR ROVER
        // ==============================================

        color =
            glm::vec3(
                1.0f,
                1.0f,
                1.0f
            );


        glUniform3fv(
            uniformColor,
            1,
            glm::value_ptr(color)
        );


        // ==============================================
        // CUERPO
        // ==============================================

        modelaux = model;

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(modelaux)
        );

        Cuerpo_M.RenderModel();


        // ==============================================
        // BRAZO - 7
        // ==============================================

        modelaux = model;

        modelaux =
            glm::rotate(
                modelaux,
                angBrazo * toRadians,
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(modelaux)
        );

        Brazo_M.RenderModel();


        // ==============================================
        // DELANTERA IZQUIERDA - 1
        // ==============================================

        modelaux = model;

        modelaux =
            glm::rotate(
                modelaux,
                angPataDI * toRadians,
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(modelaux)
        );

        PataDI_M.RenderModel();


        // ==============================================
        // CENTRO IZQUIERDA - 2
        // ==============================================

        modelaux = model;

        modelaux =
            glm::rotate(
                modelaux,
                angPataCI * toRadians,
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(modelaux)
        );

        PataCI_M.RenderModel();


        // ==============================================
        // TRASERA IZQUIERDA - 3
        // ==============================================

        modelaux = model;

        modelaux =
            glm::rotate(
                modelaux,
                angPataTI * toRadians,
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(modelaux)
        );

        PataTI_M.RenderModel();


        // ==============================================
        // DELANTERA DERECHA - 4
        // ==============================================

        modelaux = model;

        modelaux =
            glm::rotate(
                modelaux,
                angPataDD * toRadians,
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(modelaux)
        );

        PataDD_M.RenderModel();


        // ==============================================
        // CENTRO DERECHA - 5
        // ==============================================

        modelaux = model;

        modelaux =
            glm::rotate(
                modelaux,
                angPataCD * toRadians,
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(modelaux)
        );

        PataCD_M.RenderModel();


        // ==============================================
        // TRASERA DERECHA - 6
        // ==============================================

        modelaux = model;

        modelaux =
            glm::rotate(
                modelaux,
                angPataTD * toRadians,
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );

        glUniformMatrix4fv(
            uniformModel,
            1,
            GL_FALSE,
            glm::value_ptr(modelaux)
        );

        PataTD_M.RenderModel();


        // Holocron junto al rover. Todos los OBJ comparten coordenadas de Blender.
        // Trasladar al centro antes de girar conserva el cuerpo en su lugar.
        glm::mat4 baseHolocron = glm::translate(
            glm::mat4(1.0f), glm::vec3(9.0f, -0.30f, -1.5f));
        baseHolocron = glm::scale(baseHolocron, glm::vec3(0.32f));
        baseHolocron = glm::translate(baseHolocron, -centroHolocron);

        color = glm::vec3(1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
            glm::value_ptr(baseHolocron));
        HolocronCuerpo_M.RenderModel();

        for (int i = 0; i < 8; ++i)
        {
            // Eje diagonal propio: apunta del centro del cubo a esta esquina.
            glm::vec3 eje = glm::normalize(centrosEsquinas[i] - centroHolocron);
            glm::mat4 esquina = glm::translate(baseHolocron, centroHolocron);
            esquina = glm::rotate(esquina, anguloEsquina[i] * toRadians, eje);
            esquina = glm::translate(esquina, -centroHolocron);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE,
                glm::value_ptr(esquina));
            color = glm::vec3(0.0f, 0.8f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            HolocronEsquina_M[i].RenderModel();
        }

        // Satellite: common parent translation/scale, then per-part pivot.
        const glm::mat4 baseSatelite = glm::scale(
            glm::translate(glm::mat4(1.0f), posicionSatelite),
            glm::vec3(3.0f));

        color = glm::vec3(0.9f, 0.9f, 0.95f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseSatelite));
        SateliteCuerpo_M.RenderModel();

        Model* partesSatelite[3] = {
            &PanelIzquierdo_M, &PanelDerecho_M, &PalaSuperior_M
        };
        for (int i = 0; i < 3; ++i)
        {
            glm::mat4 parte = glm::translate(baseSatelite, pivotesSatelite[i]);
            parte = glm::rotate(parte, anguloSatelite[i] * toRadians, ejesSatelite[i]);
            parte = glm::translate(parte, -pivotesSatelite[i]);
            color = (i < 2) ? glm::vec3(0.20f, 0.55f, 0.95f)
                : glm::vec3(0.95f, 0.75f, 0.25f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(parte));
            partesSatelite[i]->RenderModel();
        }

        glUseProgram(0);

        mainWindow.swapBuffers();
    }


    return 0;
}
