// Practica 08 Iluminacion 2 - 320324187
#include "Scene.h"
#include <string>
int main(int argc,char** argv){
    Scene scene;
    const bool evidence=argc>1&&std::string(argv[1])=="--evidencia";
    if(!scene.Initialise(argv[0],evidence))return 1;
    // Sin declaraciones de variables, lectura de teclado o construccion de luces.
    while(!scene.ShouldClose()){
        scene.Frame();
    }
    return scene.Shutdown();
}
