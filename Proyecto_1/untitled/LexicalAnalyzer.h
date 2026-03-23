#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H
//PART 2
#include "Token.h"
#include <string>
#include <vector>
//Estructuras  para GUARDAR
struct Medic{//medicos
    string nombre;
    string especialidad;
    string codigo;
    int citas;
    int pacientes;
    string nivelCarga;
};

struct Patient{
    string nombre;
    string edad;
    string blood; //TIPO DE SANGRE
    string habita; //habitacion de hospedaje

};

struct Citas{
    string nombre_p; // aca tambien
    string nombre_dr;
    string fecha;
    string hora;


};

struct Diagnostic{
    string nombre_p; //aca hago mi magia
    string condit; //Condicion
    string medicamento; //MEDICAMENTOS
    string dosis; //Tipo de dosis DIARIA,CADA_N_HORAS
};



//ANALIZADOR LEXICO
class LexicalAnalyzer {
public:

    explicit LexicalAnalyzer(const string& fuente); //Constructor OBLICATORIO
    //Constante para que no cambie
    //Contenedores ALMACENARAN LOS DATOS AL LEER EL DOCUMENTO
    vector<Medic> medStorage; //
    vector<Citas> citStorage; //
    vector<Patient> patStorage;
    vector<Diagnostic> diagStorage;


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
    Token leerString(); //Lectura de strings
    void  registrarError(const string& lexema,const string& tipo,const string& desc);

    //REGISTRAR DATOS EN LEXICALANALYZER
    void registrarMedicos(const vector<Token>& tokens, int& pos);
    void registrarCitas(const vector<Token>& tokens, int& pos);
    void registrarPacientes(const vector<Token>& tokens, int& pos);
    void registrarDiagnosticos(const vector<Token>& tokens, int& pos);


};





#endif // LEXICALANALYZER_H
