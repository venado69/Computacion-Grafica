//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>

//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0f; //grados a radianes

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

//Shaders nuevos para asignar colores fijos a pirámides y cubos
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* vShaderMagenta = "shaders/shadermagenta.vert";
static const char* vShaderAmarillo = "shaders/shaderamarillo.vert";

float angulo = 0.0f;

using std::vector;

//Pirámide triangular regular - se usa en la torre
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
	};

	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		 0.5f, -0.5f,0.0f,	//1
		 0.0f,  0.5f,-0.25f,	//2
		 0.0f, -0.5f,-0.5f	//3
	};

	Mesh* piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//Pirámide cuadrangular unitaria - se usa en las figuras 2 y 3
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4
	};

	GLfloat piramidecuadrangular_vertices[] = {
		 0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f, 0.5f,
		 0.0f, 0.5f, 0.0f
	};

	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

//Vértices de un cubo - se usa en barras y piezas centrales
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		0, 1, 2, 2, 3, 0, // front
		1, 5, 6, 6, 2, 1, // right
		7, 6, 5, 5, 4, 7, // back
		4, 0, 3, 3, 7, 4, // left
		4, 5, 1, 1, 0, 4, // bottom
		3, 2, 6, 6, 7, 3  // top
	};

	GLfloat cubo_vertices[] = {
		-0.5f, -0.5f,  0.5f, // front
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f, // back
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		// T ROJA
		-2.40f, 0.85f,0.5f,	1.0f,0.0f,0.0f,
		-1.20f, 0.85f,0.5f,	1.0f,0.0f,0.0f,
		-1.20f, 0.55f,0.5f,	1.0f,0.0f,0.0f,
		-2.40f, 0.85f,0.5f,	1.0f,0.0f,0.0f,
		-1.20f, 0.55f,0.5f,	1.0f,0.0f,0.0f,
		-2.40f, 0.55f,0.5f,	1.0f,0.0f,0.0f,

		-1.95f, 0.55f,0.5f,	1.0f,0.0f,0.0f,
		-1.65f, 0.55f,0.5f,	1.0f,0.0f,0.0f,
		-1.65f,-0.85f,0.5f,	1.0f,0.0f,0.0f,
		-1.95f, 0.55f,0.5f,	1.0f,0.0f,0.0f,
		-1.65f,-0.85f,0.5f,	1.0f,0.0f,0.0f,
		-1.95f,-0.85f,0.5f,	1.0f,0.0f,0.0f,

		// V VERDE
		-0.95f,0.85f,0.5f,	0.0f,1.0f,0.0f,
		-0.65f,0.85f,0.5f,	0.0f,1.0f,0.0f,
		-0.05f,-0.85f,0.5f,	0.0f,1.0f,0.0f,
		-0.95f,0.85f,0.5f,	0.0f,1.0f,0.0f,
		-0.05f,-0.85f,0.5f,	0.0f,1.0f,0.0f,
		-0.30f,-0.85f,0.5f,	0.0f,1.0f,0.0f,

		 0.55f,0.85f,0.5f,	0.0f,1.0f,0.0f,
		 0.85f,0.85f,0.5f,	0.0f,1.0f,0.0f,
		 0.05f,-0.85f,0.5f,	0.0f,1.0f,0.0f,
		 0.55f,0.85f,0.5f,	0.0f,1.0f,0.0f,
		 0.05f,-0.85f,0.5f,	0.0f,1.0f,0.0f,
		-0.05f,-0.85f,0.5f,	0.0f,1.0f,0.0f,

		// M AZUL
		1.15f,-0.85f,0.5f,	0.0f,0.0f,1.0f,
		1.40f,-0.85f,0.5f,	0.0f,0.0f,1.0f,
		1.40f, 0.85f,0.5f,	0.0f,0.0f,1.0f,
		1.15f,-0.85f,0.5f,	0.0f,0.0f,1.0f,
		1.40f, 0.85f,0.5f,	0.0f,0.0f,1.0f,
		1.15f, 0.85f,0.5f,	0.0f,0.0f,1.0f,

		1.40f,0.85f,0.5f,	0.0f,0.0f,1.0f,
		1.65f,0.85f,0.5f,	0.0f,0.0f,1.0f,
		1.95f,0.05f,0.5f,	0.0f,0.0f,1.0f,
		1.40f,0.85f,0.5f,	0.0f,0.0f,1.0f,
		1.95f,0.05f,0.5f,	0.0f,0.0f,1.0f,
		1.78f,-0.10f,0.5f,	0.0f,0.0f,1.0f,

		2.25f,0.85f,0.5f,	0.0f,0.0f,1.0f,
		2.50f,0.85f,0.5f,	0.0f,0.0f,1.0f,
		2.12f,-0.10f,0.5f,	0.0f,0.0f,1.0f,
		2.25f,0.85f,0.5f,	0.0f,0.0f,1.0f,
		2.12f,-0.10f,0.5f,	0.0f,0.0f,1.0f,
		1.95f,0.05f,0.5f,	0.0f,0.0f,1.0f,

		2.50f,-0.85f,0.5f,	0.0f,0.0f,1.0f,
		2.75f,-0.85f,0.5f,	0.0f,0.0f,1.0f,
		2.75f, 0.85f,0.5f,	0.0f,0.0f,1.0f,
		2.50f,-0.85f,0.5f,	0.0f,0.0f,1.0f,
		2.75f, 0.85f,0.5f,	0.0f,0.0f,1.0f,
		2.50f, 0.85f,0.5f,	0.0f,0.0f,1.0f
	};

	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 288);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulomagenta[] = {
		-1.0f,-1.0f,0.5f,	1.0f,0.0f,1.0f,
		 1.0f,-1.0f,0.5f,	1.0f,0.0f,1.0f,
		 0.0f, 1.0f,0.5f,	1.0f,0.0f,1.0f
	};
	MeshColor* triangulomagenta = new MeshColor();
	triangulomagenta->CreateMeshColor(vertices_triangulomagenta, 18);
	meshColorList.push_back(triangulomagenta);

	GLfloat vertices_cuadradoazul[] = {
		-0.5f,-0.5f,0.5f,	0.0f,0.0f,1.0f,
		 0.5f,-0.5f,0.5f,	0.0f,0.0f,1.0f,
		 0.5f, 0.5f,0.5f,	0.0f,0.0f,1.0f,
		-0.5f,-0.5f,0.5f,	0.0f,0.0f,1.0f,
		 0.5f, 0.5f,0.5f,	0.0f,0.0f,1.0f,
		-0.5f, 0.5f,0.5f,	0.0f,0.0f,1.0f
	};
	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShaderRojo, fShader);
	shaderList.push_back(*shaderRojo);

	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(*shaderVerde);

	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(*shaderAzul);

	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(*shaderCafe);

	Shader* shaderMagenta = new Shader();
	shaderMagenta->CreateFromFiles(vShaderMagenta, fShader);
	shaderList.push_back(*shaderMagenta);

	Shader* shaderAmarillo = new Shader();
	shaderAmarillo->CreateFromFiles(vShaderAmarillo, fShader);
	shaderList.push_back(*shaderAmarillo);
}

