#include "Scene.h"
#include <algorithm>
#include <cmath>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

bool Scene::Initialise(const char* executable,bool capture){
    auto dir=std::filesystem::absolute(executable).parent_path();
    for(int level=0;level<5;++level){if(std::filesystem::exists(dir/"Models/nave.obj")){std::filesystem::current_path(dir);break;}dir=dir.parent_path();}
    evidence=capture;
    if(window.Initialise()!=0)return false;
    while(glGetError()!=GL_NO_ERROR){}
    camera=Camera(eye,glm::vec3(0,1,0),-122.5f,-20,9,.12f);
    if(!window.VerifyInput()){printf("ERROR controles Window\n");return false;}
    printf("Window: L, 1, 2, repeticion e independencia OK.\n");
    CreateMeshes();
    if(!baseE08.Initialise())return false;
    shader.CreateFromFiles("shaders/shader_light.vert","shaders/shader_light.frag");
    shader.UseShader();unlit=shader.GetUnlitLocation();
    GLint program=0;glGetIntegerv(GL_CURRENT_PROGRAM,&program);
    plasma=glGetUniformLocation(program,"plasma");
    timeUniform=glGetUniformLocation(program,"animTime");
    if(plasma<0||timeUniform<0)return false;
    if(!flameTexture.LoadTextureA())return false;
    ship.LoadModel("Models/nave.obj");
    projection=glm::perspective(glm::radians(45.f),1366.f/768,.1f,200.f);
    if(evidence){std::filesystem::create_directories("Evidencias");pixels.resize(1366*768*3);log.open("Evidencias/verificacion.txt");log<<"Prueba callback Window: L/1/2, repeticion e independencia OK.\n";}
    last=glfwGetTime();return true;
}
void Scene::CreateMeshes(){
    // Dos planos cruzados por chorro; el alpha del PNG recorta su silueta.
    GLfloat flameV[]={0,-.65f,0,0,0,1,0,0, 0,.65f,0,0,1,1,0,0, 0,-.65f,-4,1,0,1,0,0, 0,.65f,-4,1,1,1,0,0,
        -.65f,0,0,0,0,0,1,0, .65f,0,0,0,1,0,1,0, -.65f,0,-4,1,0,0,1,0, .65f,0,-4,1,1,0,1,0};
    unsigned int flameI[]={0,1,2,2,1,3,4,5,6,6,5,7};flame.CreateMeshModel(flameV,flameI,64,12);
}
void Scene::UpdateLights(){
    // Misma matriz para el modelo importado, los emisores y ambas clases de luz.
    lightPosition=glm::vec3(shipMatrix*glm::vec4(0,.65f,0,1));
    shipPoint.SetPosition(lightPosition);
    spotPosition=glm::vec3(shipMatrix*glm::vec4(0,.16f,.9f,1));
    spotDirection=glm::normalize(glm::vec3(shipMatrix*glm::vec4(0,-.8f,1,0)));
    shipSpot.SetFlash(spotPosition,spotDirection);
    pointCount=0;spotCount=0;
    if(window.ShipLightsOn()){activePoints[pointCount++]=shipPoint;activeSpots[spotCount++]=shipSpot;}
    for(index=0;index<2;++index){
        enginePosition[index]=glm::vec3(shipMatrix*glm::vec4(nozzles[index],1));
        enginePoints[index].SetPosition(enginePosition[index]+glm::vec3(shipMatrix*glm::vec4(0,0,-.16f,0)));
        if(window.EngineOn(index))activePoints[pointCount++]=enginePoints[index];
    }
    baseE08.UpdateLights(window,activePoints,pointCount,activeSpots,spotCount,eye,camera.getCameraDirection());
}
void Scene::DrawScene(){
    glClearColor(.012f,.019f,.038f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    baseE08.DrawSkybox(view,projection);
    shader.UseShader();glUniform1i(unlit,0);glUniform1i(plasma,0);glUniform1f(timeUniform,float(sceneTime));
    glUniformMatrix4fv(shader.GetProjectionLocation(),1,GL_FALSE,glm::value_ptr(projection));
    glUniformMatrix4fv(shader.GetViewLocation(),1,GL_FALSE,glm::value_ptr(view));
    glUniform3fv(shader.GetEyePositionLocation(),1,glm::value_ptr(eye));glUniform3f(shader.getColorLocation(),1,1,1);
    shader.SetDirectionalLight(&ambient);shader.SetPointLights(activePoints,pointCount);shader.SetSpotLights(activeSpots,spotCount);
    baseE08.Draw(shader,window);
    hull.UseMaterial(shader.GetSpecularIntensityLocation(),shader.GetShininessLocation());
    glUniformMatrix4fv(shader.GetModelLocation(),1,GL_FALSE,glm::value_ptr(shipMatrix));ship.RenderModel();
    glUniform1i(plasma,1);flameTexture.UseTexture();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE);glDepthMask(GL_FALSE);
    for(index=0;index<2;++index){
        // Exactamente la misma bandera controla textura y luz puntual del motor.
        if(window.EngineOn(index)){
            model=glm::rotate(glm::translate(glm::mat4(1),enginePosition[index]),glm::radians(180.f),glm::vec3(0,1,0));
            glUniformMatrix4fv(shader.GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));flame.RenderMeshModel();
        }
    }
    glDepthMask(GL_TRUE);glDisable(GL_BLEND);glUniform1i(plasma,0);
}
void Scene::Frame(){
    now=glfwGetTime();dt=std::min(float(now-last),.05f);last=now;
    glfwPollEvents();window.UpdateInput(camera,dt);
    sceneTime=evidence?1.0:now;
    if(evidence)window.SetEvidenceState(frame<4,(frame%4&1)!=0,(frame%4&2)!=0);
    shipPosition.y=4.f+.12f*std::sin(float(sceneTime)*1.5f);
    shipMatrix=glm::scale(glm::rotate(glm::translate(glm::mat4(1),shipPosition),glm::radians(180.f),glm::vec3(0,1,0)),glm::vec3(3.5f));
    eye=camera.getCameraPosition();view=camera.calculateViewMatrix();
    if(evidence||window.OrbitOn()){
        if(window.OrbitOn()&&!evidence)eye=glm::vec3(20*std::sin(now*.25),10,20*std::cos(now*.25));
        else eye=glm::vec3(18,14,28);
        view=glm::lookAt(eye,glm::vec3(0,3,-3),glm::vec3(0,1,0));
    }
    UpdateLights();window.SetTitle();DrawScene();
    if(evidence)Capture();
    window.swapBuffers();
}
void Scene::Capture(){
    screenshotName=std::string("nave-")+(window.ShipLightsOn()?"on":"off")+"-motores-"+(window.EngineOn(0)?"1":"0")+(window.EngineOn(1)?"1":"0");
    glPixelStorei(GL_PACK_ALIGNMENT,1);glReadPixels(0,0,1366,768,GL_RGB,GL_UNSIGNED_BYTE,pixels.data());
    screenshot.open("Evidencias/"+screenshotName+".ppm",std::ios::binary);screenshot<<"P6\n1366 768\n255\n";
    for(index=767;index>=0;--index)screenshot.write(reinterpret_cast<char*>(pixels.data()+index*1366*3),1366*3);
    screenshot.close();error=glGetError();
    printf("%s puntos=%u spots=%u GL=%u\n",screenshotName.c_str(),pointCount,spotCount,error);
    log<<screenshotName<<" puntos="<<pointCount<<" spots="<<spotCount<<" GL="<<error<<"\n";
    if(error!=GL_NO_ERROR){result=3;finished=true;}
    if(++frame==8)finished=true;
}
bool Scene::ShouldClose()const{return finished||window.getShouldClose();}
int Scene::Shutdown(){ship.ClearModel();baseE08.Shutdown();flame.ClearMeshModel();flameTexture.ClearTexture();shader.ClearShader();window.Shutdown();return result;}
