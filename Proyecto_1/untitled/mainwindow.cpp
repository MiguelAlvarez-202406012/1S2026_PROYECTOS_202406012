#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QGroupBox>
#include <QHeaderView>
#include <QFont>
#include "LexicalAnalyzer.h"
#include <string>
#include <iostream>



//INTERFAZ GRAFICA PARA EL ANALIZADOR
//INTERFAZ INICIAL
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MED ANALYZER (LEXER)");
    setMinimumSize(1200, 750);
    setupUI();
    aplicarEstilos();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    // Widget central
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout(central);
    layoutPrincipal->setSpacing(8);
    layoutPrincipal->setContentsMargins(10, 10, 10, 10);

    // ── Barra superior con botones ──────────────────────────────
    QHBoxLayout *barraBotones = new QHBoxLayout();

    btnCargar  = new QPushButton("Cargar Archivo .med");
    btnAnalizar = new QPushButton("Analizar");
    btnAnalizar->setEnabled(false);

    labelArchivo = new QLabel("Ningun archivo cargado");
    labelArchivo->setStyleSheet("color: #888; font-style: italic;");

    barraBotones->addWidget(btnCargar);
    barraBotones->addWidget(btnAnalizar);
    barraBotones->addSpacing(20);
    barraBotones->addWidget(labelArchivo);
    barraBotones->addStretch();

    layoutPrincipal->addLayout(barraBotones);

    // ── Splitter principal (editor | resultados) ────────────────
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    // Panel izquierdo: editor de código fuente
    QGroupBox *grupoEditor = new QGroupBox("Codigo Fuente");
    QVBoxLayout *layoutEditor = new QVBoxLayout(grupoEditor);
    editorTexto = new QTextEdit();
    editorTexto->setReadOnly(true);
    editorTexto->setFont(QFont("Courier New", 10));
    editorTexto->setPlaceholderText("Visualizacion del archivo cargado...");
    layoutEditor->addWidget(editorTexto);
    splitter->addWidget(grupoEditor);

    // Panel derecho: tabs de resultados
    QWidget *panelDerecho = new QWidget();
    QVBoxLayout *layoutDerecho = new QVBoxLayout(panelDerecho);
    layoutDerecho->setContentsMargins(0, 0, 0, 0);

    tabResultados = new QTabWidget();
    setupTablaTokens();
    setupTablaErrores();
    tabResultados->addTab(tablaTokens,  "Tabla de Tokens");
    tabResultados->addTab(tablaErrores, "Tabla de Errores");

    layoutDerecho->addWidget(tabResultados);

    // Botones de reportes HTML
    QGroupBox *grupoReportes = new QGroupBox("Reportes HTML");
    QHBoxLayout *layoutReportes = new QHBoxLayout(grupoReportes);

    btnReporte1 = new QPushButton("Historial\nPacientes");
    btnReporte2 = new QPushButton("Carga\nMedicos");
    btnReporte3 = new QPushButton("Agenda\nde Citas");
    btnReporte4 = new QPushButton("Estadistico\nGeneral");

    for (auto *btn : {btnReporte1, btnReporte2, btnReporte3, btnReporte4}) {
        btn->setEnabled(false);
        btn->setMinimumHeight(55);
        layoutReportes->addWidget(btn);
    }

    layoutDerecho->addWidget(grupoReportes);
    splitter->addWidget(panelDerecho);

    // Proporciones del splitter 40% editor / 60% resultados
    splitter->setSizes({480, 720});
    layoutPrincipal->addWidget(splitter);

    // ── Barra de estado ─────────────────────────────────────────
    statusBar()->showMessage("Listo — Carga un archivo .med para comenzar el analisis.");

    // ── Conexiones ───────────────────────────────────────────────
    connect(btnCargar,   &QPushButton::clicked, this, &MainWindow::cargarArchivo);
    connect(btnAnalizar, &QPushButton::clicked, this, &MainWindow::analizarArchivo);
    connect(btnReporte1, &QPushButton::clicked, this, &MainWindow::abrirReporte1);
    connect(btnReporte2, &QPushButton::clicked, this, &MainWindow::abrirReporte2);
    connect(btnReporte3, &QPushButton::clicked, this, &MainWindow::abrirReporte3);
    connect(btnReporte4, &QPushButton::clicked, this, &MainWindow::abrirReporte4);
}

void MainWindow::setupTablaTokens() {
    tablaTokens = new QTableWidget(0, 5);
    tablaTokens->setHorizontalHeaderLabels({"#", "Lexema", "Tipo de Token", "Linea", "Columna"});
    tablaTokens->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tablaTokens->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tablaTokens->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaTokens->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaTokens->setAlternatingRowColors(true);
    tablaTokens->verticalHeader()->setVisible(false);
}

void MainWindow::setupTablaErrores() {
    tablaErrores = new QTableWidget(0, 6);
    tablaErrores->setHorizontalHeaderLabels({"#", "Lexema", "Tipo de Error", "Descripcion", "Linea", "Columna"});
    tablaErrores->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tablaErrores->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaErrores->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaErrores->setAlternatingRowColors(true);
    tablaErrores->verticalHeader()->setVisible(false);
}

