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


//TODO LO QUE MANEJARA LA INTERFAZ
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    //FUNCIONES POR EVENTOS POR MEDIO DEL GUI
    void cargarArchivo();
    void analizarArchivo();
    void abrirReporte1();
    void abrirReporte2();
    void abrirReporte3();
    void abrirReporte4();
    void abrirReporte5();


private: //DONDE SE ALMACENARAN TODAS FUNCIONES QUE NO RESPONDEN A EVENTOS POR MEDIO DEL GUI
    //ESTAS FUNCIONES NO RESPONDEN A LOS BOTONES DEL UI, SOLAMENTE FUNCIONAN DE MANERA INTERNA
    Ui::MainWindow *ui; //Puntero a la interfaz generada (NO MOVER)

    //FUNCIONES DE RETORNO
    // RETORNO DE VALORES
    int returnMeds();
    int returnPacient();
    int returnCita();
    int returnDiag();
    double returnProm();
    string mayorCarga();
    string mayorPreS(); //Medicamento con mayor prescritura

    //vectores que utilizara
    vector<Medic> medicos;
    vector<Patient> pacientes;
    vector<Citas>cita;
    vector<Diagnostic> diagnosticos;
    vector<ErrorLexico> errLexico;
    //
    QString archivoActual;
    //REPORTES
    QString clientHist(); //HISTORIAL DE PACIENTES
    QString reportMed(); // EL MEDICO Y SU TIPO DE CARGA
    QString citasReport(); //REPORTE DE CITAS Y POSIBLES CONFLICTOS
    QString hospitalStats(); // REPORTE GENERAL (EASIEST)
    QString lexicalErrors();
    QString dotFile();
    QString clearCorrupted(const string& texto); //limpiar comillas pedorras

    void setDisable();
    void setupTablaTokens();
    void setupTablaErrores();
    void aplicarEstilos(); //ESTILOS
    void imprimirConsola();
    void conectarUI();

};
#endif // MAINWINDOW_H
