# Práctica 9-1 Animación básica

**Alumno:** Venancio Miguel Tapia Verastegui  
**Cuenta:** 320324187  
**Laboratorio:** Computación Gráfica e Interacción Humano Computadora, UNAM  
**Semestre:** 2027-1

## Actividades

1. Una nave recorre automáticamente un circuito cerrado con rectas, curvas, rampa de subida y rampa de bajada. La orientación sigue la dirección y la pendiente del recorrido.
2. El satélite vuela en una trayectoria elíptica con altura variable. Sus dos paneles y pala superior se animan automáticamente alrededor de sus pivotes originales.

## Archivos de entrega

- `P09-1-320324187.pdf`: reporte con el formato del profesor, capturas de código y ejecución, problemas, conclusiones y bibliografía.
- `P09-1-320324187.cpp`: copia del archivo principal para consulta.
- `P09-1-320324187/`: solución de Visual Studio, código fuente, modelos, texturas, shaders y dependencias.
- `P09-1-320324187/Release/`: ejecutable compilado en Release x64, bibliotecas DLL y recursos necesarios para ejecutarlo.

## Ejecutar

Descargar la rama completa y abrir `P09-1-320324187/Release/P09-1-320324187.exe` en Windows x64. Conservar las DLL y las carpetas Models, Textures y shaders junto al ejecutable. La animación comienza automáticamente.

**1:** vista general; **2:** seguir nave; **3:** seguir satélite; **0:** cámara libre. **WASD**, **Q/E** y mouse controlan la cámara libre. **Esc:** salir.

## Compilar

Abrir `P09-1-320324187/P09-1-320324187.sln` con Visual Studio 2026 y herramientas de C++ v145. Seleccionar **Release | x64** y compilar. La salida y sus recursos quedan en `Release`.

## Verificación

El programa comprueba 24 000 pasos antes de iniciar la ventana: cierre del circuito, orientación, subida y bajada, y límites de las articulaciones. El modo `--evidencia` genera siete capturas; la ejecución comprobada registró GL=0 en todas.

## Recursos

Base: ejercicio de práctica 09 y shaders del profesor. Satélite articulado reutilizado de la práctica previa. Nave de Kenney Space Kit, licencia CC0; información incluida en `Models/FUENTE.txt` y `Models/LICENCIA-Kenney.txt`.
