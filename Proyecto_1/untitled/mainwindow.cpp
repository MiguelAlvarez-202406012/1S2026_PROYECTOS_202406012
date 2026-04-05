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
#include <QDateTime>
#include "LexicalAnalyzer.h"
#include <string>
#include <iostream>
#include <set>
#include <QMessageBox> //POPUP
#include <QFile>//GENERAR
#include <QTextStream>//OUTPUT O INPUT
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QRegularExpression>


int conflicts;

//INTERFAZ GRAFICA PARA EL ANALIZADOR
//INTERFAZ INICIAL
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //VENTANA PRINCIPAL
    setWindowTitle("MEDICAL ANALYZER (HUSAC)");
    setFixedSize(720, 460);
    setupUI();
    aplicarEstilos();

    setWindowIcon(QIcon("resources/MED.ico"));


}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    // Widget central
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(central);
    layoutPrincipal->setSpacing(8);
    layoutPrincipal->setContentsMargins(15, 10, 10, 10);
    // ── Barra superior con botones ──────────────────────────────
    QHBoxLayout *barraBotones = new QHBoxLayout();
    btnCargar  = new QPushButton("Cargar Archivo .med");
    btnAnalizar = new QPushButton("Analizar");
    btnAnalizar->setEnabled(false);
    tituloPrograma = new QLabel("ANALIZADOR MEDICO");
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
    btnReporte2 = new QPushButton("Carga\nMedicos"); //DEBUGER
    btnReporte3 = new QPushButton("Agenda\nde Citas");
    btnReporte4 = new QPushButton("Estadistico\nGeneral");
    btnReporte5 = new QPushButton("Errores\nLexicos");

    for (auto *btn : {btnReporte1, btnReporte2, btnReporte3, btnReporte4,btnReporte5}) {
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
    connect(btnReporte5, &QPushButton::clicked, this, &MainWindow::abrirReporte5);

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
            background-color: #CCD3D9;
        }
        QWidget {
            background-color: #FFFFFF;
            color: #000000;
            font-family: 'Segoe UI', sans-serif;
            font-size: 13px;
        }
        QGroupBox {
            border: 1px solid #6CCCEB;
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
            color: #000000;

        }
        QPushButton {
            background-color: #739194;
            color: #FFFFFF;
            border: 1px solid #175A61;
            border-radius: 6px;
            padding: 7px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #10B08B;
            border-color: #185446;
        }
        QPushButton:pressed {
            background-color: #89CDFA;
            color: #000000;
        }
        QPushButton:disabled {
            background-color: #A6BFBD;
            color: #000000;
            border-color: #A6BFBD;
        }
        QTextEdit {
            background-color: #8CABAB;
            border: 1px solid #45475a;
            border-radius: 4px;
            color: #000000;
        }
        QTableWidget {
            background-color: #8CABAB;
            gridline-color: #313244;
            border: 1px solid #45475a;
            border-radius: 4px;
            color: #000000;
        }
        QTableWidget::item:selected {
            background-color: #45475a;
        }
        QHeaderView::section {
            background-color: #31585E;
            color: #000000;
            padding: 6px;
            border: none;
            font-weight: bold;
        }
        QTabWidget::pane {
            border: 1px solid #45475a;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #436D75;
            color: #000000;
            padding: 8px 18px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #8BB0C4;
            color: #1e1e2e;
            font-weight: bold;
        }
        QStatusBar {
            background-color: #D6D6D6;
            color: #000000;
            border-top: 1px solid #1F1F1F;
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

void MainWindow::analizarArchivo() { //ALTERACION
    if (archivoActual.isEmpty()) return;


        /// lectura de contenido
        /// PARA LA LECTURA DEL CONTENIDO
    // Leer contenido del archivo
    std::string contenido = editorTexto->toPlainText().toStdString(); //ruta

    LexicalAnalyzer lexer(contenido);
    std::vector<Token> tokens = lexer.tokenize(); //traer la tokenizacion

    //GUARDAR DE LEXER A MAIN, LECTURA DE VECTORES DE LEXICAL ANALIZER A MAINWINDOW
    medicos = lexer.medStorage; // vector de mainWindow = vector de lexical <- DONDE HAY DATOS
    pacientes = lexer.patStorage; //
    diagnosticos = lexer.diagStorage;
    cita = lexer.citStorage;
    lexicalError = lexer.errores;

    //DEBEN IR EN ORDEN

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

    for (auto *btn : {btnReporte1, btnReporte2, btnReporte3, btnReporte4,btnReporte5})
        btn->setEnabled(true);

    tabResultados->setCurrentIndex(0);
    int nTokens = tokens.size();
    int nErrores = lexer.errores.size();
    statusBar()->showMessage(QString("Analisis completado — %1 tokens, %2 errores léxicos.")
                                 .arg(nTokens).arg(nErrores));

    QMessageBox::information(this, "Lectura compeltada!","Lectura de archivo .med completado!");
}

/////////////////////////////// REPORTE: HIST CLIENTE ///////////////////////////////////////////////////////////////////////////

QString MainWindow::clientHist(){ //CREAR HTML DE HISTORIAL DE PACIENTES
    //SEGUN EL NOMBRE, DIAGNOSTICO Y TODAS ESAS COSAS


    QString html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Historial Pacientes</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 40px;
            background-color: #f5f5f5;
            color: #333;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
        }
        h1 {
            color: #2c3e50;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
        }
        .fecha {
            color: #7f8c8d;
            text-align: right;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th {
            background-color: #239923;
            color: white;
            padding: 12px;
            text-align: left;
        }

        .ACTIVE{
            background-color: #6596D6;
            color: white;
            padding: 12px;
            text-align: left;
        }

        .UNK{
            background-color: #E8C03C;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .CRIT{
            background-color: #FF0000;
            color: white;
            padding: 12px;
            text-align: left;
        }

        td {
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }
        tr:hover {
            background-color: #f5f5f5;
        }

    </style>
</head>
<body>
    <div class="container">
        <h1> HISTORIAL DE CLIENTES </h1>
        <div class="fecha">
            Fecha de generación: )" + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + R"(
        </div>

        <table>
            <thead>
                <tr>
                    <th>Paciente</th>
                    <th>Edad</th>
                    <th>Tipo \n Sangre</th>
                    <th>Diagnostico </th>
                    <th>Medicamento</th>
                    <th>Intervalo de Dosis </th>
                    <th>Estado </th>

                </tr>
            </thead>
        <tbody>
    )"; //tbody donde iran el contenido, Y EN ORDEN DE DECLARACION
    //Iteracion para pacientes
    for (size_t i = 0; i < pacientes.size(); i++) {
        string currentCondition;
        string currentMed;
        string currentDosis;

        //BUSCAR POR COINCIDENCIA

        for (int d = 0; d < diagnosticos.size(); ++d) {
            if(diagnosticos[d].nombre_p == pacientes[i].nombre){
                //MISMO NOMBRE
                 currentCondition = diagnosticos[d].condit;
                 currentMed = diagnosticos[d].medicamento;
                 currentDosis = diagnosticos[d].dosis;
            }
        }



        html += "            <tr>\n";
        html += "            <td>" + QString::fromStdString(pacientes[i].nombre) + "</td>\n"; //NOMBRE
        html += "            <td>" + QString::fromStdString(pacientes[i].edad) + "</td>\n";
        html += "            <td>" + QString::fromStdString(pacientes[i].blood) + "</td>\n";
        //REGISTRAR CONDICIONES
        if(currentCondition.empty() ||currentCondition == ""){
            html += "            <td>" + QString::fromStdString("SIN DIAGNOSTICOS") + "</td>\n";
        }else{
            html += "            <td>" + QString::fromStdString(currentMed) + "</td>\n";
        }

        //REGISTRAR MEDICAMENTOS
        if(currentMed.empty() ||currentMed == ""){
            html += "            <td>" + QString::fromStdString(" - ") + "</td>\n";
        }else{
            html += "            <td>" + QString::fromStdString(currentDosis) + "</td>\n";
        }

        //REGISTRAR INTERVALO
        if(currentDosis.empty() ||currentDosis == ""){
            html += "            <td>" + QString::fromStdString(" - ") + "</td>\n";
        }else{ //POR EL MOMENTO
            html += "            <td>" + QString::fromStdString(currentDosis) + "</td>\n";
        }

        //REGISTRAR ESTADO DE PACIENTE
        if(currentDosis.empty() ||currentDosis == ""){
            html += "            <td class='UNK'>" + QString::fromStdString("SIN DIAGNOSTICOS") + "</td>\n";
        }else if (currentDosis == "DIARIA"){ //POR EL MOMENTO
            html += "            <td class='CRIT'>" + QString::fromStdString("CRITICO") + "</td>\n";
        }else if (currentDosis == "CADA_12_HORAS" ||
                   currentDosis == "CADA_8_HORAS" ||
                   currentDosis == "SEMANAL"){ //POR EL MOMENTO
            html += "            <td class='ACTIVE'>" + QString::fromStdString("ACTIVO") + "</td>\n";
        }


        html += "        </tr>\n";
    }

    html += R"(
            </tbody>
        </table>

    </div>
</body>
</html>
    )";

    return html;


}

