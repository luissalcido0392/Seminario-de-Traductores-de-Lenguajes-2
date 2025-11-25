# Traductor LR con Construcción de Árbol Sintáctico

Proyecto desarrollado para la materia Seminario de Traductores de Lenguajes II. Implementa un analizador sintáctico LR en C++ capaz de procesar entradas basadas en una gramática definida por el usuario. Además del análisis sintáctico, el traductor genera un árbol sintáctico que se imprime en consola en formato ASCII cuando la cadena es válida.

---

## 1. Objetivo del Proyecto

Diseñar e implementar un analizador LR que:

* Procese una gramática formal y su tabla LR.
* Analice cadenas de entrada y determine si son válidas según la gramática.
* Construya un árbol sintáctico durante el proceso de análisis utilizando los pasos SHIFT y REDUCE.
* Muestre el árbol sintáctico final de manera jerárquica si la entrada es aceptada.

Este proyecto tiene fines educativos y forma parte del estudio práctico de técnicas de análisis sintáctico computacional.

---

## 2. Características Principales

* Implementación en C++ estándar (C++17).
* Construcción de árbol sintáctico durante el análisis sintáctico sin necesidad de pasos adicionales.
* Impresión del árbol sintáctico en modo texto, facilitando su visualización en consola.
* Validación completa según la gramática y tabla LR cargadas por el sistema.
* Estructura modular y separada en archivos fuente para mantenimiento y expansión.

---

## 3. Estructura del Proyecto

```
traductor_LR/
├── src/
│   ├── analizadorsintactico.cpp
│   ├── analizadorsintactico.h
│   ├── traductor.cpp
│   ├── lexer.cpp     (si aplica)
│   ├── lexer.h       (si aplica)
│   └── ...otros módulos
├── docs/
│   ├── reportes
│   └── documentación adicional
├── tests/
│   ├── casos de entrada
│   └── resultados esperados
└── README.md
```

---

## 4. Construcción del Árbol Sintáctico

Durante el análisis LR:

* En cada operación **SHIFT**, se crea un nodo hoja correspondiente al token desplazado.
* En cada **REDUCE**, se extraen los nodos implicados en la regla y se crea un nodo padre que los agrupa.
* Al concluir con una operación **ACCEPT**, la pila semántica contiene una única raíz del árbol.
* El árbol se imprime automáticamente en consola mediante una función recursiva.

Ejemplo de representación en consola:

```
Programa
├── Instrucción
│   ├── ID
│   └── NUM
└── Fin
```

---

## 5. Compilación

Desde la carpeta `src/`, utilizar un compilador compatible:

```bash
g++ -std=c++17 *.cpp -o traductor
```

---

## 6. Ejecución

El analizador puede recibir la cadena ya sea:

* por archivo
* por argumentos
* mediante entrada estándar (dependiendo de la implementación elegida)

Al procesar una cadena válida:

* el analizador confirma aceptación
* se muestra el árbol sintáctico

Si la cadena es inválida:

* se reporta el error
* no se genera el árbol

---

## 7. Requisitos

* C++17 o superior.
* Compilador recomendado: `g++`, `clang++` o equivalente.
* Sistema operativo Windows, Linux o macOS.

---

## 8. Objetivos Académicos Cubiertos

* Comprensión de gramáticas libres de contexto.
* Construcción manual de analizadores LR.
* Implementación de mecanismos profesionales de análisis sintáctico.
* Representación de estructuras sintácticas mediante árboles jerárquicos.
* Aplicación modular en C++ siguiendo buenas prácticas de organización.

---

## 9. Trabajo Futuro

El proyecto puede extenderse mediante:

* Visualización gráfica del árbol sintáctico.
* Generación de código intermedio.
* Soporte para gramáticas más complejas.
* Implementación de un lexer más robusto.
* Manejo de errores más detallado.

---

## 10. Licencia

Proyecto académico. Puede ser utilizado y modificado con fines educativos.
