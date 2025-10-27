
# Analizador Léxico en C++

Este proyecto implementa un analizador léxico (lexer) básico en C++ que identifica tres tipos de tokens:

* IDENT → Identificadores (letras seguidas opcionalmente de letras o dígitos)
* REAL → Números reales con punto decimal (por ejemplo, 3.14, 0.5)
* DESCONOCIDO → Cualquier símbolo o secuencia no reconocida

## Descripción

El programa lee texto desde la entrada estándar (stdin) y analiza carácter por carácter para detectar tokens válidos.
Cada token reconocido se imprime junto con su tipo.

Ejemplo de entrada:

```
variable1 3.14 x9 hola. 123
```

Salida esperada:

```
IDENT	variable1
REAL	3.14
IDENT	x9
DESCONOCIDO	.
DESCONOCIDO	1
```

Notas:

* El analizador no reconoce números enteros como tokens válidos (solo reales).
* Los caracteres desconocidos se clasifican como DESCONOCIDO.

## Compilación

Puedes compilar el programa con cualquier compilador de C++ moderno (C++11 o superior).

En Linux o macOS:

```
g++ Lex.cpp -o lex
```

En Windows (con MinGW):

```
g++ Lex.cpp -o lex.exe
```

## Ejecución

Ejemplo ejecutando el programa desde terminal:

```
echo "var1 25.6 test. 12x" | ./lex
```

O también puedes escribir manualmente:

```
./lex
```

(luego escribes tu entrada y presionas Ctrl+D o Ctrl+Z + Enter para finalizar).

## Estructura del código

* TokenType: Enum que define los tipos de tokens reconocidos.
* Token: Estructura que guarda el tipo, lexema y posición del token.
* Lexer: Clase encargada de recorrer el texto, reconocer patrones y devolver tokens.
* main(): Función principal que controla la lectura de entrada y muestra los resultados.
