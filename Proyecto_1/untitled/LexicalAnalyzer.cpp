#include "LexicalAnalyzer.h"
#include <cctype>
#include <vector>
#include <cctype>

//SOLO DECLARA LAS FUNCIONAS A UTILZIAR
//UTILIZACION DEL HEADER LexicalAnalyzer
//clase para el analizador lexico y utiliza el header de Lexicalanalyzer para decalrar las fucnoiones
// De que Clase ::
using namespace std;
                        // PUNTERO
//DE LEXICALANALYZER HEADER LA FUNCION
LexicalAnalyzer::LexicalAnalyzer(const string& fuente)
                //Resolucion de ambito
    : codigo(fuente), pos(0), linea(1), columna(1) {}
        //Codigo fuente, en que posicion se encentra

// ── Helpers ─────────────────────────────────────────────────────

char LexicalAnalyzer::actual() {
    if (pos < (int)codigo.size()) return codigo[pos];
    //si la posicion es inferior al codigo retorna la posicion actual
    return '\0'; //de lo contrario si no lee nada retorna cero //devuelve un cahr
}

char LexicalAnalyzer::avanzar() {
    //FUNCION PARA MOVER A LA SIGUENTE FILA
    char c = actual(); //trae la posicion actual
    pos++; //incrementa la posicion actual
    if (c == '\n') { linea++; columna = 1; } //si es un salto de linea
    else           { columna++; }
    return c; //retorna actual
}

void LexicalAnalyzer::saltarBlancos() { //salta espacios en blanco
    while (pos < (int)codigo.size() && isspace((unsigned char)actual())) //mientras la posicion sea inferior al codigo Y ENCUENTRE UN ESPACIO EN BLANCO
        avanzar();
}


//al detectar un error lexico
void LexicalAnalyzer::registrarError(const string& lex,  const string& tipo,const string& desc) {
    //llama al vector de errores y mete el error lexico
    errores.push_back({ lex, tipo, desc, linea, columna });

}

// ── AFD Principal ────────────────────────────────────────────────

Token LexicalAnalyzer::siguienteToken() { //PARA SIGUENTE TOKEN
    saltarBlancos();

    int linIni = linea; //en que linea
    int colIni = columna; //en que columna

    if (pos >= (int)codigo.size())
        return { TokenType::FIN_ARCHIVO, "EOF", linIni, colIni }; //Si la posicion es mayor o igual al tamaÑo del codigo actual regresa el token FIN?ARCHIVO

    char c = actual(); //RETORNO DEL ACTUAL

    // Delimitadores de un solo carácter
    if (c == '{') { avanzar(); return { TokenType::LLAVE_ABRE,   "{", linIni, colIni }; } //SI CH es un Bracket { avanza y retona que es una llave
    if (c == '}') { avanzar(); return { TokenType::LLAVE_CIERRA, "}", linIni, colIni }; } //SI CH es un Bracket { avanza y retona que es una llave
    if (c == ';') { avanzar(); return { TokenType::PUNTO_COMA,   ";", linIni, colIni }; } //SI CH es un SEMICOLON ; avanza y retona que es una SEMICOLON

    // Palabra reservada o identificador
    if (std::isalpha((unsigned char)c) || c == '_') //SI ES PALABRA RESERVADA O ID
        return leerPalabraReservada();

    // Carácter no reconocido → error léxico
    std::string lex(1, c);
    avanzar();
    registrarError(lex, "Carácter ilegal",
                   "El carácter '" + lex + "' no pertenece al alfabeto de MedLang.");
    return { TokenType::DESCONOCIDO, lex, linIni, colIni };
}

// ── Reconocedor de palabras reservadas ──────────────────────────

Token LexicalAnalyzer::leerPalabraReservada() { //RETORNA UN TOKEN
    int linIni = linea;
    int colIni = columna;
    string lexema;

    // Leer todos los caracteres alfanuméricos o guión bajo
    while (pos < (int)codigo.size() &&
           (std::isalnum((unsigned char)actual()) || actual() == '_')) {
        lexema += avanzar();
    }

    // Tabla de palabras reservadas (por ahora solo HOSPITAL)
    if (lexema == "HOSPITAL")
        return { TokenType::HOSPITAL, lexema, linIni, colIni };
    //CASOS
    if(lexema == "PACIENTES")
        return {TokenType::PACIENTES,lexema,linIni,colIni};
    if(lexema == "MEDICOS")
        return {TokenType::MEDICOS,lexema,linIni,colIni};
    if(lexema == "CITAS")
        return {TokenType::CITAS,lexema,linIni,colIni};
    if(lexema == "DIAGNOSTICOS")
        return {TokenType::DIAGNOSTICOS,lexema,linIni,colIni};





    // No es una palabra reservada conocida → error
    registrarError(lexema, "Token no reconocido",
                   "'" + lexema + "' no es una palabra reservada válida en MedLang.");
    return { TokenType::DESCONOCIDO, lexema, linIni, colIni };
}

// ── Tokenización completa ──────────────────────────────────────── //LEE LOS TOKENS

    vector<Token> LexicalAnalyzer::tokenize() {
        vector<Token> tokens;
    pos = 0; linea = 1; columna = 1;
    errores.clear();

    while (true) {
        Token t = siguienteToken();
        if (t.tipo == TokenType::FIN_ARCHIVO) break;
        if (t.tipo != TokenType::DESCONOCIDO)
            tokens.push_back(t);
    }
    return tokens;
}
