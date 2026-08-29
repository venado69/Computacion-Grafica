#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <vector>

#include <glew.h>
#include <glfw3.h>

const int WIDTH = 800, HEIGHT = 800;

GLuint VAO, VBO, shader;
GLint uniformColor;

float color1, color2, color3;
double ultimoCambio = 0.0;

int inicioLetras = 8;
int cantidadVerticesLetras = 0;


//=========================================================
// SHADERS
//=========================================================

static const char* vShader =
"#version 330\n"
"layout (location = 0) in vec3 pos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(pos, 1.0);\n"
"}";


static const char* fShader =
"#version 330\n"
"out vec4 color;\n"
"uniform vec4 colorFigura;\n"
"void main()\n"
"{\n"
"   color = colorFigura;\n"
"}";


//=========================================================
// PUNTO
//=========================================================

struct Punto
{
    float x;
    float y;
};


//=========================================================
// AGREGAR VERTICE
//=========================================================

void Vertice(std::vector<float>& v, float x, float y)
{
    v.push_back(x);
    v.push_back(y);
    v.push_back(0.0f);
}


//=========================================================
// AGREGAR TRIANGULO
//=========================================================

void Triangulo(std::vector<float>& v, Punto a, Punto b, Punto c)
{
    Vertice(v, a.x, a.y);
    Vertice(v, b.x, b.y);
    Vertice(v, c.x, c.y);
}


//=========================================================
// CREAR BARRA FORMADA POR TRIANGULOS
//=========================================================

void Barra(
    std::vector<float>& v,
    Punto inicio,
    Punto fin,
    float grosor,
    int divisiones)
{
    float dx = fin.x - inicio.x;
    float dy = fin.y - inicio.y;

    float longitud = sqrt(dx * dx + dy * dy);

    float nx = (-dy / longitud) * grosor / 2.0f;
    float ny = (dx / longitud) * grosor / 2.0f;


    for (int i = 0; i < divisiones; i++)
    {
        float t1 = (float)i / divisiones;
        float t2 = (float)(i + 1) / divisiones;

        Punto a =
        {
            inicio.x + dx * t1 + nx,
            inicio.y + dy * t1 + ny
        };

        Punto b =
        {
            inicio.x + dx * t2 + nx,
            inicio.y + dy * t2 + ny
        };

        Punto c =
        {
            inicio.x + dx * t2 - nx,
            inicio.y + dy * t2 - ny
        };

        Punto d =
        {
            inicio.x + dx * t1 - nx,
            inicio.y + dy * t1 - ny
        };


        //Cada parte de la barra son 2 triangulos
        Triangulo(v, a, b, c);
        Triangulo(v, a, c, d);
    }
}


//=========================================================
// CREAR FIGURAS
//=========================================================

void CrearFiguras()
{
    std::vector<float> vertices;


    //=====================================================
    // CUADRADO
    //=====================================================

    Vertice(vertices, -0.8f, 0.3f);
    Vertice(vertices, -0.2f, 0.3f);
    Vertice(vertices, -0.2f, -0.3f);
    Vertice(vertices, -0.8f, -0.3f);


    //=====================================================
    // ROMBO
    //=====================================================

    Vertice(vertices, 0.5f, 0.5f);
    Vertice(vertices, 0.8f, 0.0f);
    Vertice(vertices, 0.5f, -0.5f);
    Vertice(vertices, 0.2f, 0.0f);


    int antes = (int)vertices.size() / 3;


    //=====================================================
    // LETRA T
    // 10 TRIANGULOS
    //=====================================================

    //Barra superior = 6 triangulos
    Barra(
        vertices,
        { -0.90f, 0.84f },
        { -0.48f, 0.84f },
        0.10f,
        3
    );

    //Barra vertical = 4 triangulos
    Barra(
        vertices,
        { -0.69f, 0.84f },
        { -0.69f, 0.56f },
        0.10f,
        2
    );


    //=====================================================
    // LETRA V
    // 10 TRIANGULOS
    //=====================================================

    //Lado izquierdo = 4 triangulos
    Barra(
        vertices,
        { -0.31f, 0.87f },
        { -0.05f, 0.56f },
        0.09f,
        2
    );

    //Lado derecho = 6 triangulos
    Barra(
        vertices,
        { 0.21f, 0.87f },
        { -0.05f, 0.56f },
        0.09f,
        3
    );


    //=====================================================
    // LETRA M
    // 20 TRIANGULOS
    //=====================================================

    //Izquierda = 4
    Barra(
        vertices,
        { 0.38f, 0.87f },
        { 0.38f, 0.56f },
        0.08f,
        2
    );

    //Diagonal izquierda = 6
    Barra(
        vertices,
        { 0.38f, 0.87f },
        { 0.60f, 0.67f },
        0.08f,
        3
    );

    //Diagonal derecha = 6
    Barra(
        vertices,
        { 0.60f, 0.67f },
        { 0.82f, 0.87f },
        0.08f,
        3
    );

    //Derecha = 4
    Barra(
        vertices,
        { 0.82f, 0.87f },
        { 0.82f, 0.56f },
        0.08f,
        2
    );


    int despues = (int)vertices.size() / 3;

    cantidadVerticesLetras = despues - antes;


    printf(
        "Triangulos de las letras: %d\n",
        cantidadVerticesLetras / 3
    );


    //=====================================================
    // VAO Y VBO
    //=====================================================

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );


    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (GLvoid*)0
    );


    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