void MainWindow::abrirReporte1() { //GENERAR REPORTE PACIENTES
    //REPORTE DE CITAS

    //GENERAR CITAS
    qInfo() << "Generando reporte Historial Pacientes ";

    if (archivoActual.isEmpty()) {
        qWarning() << "ERROR: No hay archivo cargado";
        QMessageBox::warning(this, "Error", "Primero carga un archivo .med");
        return;
    }

    if(!archivoActual.isEmpty()){
        QString ruta = QFileInfo(archivoActual).absolutePath();
        QString path = ruta + "/reporte_historial_pacientes.html"; //Ruta donde se almacenara

        QDir dir(ruta);
        if(!dir.exists()){
            QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
            return; //defunde la fucnbion
        }


        QString htmlContents = clientHist(); //genera el reporte con datos
        QFile archivo(path); //guarda el archivo
        if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&archivo);
            out << htmlContents;
            archivo.close();
            //POPUP
            QMessageBox::information(this, "Historial Pacientes!","Reporte de Historial de Pacientes Generado correctamente correctamente.");

        }else{
            QMessageBox::information(this, "ERROR, Historial Pacientes!","Ocurrio un error al GENERAR el archivo");
        }
    }

}

/////////////////////////////// REPORTE: MEDICOS Y CARGA ///////////////////////////////////////////////////////////////////////////

