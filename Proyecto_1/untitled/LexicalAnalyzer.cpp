#include "LexicalAnalyzer.h"
#include <cctype>
#include <vector>
#include <cctype>
#include <QDebug>

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

//

// ── Helpers ─────────────────────────────────────────────────────

char LexicalAnalyzer::actual() {
    if (pos < (int)codigo.size()) return codigo[pos];
    //si la posicion es inferior al codigo retorna la posicion actual
    return '\0'; //de lo contrario si no lee nada retorna cero //devuelve un cahr
}

char LexicalAnalyzer::avanzar() {
    //FUNCION PARA MOVER A LA SIGUENTE FILA
    char c = actual(); //trae el valor de la posicion actual
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
    if (c == ':') { avanzar(); return { TokenType::DOS_PUNTOS,   ":", linIni, colIni }; }
    if (c == ',') { avanzar(); return { TokenType::COMA,         ",", linIni, colIni }; }
    if (c == '[') { avanzar(); return { TokenType::CORCH_ABRE, "[", linIni, colIni }; }
    if (c == ']') { avanzar(); return { TokenType::CORCH_CIERRA, "]", linIni, colIni }; }
    if (c == '"') { return leerString(); }  // Nueva función para strings

    //reconocimiento de numeros
    if(isdigit(c) || c=='-'){
        return leerNum();
    }


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
           //mientras la posciion sea inferior al tamaño del codigo y sea indefinido o _
           (std::isalnum((unsigned char)actual()) || actual() == '_')) {
        lexema += avanzar();
    }

    // Tabla de palabras reservadas, al leerla retorna un Token del tipo leido por el lexema
    if (lexema == "HOSPITAL")
        return { TokenType::HOSPITAL, lexema, linIni, colIni };
    //CASOS
    if(lexema == "PACIENTES" )
        return {TokenType::PACIENTES,lexema,linIni,colIni};
    if(lexema == "MEDICOS")
        return {TokenType::MEDICOS,lexema,linIni,colIni};
    if(lexema == "CITAS")
        return {TokenType::CITAS,lexema,linIni,colIni};
    if(lexema == "DIAGNOSTICOS")
        return {TokenType::DIAGNOSTICOS,lexema,linIni,colIni};
    //Analizar las demas palabras reservadas
    //MEDICO
    if(lexema == "medico" )
        return {TokenType::medico,lexema,linIni,colIni};
    if(lexema == "especialidad")
        return {TokenType::especialidad,lexema,linIni,colIni};
    if(lexema == "codigo") //SI EL LEXEMA LEE CODIGO MED
        return {TokenType::codigoMed,lexema,linIni,colIni};
    if(lexema == "CARDIOLOGIA")
        return {TokenType::CARDIOLOGIA,lexema,linIni,colIni};
    if(lexema == "NEUROLOGIA")
        return {TokenType::NEUROLOGIA,lexema,linIni,colIni};
    if(lexema == "PEDIATRIA")
        return {TokenType::PEDIATRIA,lexema,linIni,colIni};
    if(lexema == "CIRUGIA")
        return {TokenType::CIRUGIA,lexema,linIni,colIni};
    if(lexema == "MEDICINA_GENERAL")
        return {TokenType::MEDICINA_GENERAL,lexema,linIni,colIni};
    if(lexema == "ONCOLOGIA")
        return {TokenType::ONCOLOGIA,lexema,linIni,colIni};


    //MEDICO
    if(lexema == "cita" )
        return {TokenType::cita,lexema,linIni,colIni};
    if(lexema == "diagnostico" )
        return {TokenType::diagnostico,lexema,linIni,colIni};
    if(lexema == "paciente" )
        return {TokenType::paciente,lexema,linIni,colIni};
    //CITAS
    if(lexema == "fecha")
        return {TokenType::fecha,lexema,linIni,colIni};
    if(lexema == "hora")
        return {TokenType::hora,lexema,linIni,colIni};

    //PACIENTE
    if(lexema == "edad")
        return {TokenType::edad,lexema,linIni,colIni};
    if(lexema == "tipo_sangre")
        return {TokenType::tipo_sangre,lexema,linIni,colIni};
    if(lexema == "habitacion")
        return {TokenType::habitacion,lexema,linIni,colIni};

    //DIAGNOSTICOS

    if(lexema == "condicion")
        return {TokenType::condicion,lexema,linIni,colIni};
    if(lexema == "medicamento")
        return {TokenType::medicamento,lexema,linIni,colIni};
    if(lexema == "dosis")
        return {TokenType::dosis,lexema,linIni,colIni};
    if(lexema == "DIARIA")
        return {TokenType::DIARIA,lexema,linIni,colIni};
    if(lexema == "CADA_8_HORAS")
        return {TokenType::CADA_8_HORAS,lexema,linIni,colIni};
    if(lexema == "CADA_12_HORAS")
        return {TokenType::CADA_12_HORAS,lexema,linIni,colIni};
    if(lexema == "SEMANAL")
        return {TokenType::SEMANAL,lexema,linIni,colIni};



    // En caso que no sea una palabra reservada entonces se llama a registrar el error
    registrarError(lexema, "Token no reconocido",
                   "'" + lexema + "' no es una palabra reservada válida en MedLang.");
    return { TokenType::DESCONOCIDO, lexema, linIni, colIni };
}


