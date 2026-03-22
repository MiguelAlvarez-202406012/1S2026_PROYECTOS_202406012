#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

//PART 1
//Se declara para saber de su existencia

//Utilizacion de :: para llamar los atributos,metodos,datos y vectores de las clases
//TODOS LOS TIPOS
enum class TokenType { //TODOS LOS TIPOS DE TOKENS
    // Palabra reservada raíz
    HOSPITAL,
    // Delimitadores
    LLAVE_ABRE,     // {
    LLAVE_CIERRA,   // }
    PUNTO_COMA,     // ;
    //PALABRAS RESERVADAS V2
    PACIENTES,
    MEDICOS,
    especialidad,
    CITAS,
    DIAGNOSTICOS,
    medico,
    cita,
    diagnostico,
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
    NEUROLOGIA

};

// Convierte el enum a string legible para la tabla y retorna EL TIPO DE STRING
inline string tokenTypeToString(TokenType t) { //RETORNA
    switch (t) {

        //de la clase::ValorDeLaClase
    case TokenType::HOSPITAL:      return "PALABRA_RESERVADA"; //Viene de... :: de donde viene que trae
    case TokenType::LLAVE_ABRE:    return "LLAVE_ABRE";
    case TokenType::LLAVE_CIERRA:  return "LLAVE_CIERRA";
    case TokenType::PUNTO_COMA:    return "PUNTO_COMA";
    case TokenType::FIN_ARCHIVO:   return "FIN_ARCHIVO";
        //IMPLEMENTACION 18/03
    case TokenType::PACIENTES:     return "PACIENTES";
    case TokenType::MEDICOS:       return "MEDICOS";
    case TokenType::DIAGNOSTICOS:  return "DIAGNOSTICOS";
    case TokenType::CITAS:         return "CITAS";
    case TokenType::especialidad: return "ESPECIALIDAD";
    case TokenType::CARDIOLOGIA: return "ESPECIALIDAD";
    case TokenType::NEUROLOGIA: return "ESPECIALIDAD";

        //NEST

    case TokenType::STRING: return "STRING";


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
