#-------------------------------------------------
#
# Project created by QtCreator 2019-01-03T09:20:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAM_Qt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    glwidget.cpp \
    Mesh.cpp \
    Model.cpp \
    boundingbox.cpp \
    Camera2.cpp \
    PerspectiveMat.cpp \
    utility.cpp \
    Line.cpp \
    GLBinder.cpp \
    Data.cpp \
    Controller.cpp \
    GLMemory.cpp \
    GLProgram.cpp \
    LineLight.cpp \
    Color.cpp \
    Tee.cpp \
    TeeParameterDialog.cpp \
    DataState.cpp \
    Cylinder.cpp \
    Ring.cpp \
    TabWidget.cpp \
    CentralWidget.cpp \
    LeftRibbonLayout.cpp \
    TabBackground.cpp \
    StrechLabel.cpp

HEADERS += \
        mainwindow.h \
    glwidget.h \
    Mesh.h \
    Model.h \
    boundingbox.h \
    Camera2.h \
    PerspectiveMat.h \
    utility.h \
    config.h \
    Line.h \
    GLBinder.h \
    Data.h \
    Controller.h \
    GLMemory.h \
    GLProgram.h \
    vertex.h \
    LineLight.h \
    Color.h \
    Tee.h \
    serialization.h \
    test_only.h \
    TeeParameterDialog.h \
    DataState.h \
    Cylinder.h \
    Ring.h \
    TabWidget.h \
    CentralWidget.h \
    LeftRibbonLayout.h \
    TabBackground.h \
    StrechLabel.h
include ($$PWD/3rdparty/3rdparty.pri)

LIBS += -L$$PWD/3rdparty/libs/ -lassimp-vc140-mt

DEPENDPATH += $$PWD/3rdparty/libs

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    vertexShader.vs \
    loadingModel.fs \
    3rdparty/3rdparty.pri


# BOOST lib configuration
DEFINES += LAB
if(contains(DEFINES, ROOM)){
        BOOST_ROOT = F:\boost_1_69_0
}else{
        BOOST_ROOT = E:\boost_1_69_0
}

INCLUDEPATH += $$BOOST_ROOT
LIBS += -L$$BOOST_ROOT\stage\lib -llibboost_serialization-vc141-mt-gd-x32-1_69 \
        -L$$BOOST_ROOT\stage\lib -llibboost_wserialization-vc141-mt-gd-x32-1_69
DEPENDPATH += $$BOOST_ROOT\stage

RESOURCES += \
    res.qrc
