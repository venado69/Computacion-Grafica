#include "BaseE08.h"
#include "HolocronGeometria.h"
#include <algorithm>
#include <cmath>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/constants.hpp>
namespace { std::vector<MeshModel*> meshListModel;
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


// Piso de la practica 06: normal hacia arriba y UV repetidas.
void CrearEscenario() {
    GLfloat vertices[]={-40,-2,-40,0,0,0,1,0, 40,-2,-40,20,0,0,1,0,
                        -40,-2,40,0,20,0,1,0, 40,-2,40,20,20,0,1,0};
    unsigned int indices[]={0,2,1,1,2,3};
    auto piso=new MeshModel();piso->CreateMeshModel(vertices,indices,32,6);meshListModel.push_back(piso);
    // Esfera para representar visualmente el sol y las luces fijas.
    std::vector<GLfloat> v;std::vector<unsigned int> idx;
    const int stacks=16,slices=24;
    for(int y=0;y<=stacks;++y) for(int x=0;x<=slices;++x) {
        float theta=glm::pi<float>()*y/stacks,phi=glm::two_pi<float>()*x/slices;
        glm::vec3 n(std::sin(theta)*std::cos(phi),std::cos(theta),std::sin(theta)*std::sin(phi));
        v.insert(v.end(),{n.x,n.y,n.z,float(x)/slices,float(y)/stacks,n.x,n.y,n.z});
    }
    for(int y=0;y<stacks;++y) for(int x=0;x<slices;++x) {
        unsigned int a=y*(slices+1)+x,b=a+slices+1;
        idx.insert(idx.end(),{a,b,a+1,a+1,b,b+1});
    }
    auto esfera=new MeshModel();esfera->CreateMeshModel(v.data(),idx.data(),unsigned(v.size()),unsigned(idx.size()));meshListModel.push_back(esfera);
}


}
bool BaseE08::Initialise(){
    CrearDado();CrearHolocron();CrearEscenario();
    if(!dado.LoadTextureA()||!piso.LoadTextureA()||!blanco.LoadTextureA())return false;
    avion.LoadModel("Models/E06-320324187-avion.obj");holocron.LoadModel("Models/E06-320324187.obj");
    const char* piezas[]={"Cuerpo","Brazo","Pata_Delantera_Izq","Pata_Delantera_Der","Pata_Centro_Der","Pata_Trasera_Izq","Pata_Trasera_Der","Rueda_Centro_Izq"};
    for(int i=0;i<8;++i)rover[i].LoadModel(std::string("Models/")+piezas[i]+".obj");
    skybox=std::make_unique<Skybox>(std::vector<std::string>{"Textures/Skybox/cupertin-lake_rt.tga","Textures/Skybox/cupertin-lake_lf.tga","Textures/Skybox/cupertin-lake_up.tga","Textures/Skybox/cupertin-lake_dn.tga","Textures/Skybox/cupertin-lake_bk.tga","Textures/Skybox/cupertin-lake_ft.tga"});
    for(int i=0;i<6;++i)faros[i]=SpotLight(colores[i].r,colores[i].g,colores[i].b,0,5,0,0,0,0,-1,0,1,.025f,.008f,28);
    return true;
}
void BaseE08::UpdateLights(Window& w,PointLight* points,unsigned int& pc,SpotLight* spots,unsigned int& sc,glm::vec3 eye,glm::vec3 forward){
    avionM=glm::translate(glm::mat4(1),glm::vec3(-7,6+w.HeightOffset(),-4));
    holoM=glm::scale(glm::translate(glm::mat4(1),glm::vec3(-2,1,3)),glm::vec3(.65f));
    roverM=glm::scale(glm::translate(glm::mat4(1),glm::vec3(5+w.RoverX(),-2,0)),glm::vec3(.65f));
    brazoM=glm::translate(roverM,pivote);brazoM=glm::rotate(brazoM,glm::radians(w.ArmAngle()),glm::vec3(0,0,1));brazoM=glm::translate(brazoM,-pivote);
    faroPos=glm::vec3(avionM*glm::vec4(0,-.45f,-2,1));faroDir=glm::normalize(glm::vec3(avionM*glm::vec4(0,-1,0,0)));
    holoPos=glm::vec3(holoM*glm::vec4(0,0,0,1));pinzaPos=glm::vec3(brazoM*glm::vec4(punta,1));
    faros[w.HeadlightColor()].SetFlash(faroPos,faroDir);spots[sc++]=faros[w.HeadlightColor()];
    if(w.FlashOn()){linterna.SetFlash(eye-glm::vec3(0,.15f,0),forward);spots[sc++]=linterna;}
    luzHolo.SetPosition(holoPos);luzPinza.SetPosition(pinzaPos);
    if(w.HoloOn())points[pc++]=luzHolo;
    if(w.ClawOn())points[pc++]=luzPinza;
}
void BaseE08::DrawSkybox(glm::mat4 view,glm::mat4 projection){skybox->DrawSkybox(view,projection);}
void BaseE08::Matrix(Shader& s,const glm::mat4& m){glUniformMatrix4fv(s.GetModelLocation(),1,GL_FALSE,glm::value_ptr(m));}
void BaseE08::Emisor(Shader& s,glm::vec3 pos,glm::vec3 color){
    model=glm::scale(glm::translate(glm::mat4(1),pos),glm::vec3(.12f));Matrix(s,model);glUniform3fv(s.getColorLocation(),1,glm::value_ptr(color));meshListModel[3]->RenderMeshModel();
}
void BaseE08::Draw(Shader& s,Window& w){
    glUniform3f(s.getColorLocation(),1,1,1);Matrix(s,glm::mat4(1));suelo.UseMaterial(s.GetSpecularIntensityLocation(),s.GetShininessLocation());piso.UseTexture();meshListModel[2]->RenderMeshModel();
    metal.UseMaterial(s.GetSpecularIntensityLocation(),s.GetShininessLocation());Matrix(s,avionM);avion.RenderModel();Matrix(s,holoM);holocron.RenderModel();
    for(index=0;index<8;++index){Matrix(s,index==1?brazoM:roverM);rover[index].RenderModel();}
    model=glm::scale(glm::translate(glm::mat4(1),glm::vec3(-7,0,3)),glm::vec3(2));Matrix(s,model);mate.UseMaterial(s.GetSpecularIntensityLocation(),s.GetShininessLocation());dado.UseTexture();meshListModel[0]->RenderMeshModel();
    glUniform1i(s.GetUnlitLocation(),1);blanco.UseTexture();
    Emisor(s,faroPos,colores[w.HeadlightColor()]);
    if(w.HoloOn())Emisor(s,holoPos,glm::vec3(.35f,.75f,1));
    if(w.ClawOn())Emisor(s,pinzaPos,glm::vec3(0,1,0));
    glUniform1i(s.GetUnlitLocation(),0);glUniform3f(s.getColorLocation(),1,1,1);
}
void BaseE08::Shutdown(){avion.ClearModel();holocron.ClearModel();for(auto& pieza:rover)pieza.ClearModel();for(auto* mesh:meshListModel)delete mesh;meshListModel.clear();dado.ClearTexture();piso.ClearTexture();blanco.ClearTexture();skybox.reset();}
