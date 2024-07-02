#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "openglwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    OpenGLWidget* getOpenGLWidget() const;

private slots:
    void onOpenFileButtonClicked();

private:
    Ui::MainWindow *ui;
    OpenGLWidget *openGLWidget;
    QToolBar *mainToolBar;
    QLabel *fileName;
    QLabel *countVertex;
    QLabel *countFacets;
    void setFileName(const QString &fileName);
    void setVertex(int countVertex);
    void setFacets(int countVertex);
};

#endif // MAINWINDOW_H