QString MainWindow::reportMed(){ //GENERAR HTML PARA REPORTE MEDICOS (TEST), USA EL CONTENEDOR DE MAINWINDOW.CPP

//oi quiero descansar
//A USAR SET

    QString html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Reporte de Médicos</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 40px;
            background-color: #f5f5f5;
            color: #333;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
        }
        h1 {
            color: #2c3e50;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
        }
        .fecha {
            color: #7f8c8d;
            text-align: right;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th {
            background-color: #239923;
            color: white;
            padding: 12px;
            text-align: left;
        }

        .low{
            background-color: #6596D6;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .medium{
            background-color: #30BF43;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .high{
            background-color: #E8C03C;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .saturated{
            background-color: #FF0000;
            color: white;
            padding: 12px;
            text-align: left;
        }

        td {
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }
        tr:hover {
            background-color: #f5f5f5;
        }
        .total {
            margin-top: 20px;
            font-weight: bold;
            color: #2c3e50;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1> LISTADO DE MÉDICOS POR ESPECIALIDAD</h1>
        <div class="fecha">
            Fecha de generación: )" + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + R"(
        </div>

        <table>
            <thead>
                <tr>

                    <th>Nombre del Médico</th>
                    <th>Código</th>
                    <th>Especialidad</th>
                    <th>Citas Programadas</th>
                    <th>Pacientes </th>
                    <th>Carga </th>

                </tr>
            </thead>
        <tbody>
    )";
    //Iteracion para medicos
    for (size_t i = 0; i < medicos.size(); i++) {

        //! PARA CADA MEDICO ESTARAN ESTOS DATOS
        int citasProg = 0; //DEFECTO
        string carga_a = ""; //BAJA = 0 ,0 NORMAL = 3 , ALTA = 5 , SATURADA  5<
        set<string> pacientesUnicos; //SET SE
        int pacientesProg = 0; //DEFECTO
        //! PARA CADA MEDICO ESTARAN ESTOS DATOS

        for (size_t p = 0; p < cita.size() ; ++p) { //TERMINA EL RECORRIDO
            //LECTURA PARA REVISAR QUE TIENE MEDICOS
            if(medicos[i].nombre == cita[p].nombre_dr){
                //SI ENCUENTRA UNA COINCIDENCIA
                citasProg++;

                pacientesUnicos.insert(cita[p].nombre_p); //SET ME AHORRO UN MONTON
                //SET ES EL MALDITO GOAT
            }
            //REGISTRAR QUE EL PACIENTE YA HAYA SIDO CONTADO
        }
        pacientesProg = pacientesUnicos.size();
        //AGREGAR A TABLA
        html += "        <tr>\n";
        html += "            <td>" + QString::fromStdString(medicos[i].nombre) + "</td>\n"; //NOMBRE
        html += "            <td>" + QString::fromStdString(medicos[i].codigo) + "</td>\n";
        html += "            <td>" + QString::fromStdString(medicos[i].especialidad) + "</td>\n";
        html += "            <td>" + QString::fromStdString(to_string(citasProg)) + "</td>\n";
        html += "            <td>" + QString::fromStdString(to_string(pacientesProg)) + "</td>\n";

        //NIVEL DE CARGA

        if(citasProg <= 1){
            html += "            <td class='low'>" + QString::fromStdString("BAJA") + "</td>\n";
        }else if(citasProg >= 3 && citasProg < 5){
            html += "            <td class='medium'>" + QString::fromStdString("NORMAL") + "</td>\n";
        }else if(citasProg >= 5 && citasProg < 8){
            html += "            <td class='high'>" + QString::fromStdString("ALTA") + "</td>\n";
        }else if(citasProg > 8){
            html += "            <td class='saturated'>" + QString::fromStdString("SATURADA") + "</td>\n";
        }

        html += "        </tr>\n";
    }

    html += R"(
            </tbody>
        </table>

    </div>
</body>
</html>
    )";

    return html;
}


void MainWindow::abrirReporte2() { //GENERAR MEDS
    /*
     *     QMessageBox::information(this, "DEBUG!", "Se Genero Listado de Medicos!");
    QDesktopServices::openUrl(QUrl::fromLocalFile(
        QFileInfo(archivoActual).absolutePath() + "/reporte_medicos.html"));
    */
    qInfo() << "Generar Reporte Medicos";

    if (archivoActual.isEmpty()) {
        qWarning() << "ERROR: No hay archivo cargado";
        QMessageBox::warning(this, "Error", "Primero carga un archivo .med");
        return;
    }

    if(!archivoActual.isEmpty()){
        QString ruta = QFileInfo(archivoActual).absolutePath();
        QString path = ruta + "/reporte_med.html"; //Ruta donde se almacenara

        QDir dir(ruta);
        if(!dir.exists()){
            QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
            return; //defunde la fucnbion
        }


        QString htmlContents = reportMed(); //genera el reporte con datos
        QFile archivo(path); //guarda el archivo
        if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&archivo);
            out << htmlContents;
            archivo.close();
            //POPUP
            QMessageBox::information(this, "Reporte Medicos!","Reporte de médicos generado correctamente.");

        }else{
            QMessageBox::information(this, "ERROR, Reporte Medicos!","Ocurrio un error al GENERAR el archivo");
        }
    }


}

/////////////////////////////// REPORTE: CITAS ///////////////////////////////////////////////////////////////////////////

