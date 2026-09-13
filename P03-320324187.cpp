#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

const float PI = 3.14159265f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static double limitFPS = 1.0 / 60.0;

Camera camera;
Window mainWindow;

vector<Mesh*> meshList;
vector<Shader> shaderList;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";


// ============================================================
// CUBO
// ============================================================

void CrearCubo()
{
    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0,

        1, 5, 6,
        6, 2, 1,

        7, 6, 5,
        5, 4, 7,

        4, 0, 3,
        3, 7, 4,

        4, 5, 1,
        1, 0, 4,

        3, 2, 6,
        6, 7, 3
    };

    GLfloat vertices[] =
    {
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    Mesh* cubo = new Mesh();

    cubo->CreateMesh(
        vertices,
        indices,
        24,
        36
    );

    meshList.push_back(cubo);
}


// ============================================================
// PIRAMIDE SENCILLA PARA EL COHETE
// ============================================================

void CrearPiramide()
{
    unsigned int indices[] =
    {
        0, 1, 2,
        0, 3, 1,
        0, 2, 3,
        1, 3, 2
    };

    GLfloat vertices[] =
    {
         0.0f,  0.8f,  0.0f,
        -0.6f, -0.5f,  0.5f,
         0.6f, -0.5f,  0.5f,
         0.0f, -0.5f, -0.6f
    };

    Mesh* piramide = new Mesh();

    piramide->CreateMesh(
        vertices,
        indices,
        12,
        12
    );

    meshList.push_back(piramide);
}


// ============================================================
// CILINDRO
// ============================================================

void CrearCilindro()
{
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    const int segmentos = 32;
    const float radio = 0.5f;
    const float altura = 1.0f;

    for (int i = 0; i <= segmentos; i++)
    {
        float angulo = 2.0f * PI * i / segmentos;

        float x = radio * cos(angulo);
        float z = radio * sin(angulo);

        vertices.push_back(x);
        vertices.push_back(-altura / 2.0f);
        vertices.push_back(z);

        vertices.push_back(x);
        vertices.push_back(altura / 2.0f);
        vertices.push_back(z);
    }

    for (int i = 0; i < segmentos; i++)
    {
        unsigned int actual = i * 2;
        unsigned int siguiente = actual + 2;

        indices.push_back(actual);
        indices.push_back(actual + 1);
        indices.push_back(siguiente);

        indices.push_back(siguiente);
        indices.push_back(actual + 1);
        indices.push_back(siguiente + 1);
    }

    Mesh* cilindro = new Mesh();

    cilindro->CreateMesh(
        vertices.data(),
        indices.data(),
        (unsigned int)vertices.size(),
        (unsigned int)indices.size()
    );

    meshList.push_back(cilindro);
}


// ============================================================
// CONO
// ============================================================

void CrearCono()
{
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    const int segmentos = 32;
    const float radio = 0.5f;

    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i < segmentos; i++)
    {
        float angulo = 2.0f * PI * i / segmentos;

        float x = radio * cos(angulo);
        float z = radio * sin(angulo);

        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);
    }

    for (int i = 0; i < segmentos; i++)
    {
        unsigned int actual = 1 + i;
        unsigned int siguiente = 1 + ((i + 1) % segmentos);

        indices.push_back(0);
        indices.push_back(actual);
        indices.push_back(siguiente);
    }

    Mesh* cono = new Mesh();

    cono->CreateMesh(
        vertices.data(),
        indices.data(),
        (unsigned int)vertices.size(),
        (unsigned int)indices.size()
    );

    meshList.push_back(cono);
}


// ============================================================
// ESFERA
// ============================================================