//=========================================================
// AGREGAR SHADER
//=========================================================

void AddShader(
    GLuint programa,
    const char* codigo,
    GLenum tipo)
{
    GLuint shaderNuevo = glCreateShader(tipo);

    glShaderSource(
        shaderNuevo,
        1,
        &codigo,
        NULL
    );

    glCompileShader(shaderNuevo);


    GLint resultado;

    glGetShaderiv(
        shaderNuevo,
        GL_COMPILE_STATUS,
        &resultado
    );


    if (!resultado)
    {
        char error[1024];

        glGetShaderInfoLog(
            shaderNuevo,
            1024,
            NULL,
            error
        );

        printf(
            "Error shader: %s\n",
            error
        );
    }


    glAttachShader(
        programa,
        shaderNuevo
    );
}


//=========================================================
// COMPILAR SHADERS
//=========================================================

void CompileShaders()
{
    shader = glCreateProgram();


    AddShader(
        shader,
        vShader,
        GL_VERTEX_SHADER
    );


    AddShader(
        shader,
        fShader,
        GL_FRAGMENT_SHADER
    );


    glLinkProgram(shader);


    GLint resultado;

    glGetProgramiv(
        shader,
        GL_LINK_STATUS,
        &resultado
    );


    if (!resultado)
    {
        char error[1024];

        glGetProgramInfoLog(
            shader,
            1024,
            NULL,
            error
        );

        printf(
            "Error programa: %s\n",
            error
        );
    }


    uniformColor =
        glGetUniformLocation(
            shader,
            "colorFigura"
        );
}


//=========================================================
// CAMBIAR COLOR DEL FONDO
//=========================================================

void CambiarColorRandom()
{
    color1 = (float)rand() / RAND_MAX;
    color2 = (float)rand() / RAND_MAX;
    color3 = (float)rand() / RAND_MAX;
}


//=========================================================
// MAIN
//=========================================================

int main()
{
    srand(
        (unsigned int)time(NULL)
    );


    if (!glfwInit())
    {
        printf("Error GLFW\n");
        return 1;
    }


    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        4
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );


    GLFWwindow* ventana =
        glfwCreateWindow(
            WIDTH,
            HEIGHT,
            "TVM con triangulos",
            NULL,
            NULL
        );


    if (!ventana)
    {
        printf("Error al crear ventana\n");

        glfwTerminate();

        return 1;
    }


    glfwMakeContextCurrent(ventana);

    glewExperimental = GL_TRUE;


    if (glewInit() != GLEW_OK)
    {
        printf("Error GLEW\n");

        return 1;
    }


    int ancho, alto;

    glfwGetFramebufferSize(
        ventana,
        &ancho,
        &alto
    );


    glViewport(
        0,
        0,
        ancho,
        alto
    );


    CrearFiguras();
    CompileShaders();

    CambiarColorRandom();

    ultimoCambio =
        glfwGetTime();


    //=====================================================
    // CICLO PRINCIPAL
    //=====================================================

    while (!glfwWindowShouldClose(ventana))
    {
        glfwPollEvents();


        double tiempo =
            glfwGetTime();


        //Cambiar RGB cada 2 segundos
        if (tiempo - ultimoCambio >= 2.0)
        {
            CambiarColorRandom();

            ultimoCambio = tiempo;
        }


        glClearColor(
            color1,
            color2,
            color3,
            1.0f
        );


        glClear(
            GL_COLOR_BUFFER_BIT
        );


        glUseProgram(shader);
        glBindVertexArray(VAO);


        //=================================================
        // FIGURAS BLANCAS
        //=================================================

        glUniform4f(
            uniformColor,
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );


        glPolygonMode(
            GL_FRONT_AND_BACK,
            GL_FILL
        );


        //Cuadrado
        glDrawArrays(
            GL_TRIANGLE_FAN,
            0,
            4
        );


        //Rombo
        glDrawArrays(
            GL_TRIANGLE_FAN,
            4,
            4
        );


        //Letras
        glDrawArrays(
            GL_TRIANGLES,
            inicioLetras,
            cantidadVerticesLetras
        );


        //=================================================
        // CONTORNO NEGRO DE LOS TRIANGULOS
        //=================================================

        glUniform4f(
            uniformColor,
            0.0f,
            0.0f,
            0.0f,
            1.0f
        );


        glPolygonMode(
            GL_FRONT_AND_BACK,
            GL_LINE
        );


        glLineWidth(2.0f);


        glDrawArrays(
            GL_TRIANGLES,
            inicioLetras,
            cantidadVerticesLetras
        );


        //Volver a relleno
        glPolygonMode(
            GL_FRONT_AND_BACK,
            GL_FILL
        );


        glBindVertexArray(0);
        glUseProgram(0);


        glfwSwapBuffers(
            ventana
        );
    }


    glDeleteBuffers(
        1,
        &VBO
    );

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteProgram(
        shader
    );


    glfwDestroyWindow(
        ventana
    );

    glfwTerminate();


    return 0;
}