#include "openglwidget.h"
#include <iostream>
#include <QObject>

struct buttonConfig {
    QString text;
    int x, y, width, height;
    const char *slot;
    QString labelText;
    int labelX, labelY;
};


void connectButton(QPushButton *button, const char *signal, const QObject *receiver, const char *method) {
    QObject::connect(button, signal, receiver, method);
}

QPushButton *createButton(int x, int y, int height, int widht, const QString &text, QWidget *parents) {
    QPushButton *button = new QPushButton(text, parents);
    button->setGeometry(x, y, widht, height);
    return button;
}

OpenGLWidget::OpenGLWidget(QWidget *parent)
        : QOpenGLWidget(parent), data_object(nullptr), zoom(25.0f), pointZoom(10.0f), bl(false), sl(true),
          color(Qt::white) {


    QVector<buttonConfig> buttons = {
            {"+",             10,   20,  30, 60,  SLOT(onZoomInClicked()),           "Zoom Camera", 30,   10},
            {"-",             70,   20,  30, 60,  SLOT(onZoomOutClicked()),          "",            0,    0},
            {"+",             10,   60,  30, 60,  SLOT(onPointZoomInClicked()),      "Point Zoom",  35,   50},
            {"-",             70,   60,  30, 60,  SLOT(onPointZoomOutClicked()),     "",            0,    0},
            {"---",           10,   100, 30, 60,  SLOT(checkStraightLine()),         "Line Type",   45,   90},
            {"- -",           70,   100, 30, 60,  SLOT(checkBrokenLine()),           "",            0,    0},
            {"+",             10,   140, 30, 60,  SLOT(checkIncreaseLineWidth()),    "Line Width",  35,   130},
            {"-",             70,   140, 30, 60,  SLOT(checkDecreaseLineWidth()),    "",            0,    0},
            {"Square Vertex", 10,   170, 30, 120, SLOT(checkSquareVertex()),         "",            0,    0},
            {"Range Vertex",  10,   200, 30, 120, SLOT(checkRangeVertex()),          "",            0,    0},
            {"Color Line",    10,   230, 30, 120, SLOT(checkColorLine()),            "",            0,    0},
            {"Color Vertex",  10,   260, 30, 120, SLOT(checkColorVertex()),          "",            0,    0},
            {"BG color",      10,   290, 30, 120, SLOT(checkBackgroundColor()),      "",            0,    0},
            {"Perspective",   10,   320, 30, 120, SLOT(setPerspectiveProjection()),  "",            0,    0},
            {"Ortho",         10,   350, 30, 120, SLOT(setOrthographicProjection()), "",            0,    0},
            {"+",             20,   800, 30, 60,  SLOT(onMoveXInButtonClicked()),    "Translate",   55,   790},
            {"-",             80,   800, 30, 60,  SLOT(onMoveXOutButtonClicked()),   "X",           76,   806},
            {"+",             20,   830, 30, 60,  SLOT(onMoveYInButtonClicked()),    "Y",           76,   836},
            {"-",             80,   830, 30, 60,  SLOT(onMoveYOutButtonClicked()),   "",            0,    0},
            {"+",             20,   860, 30, 60,  SLOT(onMoveZInButtonClicked()),    "Z",           76,   866},
            {"-",             80,   860, 30, 60,  SLOT(onMoveZOutButtonClicked()),   "",            0,    0},
            {"+",             1500, 800, 30, 60,  SLOT(onRotateXInButtonClicked()),  "Rotate",      1540, 790},
            {"-",             1560, 800, 30, 60,  SLOT(onRotateXOutButtonClicked()), "X",           1556, 805},
            {"+",             1500, 830, 30, 60,  SLOT(onRotateYInButtonClicked()),  "",            0,    0},
            {"-",             1560, 830, 30, 60,  SLOT(onRotateYOutButtonClicked()), "Y",           1556, 835},
            {"+",             1500, 860, 30, 60,  SLOT(onRotateZInButtonClicked()),  "Z",           1556, 865},
            {"-",             1560, 860, 30, 60,  SLOT(onRotateZOutButtonClicked()), "",            0,    0},
            {"+",             1500, 20,  30, 60,  SLOT(onScaleInButtonClicked()),    "Scale",       1540, 10},
            {"-",             1560, 20,  30, 60,  SLOT(onScaleOutButtonClicked()),   "",            0,    0},
            {"Screenshot",    10,  380,  30, 120, SLOT(onScreenshotButtonClicked()), "",            0,    0},
            {"GIF",           10,  410,  30, 120, SLOT(onGifButtonClicked()),        "",            0,    0},
    };
    for (const buttonConfig &config: buttons) {
        QPushButton *button = createButton(config.x, config.y, config.width, config.height, config.text, this);
        connectButton(button, SIGNAL(clicked()), this, config.slot);
        QLabel *label = new QLabel(config.labelText, this);
        label->move(config.labelX, config.labelY);
        label->show();
    }
    unloadSetting();
}

