#include "Window.h"
#include <algorithm>
#include <cstdio>
Window::Window() = default;
int Window::Initialise() {
    if(!glfwInit())return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    handle=glfwCreateWindow(1366,768,"P08 - Nave espacial",nullptr,nullptr);
    if(!handle){glfwTerminate();return 1;}
    glfwMakeContextCurrent(handle);
    glewExperimental=GL_TRUE;
    if(glewInit()!=GLEW_OK){Shutdown();return 1;}
    glfwSetWindowUserPointer(handle,this);
    glfwSetKeyCallback(handle,Keyboard);
    glfwSetCursorPosCallback(handle,Mouse);
    glViewport(0,0,1366,768);glEnable(GL_DEPTH_TEST);glfwSwapInterval(1);
    return 0;
}
// GLFW_REPEAT no conmuta luces: una pulsacion produce un unico cambio.
void Window::ApplyKey(int key,int action) {
    if(key>=0&&key<1024){
        if(action==GLFW_PRESS)keys[key]=true;
        else if(action==GLFW_RELEASE)keys[key]=false;
    }
    if(action!=GLFW_PRESS)return;
    switch(key){
    case GLFW_KEY_C: headlight=(headlight+1)%6;break;
    case GLFW_KEY_H: holo=!holo;break;
    case GLFW_KEY_P: claw=!claw;break;
    case GLFW_KEY_F: flash=!flash;break;
    case GLFW_KEY_L: shipLights=!shipLights;break;
    case GLFW_KEY_1: case GLFW_KEY_KP_1: engines[0]=!engines[0];break;
    case GLFW_KEY_2: case GLFW_KEY_KP_2: engines[1]=!engines[1];break;
    case GLFW_KEY_R: orbit=!orbit;break;
    case GLFW_KEY_ESCAPE: if(handle)glfwSetWindowShouldClose(handle,GL_TRUE);break;
    default:break;
    }
}
void Window::Keyboard(GLFWwindow* w,int key,int,int action,int){
    static_cast<Window*>(glfwGetWindowUserPointer(w))->ApplyKey(key,action);
}
void Window::Mouse(GLFWwindow* w,double x,double y){
    Window* self=static_cast<Window*>(glfwGetWindowUserPointer(w));
    if(self->firstMouse){self->lastX=x;self->lastY=y;self->firstMouse=false;}
    self->xChange+=float(x-self->lastX);self->yChange+=float(self->lastY-y);
    self->lastX=x;self->lastY=y;
}
void Window::UpdateInput(Camera& camera,float dt){
    camera.keyControl(keys,dt);camera.mouseControl(xChange,yChange);xChange=yChange=0;
    if(keys[GLFW_KEY_LEFT])roverX-=3*dt;
    if(keys[GLFW_KEY_RIGHT])roverX+=3*dt;
    if(keys[GLFW_KEY_7])armAngle=std::max(-50.f,armAngle-35*dt);
    if(keys[GLFW_KEY_8])armAngle=std::min(50.f,armAngle+35*dt);
    if(keys[GLFW_KEY_I])heightOffset=std::min(14.f,heightOffset+4*dt);
    if(keys[GLFW_KEY_K])heightOffset=std::max(-5.f,heightOffset-4*dt);
}
void Window::SetTitle(){
    snprintf(title,sizeof(title),"P08 | L: luces nave %s | 1: motor 1 %s | 2: motor 2 %s | C: faro | H: Holocron | P: pinza | I/K: avion | 7/8: brazo | flechas: Rover",shipLights?"ON":"OFF",engines[0]?"ON":"OFF",engines[1]?"ON":"OFF");
    glfwSetWindowTitle(handle,title);
}
void Window::SetEvidenceState(bool ship,bool engine1,bool engine2){shipLights=ship;engines[0]=engine1;engines[1]=engine2;}
bool Window::VerifyInput(){
    SetEvidenceState(true,false,false);
    ApplyKey(GLFW_KEY_1,GLFW_PRESS);if(!engines[0]||engines[1])return false;
    ApplyKey(GLFW_KEY_1,GLFW_REPEAT);if(!engines[0])return false;
    ApplyKey(GLFW_KEY_1,GLFW_RELEASE);
    ApplyKey(GLFW_KEY_2,GLFW_PRESS);if(!engines[0]||!engines[1])return false;
    ApplyKey(GLFW_KEY_L,GLFW_PRESS);if(shipLights||!engines[0]||!engines[1])return false;
    ApplyKey(GLFW_KEY_L,GLFW_REPEAT);if(shipLights)return false;
    ApplyKey(GLFW_KEY_L,GLFW_RELEASE);ApplyKey(GLFW_KEY_L,GLFW_PRESS);ApplyKey(GLFW_KEY_L,GLFW_RELEASE);
    ApplyKey(GLFW_KEY_1,GLFW_PRESS);ApplyKey(GLFW_KEY_1,GLFW_RELEASE);if(engines[0]||!engines[1])return false;
    ApplyKey(GLFW_KEY_2,GLFW_RELEASE);ApplyKey(GLFW_KEY_2,GLFW_PRESS);ApplyKey(GLFW_KEY_2,GLFW_RELEASE);
    if(engines[0]||engines[1]||!shipLights)return false;
    SetEvidenceState(true,false,false);
    for(int i=0;i<6;++i){
        ApplyKey(GLFW_KEY_C,GLFW_PRESS);ApplyKey(GLFW_KEY_C,GLFW_REPEAT);
        if(headlight!=(i+1)%6)return false;ApplyKey(GLFW_KEY_C,GLFW_RELEASE);
    }
    ApplyKey(GLFW_KEY_H,GLFW_PRESS);ApplyKey(GLFW_KEY_H,GLFW_REPEAT);if(holo||!claw)return false;
    ApplyKey(GLFW_KEY_H,GLFW_RELEASE);ApplyKey(GLFW_KEY_H,GLFW_PRESS);ApplyKey(GLFW_KEY_H,GLFW_RELEASE);
    ApplyKey(GLFW_KEY_P,GLFW_PRESS);ApplyKey(GLFW_KEY_P,GLFW_REPEAT);if(claw||!holo)return false;
    ApplyKey(GLFW_KEY_P,GLFW_RELEASE);ApplyKey(GLFW_KEY_P,GLFW_PRESS);ApplyKey(GLFW_KEY_P,GLFW_RELEASE);
    return holo&&claw&&headlight==0;
}
bool Window::getShouldClose()const{return glfwWindowShouldClose(handle)!=0;}
void Window::swapBuffers(){glfwSwapBuffers(handle);}
void Window::Shutdown(){if(handle){glfwDestroyWindow(handle);handle=nullptr;glfwTerminate();}}
