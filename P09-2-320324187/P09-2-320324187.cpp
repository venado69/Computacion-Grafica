/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
// STB_IMAGE_IMPLEMENTATION esta definido una sola vez en Texture.cpp.

#include <stdio.h>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"

#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
bool avanza;

float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float movCoche = 0.0f;
float movOffset = 0.01f;

Window mainWindow;
std::vector<MeshModel*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;



Model Rover_M;
Model Avion;
Model Patas[6];
Model Llantas[6];
Model Brazo;
const float escalaRover = 0.65f;
const float limiteIzquierdo = -295.0f; // piso original -300; margen para el modelo
const float limiteDerecho = 295.0f;   // piso original +300; margen para el modelo
const float velocidadRover = 60.0f;
const float radioLlanta = 1.241f;
float giroLlantas = 0.0f;
float distanciaRover = 0.0f;
float faseAvion = 0.0f;
float orientacionAvion = 0.0f;
glm::vec3 posicionAvion(0.0f);
glm::vec3 centroLlantas[6] = {glm::vec3(5.712381f,1.704992f,2.752876f),glm::vec3(5.780237f,1.690408f,-2.722878f),glm::vec3(-0.888113f,1.509791f,3.854488f),glm::vec3(-5.130509f,1.412099f,3.841826f),glm::vec3(-5.113412f,1.385755f,-3.841825f),glm::vec3(-0.947183f,1.412099f,-3.841826f)};
bool evidencia = false;
unsigned int cuadroEvidencia = 0;
unsigned int indicePieza = 0;
GLfloat now = 0.0f;
glm::mat4 ruedaModel(1.0f);
glm::mat4 vista(1.0f);
glm::vec3 ojo(0.0f);
glm::vec3 frente(0.0f);
int modoVista=2;