//DECLARACION DE FUNCION PARA LECTURA DE STRINGS
Token LexicalAnalyzer::leerString(){
    int linIn = linea;
    int colIn = columna;
    string lexema; //llegara con cualquier contenido

    avanzar(); // Saltara la comilla inicial

    while (pos < (int)codigo.size() && actual() != '"') {
        lexema += avanzar();
    }

    // Verificar que se encontró la comilla de cierre
    if (actual() == '"') {
        avanzar(); // Saltar la comilla final
        return { TokenType::STRING, lexema, linIn, colIn };
    } else {
        registrarError(lexema, "String mal formado", "Falta comilla de cierre");
        return { TokenType::DESCONOCIDO, lexema, linIn, colIn };
    }
}

Token LexicalAnalyzer::leerNum(){ //LECTURA DE NUMEROS
    int LinIn = linea;
    int ColIn = columna;
    string lexema;
    while (pos < (int)codigo.size() && (isdigit((unsigned char)actual()) || actual() == '-' || actual() == ':')) { //si lee numeros y tienen los siguentes digitos
        lexema += avanzar();
        //Si encuentra que es un digito y contiene los simbolos especiales
    }

    if (lexema.find('-') != string::npos) {
        return { TokenType::DATE, lexema, LinIn, ColIn };
    } else if (lexema.find(':') != string::npos) {
        return { TokenType::HOUR, lexema, LinIn, ColIn };
    } else {
        return { TokenType::NUMERO, lexema, LinIn, ColIn };
    }

}

// ── Tokenización completa ──────────────────────────────────────── //LEE LOS TOKENS
//CREACION DE TOKENS
    vector<Token> LexicalAnalyzer::tokenize() {
        vector<Token> tokens; //invoca al vector de tokens de LexicaAnalyzer
    pos = 0; linea = 1; columna = 1;

    //Limpieza de vectores
    errores.clear();
    medStorage.clear();
    diagStorage.clear();
    patStorage.clear();
    citStorage.clear();
     //Parseo GENERAL
    while (true) {
        Token t = siguienteToken(); //lee os tokens
        if (t.tipo == TokenType::FIN_ARCHIVO) break;
        if (t.tipo != TokenType::DESCONOCIDO)
            tokens.push_back(t);
    }
     //Parseo de Medicos, GENERA LOS TOKENS PARA LOS VECTORES
    int posTok = 0;
    while(posTok < (int)tokens.size()){
        if(tokens[posTok].tipo == TokenType::MEDICOS){
            posTok++;
            registrarMedicos(tokens,posTok);
        }else if(tokens[posTok].tipo == TokenType::PACIENTES){
            posTok++;
            registrarPacientes(tokens,posTok);
        }else if(tokens[posTok].tipo == TokenType::CITAS){
            posTok++;
            registrarCitas(tokens,posTok);
        }else if(tokens[posTok].tipo == TokenType::DIAGNOSTICOS){
            posTok++;
            registrarDiagnosticos(tokens,posTok);
        }else{
            posTok++;
        }

    }

    return tokens;//retorno
}

