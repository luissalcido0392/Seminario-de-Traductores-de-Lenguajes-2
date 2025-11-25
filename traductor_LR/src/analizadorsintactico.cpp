#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <memory>

enum class TokenType {
    IDENT,   // identificador
    ENTERO,  // número entero
    REAL,    // número real: entero '.' entero+
    OP_SUMA, // +, -
    OP_MUL,  // *, /
    OP_ASIG, // =
    OP_RELAC,// <, >, <=, >=, !=, ==
    OP_AND,  // &&
    OP_OR,   // ||
    OP_NOT,  // !
    PARENTESIS_ABRE, // (
    PARENTESIS_CIERRA, // )
    LLAVE_ABRE, // {
    LLAVE_CIERRA, // }
    PUNTO_Y_COMA, // ;
    RESERVADA_IF, // if
    RESERVADA_WHILE, // while
    RESERVADA_RETURN, // return
    RESERVADA_ELSE, // else
    TIPO_INT, // int
    TIPO_FLOAT, // float
    PESOS, // $
    DESCONOCIDO,
    FIN
};

struct Token {
    TokenType type;
    std::string lexeme;
    size_t pos;
};

class Lexer {
public:
    explicit Lexer(const std::string& src) : s(src), n(src.size()), i(0) {}

    Token next() {
        saltarBlancos();

        if (i >= n) return {TokenType::FIN, "", i};

        char c = s[i];

        // identificador: letra (letra | digito)*
        if (esLetra(c)) {
            size_t start = i++;
            while (i < n && (esLetra(s[i]) || std::isdigit(static_cast<unsigned char>(s[i])))) {
                ++i;
            }
            std::string lexeme = s.substr(start, i - start);
            if (lexeme == "if") return {TokenType::RESERVADA_IF, lexeme, start};
            if (lexeme == "while") return {TokenType::RESERVADA_WHILE, lexeme, start};
            if (lexeme == "return") return {TokenType::RESERVADA_RETURN, lexeme, start};
            if (lexeme == "else") return {TokenType::RESERVADA_ELSE, lexeme, start};
            if (lexeme == "int") return {TokenType::TIPO_INT, lexeme, start};
            if (lexeme == "float") return {TokenType::TIPO_FLOAT, lexeme, start};
            return {TokenType::IDENT, lexeme, start};
        }

        // Entero: dígito+
        if (std::isdigit(static_cast<unsigned char>(c))) {
            size_t start = i++;
            while (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) ++i;
            return {TokenType::ENTERO, s.substr(start, i - start), start};
        }

        // Real: entero '.' entero+
        if (std::isdigit(static_cast<unsigned char>(c))) {
            size_t start = i;
            while (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) ++i;

            if (i < n && s[i] == '.') {
                size_t punto = i++;
                if (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) {
                    while (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) ++i;
                    return {TokenType::REAL, s.substr(start, i - start), start};
                } else {
                    i = punto; // retrocede si no hubo dígitos tras el punto
                }
            }
            ++i; // consume un carácter
            return {TokenType::DESCONOCIDO, s.substr(start, 1), start};
        }

        // Operadores
        if (c == '+') { i++; return {TokenType::OP_SUMA, "+", i - 1}; }
        if (c == '-') { i++; return {TokenType::OP_SUMA, "-", i - 1}; }
        if (c == '*') { i++; return {TokenType::OP_MUL, "*", i - 1}; }
        if (c == '/') { i++; return {TokenType::OP_MUL, "/", i - 1}; }
        if (c == '=') { i++; return {TokenType::OP_ASIG, "=", i - 1}; }
        if (c == '<') { i++; return {TokenType::OP_RELAC, "<", i - 1}; }
        if (c == '>') { i++; return {TokenType::OP_RELAC, ">", i - 1}; }
        if (c == ';') { i++; return {TokenType::PUNTO_Y_COMA, ";", i - 1}; }
        if (c == '(') { i++; return {TokenType::PARENTESIS_ABRE, "(", i - 1}; }
        if (c == ')') { i++; return {TokenType::PARENTESIS_CIERRA, ")", i - 1}; }
        if (c == '{') { i++; return {TokenType::LLAVE_ABRE, "{", i - 1}; }
        if (c == '}') { i++; return {TokenType::LLAVE_CIERRA, "}", i - 1}; }
        if (c == '&' && i + 1 < n && s[i + 1] == '&') { i += 2; return {TokenType::OP_AND, "&&", i - 2}; }
        if (c == '|' && i + 1 < n && s[i + 1] == '|') { i += 2; return {TokenType::OP_OR, "||", i - 2}; }
        if (c == '!') { i++; return {TokenType::OP_NOT, "!", i - 1}; }
        if (c == '=' && i + 1 < n && s[i + 1] == '=') { i += 2; return {TokenType::OP_RELAC, "==", i - 2}; }
        if (c == '!' && i + 1 < n && s[i + 1] == '=') { i += 2; return {TokenType::OP_RELAC, "!=", i - 2}; }

        // Cualquier otro símbolo: DESCONOCIDO (consume 1 char)
        size_t start = i++;
        return {TokenType::DESCONOCIDO, s.substr(start - 1, 1), start - 1};
    }

private:
    const std::string& s;
    size_t n;
    size_t i;

