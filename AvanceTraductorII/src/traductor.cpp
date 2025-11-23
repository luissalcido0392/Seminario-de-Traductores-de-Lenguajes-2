// compilador_lr_parser.cpp
// Compilar: g++ -std=c++17 compilador_lr_parser.cpp -o compilador_lr_parser
// Ejecutar: ./compilador_lr_parser compilador.lr compilador.inf < entrada.txt

#include <bits/stdc++.h>
using namespace std;

/* ------------------ Definición de Tokens ------------------ */
enum class TokenType {
    IDENT, ENTERO, REAL, CADENA,
    TIPO_INT, TIPO_FLOAT,
    OP_SUMA, OP_MUL, OP_ASIG,
    OP_RELAC, OP_AND, OP_OR, OP_NOT, OP_IGUALDAD,
    PUNTO_Y_COMA, COMA,
    PARENTESIS_ABRE, PARENTESIS_CIERRA,
    LLAVE_ABRE, LLAVE_CIERRA,
    RESERVADA_IF, RESERVADA_WHILE, RESERVADA_RETURN, RESERVADA_ELSE,
    DESCONOCIDO, FIN
};

struct Token {
    TokenType type;
    string lexeme;
    size_t pos; // Posición inicial del lexema en la cadena de entrada
    // Podríamos añadir línea y columna para un reporte de errores más preciso
    // size_t line;
    // size_t col;
};

static bool esLetra(char c){ return isalpha((unsigned char)c)!=0; }

/* ------------------ LEXER ------------------ */
class Lexer {
public:
    explicit Lexer(const string& src) : s(src), n(src.size()), i(0) {}
    Token next(){
        saltarBlancos();
        if (i >= n) return {TokenType::FIN, "$", i};
        char c = s[i];

        // Identificadores y palabras reservadas
        if (esLetra(c)) {
            size_t start = i++;
            while (i < n && (esLetra(s[i]) || isdigit((unsigned char)s[i]))) ++i;
            string lex = s.substr(start, i-start);
            if (lex=="if") return {TokenType::RESERVADA_IF, lex, start};
            if (lex=="while") return {TokenType::RESERVADA_WHILE, lex, start};
            if (lex=="return") return {TokenType::RESERVADA_RETURN, lex, start};
            if (lex=="else") return {TokenType::RESERVADA_ELSE, lex, start};
            if (lex=="int") return {TokenType::TIPO_INT, lex, start};
            if (lex=="float") return {TokenType::TIPO_FLOAT, lex, start};
            return {TokenType::IDENT, lex, start};
        }

        // Números (Entero y Real)
        if (isdigit((unsigned char)c)) {
            size_t start = i++;
            while (i < n && isdigit((unsigned char)s[i])) ++i;
            if (i < n && s[i]=='.') {
                size_t p = i++; // Guarda la posición del punto
                if (i < n && isdigit((unsigned char)s[i])) {
                    while (i < n && isdigit((unsigned char)s[i])) ++i;
                    return {TokenType::REAL, s.substr(start, i-start), start};
                } else {
                    // Si hay un punto pero no dígitos después, retrocede y trata como entero
                    i = p;
                }
            }
            return {TokenType::ENTERO, s.substr(start, i-start), start};
        }

        // Cadenas
        if (c=='"') {
            size_t start = i++;
            while (i<n && s[i]!='"') i++;
            if (i<n) i++; // Consumir la comilla de cierre
            return {TokenType::CADENA, s.substr(start, i-start), start};
        }

        // Operadores y símbolos
        if (c=='+'){ i++; return {TokenType::OP_SUMA, "+", i-1}; }
        if (c=='-'){ i++; return {TokenType::OP_SUMA, "-", i-1}; }
        if (c=='*'){ i++; return {TokenType::OP_MUL, "*", i-1}; }
        if (c=='/'){ i++; return {TokenType::OP_MUL, "/", i-1}; }
        if (c=='='){
            if (i+1 < n && s[i+1]=='='){ i+=2; return {TokenType::OP_IGUALDAD,"==",i-2}; }
            i++; return {TokenType::OP_ASIG, "=", i-1};
        }
        if (c=='!'){
            if (i+1<n && s[i+1]=='='){ i+=2; return {TokenType::OP_IGUALDAD,"!=",i-2}; }
            i++; return {TokenType::OP_NOT,"!",i-1};
        }
        if (c=='<' || c=='>'){
            // Manejo de <= y >=
            if (i+1 < n && s[i+1]=='='){ i+=2; return {TokenType::OP_RELAC, s.substr(i-2,2), i-2}; }
            i++; return {TokenType::OP_RELAC, string(1,c), i-1};
        }
        if (c=='&' && i+1<n && s[i+1]=='&'){ i+=2; return {TokenType::OP_AND,"&&",i-2}; }
        if (c=='|' && i+1<n && s[i+1]=='|'){ i+=2; return {TokenType::OP_OR,"||",i-2}; }

        if (c==';'){ i++; return {TokenType::PUNTO_Y_COMA, ";", i-1}; }
        if (c==','){ i++; return {TokenType::COMA, ",", i-1}; }
        if (c=='('){ i++; return {TokenType::PARENTESIS_ABRE, "(", i-1}; }
        if (c==')'){ i++; return {TokenType::PARENTESIS_CIERRA, ")", i-1}; }
        if (c=='{'){ i++; return {TokenType::LLAVE_ABRE, "{", i-1}; }
        if (c=='}'){ i++; return {TokenType::LLAVE_CIERRA, "}", i-1}; }

        // Desconocido
        size_t start = i++;
        return {TokenType::DESCONOCIDO, s.substr(start,1), start};
    }
private:
    const string& s;
    size_t n;
    size_t i;
    void saltarBlancos(){
        while (i<n && isspace((unsigned char)s[i])) ++i;
    }
};