void MainWindow::aplicarEstilos() {
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e2e;
        }
        QWidget {
            background-color: #1e1e2e;
            color: #cdd6f4;
            font-family: 'Segoe UI', sans-serif;
            font-size: 13px;
        }
        QGroupBox {
            border: 1px solid #45475a;
            border-radius: 6px;
            margin-top: 8px;
            padding-top: 8px;
            font-weight: bold;
            color: #89b4fa;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 4px;
        }
        QPushButton {
            background-color: #313244;
            color: #cdd6f4;
            border: 1px solid #45475a;
            border-radius: 6px;
            padding: 7px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #45475a;
            border-color: #89b4fa;
        }
        QPushButton:pressed {
            background-color: #89b4fa;
            color: #1e1e2e;
        }
        QPushButton:disabled {
            background-color: #27273a;
            color: #585b70;
            border-color: #313244;
        }
        QTextEdit {
            background-color: #181825;
            border: 1px solid #45475a;
            border-radius: 4px;
            color: #cdd6f4;
        }
        QTableWidget {
            background-color: #181825;
            gridline-color: #313244;
            border: 1px solid #45475a;
            border-radius: 4px;
            color: #cdd6f4;
        }
        QTableWidget::item:selected {
            background-color: #45475a;
        }
        QHeaderView::section {
            background-color: #313244;
            color: #89b4fa;
            padding: 6px;
            border: none;
            font-weight: bold;
        }
        QTabWidget::pane {
            border: 1px solid #45475a;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #313244;
            color: #cdd6f4;
            padding: 8px 18px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #89b4fa;
            color: #1e1e2e;
            font-weight: bold;
        }
        QStatusBar {
            background-color: #181825;
            color: #a6e3a1;
            border-top: 1px solid #313244;
        }
    )");

    // Botón analizar con color destacado
    btnAnalizar->setStyleSheet(R"(
        QPushButton {
            background-color: #a6e3a1;
            color: #1e1e2e;
            border: none;
            border-radius: 6px;
            padding: 7px 20px;
            font-weight: bold;
            font-size: 13px;
        }
        QPushButton:hover { background-color: #94d0a0; }
        QPushButton:disabled { background-color: #27273a; color: #585b70; }
    )");
}

// ── Slots ────────────────────────────────────────────────────────

void MainWindow::cargarArchivo() {
    QString ruta = QFileDialog::getOpenFileName(
        this, "Abrir archivo MedLang", "", "Archivos MedLang (*.med);;Todos los archivos (*)");

    if (ruta.isEmpty()) return;

    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }

    QTextStream in(&archivo);
    editorTexto->setText(in.readAll());
    archivo.close();

    archivoActual = ruta;
    labelArchivo->setText(QFileInfo(ruta).fileName());
    labelArchivo->setStyleSheet("color: #a6e3a1; font-weight: bold;");
    btnAnalizar->setEnabled(true);

    // Limpiar resultados anteriores
    tablaTokens->setRowCount(0);
    tablaErrores->setRowCount(0);
    for (auto *btn : {btnReporte1, btnReporte2, btnReporte3, btnReporte4})
        btn->setEnabled(false);

    statusBar()->showMessage("Archivo cargado: " + ruta + " — Presiona 'Analizar' para comenzar.");
}

void MainWindow::analizarArchivo() {
    cout << "LECTURA COMPLETADA!" << endl;
    if (archivoActual.isEmpty()) return;


        /// lectura de contenido
        /// PARA LA LECTURA DEL CONTENIDO
    // Leer contenido del archivo
    std::string contenido = editorTexto->toPlainText().toStdString(); //ruta

    LexicalAnalyzer lexer(contenido);
    std::vector<Token> tokens = lexer.tokenize(); //traer la tokenizacion

    // Poblar tabla de tokens
    tablaTokens->setRowCount(0);
    for (int i = 0; i < (int)tokens.size(); i++) {
        tablaTokens->insertRow(i);
        tablaTokens->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        tablaTokens->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(tokens[i].lexema)));
        tablaTokens->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tokenTypeToString(tokens[i].tipo))));
        tablaTokens->setItem(i, 3, new QTableWidgetItem(QString::number(tokens[i].linea)));
        tablaTokens->setItem(i, 4, new QTableWidgetItem(QString::number(tokens[i].columna)));
    }

    // Poblar tabla de errores
    tablaErrores->setRowCount(0);
    for (int i = 0; i < (int)lexer.errores.size(); i++) {
        auto& e = lexer.errores[i];
        tablaErrores->insertRow(i);
        tablaErrores->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        tablaErrores->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(e.lexema)));
        tablaErrores->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(e.tipoError)));
        tablaErrores->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(e.descripcion)));
        tablaErrores->setItem(i, 4, new QTableWidgetItem(QString::number(e.linea)));
        tablaErrores->setItem(i, 5, new QTableWidgetItem(QString::number(e.columna)));
    }

    for (auto *btn : {btnReporte1, btnReporte2, btnReporte3, btnReporte4})
        btn->setEnabled(true);

    tabResultados->setCurrentIndex(0);
    int nTokens = tokens.size();
    int nErrores = lexer.errores.size();
    statusBar()->showMessage(QString("Analisis completado — %1 tokens, %2 errores léxicos.")
                                 .arg(nTokens).arg(nErrores));




}

void MainWindow::abrirReporte1() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(
        QFileInfo(archivoActual).absolutePath() + "/reporte_pacientes.html"));
}
void MainWindow::abrirReporte2() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(
        QFileInfo(archivoActual).absolutePath() + "/reporte_medicos.html"));
}
void MainWindow::abrirReporte3() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(
        QFileInfo(archivoActual).absolutePath() + "/reporte_citas.html"));
}
void MainWindow::abrirReporte4() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(
        QFileInfo(archivoActual).absolutePath() + "/reporte_estadistico.html"));
}