//============================================================
// MAPA DE GEOMETRÍAS USADAS EN EL EJERCICIO 2
// meshList[0] = PIRÁMIDE TRIANGULAR
// meshList[1] = CUBO
// meshList[2] = PIRÁMIDE CUADRANGULAR
// Las 3 figuras se construyen únicamente instanciando estos sólidos.
// No se usan triángulos ni cuadrados 2D para las composiciones.
//============================================================

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreaPiramide(); //0
	CrearCubo(); //1
	CrearPiramideCuadrangular(); //2
	CrearLetrasyFiguras();
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight(), 0.1f, 100.0f);
	glm::mat4 model(1.0f);
	bool mostrarLetras = false;

	while (!mainWindow.getShouldClose())
	{
		glfwPollEvents();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (mostrarLetras == true)
		{
			shaderList[1].useShader();
			uniformModel = shaderList[1].getModelLocation();
			uniformProjection = shaderList[1].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshColorList[0]->RenderMeshColor();
		}
		else
		{
			//================================================
			// FIGURA 1: TORRE CON PIRÁMIDES TRIANGULARES Y CUBOS
			//================================================

			//CUBO CAFÉ - BARRA IZQUIERDA
			shaderList[5].useShader();
			uniformModel = shaderList[5].getModelLocation();
			uniformProjection = shaderList[5].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-4.20f, 0.0f, -9.0f));
			model = glm::scale(model, glm::vec3(0.18f, 3.25f, 0.06f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[1]->RenderMesh();

			//CUBO CAFÉ - BARRA DERECHA
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-2.75f, 0.0f, -9.0f));
			model = glm::scale(model, glm::vec3(0.18f, 3.25f, 0.06f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[1]->RenderMesh();

			//PIRÁMIDE TRIANGULAR AMARILLA - PARTE SUPERIOR
			shaderList[7].useShader();
			uniformModel = shaderList[7].getModelLocation();
			uniformProjection = shaderList[7].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-3.475f, 1.05f, -8.8f));
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.25f, 1.05f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[0]->RenderMesh();

			//PIRÁMIDE TRIANGULAR ROJA - PARTE MEDIA
			shaderList[2].useShader();
			uniformModel = shaderList[2].getModelLocation();
			uniformProjection = shaderList[2].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-3.475f, 0.0f, -8.8f));
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.25f, 1.05f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[0]->RenderMesh();

			//PIRÁMIDE TRIANGULAR VERDE - PARTE INFERIOR
			shaderList[3].useShader();
			uniformModel = shaderList[3].getModelLocation();
			uniformProjection = shaderList[3].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-3.475f, -1.05f, -8.8f));
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.25f, 1.05f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[0]->RenderMesh();

			//================================================
			// FIGURA 2: PIRÁMIDES CUADRANGULARES Y CUBOS ROTADOS
			//================================================

			//PIRÁMIDE CUADRANGULAR AMARILLA - ARRIBA IZQUIERDA
			shaderList[7].useShader();
			uniformModel = shaderList[7].getModelLocation();
			uniformProjection = shaderList[7].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.663f, 0.663f, -9.0f));
			model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.25f, 0.625f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();

			//PIRÁMIDE CUADRANGULAR ROJA - ARRIBA DERECHA
			shaderList[2].useShader();
			uniformModel = shaderList[2].getModelLocation();
			uniformProjection = shaderList[2].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.663f, 0.663f, -9.0f));
			model = glm::rotate(model, -45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.25f, 0.625f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();

			//PIRÁMIDE CUADRANGULAR MAGENTA - ABAJO IZQUIERDA
			shaderList[6].useShader();
			uniformModel = shaderList[6].getModelLocation();
			uniformProjection = shaderList[6].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.663f, -0.663f, -9.0f));
			model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.25f, 0.625f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();

			//PIRÁMIDE CUADRANGULAR VERDE - ABAJO DERECHA
			shaderList[3].useShader();
			uniformModel = shaderList[3].getModelLocation();
			uniformProjection = shaderList[3].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.663f, -0.663f, -9.0f));
			model = glm::rotate(model, -135.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.25f, 0.625f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();

			//CUBO AZUL ROTADO 45 GRADOS SOBRE EL EJE Z
			shaderList[4].useShader();
			uniformModel = shaderList[4].getModelLocation();
			uniformProjection = shaderList[4].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.40f));
			model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.25f, 1.25f, 0.06f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[1]->RenderMesh();

			//CUBO CAFÉ ROTADO 45 GRADOS SOBRE EL EJE Z
			shaderList[5].useShader();
			uniformModel = shaderList[5].getModelLocation();
			uniformProjection = shaderList[5].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.10f));
			model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.05f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[1]->RenderMesh();

			//================================================
			// FIGURA 3: COMPOSICIÓN CON PIRÁMIDES CUADRANGULARES
			//================================================

			//PIRÁMIDE CUADRANGULAR MAGENTA SUPERIOR
			shaderList[6].useShader();
			uniformModel = shaderList[6].getModelLocation();
			uniformProjection = shaderList[6].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(3.65f, 1.05f, -8.8f));
			model = glm::scale(model, glm::vec3(1.45f, 1.45f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();

			//PIRÁMIDE CUADRANGULAR VERDE INFERIOR IZQUIERDA
			shaderList[3].useShader();
			uniformModel = shaderList[3].getModelLocation();
			uniformProjection = shaderList[3].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.92f, -0.40f, -8.8f));
			model = glm::scale(model, glm::vec3(1.45f, 1.45f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();

			//PIRÁMIDE CUADRANGULAR ROJA INFERIOR DERECHA
			shaderList[2].useShader();
			uniformModel = shaderList[2].getModelLocation();
			uniformProjection = shaderList[2].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(4.38f, -0.40f, -8.8f));
			model = glm::scale(model, glm::vec3(1.45f, 1.45f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();

			//PIRÁMIDE CUADRANGULAR AMARILLA CENTRAL INVERTIDA
			shaderList[7].useShader();
			uniformModel = shaderList[7].getModelLocation();
			uniformProjection = shaderList[7].getProjectLocation();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(3.65f, -0.40f, -8.8f));
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.45f, 1.45f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();
		}

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}