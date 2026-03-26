#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QStatusBar>
#include <vector>
#include <string>
#include "LexicalAnalyzer.h"

class MainWindow : public QMainWindow { //Hereda de una ventana principal
    Q_OBJECT //   SEÑAL PARA OTROS QTWIDGETS

public: //DECLARACION DE METODOS PUBLICOS PARA SER LLAMADOS
    explicit MainWindow(QWidget *parent = nullptr); //CONSTRUCTOR
    ~MainWindow(); //DESTRUCTOR (?)

private slots: //todo los metodos que responden eventos
    void cargarArchivo();
    void analizarArchivo();
    void abrirReporte1();
    void abrirReporte2();
    void abrirReporte3();
    void abrirReporte4();
    void abrirReporte5(); //TOKENS


    // RETORNO DE VALORES
    int returnMeds();
    int returnPacient();
    int returnCita();


private:

    //GENERACION
    //CONTENEDOR PARA VECTORES DE LEXICALANALYZER
    vector<Medic> medicos;//MEDICOS
    vector<Patient> pacientes;
    vector<Citas> cita;
    vector<Diagnostic> diagnosticos;
    //SE AGREGARAN MAS EN EL FUTURO


    //REPORTES
    QString clientHist(); //HISTORIAL DE PACIENTES
    QString reportMed(); // EL MEDICO Y SU TIPO DE CARGA
    QString citasReport(); //REPORTE DE CITAS Y POSIBLES CONFLICTOS
    QString hospitalStats(); // REPORTE GENERAL (EASIEST)
    QString lexicalErrors();

    // Panel izquierdo - editor
    QTextEdit    *editorTexto;
    QLabel       *labelArchivo;

    // Tabs de resultados
    QTabWidget   *tabResultados;
    QTableWidget *tablaTokens;
    QTableWidget *tablaErrores;

    // Botones principales
    QPushButton  *btnCargar;
    QPushButton  *btnAnalizar;

    // Botones de reportes
    QPushButton  *btnReporte1;
    QPushButton  *btnReporte2;
    QPushButton  *btnReporte3;
    QPushButton  *btnReporte4;
    QPushButton  *btnReporte5;

    QString archivoActual;

    void setupUI();
    void setupTablaTokens();
    void setupTablaErrores();
    void aplicarEstilos();
    void imprimirConsola();

    //futuros void


};

#endif // MAINWINDOW_H
