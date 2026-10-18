#pragma once
#include <glew.h>
#include <glfw3.h>
#include "Camera.h"
// Todo el teclado y las banderas de las luces pertenecen a Window.
class Window {
public:
    Window();
    int Initialise();
    bool getShouldClose() const;
    void swapBuffers();
    void UpdateInput(Camera& camera, float dt);
    bool ShipLightsOn() const { return shipLights; }
    bool EngineOn(int index) const { return engines[index]; }
    bool OrbitOn() const { return orbit; }
    float HeightOffset() const { return heightOffset; }
    bool HoloOn() const { return holo; }
    bool ClawOn() const { return claw; }
    bool FlashOn() const { return flash; }
    int HeadlightColor() const { return headlight; }
    float ArmAngle() const { return armAngle; }
    float RoverX() const { return roverX; }
    void SetTitle();
    void SetEvidenceState(bool ship, bool engine1, bool engine2);
    bool VerifyInput();
    void Shutdown();
private:
    GLFWwindow* handle=nullptr;
    bool keys[1024]{};
    bool shipLights=true, engines[2]{false,false}, orbit=false;
    bool holo=true,claw=true,flash=false;
    int headlight=0;
    float armAngle=0,roverX=0;
    bool firstMouse=true;
    double lastX=0,lastY=0;
    float xChange=0,yChange=0,heightOffset=0;
    char title[256]{};
    void ApplyKey(int key,int action);
    static void Keyboard(GLFWwindow*,int,int,int,int);
    static void Mouse(GLFWwindow*,double,double);
};
