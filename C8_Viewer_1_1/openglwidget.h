#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <OpenGL/glu.h>
#include <QWheelEvent>
#include <QPushButton>
#include <QColorDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QFile>
#include <QString>
#include <QLabel>

#include <QBuffer>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QPixmap>
#include <QScreen>
#include <QApplication>
#include <qgifimage.h>
#include "./QtGifImage/src/gifimage/qgifimage.h"
#include "./QtGifImage/src/gifimage/qgifglobal.h"

extern "C" {
#include "../headers/struct.h"
#include "../headers/include.h"
#include "../headers/function.h"
#include "../headers/struct.h"
}

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget() override;
    QSettings settings;
    void setData(DATA *data_object);
    void openFile(const QString &filename);

    int frames_counter = 0;
    QTimer *gifTimer;
    QFile *file;
    QGifImage *gif;
    QString *fileName;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void closeEvent(QCloseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onZoomInClicked();
    void onZoomOutClicked();
    void onPointZoomInClicked();
    void onPointZoomOutClicked();

    void checkBrokenLine();
    void checkStraightLine();
    void checkColorLine();
    void checkIncreaseLineWidth();
    void checkDecreaseLineWidth();
    void checkColorVertex();
    void checkSquareVertex();
    void checkRangeVertex();
    void checkBackgroundColor();

    void setOrthographicProjection();
    void setPerspectiveProjection();

    void onMoveXInButtonClicked();
    void onMoveXOutButtonClicked();
    void onMoveYInButtonClicked();
    void onMoveYOutButtonClicked();
    void onMoveZInButtonClicked();
    void onMoveZOutButtonClicked();

    void onRotateXInButtonClicked();
    void onRotateXOutButtonClicked();
    void onRotateYInButtonClicked();
    void onRotateYOutButtonClicked();
    void onRotateZInButtonClicked();
    void onRotateZOutButtonClicked();

    void onScaleInButtonClicked();
    void onScaleOutButtonClicked();

    void onScreenshotButtonClicked();
    void onGifButtonClicked();

    void makeGif();


private:
    DATA *data_object;

    float zoom = 20.0f;
    float pointZoom = 10.0f;
    float lineWidth = 10.0f;

    bool bl;
    bool sl;
    bool sv;
    bool rv;
    bool projection;
    bool rotating;
    bool isGifRecording;

    QPoint lastMousePosition;

    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;
    QPushButton *zoomPointInButton;
    QPushButton *zoomPointOutButton;
    QPushButton *brokenLine;
    QPushButton *straightLine;
    QPushButton *colorLine;
    QPushButton *colorVert;
    QPushButton *increaseLineWidth;
    QPushButton *decreaseLineWidth;
    QPushButton *squareVertex;
    QPushButton *rangeVertex;
    QPushButton *backgroundColorButton;
    QPushButton *perspectiveButton;
    QPushButton *orthoButton;
    QPushButton *moveXInButton;
    QPushButton *moveXOutButton;
    QPushButton *moveYInButton;
    QPushButton *moveYOutButton;
    QPushButton *moveZInButton;
    QPushButton *moveZOutButton;
    QPushButton *rotateXInButton;
    QPushButton *rotateXOutButton;
    QPushButton *rotateYInButton;
    QPushButton *rotateYOutButton;
    QPushButton *rotateZInButton;
    QPushButton *rotateZOutButton;
    QPushButton *scaleInButton;
    QPushButton *scaleOutButton;

    QPushButton *screenshotButton;
    QPushButton *gifButton;

    QColor colorVertex;
    QColor color;
    QColor backgroundColor;

    void saveSetting();
    void unloadSetting();
    void clearData();
    void saveDrawingData(const QString &filename);
    void loadDrawingData(const QString &filename);
    void setProjectionMatrix();
};

#endif // OPENGLWIDGET_H