OpenGLWidget::~OpenGLWidget() {
    saveSetting();
    clearData();
}

void OpenGLWidget::unloadSetting() {
    backgroundColor = settings.value("BG color", QColor(Qt::black)).value<QColor>();
    color = settings.value("Color", QColor(Qt::white)).value<QColor>();
    colorVertex = settings.value("Color Vertex", QColor(Qt::white)).value<QColor>();
    lineWidth = settings.value("lineWidth", 1.0f).toFloat();
    zoom = settings.value("Zoom", 25.0f).toFloat();
    pointZoom = settings.value("pointZoom", 10.0f).toFloat();
    sl = settings.value("straightLine").toBool();
    bl = settings.value("brokeLine").toBool();
    rv = settings.value("rangeVertex").toBool();
    sv = settings.value("squareVertex").toBool();
    projection = settings.value("projection").toBool();
}

void OpenGLWidget::saveSetting() {
    settings.setValue("BG color", backgroundColor);
    settings.setValue("Color Vertex", colorVertex);
    settings.setValue("Color", color);
    settings.setValue("pointZoom", pointZoom);
    settings.setValue("Zoom", zoom);
    settings.setValue("lineWidth", lineWidth);
    settings.setValue("straightLine", sl);
    settings.setValue("brokeLine", bl);
    settings.setValue("rangeVertex", rv);
    settings.setValue("squareVertex", sv);
    settings.setValue("projection", projection);
}

void OpenGLWidget::onScreenshotButtonClicked() {

    QDir desktopDir (QDir::home().absoluteFilePath("Desktop"));
    QDir screenshotDir(desktopDir.absoluteFilePath("3D_Viewer_Screenshots_"));
    if(!screenshotDir.exists()){
        if (!screenshotDir.mkpath(".")){
            qDebug() << "Failed to create the directory";
            return;
        }
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QPixmap screenshot;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
           screenshot = screen->grabWindow(QApplication::activeWindow()->winId());
     }
    QString bmpFilePath = screenshotDir.absoluteFilePath(timestamp + ".bmp");
    if(!screenshot.save(bmpFilePath, "BMP")){
        qDebug() << "Failed to save screenshot in BMP format";
    }
    QString jpegFilePath = screenshotDir.absoluteFilePath(timestamp + ".jpg");
    if(!screenshot.save(jpegFilePath, "JPEG")){
        qDebug() << "Failed to create screenshot in BMP format";
    }
}

void OpenGLWidget::onGifButtonClicked(){
    if (isGifRecording) {
        qDebug() << "GIF recording is already in progress.";
        return;
    }

    isGifRecording = true;

    QDir desktopDir (QDir::home().absoluteFilePath("Desktop"));
    QDir gifDir(desktopDir.absoluteFilePath("3D_Viewer_GIF_"));
    QString giftimestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    if(!gifDir.exists()){
        if (!gifDir.mkpath(".")){
            qDebug() << "Failed to create the directory";
            return;
        }
    }
    fileName = new QString;
    QFileInfo fileInfo(gifDir, giftimestamp + ".gif");
     *fileName = fileInfo.absoluteFilePath();

    gif = new QGifImage(QSize(640, 480));

    gif->setDefaultDelay(1);
    gif->setLoopCount(-1);

    gifTimer = new QTimer();
    connect(gifTimer, SIGNAL(timeout()), this, SLOT(makeGif()));
    gifTimer->start(100);
    frames_counter = 0;

}

void OpenGLWidget::makeGif(){
    QImage shot;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen && QApplication::activeWindow()) {
        QPixmap pix = screen->grabWindow(QApplication::activeWindow()->winId());
        shot = pix.toImage().scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        gif->addFrame(shot);
        frames_counter++;
  }
    if(frames_counter == 50){
        gifTimer->stop();
        isGifRecording = false;

        if(fileName != nullptr){
            QBuffer buffer;
            buffer.open(QIODevice::WriteOnly);
            gif->save(&buffer);
            QFile file(*fileName);
            file.open(QIODevice::WriteOnly);
            file.write(buffer.data());
            file.close();
            delete gif;
            delete gifTimer;
            delete fileName;
            buffer.close();
            fileName = nullptr;
        }
    }
}



void OpenGLWidget::setPerspectiveProjection() {
    projection = true;
    update();
}

void OpenGLWidget::setOrthographicProjection() {
    projection = false;
    update();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, width() / height(), -200.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, zoom,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    setProjectionMatrix();
}