    static bool esLetra(char c) {
        return std::isalpha(static_cast<unsigned char>(c)) != 0;
    }

    void saltarBlancos() {
        while (i < n && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    }
};

// Clase base para elementos de la pila
class ElementoPila {
public:
    virtual ~ElementoPila() = default;
    virtual std::string toString() const = 0; // Método virtual puro para convertir a string
};

// Clase para terminales
class Terminal : public ElementoPila {
public:
    Terminal(TokenType tipo) : tipo(tipo) {}
    std::string toString() const override {
        return "Terminal: " + std::to_string(static_cast<int>(tipo));
    }
private:
    TokenType tipo;
};

// Clase para no terminales
class NoTerminal : public ElementoPila {
public:
    NoTerminal(int id) : id(id) {}
    std::string toString() const override {
        return "NoTerminal: " + std::to_string(id);
    }
private:
    int id;
};

// Clase para estados
class Estado : public ElementoPila {
public:
    Estado(int estado) : estado(estado) {}
    std::string toString() const override {
        return "Estado: " + std::to_string(estado);
    }
private:
    int estado;
};

class Pila {
public:
    void push(ElementoPila* elemento) {
        data.push_back(std::unique_ptr<ElementoPila>(elemento));
    }

    ElementoPila* pop() {
        if (data.empty()) throw std::out_of_range("Pila vacía");
        ElementoPila* elemento = data.back().release();
        data.pop_back();
        return elemento;
    }

    ElementoPila* top() const {
        if (data.empty()) throw std::out_of_range("Pila vacía");
        return data.back().get();
    }

    void muestra() const {
        for (const auto& elem : data) {
            std::cout << elem->toString() << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<std::unique_ptr<ElementoPila>> data;
};

// Tabla LR(1)
int tablaLR[3][3] = {
    {2, 0, 1},  // Estado 0
    {0, -1, 0}, // Estado 1
    {0, -2, 0}  // Estado 2
};

// Identificadores de reglas
int idReglas[2] = {2, 2}; // Identificadores de E
int lonReglas[2] = {3, 0}; // Longitudes de las reglas

void ejemplo1() {
    Pila pila;
    pila.push(new Estado(2));
    pila.push(new Estado(3));
    pila.push(new Estado(4));
    pila.push(new Estado(5));
    pila.muestra();
    std::cout << pila.top()->toString() << std::endl;
    std::cout << pila.top()->toString() << std::endl;
    std::cout << pila.pop()->toString() << std::endl;
    std::cout << pila.pop()->toString() << std::endl;
}

void ejemplo2() {
    Lexer lexico("+-+");
    while (true) {
        Token t = lexico.next();
        if (t.type == TokenType::FIN) break;
        std::cout << t.lexeme << std::endl;
    }
}

void ejemplo3() {
    Pila pila;
    int fila, columna, accion;
    bool aceptacion = false;
    Lexer lexico("a$");

    pila.push(new Estado(static_cast<int>(TokenType::PESOS))); // PESOS es 1
    pila.push(new Estado(0)); // Estado inicial
    lexico.next(); // Leer el primer símbolo

    while (true) {
        fila = static_cast<int>(pila.top()->toString()[7]) - '0'; // Obtener el estado
        columna = static_cast<int>(lexico.next().type); // Obtener el tipo del símbolo
        accion = tablaLR[fila][columna];

        pila.muestra();
        std::cout << "entrada: " << lexico.next().lexeme << std::endl;
        std::cout << "accion: " << accion << std::endl;

        if (accion > 0) { // Desplazamiento
            pila.push(new Terminal(lexico.next().type));
            pila.push(new Estado(accion));
            lexico.next(); // Leer el siguiente símbolo
        } else if (accion < 0) { // Reducción
            int regla = -accion - 1; // Obtener el índice de la regla
            for (int j = 0; j < lonReglas[regla]; ++j) {
                delete pila.pop(); // Sacar elementos de la pila
            }
            int nuevoSimbolo = idReglas[regla]; // Obtener el no terminal
            fila = static_cast<int>(pila.top()->toString()[7]) - '0';
            accion = tablaLR[fila][nuevoSimbolo];
            pila.push(new NoTerminal(nuevoSimbolo));
            pila.push(new Estado(accion));
        } else {
            break; // Error
        }

        if (accion == -1) { // Aceptación
            aceptacion = true;
            break;
        }
    }

    if (aceptacion) {
        std::cout << "aceptación" << std::endl;
    } else {
        std::cout << "error" << std::endl;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Ejecutar ejemplos
    ejemplo1();
    ejemplo2();
    ejemplo3();

    return 0;
}
