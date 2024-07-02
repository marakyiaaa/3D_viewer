QT       += core gui openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(QtGifImage/src/gifimage/qtgifimage.pri)

SOURCES += \
    ../Backend/CountVertexAndFacets.c \
    ../Backend/CreateMatrix.c \
    ../Backend/FreePolygon.c \
    ../Backend/ParsingFile.c \
    ../Backend/affine_transfer.c \
    main.cpp \
    mainwindow.cpp \
    overrideOpenglwidget.cpp

HEADERS += \
    ../headers/define.h \
    ../headers/function.h \
    ../headers/include.h \
    ../headers/struct.h \
    mainwindow.h \
    openglwidget.h \
    gifanimation.h

FORMS += \
    mainwindow.ui

#LIBS += -lGLU

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    apple.obj \
    cube.obj
