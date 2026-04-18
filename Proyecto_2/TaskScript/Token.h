#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

//PART 1
//Se declara para saber de su existencia

//Utilizacion de :: para llamar los atributos,metodos,datos y vectores de las clases
//TODOS LOS TIPOS

//se declaran para ser leidos por el lexicalAnalyzer y poder generar esturctras de tokens

enum class TokenType { //TODOS LOS TIPOS DE TOKENS
    // Palabra reservada raíz
    NUMERO,
    HOSPITAL,
    // Delimitadores
    LLAVE_ABRE,     // {
    LLAVE_CIERRA,   // }
    PUNTO_COMA,     // ;
    //PALABRAS RESERVADAS V2
    PACIENTES,
    edad,
    tipo_sangre,
    habitacion,
    MEDICOS,
    especialidad,
    CITAS,
    DATE,
    HOUR,
    DIAGNOSTICOS,
    medico,
    cita,
    diagnostico,
    condicion,
    medicamento,
    dosis,
    paciente,
    codigoMed,
    // Especiales
    DESCONOCIDO,
    FIN_ARCHIVO,
    STRING,
    //SIMBOLOS ESPECIALES
    COMA,
    DOS_PUNTOS,
    CORCH_ABRE,
    CORCH_CIERRA,
    CARDIOLOGIA,
    NEUROLOGIA,
    PEDIATRIA,
    CIRUGIA,
    MEDICINA_GENERAL,
    ONCOLOGIA,
    fecha,
    hora,

    //TOKENS PARA DIAGNOSTICO
    DIARIA,
    CADA_8_HORAS,
    CADA_12_HORAS,
    SEMANAL


};

// Convierte el enum a string legible para la tabla y retorna EL TIPO DE STRING
inline string tokenTypeToString(TokenType t) { //RETORNA el tipo de string leido
    switch (t) {
        //SI NO INCLUYE COMILLAS ES UN TOKEN
        //de la clase::ValorDeLaClase
    case TokenType::HOSPITAL:      return "PALABRA_RESERVADA"; //Viene de... :: de donde viene que trae
    case TokenType::LLAVE_ABRE:    return "LLAVE_ABRE";
    case TokenType::LLAVE_CIERRA:  return "LLAVE_CIERRA";
    case TokenType::PUNTO_COMA:    return "PUNTO_COMA";
    case TokenType::CORCH_CIERRA: return "CORCHETE_CIERRA";
    case TokenType::CORCH_ABRE: return "CORCHETE_CIERRA";
    case TokenType::DOS_PUNTOS: return "DOS_PUNTOS";

    case TokenType::FIN_ARCHIVO:   return "FIN_ARCHIVO";
        //IMPLEMENTACION 18/03
    case TokenType::PACIENTES:     return "PACIENTES";
    case TokenType::paciente: return "paciente";
    case TokenType::edad: return "edad";
    case TokenType::tipo_sangre: return "Tipo de Sangre";
    case TokenType::habitacion: return "habitacion";
    case TokenType::cita: return"cita";


    case TokenType::MEDICOS:       return "MEDICOS";
    case TokenType::DIAGNOSTICOS:  return "DIAGNOSTICOS";
    case TokenType::CITAS:         return "CITAS";
    case TokenType::especialidad: return "ESPECIALIDAD";
    //ESPECIALIDAD
    case TokenType::CARDIOLOGIA: return "ESPECIALIDAD";
    case TokenType::NEUROLOGIA: return "ESPECIALIDAD";
    case TokenType::PEDIATRIA: return "ESPECIALIDAD";
    case TokenType::CIRUGIA: return "ESPECIALIDAD";
    case TokenType::MEDICINA_GENERAL: return "ESPECIALIDAD";
    case TokenType::ONCOLOGIA: return "ESPECIALIDAD";
    //MEDICOS
    case TokenType::medico: return "medico";
    case TokenType::codigoMed: return "codigo-Medico";

    //DOSIS
    case TokenType::DIARIA: return "DOSIS";
    case TokenType::CADA_8_HORAS: return "DOSIS";
    case TokenType::CADA_12_HORAS: return "DOSIS";
    case TokenType::SEMANAL: return "DOSIS";

    case TokenType::DATE: return "FECHA";
    case TokenType::HOUR: return "HORA";
    case TokenType::hora: return "hora_L";
        //NEST
    case TokenType::STRING: return "STRING";
    case TokenType::NUMERO: return "NUMERO";


    default:                       return "DESCONOCIDO";
    }
}

struct Token { //Estructura del Token
    TokenType tipo; //Token
    string lexema; //Entrada
    int linea; //linea
    int columna; //columna
};

#endif // TOKEN_H