void CrearEsfera()
{
    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    const int sectores = 24;
    const int anillos = 16;
    const float radio = 0.5f;

    for (int i = 0; i <= anillos; i++)
    {
        float phi = PI / 2.0f - i * PI / anillos;

        float y = radio * sin(phi);

        float radioHorizontal =
            radio * cos(phi);

        for (int j = 0; j <= sectores; j++)
        {
            float theta =
                j * 2.0f * PI / sectores;

            float x =
                radioHorizontal * cos(theta);

            float z =
                radioHorizontal * sin(theta);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (int i = 0; i < anillos; i++)
    {
        int k1 = i * (sectores + 1);
        int k2 = k1 + sectores + 1;

        for (int j = 0; j < sectores; j++)
        {
            indices.push_back(k1 + j);
            indices.push_back(k2 + j);
            indices.push_back(k1 + j + 1);

            indices.push_back(k1 + j + 1);
            indices.push_back(k2 + j);
            indices.push_back(k2 + j + 1);
        }
    }

    Mesh* esfera = new Mesh();

    esfera->CreateMesh(
        vertices.data(),
        indices.data(),
        (unsigned int)vertices.size(),
        (unsigned int)indices.size()
    );

    meshList.push_back(esfera);
}


// ============================================================
// CARA TRIANGULAR DE PIRAMIDE CUADRANGULAR
// ============================================================

void CrearCaraPiramideCuadrangular()
{
    unsigned int indices[] =
    {
        0, 1, 2
    };

    GLfloat vertices[] =
    {
         0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f,
         0.5f, 0.0f, 0.5f
    };

    Mesh* cara = new Mesh();

    cara->CreateMesh(
        vertices,
        indices,
        9,
        3
    );

    meshList.push_back(cara);
}


// ============================================================
// BASE CUADRADA
// ============================================================

void CrearBaseCuadrada()
{
    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLfloat vertices[] =
    {
       -0.5f, 0.0f,  0.5f,
        0.5f, 0.0f,  0.5f,
        0.5f, 0.0f, -0.5f,
       -0.5f, 0.0f, -0.5f
    };

    Mesh* base = new Mesh();

    base->CreateMesh(
        vertices,
        indices,
        12,
        6
    );

    meshList.push_back(base);
}


// ============================================================
// SHADER
// ============================================================

void CreateShaders()
{
    Shader* shader1 = new Shader();

    shader1->CreateFromFiles(
        vShader,
        fShader
    );

    shaderList.push_back(*shader1);
}


// ============================================================
// DIBUJAR OBJETO
// ============================================================

void DibujarObjeto(
    int indiceMesh,
    GLuint uniformModel,
    GLuint uniformColor,
    glm::mat4 matrizBase,
    glm::vec3 posicion,
    glm::vec3 rotacion,
    glm::vec3 escala,
    glm::vec3 color
)
{
    glm::mat4 model = matrizBase;

    model =
        glm::translate(
            model,
            posicion
        );

    model =
        glm::rotate(
            model,
            glm::radians(rotacion.x),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

    model =
        glm::rotate(
            model,
            glm::radians(rotacion.y),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

    model =
        glm::rotate(
            model,
            glm::radians(rotacion.z),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

    model =
        glm::scale(
            model,
            escala
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

    meshList[indiceMesh]->RenderMesh();
}


// ============================================================
// PIRAMIDE CUADRANGULAR CON 5 COLORES
// ============================================================

void DibujarPiramideCuadrangularColorida(
    GLuint uniformModel,
    GLuint uniformColor,
    glm::mat4 matrizBase,
    glm::vec3 posicion,
    glm::vec3 rotacion,
    glm::vec3 escala,
    glm::vec3 colorFrente,
    glm::vec3 colorDerecha,
    glm::vec3 colorAtras,
    glm::vec3 colorIzquierda,
    glm::vec3 colorBase
)
{
    glm::mat4 piramide =
        matrizBase;

    piramide =
        glm::translate(
            piramide,
            posicion
        );

    piramide =
        glm::rotate(
            piramide,
            glm::radians(rotacion.x),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

    piramide =
        glm::rotate(
            piramide,
            glm::radians(rotacion.y),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

    piramide =
        glm::rotate(
            piramide,
            glm::radians(rotacion.z),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

    piramide =
        glm::scale(
            piramide,
            escala
        );


    // BASE AZUL
    DibujarObjeto(
        6,
        uniformModel,
        uniformColor,
        piramide,
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        colorBase
    );


    // FRENTE ROJO
    DibujarObjeto(
        5,
        uniformModel,
        uniformColor,
        piramide,
        glm::vec3(0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f),
        colorFrente
    );


    // DERECHA AMARILLO
    DibujarObjeto(
        5,
        uniformModel,
        uniformColor,
        piramide,
        glm::vec3(0.0f),
        glm::vec3(0.0f, 90.0f, 0.0f),
        glm::vec3(1.0f),
        colorDerecha
    );


    // ATRAS MAGENTA
    DibujarObjeto(
        5,
        uniformModel,
        uniformColor,
        piramide,
        glm::vec3(0.0f),
        glm::vec3(0.0f, 180.0f, 0.0f),
        glm::vec3(1.0f),
        colorAtras
    );


    // IZQUIERDA VERDE
    DibujarObjeto(
        5,
        uniformModel,
        uniformColor,
        piramide,
        glm::vec3(0.0f),
        glm::vec3(0.0f, -90.0f, 0.0f),
        glm::vec3(1.0f),
        colorIzquierda
    );
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    mainWindow =
        Window(
            800,
            600
        );

    mainWindow.Initialise();


    CrearCubo();
    CrearPiramide();
    CrearCilindro();
    CrearCono();
    CrearEsfera();
    CrearCaraPiramideCuadrangular();
    CrearBaseCuadrada();

    CreateShaders();


    camera =
        Camera(
            glm::vec3(
                0.0f,
                0.0f,
                0.0f
            ),

            glm::vec3(
                0.0f,
                1.0f,
                0.0f
            ),

            -90.0f,
            0.0f,
            0.3f,
            0.3f
        );


    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;


    glm::mat4 projection =
        glm::perspective(
            glm::radians(60.0f),

            (GLfloat)
            mainWindow.getBufferWidth()
            /
            (GLfloat)
            mainWindow.getBufferHeight(),

            0.1f,
            100.0f
        );


    // ========================================================
    // COLORES
    // ========================================================

    glm::vec3 gris =
        glm::vec3(
            0.70f,
            0.70f,
            0.75f
        );

    glm::vec3 rojo =
        glm::vec3(
            0.90f,
            0.10f,
            0.10f
        );

    glm::vec3 verde =
        glm::vec3(
            0.00f,
            0.90f,
            0.15f
        );

    glm::vec3 amarillo =
        glm::vec3(
            1.00f,
            0.80f,
            0.00f
        );

    glm::vec3 magenta =
        glm::vec3(
            0.85f,
            0.00f,
            1.00f
        );

    glm::vec3 azul =
        glm::vec3(
            0.05f,
            0.45f,
            1.00f
        );

    glm::vec3 azulBase =
        glm::vec3(
            0.08f,
            0.38f,
            1.00f
        );

    glm::vec3 naranja =
        glm::vec3(
            1.00f,
            0.35f,
            0.00f
        );


    while (
        !mainWindow.getShouldClose()
        )
    {
        GLfloat now =
            glfwGetTime();

        deltaTime =
            now - lastTime;

        deltaTime +=
            (now - lastTime)
            /
            limitFPS;

        lastTime =
            now;


        glfwPollEvents();


        camera.keyControl(
            mainWindow.getsKeys(),
            deltaTime
        );


        camera.mouseControl(
            mainWindow.getXChange(),
            mainWindow.getYChange()
        );


        glClearColor(
            0.02f,
            0.02f,
            0.08f,
            1.0f
        );


        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );


        shaderList[0].useShader();


        uniformModel =
            shaderList[0].
            getModelLocation();

        uniformProjection =
            shaderList[0].
            getProjectLocation();

        uniformView =
            shaderList[0].
            getViewLocation();

        uniformColor =
            shaderList[0].
            getColorLocation();


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


        // ====================================================
        // COHETE
        // ====================================================

        glm::mat4 cohete =
            glm::mat4(1.0f);


        cohete =
            glm::translate(
                cohete,
                glm::vec3(
                    -5.0f,
                    0.0f,
                    -15.0f
                )
            );


        cohete =
            glm::rotate(
                cohete,
                glm::radians(
                    mainWindow.getrotax()
                ),
                glm::vec3(
                    1.0f,
                    0.0f,
                    0.0f
                )
            );


        cohete =
            glm::rotate(
                cohete,
                glm::radians(
                    mainWindow.getrotay()
                ),
                glm::vec3(
                    0.0f,
                    1.0f,
                    0.0f
                )
            );


        cohete =
            glm::rotate(
                cohete,
                glm::radians(
                    mainWindow.getrotaz()
                ),
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );


        // CUERPO
        DibujarObjeto(
            2,
            uniformModel,
            uniformColor,
            cohete,
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(
                2.0f,
                5.0f,
                2.0f
            ),
            gris
        );


        // PUNTA
        DibujarObjeto(
            3,
            uniformModel,
            uniformColor,
            cohete,
            glm::vec3(
                0.0f,
                2.5f,
                0.0f
            ),
            glm::vec3(0.0f),
            glm::vec3(
                2.1f,
                2.0f,
                2.1f
            ),
            rojo
        );


        // VENTANA
        DibujarObjeto(
            4,
            uniformModel,
            uniformColor,
            cohete,
            glm::vec3(
                0.0f,
                0.8f,
                1.02f
            ),
            glm::vec3(0.0f),
            glm::vec3(
                0.8f,
                0.8f,
                0.35f
            ),
            azul
        );


        // ALETA IZQUIERDA
        DibujarObjeto(
            1,
            uniformModel,
            uniformColor,
            cohete,
            glm::vec3(
                -1.25f,
                -1.85f,
                0.0f
            ),
            glm::vec3(
                0.0f,
                0.0f,
                145.0f
            ),
            glm::vec3(
                1.1f,
                1.5f,
                0.7f
            ),
            rojo
        );


        // ALETA DERECHA
        DibujarObjeto(
            1,
            uniformModel,
            uniformColor,
            cohete,
            glm::vec3(
                1.25f,
                -1.85f,
                0.0f
            ),
            glm::vec3(
                0.0f,
                0.0f,
                -145.0f
            ),
            glm::vec3(
                1.1f,
                1.5f,
                0.7f
            ),
            rojo
        );


        // MOTOR IZQUIERDO
        DibujarObjeto(
            0,
            uniformModel,
            uniformColor,
            cohete,
            glm::vec3(
                -0.48f,
                -3.0f,
                0.0f
            ),
            glm::vec3(0.0f),
            glm::vec3(
                0.65f,
                1.0f,
                0.65f
            ),
            naranja
        );


        // MOTOR DERECHO
        DibujarObjeto(
            0,
            uniformModel,
            uniformColor,
            cohete,
            glm::vec3(
                0.48f,
                -3.0f,
                0.0f
            ),
            glm::vec3(0.0f),
            glm::vec3(
                0.65f,
                1.0f,
                0.65f
            ),
            naranja
        );


        // MOTOR CENTRAL
        DibujarObjeto(
            0,
            uniformModel,
            uniformColor,
            cohete,
            glm::vec3(
                0.0f,
                -2.75f,
                0.0f
            ),
            glm::vec3(0.0f),
            glm::vec3(
                0.35f,
                1.2f,
                0.35f
            ),
            amarillo
        );


        // ====================================================
        // FIGURA DE 8 PIRAMIDES
        // ====================================================

        glm::mat4 figura8 =
            glm::mat4(1.0f);


        figura8 =
            glm::translate(
                figura8,
                glm::vec3(
                    5.0f,
                    0.0f,
                    -18.0f
                )
            );


        figura8 =
            glm::rotate(
                figura8,
                glm::radians(
                    mainWindow.getrotax()
                ),
                glm::vec3(
                    1.0f,
                    0.0f,
                    0.0f
                )
            );


        figura8 =
            glm::rotate(
                figura8,
                glm::radians(
                    mainWindow.getrotay()
                ),
                glm::vec3(
                    0.0f,
                    1.0f,
                    0.0f
                )
            );


        figura8 =
            glm::rotate(
                figura8,
                glm::radians(
                    mainWindow.getrotaz()
                ),
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );


        // ====================================================
        // AJUSTES DE POSICION
        // ====================================================

        float offsetVertical =
            2.0f;

        // ANTES ERA 1.45.
        // 1.28 cierra el pequeño espacio central.
        float offsetHorizontal =
            1.28f;


        glm::vec3 escalaPiramide =
            glm::vec3(
                1.6f,
                2.0f,
                1.6f
            );


        // ====================================================
        // PIRAMIDES 1 Y 2 - ARRIBA
        // ====================================================

        DibujarPiramideCuadrangularColorida(
            uniformModel,
            uniformColor,
            figura8,

            glm::vec3(
                0.0f,
                offsetVertical,
                0.0f
            ),

            glm::vec3(
                0.0f,
                45.0f,
                0.0f
            ),

            escalaPiramide,

            rojo,
            amarillo,
            magenta,
            verde,
            azulBase
        );


        DibujarPiramideCuadrangularColorida(
            uniformModel,
            uniformColor,
            figura8,

            glm::vec3(
                0.0f,
                offsetVertical,
                0.0f
            ),

            glm::vec3(
                180.0f,
                45.0f,
                0.0f
            ),

            escalaPiramide,

            rojo,
            amarillo,
            magenta,
            verde,
            azulBase
        );


        // ====================================================
        // PIRAMIDES 3 Y 4 - ABAJO
        // ====================================================

        DibujarPiramideCuadrangularColorida(
            uniformModel,
            uniformColor,
            figura8,

            glm::vec3(
                0.0f,
                -offsetVertical,
                0.0f
            ),

            glm::vec3(
                0.0f,
                45.0f,
                0.0f
            ),

            escalaPiramide,

            rojo,
            amarillo,
            magenta,
            verde,
            azulBase
        );


        DibujarPiramideCuadrangularColorida(
            uniformModel,
            uniformColor,
            figura8,

            glm::vec3(
                0.0f,
                -offsetVertical,
                0.0f
            ),

            glm::vec3(
                180.0f,
                45.0f,
                0.0f
            ),

            escalaPiramide,

            rojo,
            amarillo,
            magenta,
            verde,
            azulBase
        );


        // ====================================================
        // PIRAMIDES 5 Y 6 - IZQUIERDA
        // ====================================================

        DibujarPiramideCuadrangularColorida(
            uniformModel,
            uniformColor,
            figura8,

            glm::vec3(
                -offsetHorizontal,
                0.0f,
                0.0f
            ),

            glm::vec3(
                0.0f,
                45.0f,
                0.0f
            ),

            escalaPiramide,

            rojo,
            amarillo,
            magenta,
            verde,
            azulBase
        );


        DibujarPiramideCuadrangularColorida(
            uniformModel,
            uniformColor,
            figura8,

            glm::vec3(
                -offsetHorizontal,
                0.0f,
                0.0f
            ),

            glm::vec3(
                180.0f,
                45.0f,
                0.0f
            ),

            escalaPiramide,

            rojo,
            amarillo,
            magenta,
            verde,
            azulBase
        );


        // ====================================================
        // PIRAMIDES 7 Y 8 - DERECHA
        // ====================================================

        DibujarPiramideCuadrangularColorida(
            uniformModel,
            uniformColor,
            figura8,

            glm::vec3(
                offsetHorizontal,
                0.0f,
                0.0f
            ),

            glm::vec3(
                0.0f,
                45.0f,
                0.0f
            ),

            escalaPiramide,

            rojo,
            amarillo,
            magenta,
            verde,
            azulBase
        );


        DibujarPiramideCuadrangularColorida(
            uniformModel,
            uniformColor,
            figura8,

            glm::vec3(
                offsetHorizontal,
                0.0f,
                0.0f
            ),

            glm::vec3(
                180.0f,
                45.0f,
                0.0f
            ),

            escalaPiramide,

            rojo,
            amarillo,
            magenta,
            verde,
            azulBase
        );


        glUseProgram(0);

        mainWindow.swapBuffers();
    }


    return 0;
}