/* ------------------ LR Loader ------------------ */
struct LRGram {
    int nReglas;
    vector<int> idRegla;
    vector<int> lonRegla;
    vector<string> nomRegla;
    int nFilas, nCols;
    vector<vector<int>> tabla;
};

bool cargarLR(const string &path, LRGram &G){
    ifstream f(path);
    if(!f) { cerr<<"Error: No se puede abrir el archivo LR: "<<path<<"\n"; return false; }
    if(!(f >> G.nReglas)){ cerr<<"Error de formato .lr: nReglas\n"; return false; }
    G.idRegla.resize(G.nReglas);
    G.lonRegla.resize(G.nReglas);
    G.nomRegla.resize(G.nReglas);
    for(int i=0;i<G.nReglas;i++){
        int id, lon; string nombre;
        f >> id >> lon >> nombre;
        G.idRegla[i] = id;
        G.lonRegla[i] = lon;
        G.nomRegla[i] = nombre;
    }
    if(!(f >> G.nFilas >> G.nCols)){ cerr<<"Error de formato .lr: dimensiones de tabla\n"; return false; }
    G.tabla.assign(G.nFilas, vector<int>(G.nCols, 0));
    for(int r=0;r<G.nFilas;r++)
        for(int c=0;c<G.nCols;c++)
            f >> G.tabla[r][c];
    return true;
}

unordered_map<string,int> leerInf(const string &path){
    unordered_map<string,int> mapa;
    ifstream f(path);
    if(!f) { cerr<<"Error: No se puede abrir el archivo INF: "<<path<<"\n"; return mapa; }
    string key; int val;
    while(f >> key >> val){
        mapa[key] = val;
    }
    return mapa;
}

/* ------------------ Token -> clave de .inf ------------------ */
string tokenToKey(const Token &t){
    switch(t.type){
        case TokenType::IDENT: return "identificador";
        case TokenType::ENTERO: return "entero";
        case TokenType::REAL: return "real";
        case TokenType::CADENA: return "cadena";
        case TokenType::TIPO_INT:
        case TokenType::TIPO_FLOAT: return "tipo"; // Agrupados como "tipo"
        case TokenType::OP_SUMA: return "opSuma";
        case TokenType::OP_MUL: return "opMul";
        case TokenType::OP_ASIG: return "=";
        case TokenType::OP_RELAC: return "opRelac";
        case TokenType::OP_AND: return "opAnd";
        case TokenType::OP_OR: return "opOr";
        case TokenType::OP_NOT: return "opNot";
        case TokenType::OP_IGUALDAD: return "opIgualdad";
        case TokenType::PUNTO_Y_COMA: return ";";
        case TokenType::COMA: return ",";
        case TokenType::PARENTESIS_ABRE: return "(";
        case TokenType::PARENTESIS_CIERRA: return ")";
        case TokenType::LLAVE_ABRE: return "{";
        case TokenType::LLAVE_CIERRA: return "}";
        case TokenType::RESERVADA_IF: return "if";
        case TokenType::RESERVADA_WHILE: return "while";
        case TokenType::RESERVADA_RETURN: return "return";
        case TokenType::RESERVADA_ELSE: return "else";
        case TokenType::FIN: return "$";
        case TokenType::DESCONOCIDO: return "DESCONOCIDO"; // Para errores léxicos
        default: return t.lexeme; // Fallback, aunque DESCONOCIDO debería cubrirlo
    }
}

