#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QStatusBar>

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

private:
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

    QString archivoActual;

    void setupUI();
    void setupTablaTokens();
    void setupTablaErrores();
    void aplicarEstilos();
};

#endif // MAINWINDOW_H