const float PI=3.14159265359f;
float tiempoAnimacion=0;
glm::vec3 posicionNave(0),direccionNave(0,1,0),posicionSatelite(-25,14,0);
float anguloSatelite[3]={0,0,0};
Model Holocron,Esquinas[8],Cohete[4];
glm::vec3 centro(-47.987f,5.376f,.651f);
glm::vec3 pivotes[8]={{-50.606f,7.936f,3.332f},{-45.409f,7.956f,3.332f},{-45.372f,8.085f,-1.873f},{-50.601f,8.075f,-1.892f},{-50.596f,2.794f,-2.024f},{-45.375f,2.818f,-2.018f},{-45.389f,2.681f,3.174f},{-50.611f,2.684f,3.207f}};
// Una Bezier cubica comienza vertical y curva gradualmente hacia la derecha.
glm::vec3 Trayectoria(float u){
 float v=1-u;return v*v*v*glm::vec3(20,2,0)+3*v*v*u*glm::vec3(20,48,0)+3*v*u*u*glm::vec3(30,85,-20)+u*u*u*glm::vec3(110,100,-45);
}
glm::vec3 Tangente(float u){
 float v=1-u;return 3*v*v*glm::vec3(0,46,0)+6*v*u*glm::vec3(10,37,-20)+3*u*u*glm::vec3(80,15,-25);
}
void ActualizarAnimacion(float dt){
 tiempoAnimacion+=dt;
 float ciclo=std::fmod(tiempoAnimacion,24.f);
 float u=glm::clamp((ciclo-2)/20,0.f,1.f);u=u*u; // aceleracion de despegue
 posicionNave=Trayectoria(u);direccionNave=glm::normalize(Tangente(u));
 anguloSatelite[0]=std::fmod(tiempoAnimacion*PI/3,2*PI);
}
bool VerificarAnimacion(){
 for(int i=0;i<=1200;++i){float u=i/1200.f;auto p=Trayectoria(u);auto t=Tangente(u);
 if(!std::isfinite(p.x)||p.y<2||glm::length(t)<.01f)return false;
 if(i&&p.y<Trayectoria((i-1)/1200.f).y)return false;}
 printf("P09-2: 1201 muestras Bezier correctas; ascenso continuo y tangente no nula.\n");return true;
}
void DibujarPractica(GLuint um,GLuint uc){
 auto base=glm::translate(glm::mat4(1),posicionSatelite);
 base=glm::scale(base,glm::vec3(2));base=glm::translate(base,-centro);
 glUniform3f(uc,.2f,.75f,1);glUniformMatrix4fv(um,1,GL_FALSE,glm::value_ptr(base));Holocron.RenderModel();
 for(int i=0;i<8;++i){
 // T(p) R T(-p) mantiene fijo el centro de cada esquina.
 auto m=glm::translate(base,pivotes[i]);m=glm::rotate(m,anguloSatelite[0],glm::normalize(pivotes[i]-centro));m=glm::translate(m,-pivotes[i]);
 glUniform3f(uc,1,.68f,.18f);glUniformMatrix4fv(um,1,GL_FALSE,glm::value_ptr(m));Esquinas[i].RenderModel();}
 glm::vec3 y=direccionNave,x=glm::normalize(glm::cross(y,glm::vec3(0,0,1))),z=glm::cross(x,y);
 glm::mat4 rot(1);rot[0]=glm::vec4(x,0);rot[1]=glm::vec4(y,0);rot[2]=glm::vec4(z,0);
 auto m=glm::translate(glm::mat4(1),posicionNave)*rot;
 glUniform3f(uc,1,1,1);
 glUniformMatrix4fv(um,1,GL_FALSE,glm::value_ptr(m));Cohete[0].RenderModel();
}
void CapturarEvidencia() {
    std::filesystem::create_directories("Evidencias");
    std::vector<unsigned char> pix(1366*768*3);
    glPixelStorei(GL_PACK_ALIGNMENT,1);glReadPixels(0,0,1366,768,GL_RGB,GL_UNSIGNED_BYTE,pix.data());
    std::ofstream f("Evidencias/escena-"+std::to_string(cuadroEvidencia)+".ppm",std::ios::binary);
    f<<"P6\n1366 768\n255\n";
    for(int y=767;y>=0;--y)f.write(reinterpret_cast<char*>(pix.data()+y*1366*3),1366*3);
    printf("Captura %u x=%.3f direccion=%s giro=%.3f fase=%.3f GL=%u\n",cuadroEvidencia,posicionNave.y,"nave",anguloSatelite[0],tiempoAnimacion,glGetError());
}




Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	MeshModel* obj5 = new MeshModel();
	obj5->CreateMeshModel(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	MeshModel* obj6 = new MeshModel();
	obj6->CreateMeshModel(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	MeshModel* obj7 = new MeshModel();
	obj7->CreateMeshModel(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main(int argc, char** argv)
{
	std::filesystem::current_path(std::filesystem::absolute(argv[0]).parent_path());
    evidencia = argc>1 && std::string(argv[1])=="--evidencia";
    if(!VerificarAnimacion())return 2;
    mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	if(mainWindow.Initialise()!=0)return 1;
    glfwSwapInterval(evidencia?0:1);
    while(glGetError()!=GL_NO_ERROR){}

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(44.0f, 32.0f, 52.0f), glm::vec3(0.0f, 1.0f, 0.0f), -130.0f, -24.0f, 16.0f, 0.12f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

    Holocron.LoadModel("Models/Holocron_Cuerpo.obj");
    for(int i=0;i<8;++i)Esquinas[i].LoadModel("Models/Holocron_Esquina_"+std::to_string(i+1)+".obj");
    Cohete[0].LoadModel("Models/saturn-v.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.55f, 0.5f,
		-0.3f, -1.0f, -0.25f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.02f, 0.004f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.02f, 0.004f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;


	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	lastTime = static_cast<float>(glfwGetTime());
    avanza=true;
    ////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
        now = static_cast<float>(glfwGetTime());
        deltaTime = evidencia ? 1.0f/60.0f : std::min(now-lastTime,0.05f);
        lastTime = now;
        ActualizarAnimacion(deltaTime);
        angulovaria += 0.5f*deltaTime;
        //Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		modoVista=mainWindow.GetViewMode();
        if(evidencia)modoVista=cuadroEvidencia==0?1:(cuadroEvidencia>=900?3:2);
        ojo=camera.getCameraPosition();frente=camera.getCameraDirection();
        if(modoVista==1){ojo=glm::vec3(110,85,140);frente=glm::normalize(glm::vec3(30,40,0)-ojo);}
        if(modoVista==2){ojo=posicionNave+glm::vec3(30,18,36);frente=glm::normalize(posicionNave+direccionNave*6.f-ojo);}
        if(modoVista==3){ojo=posicionSatelite+glm::vec3(32,20,40);frente=glm::normalize(posicionSatelite-ojo);}
        vista=glm::lookAt(ojo,ojo+frente,glm::vec3(0,1,0));
        skybox.DrawSkybox(vista, projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(vista));
		glUniform3f(uniformEyePosition, ojo.x, ojo.y, ojo.z);

		// luz ligada a la cámara de tipo flash
		lowerLight = ojo;
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, frente);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMeshModel();



        DibujarPractica(uniformModel,uniformColor);

		glUseProgram(0);

		if(evidencia && (cuadroEvidencia==0||cuadroEvidencia==120||cuadroEvidencia==180||cuadroEvidencia==270||cuadroEvidencia==600||cuadroEvidencia==900||cuadroEvidencia==1200))CapturarEvidencia();
        mainWindow.swapBuffers();
        if(evidencia && ++cuadroEvidencia>1200)break;
	}

	return 0;
}


