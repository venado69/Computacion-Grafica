#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

//Variables para cambiar el color del fondo
float color1, color2, color3;

//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = " \n\
#version 330 \n\
layout (location =0) in vec3 pos; \n\
void main() \n\
{ \n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); \n\
}";

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = " \n\
#version 330 \n\
out vec4 color; \n\
void main() \n\
{ \n\
color = vec4(1.0f,1.0f,1.0f,1.0f); \n\
}";


void CrearFiguras()
{
    GLfloat vertices[] = {

        //CUADRADO - IZQUIERDA
        -0.8f,  0.3f, 0.0f,
        -0.2f,  0.3f, 0.0f,
        -0.2f, -0.3f, 0.0f,
        -0.8f, -0.3f, 0.0f,

        //ROMBO - DERECHA
         0.5f,  0.5f, 0.0f,
         0.8f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.2f,  0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO); //generar 1 VAO
    glBindVertexArray(VAO);//asignar VAO

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(GL_FLOAT), (GLvoid*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    //verificaciones y prevención de errores
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("EL error al compilar el shader %d es: %s \n",
            shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}


void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Error creando el shader");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);

    //verificaciones y prevención de errores
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("EL error al linkear es: %s \n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("EL error al validar es: %s \n", eLog);
        return;
    }
}


int main()
{
    //Inicialización de GLFW
    if (!glfwInit())
    {
        printf("Falló inicializar GLFW");
        glfwTerminate();
        return 1;
    }

    //Asignando variables de GLFW y propiedades de ventana
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //para solo usar el core profile de OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //CREAR VENTANA
    GLFWwindow* mainWindow =
        glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

    if (!mainWindow)
    {
        printf("Fallo en crearse la ventana con GLFW");
        glfwTerminate();
        return 1;
    }

    //Obtener tamaño de Buffer
    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

    //asignar el contexto
    glfwMakeContextCurrent(mainWindow);

    //permitir nuevas extensiones
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("Falló inicialización de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //Asignar Viewport
    glViewport(0, 0, BufferWidth, BufferHeight);

    //Inicializamos colores
    color1 = color2 = color3 = 0.0f;

    //Llamada a las funciones creadas antes del main
    CrearFiguras();
    CompileShaders();

    //Loop mientras no se cierra la ventana
    while (!glfwWindowShouldClose(mainWindow))
    {
        //Recibir eventos del usuario
        glfwPollEvents();

        //Tiempo actual
        int colorActual = ((int)(glfwGetTime() / 2.0)) % 3;

        //ROJO
        if (colorActual == 0)
        {
            color1 = 1.0f;
            color2 = 0.0f;
            color3 = 0.0f;
        }

        //VERDE
        else if (colorActual == 1)
        {
            color1 = 0.0f;
            color2 = 1.0f;
            color3 = 0.0f;
        }

        //AZUL
        else
        {
            color1 = 0.0f;
            color2 = 0.0f;
            color3 = 1.0f;
        }

        //Limpiar la ventana
        glClearColor(color1, color2, color3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);

        //Cuadrado relleno
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        //Rombo relleno
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);

        //NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow);
    }

    return 0;
}