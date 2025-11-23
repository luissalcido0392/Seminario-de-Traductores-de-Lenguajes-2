# Traductor C/C++ -> Ensamblador (Proyecto - Seminario de Traductores)

## Descripción
Proyecto de práctica: traductor simple de subset de C/C++ a ensamblador. Incluye:
- Análisis léxico (lexer)
- Parser LR manual
- Construcción de árbol sintáctico y visualización ASCII
- Generación básica de ensamblador (ejemplos)

## Estructura del repo
- `/src` : código fuente (lexer, parser, main)
- `/tests`: archivos de entrada para pruebas
- `/asm`  : ejemplos de salida ensamblador generada
- `/docs` : imágenes y documentación (tabla LR, diagrama AST)

## Requisitos
- GCC / G++ (recomiendo g++ >= 9)
- Make (opcional)

## Compilar
```bash
cd src
g++ -std=c++17 parser.cpp lexer.cpp -o traductor
# o si tienes main.cpp:
g++ -std=c++17 main.cpp parser.cpp lexer.cpp -o traductor
