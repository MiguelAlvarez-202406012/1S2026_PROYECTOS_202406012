#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H
//PART 2
#include "Token.h"
#include <string>
#include <vector>

class LexicalAnalyzer {
public:

    explicit LexicalAnalyzer(const string& fuente); //Constructor OBLICATORIO
    //Constante para que no cambie
    // Retorna todos los tokens del archivo
    vector<Token> tokenize(); //declara vector para tokenizacion
    // Errores acumulados durante el análisis
    struct ErrorLexico {
        string lexema;
        string tipoError;
        string descripcion;
        int linea;
        int columna;
    };
    vector<ErrorLexico> errores;

private:
    //METODOS Y VARIABLES ABSTRACTAS
    string codigo;   // Contenido completo del archivo
    int pos;         // Posición actual en el string
    int linea;       //EN QUE LINEA
    int columna;        //EN QUE COLUMNA
    char actual();        // Carácter en pos actual
    char avanzar();       // Avanza y retorna el siguiente carácter
    void saltarBlancos(); // Ignora espacios, tabs, saltos de línea
    Token siguienteToken();        // AFD principal
    Token leerPalabraReservada(); // Reconoce HOSPITAL (y futuras), retorna
    void  registrarError(const string& lexema,const string& tipo,const string& desc);
};

#endif // LEXICALANALYZER_H
