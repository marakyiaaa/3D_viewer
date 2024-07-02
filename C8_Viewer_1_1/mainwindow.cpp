#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>
#include <QVBoxLayout>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      openGLWidget(new OpenGLWidget(this)),
      fileName(new QLabel(this)),
      countVertex(new QLabel(this)),
      countFacets(new QLabel(this))
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    fileName->setAlignment(Qt::AlignCenter);
    fileName->setFixedHeight(15);

    countVertex->setAlignment(Qt::AlignLeft);
    countVertex->setFixedHeight(15);

    countFacets->setAlignment(Qt::AlignRight);
    countFacets->setFixedHeight(15);


    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(countVertex);
    infoLayout->addStretch();
    infoLayout->addWidget(countFacets);

    mainToolBar = new QToolBar(this);
    QPushButton *openFileButton = new QPushButton("Open File", this);
    mainToolBar->addWidget(openFileButton);
    connect(openFileButton, &QPushButton::clicked, this, &MainWindow::onOpenFileButtonClicked);
    layout->addWidget(mainToolBar);
    layout->addWidget(fileName);
    layout->addLayout(infoLayout);
    layout->addWidget(openGLWidget);

    setCentralWidget(centralWidget);

    setWindowTitle("3D Viewer");
    resize(1700, 1300);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete openGLWidget;
    delete mainToolBar;
}

OpenGLWidget* MainWindow::getOpenGLWidget() const {
    return openGLWidget;
}

void MainWindow::setFileName(const QString &filename) {
    QFileInfo file(filename);
    fileName->setText(file.fileName());
}

void MainWindow::setVertex(int countVertex2){
    countVertex->setText("Кол-во вершин : " + QString::number(countVertex2));
}

void MainWindow::setFacets(int countFacets2){
    countFacets->setText("Кол-во граней : " + QString::number(countFacets2));
}

void MainWindow::onOpenFileButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("OBJ Files (*.obj)"));
    if (!fileName.isEmpty()) {
        QByteArray byteArray = fileName.toUtf8();
        char *filename = byteArray.data();

        int check = 0;

        DATA *data_object = new DATA{0, 0, {nullptr, 0, 0}, nullptr};
        int status = CountVertexAndFacets(filename, data_object);
        CreateMatrix(data_object, &check);
        check = ParsingFile(filename, data_object);
        if (check == 1) {
            getOpenGLWidget()->setData(data_object);
            getOpenGLWidget()->update();
            setFileName(fileName);
            setVertex(data_object->object_matrix.rows - 1);
            setFacets(data_object->count_of_facets);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Failed."));
            delete data_object;
        }
    }
}
