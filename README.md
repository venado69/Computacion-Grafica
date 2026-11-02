# Práctica 9-2 Animación compleja

Venancio Miguel Tapia Verastegui · 320324187 · Laboratorio 02 · Semestre 2027-1

Rama derivada de Practica-9. Se conserva la práctica 9-1 y se agrega la entrega de la 9-2.

## Actividades

1. Holocrón original con sus ocho esquinas girando sobre su propio eje diagonal a 60 grados por segundo.
2. Cohete Saturn V original del previo 07, con texturas, despegue mediante una curva Bézier cúbica y orientación según la tangente.

## Entrega

- P09-2-320324187.pdf: reporte en el formato del profesor, fecha de entrega 01/11/2026, capturas de código y ejecución, problemas, experiencia, conclusión y bibliografía APA.
- P09-2-320324187.cpp: archivo principal para consulta.
- P09-2-320324187/: solución Visual Studio, código C++ y cabeceras, modelos, texturas, shaders y dependencias.
- P09-2-320324187/Release/: ejecutable Windows x64, DLL y recursos listos para ejecutar.

## Ejecutar

Descargar la rama completa, extraer y abrir EJECUTAR-P09-2.cmd o P09-2-320324187/Release/P09-2-320324187.exe. Conservar los recursos junto al ejecutable.

1: vista general. 2: seguir cohete. 3: holocrón. 0: cámara libre. WASD, Q/E y mouse: cámara libre. Esc: salir.

Las animaciones empiezan automáticamente. El cohete repite un ciclo de 24 segundos: espera de 2 s, vuelo de 20 s y espera final de 2 s; después regresa al inicio para repetir la demostración.

## Compilar

Abrir P09-2-320324187/P09-2-320324187.sln con Visual Studio 2026, herramientas C++ v145 y Windows SDK. Seleccionar Release | x64. El proyecto copia ejecutable, DLL y recursos a Release.

## Verificación

Compilación Release x64 correcta. La verificación numérica comprueba 1201 muestras de la trayectoria, ascenso continuo y tangente no nula. El modo --evidencia genera siete capturas; todas registraron GL=0.

## Modelo del cohete

NASA / Michael D. Carbajal. Saturn V. https://science.nasa.gov/3d-resources/saturn-v/

Archivo original recuperado de Practica07/Previo07-insumos/Saturn V.glb. Conversión local a OBJ con texturas originales, centrado y escalado uniforme a 24 unidades de altura. La fuente se conserva en Models/FUENTE-SATURN-V.txt.