QString MainWindow::citasReport(){ //HTML CITAS
     //EN LA GENERACION DE REPORTE SE REALIZARA LA INSPECCION DE SI EXISTEN DATOS QUE COINCIDEN O NO


     QString html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Agenda de Citas</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 40px;
            background-color: #f5f5f5;
            color: #333;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
        }
        h1 {
            color: #2c3e50;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
        }
        .fecha {
            color: #7f8c8d;
            text-align: right;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th {
            background-color: #239923;
            color: white;
            padding: 12px;
            text-align: left;
        }

        .confirm{
            background-color: #239923;
            color: white;
            padding: 12px;
            text-align: left;
        }

        .pending{
            background-color: #E8C03C;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .conflict{
            background-color: #FF0000;
            color: white;
            padding: 12px;
            text-align: left;
        }

        td {
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }
        tr:hover {
            background-color: #f5f5f5;
        }
        .total {
            margin-top: 20px;
            font-weight: bold;
            color: #2c3e50;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1> AGENDA DE CITAS</h1>
        <div class="fecha">
            Fecha de generación: )" + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + R"(
        </div>

        <table>
            <thead>
                <tr>

                    <th>Fecha</th>
                    <th>Hora</th>
                    <th>Paciente</th>
                    <th>Medico \n Encargado</th>
                    <th>Especialidad </th>
                    <th>Estado </th>

                </tr>
            </thead>
        <tbody>
    )";
     //Iteracion para citas
     for (size_t i = 0; i < cita.size(); i++) {

         string especialidad;
         bool hasConflict = false;

         for(size_t d = 0 ; d < medicos.size(); d++){
             if(medicos[d].nombre == cita[i].nombre_dr){
                 especialidad = medicos[d].especialidad;
             }
         }

         //validar conflcitos
         for(size_t c = 0 ; c< cita.size();c++){
             if(i != c){ //compara las demas excepto a la cita actual

                 if(cita[i].nombre_dr == cita[c].nombre_dr &&
                     cita[i].fecha == cita[c].fecha && // RECORRE UN FOR APARTE PARA REVISAR QUE NO HAYAN CONFLCITOS
                     cita[i].hora == cita[c].hora){
                     hasConflict = true;
                     conflicts++;
                     break;
                 }
             }
         }


         //AGREGAR A TABLA
         html += "        <tr>\n";
         html += "            <td>" + QString::fromStdString(cita[i].fecha) + "</td>\n"; //NOMBRE
         html += "            <td>" + QString::fromStdString(cita[i].hora) + "</td>\n";
         html += "            <td>" + QString::fromStdString(cita[i].nombre_p) + "</td>\n";
         html += "            <td>" + QString::fromStdString(cita[i].nombre_dr) + "</td>\n";
         html += "            <td>" + QString::fromStdString(especialidad) + "</td>\n";


         //CITAS CON CONFLICTOS
         if(hasConflict){
             html += "            <td class='conflict'>" + QString::fromStdString("CONFLICTO") + "</td>\n";
         }if(cita[i].fecha.empty() ||
             cita[i].fecha == "" ||
             cita[i].hora == "" ||
             cita[i].hora.empty()){
             html += "            <td class='pending'>" + QString::fromStdString("PENDIENTE") + "</td>\n";
         }else{
             html += "            <td class='confirm'>" + QString::fromStdString("CONFIRMADO") + "</td>\n";
         }


         html += "        </tr>\n";
     }

     html += R"(
            </tbody>
        </table>

    </div>
</body>
</html>
    )";

     return html;


}

void MainWindow::abrirReporte3() { //REPORTE DE CITAS

    //GENERAR CITAS
    qInfo() << "Generando reporte Citas ";

    if (archivoActual.isEmpty()) {
        qWarning() << "ERROR: No hay archivo cargado";
        QMessageBox::warning(this, "Error", "Primero carga un archivo .med");
        return;
    }

    if(!archivoActual.isEmpty()){
        QString ruta = QFileInfo(archivoActual).absolutePath();
        QString path = ruta + "/reporte_citas.html"; //Ruta donde se almacenara

        QDir dir(ruta);
        if(!dir.exists()){
            QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
            return; //defunde la fucnbion
        }


        QString htmlContents = citasReport(); //genera el reporte con datos
        QFile archivo(path); //guarda el archivo
        if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&archivo);
            out << htmlContents;
            archivo.close();
            //POPUP
            QMessageBox::information(this, "Citas!","Reporte de Citas generado correctamente.");

        }else{
            QMessageBox::information(this, "ERROR, Reporte Citas!","Ocurrio un error al GENERAR el archivo");
        }
    }



}

/////////////////////////////// REPORTE: GENERAL HOSPITAL ///////////////////////////////////////////////////////////////////////////

int MainWindow::returnMeds(){
    return medicos.size();
}

int MainWindow::returnPacient(){
    return pacientes.size();
}

int MainWindow::returnCita(){
    return cita.size();
}

int MainWindow::returnDiag(){

    return diagnosticos.size();
}

double MainWindow::returnProm(){
    int edadTotal = 0;

    //recoger edades
    for(int a =0; a< pacientes.size();a++){
        int edadPicker = 0;
        edadPicker = stoi(pacientes[a].edad); //nombre mas pedorro para un string a integer
        edadTotal += edadPicker;
    }

    double prom = edadTotal/pacientes.size();
    return prom;
}