void OpenGLWidget::checkBackgroundColor() {
    QColor newBackgroundColor = QColorDialog::getColor(backgroundColor, this);
    if (newBackgroundColor.isValid()) {
        backgroundColor = newBackgroundColor;
        update();
    }
}

void OpenGLWidget::checkColorVertex() {
    QColor newColorVertex = QColorDialog::getColor(colorVertex, this);
    if (newColorVertex.isValid()) {
        colorVertex = newColorVertex;
        update();
    }
}

void OpenGLWidget::checkIncreaseLineWidth() {
    lineWidth += 1.5f;
    if (lineWidth > 20) lineWidth = 20;
    update();
}

void OpenGLWidget::checkDecreaseLineWidth() {
    lineWidth -= 1.5f;
    if (lineWidth < 1.0) lineWidth = 1.0f;
    update();
}

void OpenGLWidget::checkColorLine() {
    QColor newColor = QColorDialog::getColor(color, this);
    if (newColor.isValid()) {
        color = newColor;
        update();
    }
}

void OpenGLWidget::checkSquareVertex() {
    sv = true;
    rv = false;
    update();
}

void OpenGLWidget::checkRangeVertex() {
    sv = false;
    rv = true;
    update();
}

void OpenGLWidget::checkBrokenLine() {
    bl = true;
    sl = false;
    update();
}

void OpenGLWidget::checkStraightLine() {
    bl = false;
    sl = true;
    update();
}

void OpenGLWidget::onZoomInClicked() {
    zoom -= 2.0f;
    if (zoom < 1.0f) zoom = 1.0f;
    update();
}

void OpenGLWidget::onZoomOutClicked() {
    zoom += 2.0f;
    update();
}

void OpenGLWidget::onPointZoomOutClicked() {
    pointZoom -= 2.0f;
    if (pointZoom < 1.0f) pointZoom = 1.0f;
    update();
}

void OpenGLWidget::onPointZoomInClicked() {
    pointZoom += 2.0f;
    if (pointZoom > 20) pointZoom = 20;
    update();
}

void OpenGLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    setProjectionMatrix();
}

void OpenGLWidget::setData(DATA *data_object) {
    clearData();
    this->data_object = data_object;
    update();
}

void OpenGLWidget::closeEvent(QCloseEvent *event) {
    saveSetting();
    QOpenGLWidget::closeEvent(event);
}

void OpenGLWidget::setProjectionMatrix() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble zNear = 0.01;
    GLdouble zFar = 350.0;
    if (!projection) {
        GLdouble fovy = 75.0;
        GLdouble aspect = static_cast<GLdouble>(width()) / height();
        GLdouble fovyRad = fovy * M_PI / 180.0;
        GLdouble top = zNear * tan(fovyRad / 2.0);
        GLdouble bottom = -top;
        GLdouble right = top * aspect;
        GLdouble left = -right;
        glFrustum(left, right, bottom, top, zNear, zFar);
    } else {
        GLdouble orthoSize = 15.0;
        GLdouble aspect = static_cast<GLdouble>(width()) / height();
        glOrtho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, zNear, zFar);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void OpenGLWidget::paintGL() {
    glClearColor(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setProjectionMatrix();
    gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    if (data_object) {
        glClearColor(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), 1.0f);
        glPointSize(pointZoom);
        glBegin(GL_POINTS);
        glColor3f(colorVertex.redF(), colorVertex.greenF(), colorVertex.blueF());
        for (int i = 0; i < data_object->object_matrix.rows - 1; ++i) {
            glVertex3f(data_object->object_matrix.matrix[i][0], data_object->object_matrix.matrix[i][1],
                       data_object->object_matrix.matrix[i][2]);
        }
        glEnd();
        glColor3f(color.redF(), color.greenF(), color.blueF());
        glLineWidth(lineWidth);
        if (bl) {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x00FF);
        } else {
            glDisable(GL_LINE_STIPPLE);
        }
        if (sv) {
            glEnable(GL_POINT_SMOOTH);
        } else {
            glDisable(GL_POINT_SMOOTH);
        }
        int c = 0, count = 0, count_facets_in_line = 0;
        glBegin(GL_LINES);
        while (data_object->object_polygon->count_facets_in_line[count_facets_in_line] != 0) {
            int num_vertices_in_facet = data_object->object_polygon->count_facets_in_line[count_facets_in_line];
            for (int i = 0; i < num_vertices_in_facet; ++i) {
                int v1 = 0, v2 = 0;
                v1 = data_object->object_polygon->vertexes[(c + i)] - 1;
                v2 = data_object->object_polygon->vertexes[(c + (i + 1) % num_vertices_in_facet)] - 1;
                    glVertex3f(
                            data_object->object_matrix.matrix[v1][0],
                            data_object->object_matrix.matrix[v1][1],
                            data_object->object_matrix.matrix[v1][2]
                    );

                    glVertex3f(
                            data_object->object_matrix.matrix[v2][0],
                            data_object->object_matrix.matrix[v2][1],
                            data_object->object_matrix.matrix[v2][2]
                    );
            }
            c += num_vertices_in_facet;
            count_facets_in_line++;
        }
        glEnd();
        if (bl) {
            glDisable(GL_LINE_STIPPLE);
        }
    }
}


