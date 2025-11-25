#include <iostream>
#include <string>
#include <cctype>
#include <vector>

enum class TokenType {
    IDENT,   // identificador
    REAL,    // numero real: entero '.' entero+
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
            return {TokenType::IDENT, s.substr(start, i - start), start};
        }

        // Real: entero '.' entero+
        if (std::isdigit(static_cast<unsigned char>(c))) {
            size_t start = i;
            // parte entera (>=1 dígito)
            while (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) ++i;

            if (i < n && s[i] == '.') {
                size_t punto = i++;
                size_t afterDot = i;
                // al menos 1 dígito después del punto
                if (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) {
                    while (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) ++i;
                    return {TokenType::REAL, s.substr(start, i - start), start};
                } else {
                    // retrocede si no hubo dígitos tras el punto, para no “consumir” el '.'
                    i = punto; // dejamos el '.' para que se marque como desconocido abajo
                }
            }

            // Si llegamos aquí, eran solo dígitos (entero) o algo invalida el real
            // Como no se pidió token ENTERO, lo reportamos como desconocido completo.
            // Consumimos un carácter para no quedarnos estancados.
            ++i;
            return {TokenType::DESCONOCIDO, s.substr(start, 1), start};
        }

        // Cualquier otro símbolo: DESCONOCIDO (consume 1 char)
        size_t start = i++;
        return {TokenType::DESCONOCIDO, s.substr(start, 1), start};
    }

private:
    const std::string& s;
    size_t n;
    size_t i;

    static bool esLetra(char c) {
        // Solo letras ASCII como “letra”; si quieres permitir '_', cámbialo aquí.
        return std::isalpha(static_cast<unsigned char>(c)) != 0;
    }

    void saltarBlancos() {
        while (i < n && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string entrada, linea;
    // Lee todo stdin (múltiples líneas) en un solo string
    while (std::getline(std::cin, linea)) {
        if (!entrada.empty()) entrada.push_back('\n');
        entrada += linea;
    }

    Lexer lx(entrada);
    for (;;) {
        Token t = lx.next();
        if (t.type == TokenType::FIN) break;

        switch (t.type) {
            case TokenType::IDENT:
                std::cout << "IDENT\t" << t.lexeme << "\n";
                break;
            case TokenType::REAL:
                std::cout << "REAL\t" << t.lexeme << "\n";
                break;
            case TokenType::DESCONOCIDO:
                std::cout << "DESCONOCIDO\t" << t.lexeme << "\n";
                break;
            default:
                break;
        }
    }
    return 0;
}