string MainWindow::mayorPreS(){
    string mayorPres;
    int maxFrec = 0;
    //TOCA USAR MAP ME CAGO EN TODO
    if(diagnosticos.empty()){
        return "NO HAY MEDICAMENTOS REGISTRADOS";
    }
    //mapa sin orden
    //Almacena datos sin ordenarlos
    unordered_map<string,int> frecuenciaMed;


    for(int d =0; d<diagnosticos.size();d++){
        if(!diagnosticos[d].medicamento.empty()){ //si no esta vacio el espacio de medicamento
            frecuenciaMed[diagnosticos[d].medicamento]++;
        }
    }

    if(frecuenciaMed.empty()){
        return "NO HAY MEDICAMENTOS REGISTRADOS";
    }

    //foreach para los dos datos

    for(const auto& [medicamento,frecuencia]: frecuenciaMed){
        if(frecuencia > maxFrec){
            maxFrec = frecuencia;
            mayorPres = medicamento;
        }
    }
    return mayorPres + " || Veces Recetada: " + to_string(maxFrec);

}


string MainWindow::mayorCarga(){

    int CARDIOLOGIA;
    int NEUROLOGIA;
    int PEDIATRIA;
    int CIRUGIA;
    int MEDICINA_GENERAL;
    int ONCOLOGIA;


    //REVISAR POR CITAS LUEGO BUSCAR EL NOMBRE DEL DOCTOR Y SU ESPECIALIDAD, ir sumando las especialidades aparte y finalmente comparar cada una la mayor sera la que salga del string;
    for (int cit = 0; cit < cita.size(); ++cit) {
        for(int m = 0; m < medicos.size(); ++m){
            if(medicos[m].nombre == cita[cit].nombre_dr){
            //Si al leer el vector de medicos coincide la cita
            //ACA VIENE UN IF FEO
            if(medicos[m].especialidad == "CARDIOLOGIA")
                CARDIOLOGIA++;
            }else if(medicos[m].especialidad == "NEUROLOGIA"){
                NEUROLOGIA++;
            }else if(medicos[m].especialidad == "PEDIATRIA"){
                PEDIATRIA++;
            }else if(medicos[m].especialidad == "CIRUGIA"){
                CIRUGIA++;
            }else if(medicos[m].especialidad == "MEDICINA_GENERAL"){
                MEDICINA_GENERAL++;
            }else if(medicos[m].especialidad == "ONCOLOGIA"){
                ONCOLOGIA++;
            }
        }
    }
    //FIN DE IF

    if(CARDIOLOGIA > NEUROLOGIA &&
       CARDIOLOGIA > PEDIATRIA &&
        CARDIOLOGIA > CIRUGIA &&
        CARDIOLOGIA > MEDICINA_GENERAL &&
        CARDIOLOGIA > ONCOLOGIA){
        return "CARDIOLOGIA";
    }else if(NEUROLOGIA > CARDIOLOGIA &&
               NEUROLOGIA > PEDIATRIA &&
               NEUROLOGIA > CIRUGIA &&
               NEUROLOGIA > MEDICINA_GENERAL &&
               NEUROLOGIA > ONCOLOGIA){
        return "NEUROLOGIA";
    }else if(PEDIATRIA > CARDIOLOGIA &&
               PEDIATRIA > NEUROLOGIA &&
               PEDIATRIA > CIRUGIA &&
               PEDIATRIA > MEDICINA_GENERAL &&
               PEDIATRIA > ONCOLOGIA){
        return "PEDIATRIA";
    }else if(CIRUGIA > CARDIOLOGIA &&
               CIRUGIA > NEUROLOGIA &&
               CIRUGIA > PEDIATRIA &&
               CIRUGIA > MEDICINA_GENERAL &&
               CIRUGIA > ONCOLOGIA){
        return "CIRUGIA";
    }else if(MEDICINA_GENERAL > CARDIOLOGIA &&
               MEDICINA_GENERAL > NEUROLOGIA &&
               MEDICINA_GENERAL > CIRUGIA &&
               MEDICINA_GENERAL > PEDIATRIA &&
               MEDICINA_GENERAL > ONCOLOGIA){
        return "MEDICINA_GENERAL";
    }else if(ONCOLOGIA > CARDIOLOGIA &&
               ONCOLOGIA > NEUROLOGIA &&
               ONCOLOGIA > CIRUGIA &&
               ONCOLOGIA > MEDICINA_GENERAL &&
               ONCOLOGIA > PEDIATRIA){
        return "ONCOLOGIA";
    }

    return "DESCONOCIDO";
}
QString MainWindow::clearCorrupted(const string& texto){
    QString result = QString::fromStdString(texto);


    result.replace("\"", "\\\"");
    result.replace("\\", "\\\\");
    result.replace("\n", "\\n");
    result.replace("\r", "");
    result.remove(QRegularExpression("[\\x00-\\x1F\\x7F]"));

    return result;

}