void OpenGLWidget::onMoveXInButtonClicked() {
    if (data_object) {
        move_affin(&data_object->object_matrix, 1.0, 0.0, 0.0);
        update();
    }
}

void OpenGLWidget::onMoveXOutButtonClicked() {
    if (data_object) {
        move_affin(&data_object->object_matrix, -1.0, 0.0, 0.0);
        update();
    }
}

void OpenGLWidget::onMoveYInButtonClicked() {
    if (data_object) {
        move_affin(&data_object->object_matrix, 0.0, 1.0, 0.0);
        update();
    }
}

void OpenGLWidget::onMoveYOutButtonClicked() {
    if (data_object) {
        move_affin(&data_object->object_matrix, 0.0, -1.0, 0.0);
        update();
    }
}

void OpenGLWidget::onMoveZInButtonClicked() {
    if (data_object) {
        move_affin(&data_object->object_matrix, 0.0, 0.0, 1.0);
        update();
    }
}

void OpenGLWidget::onMoveZOutButtonClicked() {
    if (data_object) {
        move_affin(&data_object->object_matrix, 0.0, 0.0, -1.0);
        update();
    }
}

void OpenGLWidget::onRotateXInButtonClicked() {
    if (data_object) {
        rotation_by_OX(&data_object->object_matrix, 1.0);
        update();
    }
}

void OpenGLWidget::onRotateXOutButtonClicked() {
    if (data_object) {
        rotation_by_OX(&data_object->object_matrix, -1.0);
        update();
    }
}

void OpenGLWidget::onRotateYInButtonClicked() {
    if (data_object) {
        rotation_by_OY(&data_object->object_matrix, 1.0);
        update();
    }
}

void OpenGLWidget::onRotateYOutButtonClicked() {
    if (data_object) {
        rotation_by_OY(&data_object->object_matrix, -1.0);
        update();
    }
}

void OpenGLWidget::onRotateZInButtonClicked() {
    if (data_object) {
        rotation_by_OZ(&data_object->object_matrix, 1.0);
        update();
    }
}

void OpenGLWidget::onRotateZOutButtonClicked() {
    if (data_object) {
        rotation_by_OZ(&data_object->object_matrix, -1.0);
        update();
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && data_object) {
        lastMousePosition = event->pos();
        rotating = true;
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {

    if (rotating && data_object) {
        QPoint currentMousePosition = event->pos();
        int dx = currentMousePosition.x() - lastMousePosition.x();
        int dy = currentMousePosition.y() - lastMousePosition.y();

        if (event->buttons() & Qt::LeftButton) {
            rotation_by_OX(&data_object->object_matrix, dy * 0.01);
            rotation_by_OY(&data_object->object_matrix, dx * 0.01);
        } else if (event->buttons() & Qt::RightButton) {
            rotation_by_OZ(&data_object->object_matrix, dx * 0.01);
        }

        lastMousePosition = currentMousePosition;
        update();
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        rotating = false;
    }
}

void OpenGLWidget::onScaleInButtonClicked() {
    if (data_object) {
        scale_affine(&data_object->object_matrix, 1.1, 1.1, 1.1);
        update();
    }
}

void OpenGLWidget::onScaleOutButtonClicked() {
    if (data_object) {
        scale_affine(&data_object->object_matrix, 0.9, 0.9, 0.9);
        update();
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
    int delta = event->angleDelta().y();
    if (delta > 0) {
        onScaleInButtonClicked();
    } else {
        onScaleOutButtonClicked();
    }
    event->accept();
}

void OpenGLWidget::clearData() {
    if (data_object == nullptr) {
        return;
    }
    if (data_object->object_matrix.matrix != nullptr) {
        for (int i = 0; i < data_object->object_matrix.rows; ++i) {
            delete[] data_object->object_matrix.matrix[i];
        }
        delete[] data_object->object_matrix.matrix;
        data_object->object_matrix.matrix = nullptr;
    }
    if (data_object->object_polygon) {
        delete[] data_object->object_polygon->vertexes;
        delete[] data_object->object_polygon->count_facets_in_line;
        delete data_object->object_polygon;
        data_object->object_polygon = nullptr;
    }
    delete data_object;
}