/* ------------------ Parser LR ------------------ */
bool parseLR(const LRGram &G, const unordered_map<string,int> &mapa, const string &entrada){
    Lexer lx(entrada);
    stack<int> estados;
    estados.push(0); // Estado inicial

    Token tk = lx.next();
    while(true){
        string key = tokenToKey(tk);

        // Manejo de errores léxicos antes del parsing
        if (tk.type == TokenType::DESCONOCIDO) {
            cerr << "Error léxico: Carácter desconocido '" << tk.lexeme << "' en posición " << tk.pos << "\n";
            return false; // Detener el parsing ante un error léxico
        }

        auto it = mapa.find(key);
        if(it == mapa.end()){
            cerr << "Error sintáctico: Token '" << key << "' (lexeme='" << tk.lexeme << "') en posición " << tk.pos << " no tiene mapeo en el archivo .inf. Esto puede indicar un token inesperado o una configuración incorrecta.\n";
            return false;
        }
        int col = it->second;
        int estado = estados.top();
        int accion = G.tabla[estado][col];

        // cout << "Estado: " << estado << ", Token: " << key << " (col " << col << "), Accion: " << accion << "\n"; // Debugging

        if(accion > 0){ // Shift (Desplazamiento)
            estados.push(accion);
            tk = lx.next(); // Leer el siguiente token
        } else if(accion < 0){ // Reduce (Reducción)
            int regla = -accion;
            if(regla <=0 || regla > G.nReglas){
                cerr << "Error sintáctico: Regla inválida " << regla << " en estado " << estado << " con token '" << key << "'\n";
                return false;
            }
            int lon = G.lonRegla[regla-1];
            for(int i=0;i<lon;i++) {
                if(!estados.empty()) estados.pop();
                else {
                    cerr << "Error interno del parser: Pila vacía durante reducción de la regla " << regla << "\n";
                    return false;
                }
            }
            int estadoPrev = estados.top();
            int idNoTerm = G.idRegla[regla-1];
            int gotoEstado = G.tabla[estadoPrev][idNoTerm];
            if(gotoEstado==0){
                cerr << "Error sintáctico: Goto inválido (0) después de reducción de la regla " << G.nomRegla[regla-1] << " en estado " << estadoPrev << " con no-terminal " << idNoTerm << "\n";
                return false;
            }
            estados.push(gotoEstado);

            // Condición de aceptación: Si el token actual es FIN ($) y el estado en la pila es 1 (estado de aceptación de la gramática aumentada)
            if(key == "$" && estados.top()==1){
                cout << "Entrada aceptada.\n";
                return true;
            }
        } else { // 0 = Error sintáctico
            cerr << "Error sintáctico: No se esperaba el token '" << key << "' (lexeme='" << tk.lexeme << "') en estado " << estado << " en la posición " << tk.pos << ".\n";
            return false;
        }
    }
    // Si el bucle termina sin aceptación explícita (ej. por un FIN inesperado o error)
    return false;
}

/* ------------------ MAIN ------------------ */
int main(int argc, char **argv){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if(argc < 3){
        cerr << "Uso: " << argv[0] << " <archivo_gramatica.lr> <archivo_mapeo.inf> < entrada.txt\n";
        return 1;
    }

    LRGram G;
    if(!cargarLR(argv[1], G)) return 1;

    auto mapa = leerInf(argv[2]);
    if(mapa.empty()){
        cerr << "Error: El archivo .inf está vacío o no se pudo leer.\n";
        return 1;
    }

    string entrada, linea;
    while(getline(cin, linea)){
        entrada += linea + "\n";
    }

    cout << "Iniciando análisis léxico y sintáctico...\n";
    bool ok = parseLR(G, mapa, entrada);

    if(ok) {
        cout << "Análisis completado: OK\n";
    } else {
        cout << "Análisis completado: FALLIDO\n";
    }

    return 0;
}