QString MainWindow::dotFile(){
    QString dot = "digraph Hospital {\n";
    dot += "    // Configuración global\n";
    dot += "    rankdir=TB;\n";
    dot += "    splines=ortho;\n";
    dot += "    nodesep=0.5;\n";
    dot += "    ranksep=0.8;\n";
    dot += "    node [fontname=\"Arial\", fontsize=10];\n";
    dot += "    edge [fontname=\"Arial\", fontsize=8];\n\n";
    dot += "    // Encoding\n";
    dot += "    charset=\"UTF-8\";\n\n";
    //main
    dot += "    // Nodo principal\n";
    dot += "    Hospital [shape=ellipse, style=filled, fillcolor=lightgreen, fontsize=14];\n\n";
    set<string> especialidades;
    for (const auto& m : medicos) {
        if (!m.especialidad.empty()) {
            especialidades.insert(m.especialidad);
        }
    }

    if (!especialidades.empty()) {
        dot += "    // Especialidades\n";
        dot += "    subgraph cluster_especialidades {\n";
        dot += "        label=\"Especialidades\";\n";
        dot += "        style=filled;\n";
        dot += "        fillcolor=lightgray;\n";
        dot += "        node [shape=ellipse, style=filled, fillcolor=lightyellow];\n\n";

        for (const auto& esp : especialidades) {
            QString espEscapado = clearCorrupted(esp);
            dot += "        \"" + espEscapado + "\";\n";
        }
        dot += "    }\n\n";

        for (const auto& esp : especialidades) {
            QString espEscapado = clearCorrupted(esp);
            dot += "    Hospital -> \"" + espEscapado + "\" [label=\"tiene\"];\n";
        }
        dot += "\n";
    }

    //!medicos
    dot += "    // Médicos\n";
    dot += "    subgraph cluster_medicos {\n";
    dot += "        label=\"Médicos\";\n";
    dot += "        style=filled;\n";
    dot += "        fillcolor=aliceblue;\n";
    dot += "        node [shape=box, style=filled];\n\n";

    //!contar numero de citas
    unordered_map<string, int> citasPorMedico;
    for (const auto& c : cita) {
        citasPorMedico[c.nombre_dr]++;
    }

    for (const auto& m : medicos) {
        int numCitas = citasPorMedico[m.nombre];
        QString color;

        if (numCitas <= 1) {
            color = "lightgreen";
        } else if (numCitas <= 3) {
            color = "lightyellow";
        } else if (numCitas <= 6) {
            color = "orange";
        } else {
            color = "lightcoral";
        }

        QString nombreEscapado = clearCorrupted(m.nombre);
        QString especialidadEscapada = clearCorrupted(m.especialidad);

        dot += "        \"" + nombreEscapado + "\" ";
        dot += "[label=\"" + nombreEscapado + "\\n" + especialidadEscapada +
               "\\n(" + QString::number(numCitas) + " citas)\", ";
        dot += "fillcolor=" + color + "];\n";
    }
    dot += "    }\n\n";

    //!ESPECIALIDADES
    for (const auto& m : medicos) {
        if (!m.especialidad.empty()) {
            QString espEscapado = clearCorrupted(m.especialidad);
            QString nombreEscapado = clearCorrupted(m.nombre);
            dot += "    \"" + espEscapado + "\" -> \"" + nombreEscapado + "\" [label=\"pertenece\"];\n";
        }
    }
    dot += "\n";

    //!PACIENTES
    set<string> pacientesUnicos;
    for (const auto& c : cita) {
        pacientesUnicos.insert(c.nombre_p);
    }

    if (!pacientesUnicos.empty()) {
        dot += "    // Pacientes\n";
        dot += "    subgraph cluster_pacientes {\n";
        dot += "        label=\"Pacientes\";\n";
        dot += "        style=filled;\n";
        dot += "        fillcolor=seashell;\n";
        dot += "        node [shape=ellipse, style=filled, fillcolor=lightgreen];\n\n";

        for (const auto& p : pacientesUnicos) {
            QString pacienteEscapado = clearCorrupted(p);
            dot += "        \"" + pacienteEscapado + "\";\n";
        }
        dot += "    }\n\n";
        for (const auto& c : cita) {
            QString drEscapado = clearCorrupted(c.nombre_dr);
            QString pacienteEscapado = clearCorrupted(c.nombre_p);
            dot += "    \"" + drEscapado + "\" -> \"" + pacienteEscapado + "\" [label=\"cita\"];\n";
        }
        dot += "\n";
    }

    dot += "}\n";

    return dot;
}