// -- LECTURA DE DATOS --
        //! REGISTRAR MEDICOS
    void LexicalAnalyzer::registrarMedicos (const vector<Token>& tokens, int& pos){
        qDebug() << "DOCTOR!" ;
        // Verificar que hay una llave de apertura
        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_ABRE) {
            pos++; // Saltar '{'
            // Mientras no lleguemos al cierre de la sección
            while (pos < (int)tokens.size() && tokens[pos].tipo != TokenType::LLAVE_CIERRA) {

                // Buscar la palabra clave "medico"
                if (tokens[pos].tipo == TokenType::medico) {
                    Medic nuevoMedico;
                    pos++; // Saltar "medico"
                    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                        pos++;
                    }
                    // Leer nombre del médico
                    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING) {
                        nuevoMedico.nombre = tokens[pos].lexema;
                        qDebug() << "SE AGREGO NOMBRE";
                        pos++;
                    }
                    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_ABRE) { //CUANDO DETECTE CORCH_ABRE
                        pos++;
                    }

                    while (pos < (int)tokens.size() && tokens[pos].tipo != TokenType::CORCH_CIERRA) { //MIENTRAS NO ENCUENTRE CORCH_CIERRA
                            //RECONOCE EL TIPO DE ESPECIALIDAD
                        if (tokens[pos].tipo == TokenType::especialidad) { //SI ENCUENTRA LA PALABRA RESERVADA
                            pos++; // Saltar

                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                pos++;
                            }

                            // El valor puede ser CARDIOLOGIA, NEUROLOGIA , etc...
                            if (pos < (int)tokens.size() &&
                                tokens[pos].tipo == TokenType::STRING ||
                                tokens[pos].tipo == TokenType::CARDIOLOGIA ||
                                tokens[pos].tipo == TokenType::NEUROLOGIA||
                                tokens[pos].tipo == TokenType::PEDIATRIA||
                                tokens[pos].tipo == TokenType::CIRUGIA||
                                tokens[pos].tipo == TokenType::MEDICINA_GENERAL ||
                                tokens[pos].tipo == TokenType::ONCOLOGIA){
                                nuevoMedico.especialidad = tokens[pos].lexema; //ASIGNA LA ESPECIALIDAD
                                pos++;
                                qDebug() << "SE AGREGO CATEGORIA DE MEDICO";
                            }
                        }
                        else if (tokens[pos].tipo == TokenType::codigoMed) {
                            pos++; // Saltar "codigo"

                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                pos++;
                            }

                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING) {
                                nuevoMedico.codigo = tokens[pos].lexema;
                                qDebug() << "SE AGREGO CODIGO DE MEDICO";
                                pos++;
                            }
                        }
                        else if (tokens[pos].tipo == TokenType::COMA) {
                            pos++; // Saltar comas
                        }
                        else {
                            pos++; // Saltar tokens desconocidos
                        }
                    }

                    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_CIERRA) {
                        pos++;
                    }


                    if (pos < (int)tokens.size() &&
                        (tokens[pos].tipo == TokenType::COMA ||tokens[pos].tipo == TokenType::PUNTO_COMA)) {
                        pos++;
                    }

                    // Agregar Medico al Struct
                    medStorage.push_back(nuevoMedico);
                }
                else {
                    //Sigue la busqueda
                    pos++;
                }
            }


            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_CIERRA) {
                pos++;
            }
        }
        //Console Log para mostrar lo que se Agrego
        qDebug() << "Se agregaron: " << medStorage.size() << " Medicos";
        for(int i = 0; i < medStorage.size(); i++){
            qDebug() << " CODIGO Medico: "<< medStorage[i].codigo ;
            qDebug() << " Nombre Medico: "<< medStorage[i].nombre ;
            qDebug() << " ESPECIALIDAD Medico: "<< medStorage[i].especialidad ;

        }

    }
        //! REGISTRAR Citas
    void LexicalAnalyzer::registrarCitas(const vector<Token>& tokens, int& pos){
        //esperar llave apertura
        //lectura de contenido FUERA DE LLAVES
        if(pos< (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_ABRE){
            pos++;
            //While para Leer hasta que encuentre la llave cerrada

                while(pos < (int)tokens.size() && tokens[pos].tipo != TokenType::LLAVE_CIERRA){
                    //Busca token de cita y lee su contenido
                    if(tokens[pos].tipo == TokenType::cita){
                        Citas newCita; //se declara estructura vacia
                        pos++; //salta token de cita
                        //Si encuentra dos puntos  lo salta
                        if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS){
                                pos++;
                        }

                        //lee el nombre de la cita
                        if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING){
                            newCita.nombre_p = tokens[pos].lexema;
                            qDebug() << "SE AGREGO PACIENTE A LA CITA";
                            pos++;
                        }


                        if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING){
                            newCita.nombre_dr = tokens[pos].lexema;
                            qDebug() << "SE AGREGO DOCTOR A LA CITA";
                            pos++;
                        }

                        //Lectura de contenido DENTRO DE CORCHETES
                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_ABRE) { //CUANDO DETECTE CORCH_ABRE
                            pos++;
                        }
                        //Minetras no encuentre un corchete cerrado
                        while (pos < (int)tokens.size() && tokens[pos].tipo != TokenType::CORCH_CIERRA) {
                            //SI ENCUENTRA EL TOKEN FECHA
                            if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::fecha){
                                pos++; //salta fecha
                                //Si encuentra dos puntos los salta
                                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                    pos++;
                                }
                                //si encuentra la fecha
                                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DATE || tokens[pos].tipo == TokenType::STRING ) {
                                    newCita.fecha = tokens[pos].lexema;
                                    pos++; //salta la coma
                                }

                                //SI ENCUENTRA TOKEN HORA
                            }else if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::hora){
                                pos++;
                                //salto de los dos puntos
                                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                    pos++;
                                }
                                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::HOUR || tokens[pos].tipo == TokenType::STRING ) {
                                    newCita.hora = tokens[pos].lexema;
                                    pos++; //salta la coma
                                }
                            }else if(tokens[pos].tipo == TokenType::COMA){
                                pos++; //SALTO DE COMA
                            }else{
                                pos++; // MANDATORIO
                            }
                        } //FIN WHILE
                        if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_CIERRA) {
                            pos++;
                        }


                        if (pos < (int)tokens.size() &&
                            (tokens[pos].tipo == TokenType::COMA ||tokens[pos].tipo == TokenType::PUNTO_COMA)) {
                            pos++;
                        }

                        //agregar contenido de la cita al vector de lexical analyzer y luego se transfiere al de mainwindow
                        citStorage.push_back(newCita);



                    }//fin if cita
                    else {
                        //Sigue la busqueda
                        pos++;
                    }
                }//Fin While
                if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_CIERRA) {
                    pos++;
                }

        }//Fin Llave Abre

        //DEBUGGER
        qDebug() << "Se agregaron: " << citStorage.size() << " CITAS CON SUS MEDICOS";
        for(int i = 0; i < citStorage.size(); i++){
            qDebug() << " NOMBRE Medico: "<< citStorage[i].nombre_dr ;
            qDebug() << " Nombre Paciente: "<< citStorage[i].nombre_p ;
            qDebug() << " FECHA CITA: "<< citStorage[i].fecha;
            qDebug() << " HORA CITA:" << citStorage[i].hora;
        }//FinFOR



    }
        //! REGISTRAR Pacientes
    void LexicalAnalyzer::registrarPacientes(const vector<Token>& tokens, int& pos){
        //esperar llave apertura
        //lectura de contenido FUERA DE LLAVES
        if(pos< (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_ABRE){
            pos++;
            //While para Leer hasta que encuentre la llave cerrada

            while(pos < (int)tokens.size() && tokens[pos].tipo != TokenType::LLAVE_CIERRA){
                //Busca token de cita y lee su contenido
                if(tokens[pos].tipo == TokenType::paciente){
                    Patient newPacient; //Structura
                    pos++; //salta token de cita
                    //Si encuentra dos puntos  lo salta
                    if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS){
                        pos++;
                    }

                    //lee el nombre del paciente
                    if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING){
                        newPacient.nombre = tokens[pos].lexema;
                        qDebug() << "SE AGREGO PACIENTE";
                        pos++;
                    }


                    //Lectura de contenido DENTRO DE CORCHETES
                    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_ABRE) { //CUANDO DETECTE CORCH_ABRE
                        pos++;
                    }
                    //Minetras no encuentre un corchete cerrado
                    while (pos < (int)tokens.size() && tokens[pos].tipo != TokenType::CORCH_CIERRA) {
                        //SI ENCUENTRA EL TOKEN edad
                        if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::edad){
                            pos++; //salta fecha
                            //Si encuentra dos puntos los salta
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                pos++;
                            }
                            //si encuentra la EDAD
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::NUMERO ) {
                                newPacient.edad = tokens[pos].lexema;
                                pos++; //salta la coma
                            }
                            //SI ENCUENTRA TOKEN haabitacion
                        }else if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::habitacion){
                            pos++;
                            //salto de los dos puntos
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                pos++;
                            }
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::NUMERO ) {
                                newPacient.habita = tokens[pos].lexema;
                                pos++; //salta la coma
                            }
                            //SI ENCUNETRA TOKEN tipo desangre
                        }else if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::tipo_sangre){
                            pos++;
                            //salto de los dos puntos
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                pos++;
                            }
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING) {
                                newPacient.blood = tokens[pos].lexema;
                                pos++; //salta la coma
                            }


                        }else if(tokens[pos].tipo == TokenType::COMA){
                            pos++; //SALTO DE COMA
                        }else{
                            pos++; // MANDATORIO
                        }
                    } //FIN WHILE
                    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_CIERRA) {
                        pos++;
                    }


                    if (pos < (int)tokens.size() &&
                        (tokens[pos].tipo == TokenType::COMA ||tokens[pos].tipo == TokenType::PUNTO_COMA)) {
                        pos++;
                    }

                    //agregar contenido de la cita al vector de lexical analyzer y luego se transfiere al de mainwindow
                    patStorage.push_back(newPacient);
                }//fin if cita
                else {
                    //Sigue la busqueda
                    pos++;
                }
            }//Fin While
            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_CIERRA) {
                pos++;
            }

        }//Fin Llave Abre

        //DEBUGGER
        qDebug() << "Se agregaron: " << patStorage.size() << " PACIENTES";
        for(int i = 0; i < patStorage.size(); i++){
            qDebug() << " NOMBRE PACIENTE: "<< patStorage[i].nombre;
            qDebug() << " EDAD: "<< patStorage[i].edad ;
            qDebug() << " TIPO DE SANGRE: "<< patStorage[i].blood;
            qDebug() << " HABITACION:" << patStorage[i].habita;
        }//FinFOR




    }
        //! REGISTRAR Diagnosticos
    void LexicalAnalyzer::registrarDiagnosticos(const vector<Token>& tokens, int& pos){
        //esperar llave apertura
        //lectura de contenido FUERA DE LLAVES
        if(pos< (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_ABRE){
            pos++;
            //While para Leer hasta que encuentre la llave cerrada

            while(pos < (int)tokens.size() && tokens[pos].tipo != TokenType::LLAVE_CIERRA){
                //Busca token de cita y lee su contenido
                if(tokens[pos].tipo == TokenType::diagnostico){
                    Diagnostic newDiag; //Structura
                    pos++; //salta token de cita
                    //Si encuentra dos puntos  lo salta
                    if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS){
                        pos++;
                    }

                    //lee el diagnostico
                    if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING){
                        newDiag.nombre_p = tokens[pos].lexema;
                        qDebug() << "SE AGREGO PACIENTE AL DIAGNOSTICO";
                        pos++;
                    }
                    //Lectura de contenido DENTRO DE CORCHETES
                    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_ABRE) { //CUANDO DETECTE CORCH_ABRE
                        pos++;
                    }

                    //Minetras no encuentre un corchete cerrado
                    while (pos < (int)tokens.size() && tokens[pos].tipo != TokenType::CORCH_CIERRA) {
                        //SI ENCUENTRA EL TOKEN edad
                        if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::condicion){
                            pos++; //salta condicion
                            //Si encuentra dos puntos los salta
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                pos++;
                            }
                            //si encuentra el String de condicion
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING ) {
                                newDiag.condit = tokens[pos].lexema;
                                qDebug() << "CONDICION DETECTADA PARA" << newDiag.nombre_p << ":" << newDiag.condit;
                                pos++; //salta la coma
                            }
                            //SI ENCUENTRA TOKEN MEDICAMENTO
                        }else if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::medicamento){

                            pos++;
                            //salto de los dos puntos
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                pos++;
                            }
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::STRING) {
                                newDiag.medicamento = tokens[pos].lexema;
                                qDebug() << "MEDICAMENTO DETECTADO";
                                pos++; //salta la coma
                            }
                            //SI ENCUNETRA TOKEN DOSIS
                        }else if(pos < (int)tokens.size() && tokens[pos].tipo == TokenType::dosis){
                            pos++;
                            //salto de los dos puntos
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DOS_PUNTOS) {
                                pos++;
                            }
                            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::DIARIA ||
                                               tokens[pos].tipo == TokenType::CADA_8_HORAS ||
                                                tokens[pos].tipo == TokenType::CADA_12_HORAS ||
                                                tokens[pos].tipo == TokenType::SEMANAL) {

                                newDiag.dosis = tokens[pos].lexema;
                                qDebug() << "DOSIS DETECTADO";
                                pos++; //salta la coma
                            }


                        }else if(tokens[pos].tipo == TokenType::COMA){
                            pos++; //SALTO DE COMA
                        }else{
                            pos++; // MANDATORIO
                        }
                    } //FIN WHILE
                    if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::CORCH_CIERRA) {
                        pos++;
                    }


                    if (pos < (int)tokens.size() &&
                        (tokens[pos].tipo == TokenType::COMA ||tokens[pos].tipo == TokenType::PUNTO_COMA)) {
                        pos++;
                    }

                    //agregar contenido de la cita al vector de lexical analyzer y luego se transfiere al de mainwindow
                    diagStorage.push_back(newDiag);
                }//fin if cita
                else {
                    //Sigue la busqueda
                    pos++;
                }
            }//Fin While
            if (pos < (int)tokens.size() && tokens[pos].tipo == TokenType::LLAVE_CIERRA) {
                pos++;
            }

        }//Fin Llave Abre

        //DEBUGGER
        qDebug() << "Se agregaron: " << diagStorage.size() << " DIAGNOSTICADOS";
        for(int i = 0; i < diagStorage.size(); i++){
            qDebug() << " NOMBRE PACIENTE: "<< diagStorage[i].nombre_p;
            qDebug() << " CONDICION: "<< diagStorage[i].condit ;
            qDebug() << " TRATAMIENTO/MEDICAMENTO: "<< diagStorage[i].medicamento;
            qDebug() << " DOSIS:" << diagStorage[i].dosis;
        }//FinFOR





    }