QString MainWindow::hospitalStats(){ //GENERACION DE HTML GENERAL HOSPITAL


    QString html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Reporte General</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 40px;
            background-color: #f5f5f5;
            color: #333;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
        }
        h1 {
            color: #2c3e50;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
        }
        .fecha {
            color: #7f8c8d;
            text-align: right;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th {
            background-color: #239923;
            color: white;
            padding: 12px;
            text-align: left;
        }

        .low{
            background-color: #6596D6;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .medium{
            background-color: #30BF43;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .high{
            background-color: #E8C03C;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .saturated{
            background-color: #FF0000;
            color: white;
            padding: 12px;
            text-align: left;
        }

        td {
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }
        tr:hover {
            background-color: #f5f5f5;
        }
        .total {
            margin-top: 20px;
            font-weight: bold;
            color: #2c3e50;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>REPORTE GENERAL HOSPITAL</h1>
        <div class="fecha">
            Fecha de generación: )" + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + R"(
        </div>

        <h2> INDICADOR CALVE DE HOSPITAL</h2>
        <table>
            <thead>
                <tr>

                    <th>Indicador</th>
                    <th>Valor</th>


                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>NOMBRE HOSPITAL</td>
                    <td>)" + "HOSPITAL SAN CARLOS DE GUATEMALA" + R"(</td>
                </tr>

                <tr>
                    <td>PACIENTES REGISTRADOS</td>
                    <td>)" + QString::number(returnPacient()) + R"(</td>
                </tr>
                <tr>
                    <td>MEDICOS REGISTRADOS</td>
                    <td>)" + QString::number(returnMeds()) + R"(</td>
                </tr>
                <tr>
                    <td>CITAS REGISTRADAS</td>
                    <td>)" + QString::number(returnCita()) + R"(</td>
                </tr>
                <tr>
                    <td>CITAS CON CONFLICTOS</td>
                    <td>)" + QString::number(conflicts) + R"(</td>
                </tr>
                <tr>
                    <td>PACIENTES CON DIAGNOSTICOS ACTIVOS</td>
                    <td>)" + QString::number(returnDiag()) + R"(</td>
                </tr>
                <tr>
                    <td>MEDICAMENTO CON MAS PREESCRITURAS</td>
                    <td>)" + QString::fromStdString(mayorPreS()) + R"(</td>
                </tr>
                <tr>
                    <td>ESPECIALIDAD CON MAYOR CARGA DE CITAS</td>
                    <td>)" + QString::fromStdString(mayorCarga()) + R"(</td>
                </tr>
                <tr>
                    <td>EDAD PROMEDIO DE PACIENTES</td>
                    <td>)" + QString::number(returnProm()) + R"(</td>
                </tr>

            </tbody>
        </table>

        <h2> POR ESPECIALIDAD </h2>

        <table>
            <thead>
                <tr>

                    <th>Especialidad</th>
                    <th>Medicos</th>
                    <th>Citas</th>
                    <th>Pacientes</th>
                    <th>Ocupacion </th>

                </tr>
            </thead>
        <tbody>
    )";
    //Contenido General
    //CADA TR es TABLE ROW
    // |_> TD es el contenido que se desplaza ala derecha
    //DEBO ITERAR POR CADA TABLE ROW

    set<string>especialidades;
    //LECTURA POR ESPECIALIDAD

    for(const auto& med : medicos){
        if(!med.especialidad.empty()){ //si en el pivote, encuentra una estructura y especialidad no esta vacio
            especialidades.insert(med.especialidad);
        }

    }
    //LECTURA POR MAP
    unordered_map<string,int>c_medicos; //Almacenan las especialidades reconocidas por el set
    unordered_map<string,int>c_citas;
    unordered_map<string,int>c_pacientes;
    for(const auto& esp_a : especialidades){
        c_medicos[esp_a]=0;
        c_citas[esp_a]=0;
        c_pacientes[esp_a]=0;
        //cada nueva especialidad encontrada sera con valor 0 en cada una
        // si encuentra 9 especialidades, reconocera esas 9 pero actualmente con valor 0
    }
    //Contar medicos por especialidad

    for(const auto& howMed : medicos){
        c_medicos[howMed.especialidad]++;
        //conteo por especialidad
    }

    //contar citas por especialidad

    for(const auto& howCita : cita){ //recorre las citas
        for(const auto& howMed : medicos){ //busca la especialidad
            if(howMed.nombre == howCita.nombre_dr){ //si el nombre del medico coincide con el de la cita
                c_citas[howMed.especialidad]++;
                break;
                //conteo por especialidad
            }
        }
    }

    //contar pacientes por especialidad de medicos quienes atiende
    unordered_map<string, string> med_especial; //
    unordered_map<string, set<string>> patient_especial; //
                //medico como llave,especialidad como valor
    for (const auto& medico : medicos) {
        med_especial[medico.nombre] = medico.especialidad;
        //registra el nombre del medico con su valor de especialidad
        //para cada nombre del medico en el map, se le reconocera su especialidad
    }

    for (const auto& cita : cita) {
        string especialidad = med_especial[cita.nombre_dr]; //trae la especialidad segun el nombre del doctor
        patient_especial[especialidad].insert(cita.nombre_p); //segun la especialidad la inserta a patient_especial
    }

    // Contar
    for (const auto& [esp, pacientesSet] : patient_especial) { //pivotes de paciente especial
        c_pacientes[esp] = pacientesSet.size();
    }



    foreach (const auto& esp, especialidades) {        html += "        <tr>\n";
        string ocupaciones;
        int numMeds = c_medicos[esp];//segun la especialidad muestra el numero que fue aumentando
        int numCitas = c_citas[esp]; //segun la especialidad muestra el numero que fue aumentando
        int numPac = c_pacientes[esp]; //segun la especialidad muestra el numero que fue aumentando

        //Nivel de ocupacion por especialidad
        double occ = (numMeds > 0) ?
        static_cast<double>(numCitas) / numMeds : 0;

        if(occ <= 2){
            ocupaciones = "BAJA";
        }else if(occ <= 5){
            ocupaciones = "MEDIANA";
        }else if(occ <= 7){
            ocupaciones = "ALTA";
        }else if(occ <= 10){
            ocupaciones = "SATURADO";
        }


        html += "        <td>" + QString::fromStdString(esp) + "</td>\n"; //NOMBRE
        html += "        <td>" + QString::number(numMeds) + "</td>\n"; //NOMBRE
        html += "        <td>" + QString::number(numCitas) + "</td>\n"; //NOMBRE
        html += "        <td>" + QString::number(numPac) + "</td>\n"; //NOMBRE
        html += "        <td>" + QString::fromStdString(ocupaciones) + "</td>\n"; //NOMBRE
        html += "        </tr>\n";


    }



    //RESUMIR HTML
    html += R"(
            </tbody>
        </table>

    </div>
</body>
</html>
    )";

    return html;


}

void MainWindow::abrirReporte4() { // REPORTE GENERAL

    //GENERAR REPORTE PACIENTES
    //REPORTE DE CITAS

    //GENERAR REPORE
    qInfo() << "Generando reporte General Hospital + Archivo.dot ";


    if (archivoActual.isEmpty()) {
        qWarning() << "ERROR: No hay archivo cargado";
        QMessageBox::warning(this, "Error", "Primero carga un archivo .med");
        return;
    }

    if(!archivoActual.isEmpty()){

        QString ruta = QFileInfo(archivoActual).absolutePath();
        QString dotPath = ruta + "/jerarquiaHospital.dot"; //Ruta donde se almacenara el archivo.dot
        QString RePath = ruta + "/reporte_g_hospital.html"; //Ruta donde se almacenara reporte de Hospital

        QDir dir(ruta);
        if(!dir.exists()){
            QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
            return; //defunde la fucnbion
        }

        QString dotContents = dotFile();
        QString htmlContents = hospitalStats(); //genera el reporte con datos
        QFile archivo(RePath); //guarda el archivo
        QFile archivoDot(dotPath); //guarda archivo dot
        if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&archivo);
            out << htmlContents;
            archivo.close();
            //POPUP
            QMessageBox::information(this, "Historial Pacientes!","Reporte de Historial de Pacientes Generado correctamente correctamente.");

        }else{
            QMessageBox::information(this, "ERROR, Historial Pacientes!","Ocurrio un error al GENERAR el archivo");
        }

        if(archivoDot.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&archivoDot);
            out << dotContents;
            archivoDot.close();
            //POPUP
            QMessageBox::information(this, "Jerarquia de hospital!","Reporte de Jerarquia de hospital Generado correctamente correctamente.");

        }else{
            QMessageBox::information(this, "ERROR, Jerarquia de hospital!","Ocurrio un error al GENERAR el archivo de \n Jerarquia de hospital ");
        }





    }

}


QString MainWindow::lexicalErrors(){ //GENERACION DE HTML GENERAL HOSPITAL


    QString html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Reporte de Errores Lexicos</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 40px;
            background-color: #f5f5f5;
            color: #333;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            padding: 30px;
        }
        h1 {
            color: #2c3e50;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
        }
        .fecha {
            color: #7f8c8d;
            text-align: right;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th {
            background-color: #611111;
            color: white;
            padding: 12px;
            text-align: left;
        }
        .CRIT{
            background-color: #FF0000;
            color: white;
            padding: 12px;
            text-align: left;

        }


        td {
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }
        tr:hover {
            background-color: #f5f5f5;
        }
        .total {
            margin-top: 20px;
            font-weight: bold;
            color: #2c3e50;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>ERRORES LEXICOS</h1>
        <div class="fecha">
            Fecha de generación: )" + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + R"(
        </div>


        <table>
            <thead>
                <tr>

                    <th>No.</th>
                    <th>Lexema Intro.</th>
                    <th>Error</th>
                    <th>Descripcion</th>
                    <th>Linea </th>
                    <th>Columna </th>
                    <th>Gravedad </th>

                </tr>
            </thead>
        <tbody>
    )";
    //Iteracion para Errores lexicos
    for(int e =0; e< lexicalError.size() ; e++ ){
        html += "            <td>" + QString::number(e + 1) + "</td>\n"; //NUMERO
        html += "            <td>" + QString::fromStdString(lexicalError[e].lexema) + "</td>\n";
        html += "            <td>" + QString::fromStdString(lexicalError[e].tipoError) + "</td>\n";
        html += "            <td>" + QString::fromStdString(lexicalError[e].descripcion) + "</td>\n";
        html += "            <td>" + QString::number(lexicalError[e].linea) + "</td>\n";
        html += "            <td>" + QString::number(lexicalError[e].columna) + "</td>\n";
        html += "            <td>" + QString::fromStdString("CRITICO") + "</td class='CRIT'>\n";
         html += "        </tr>\n";

    }

    html += R"(
            </tbody>
        </table>

    </div>
</body>
</html>
    )";

    return html;


}

void MainWindow::abrirReporte5() { // REPORTE GENERAL

    //GENERAR REPORTE PACIENTES
    //REPORTE DE CITAS

    //GENERAR CITAS
    qInfo() << "Generando reporte de TOKENS";

    if (archivoActual.isEmpty()) {
        qWarning() << "ERROR: No hay archivo cargado";
        QMessageBox::warning(this, "Error", "Primero carga un archivo .med");
        return;
    }

    if(!archivoActual.isEmpty()){
        QString ruta = QFileInfo(archivoActual).absolutePath();
        QString path = ruta + "/reporte_errorLexico.html"; //Ruta donde se almacenara

        QDir dir(ruta);
        if(!dir.exists()){
            QMessageBox::critical(this, "Error", "La ruta: " + ruta + " No existe");
            return; //defunde la fucnbion
        }


        QString htmlContents = lexicalErrors(); //genera el reporte con datos
        QFile archivo(path); //guarda el archivo
        if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&archivo);
            out << htmlContents;
            archivo.close();
            //POPUP
            QMessageBox::information(this, "Historial Pacientes!","Reporte de Historial de Pacientes Generado correctamente correctamente.");

        }else{
            QMessageBox::information(this, "ERROR, Historial Pacientes!","Ocurrio un error al GENERAR el archivo");
        }
    }

}