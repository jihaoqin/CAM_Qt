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
    StrechLabel.cpp \
    NewCurveTab.cpp \
    GuiConnector.cpp \
    Point.cpp \
    IdGenerator.cpp \
    DataObject.cpp \
    Node.cpp \
    find_polynomial_roots_jenkins_traub.cpp \
    polynomial.cpp \
    RingAssist.cpp \
    numpy.cpp \
    Plane.cpp \
    HalfPoint.cpp \
    TCurve.cpp \
    PosDir.cpp \
    CPPara.cpp \
    RingCurveAssist.cpp \
    Curve.cpp \
    RingCurve.cpp \
    CurveController.cpp \
    Edge.cpp \
    TriEdgePlane.cpp \
    TriEdgePlaneAssist.cpp \
    TriCurveAssist.cpp \
    CylinderAssist.cpp \
    CyCurveAssist.cpp \
    TriEdgePlaneCurve.cpp \
    CylinderCurve.cpp \
    NewCurveController.cpp \
    TCurveAssist.cpp \
    TeeCurveAssist.cpp \
    TeeCurve.cpp \
    TBandOnPoint.cpp \
    Band.cpp \
    PointSym.cpp \
    ObjTreeWidget.cpp \
    ObjModel.cpp \
    TreeItem.cpp \
    LeftCylinderAssist.cpp \
    GeneralBand.cpp \
    GenCurveProgressDialog.cpp \
    OpenBandThread.cpp \
    GenGeneralBandThread.cpp \
    ClosePathThread.cpp \
    ClosePathDialog.cpp \
    AnimateController.cpp \
    SimulationTab.cpp \
    EnvelopAssist.cpp \
    HangingBandSet.cpp \
    FromPipeCurveTab.cpp \
    CBandOnPoint.cpp \
    FromPipeCurveController.cpp

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
    StrechLabel.h \
    numpy.h \
    NewCurveTab.h \
    GuiConnector.h \
    Point.h \
    IdGenerator.h \
    DataObject.h \
    Node.h \
    find_polynomial_roots_jenkins_traub.h \
    polynomial.h \
    RingAssist.h \
    Plane.h \
    HalfPoint.h \
    TCurve.h \
    PosDir.h \
    CPPara.h \
    RingCurveAssist.h \
    Curve.h \
    RingCurve.h \
    CurveController.h \
    Edge.h \
    TriEdgePlane.h \
    TriEdgePlaneAssist.h \
    TriCurveAssist.h \
    CylinderAssist.h \
    CyCurveAssist.h \
    TriEdgePlaneCurve.h \
    CylinderCurve.h \
    NewCurveController.h \
    TCurveAssist.h \
    TeeCurveAssist.h \
    TeeCurve.h \
    TBandOnPoint.h \
    Band.h \
    PointSym.h \
    ObjTreeWidget.h \
    ObjModel.h \
    TreeItem.h \
    LeftCylinderAssist.h \
    GeneralBand.h \
    GenCurveProgressDialog.h \
    OpenBandThread.h \
    GenGeneralBandThread.h \
    ClosePathThread.h \
    ClosePathDialog.h \
    AnimateController.h \
    NameDeclare.h \
    SimulationTab.h \
    EnvelopAssist.h \
    HangingBandSet.h \
    FromPipeCurveTab.h \
    CBandOnPoint.h \
    FromPipeCurveController.h
include ($$PWD/3rdparty/3rdparty.pri)

LIBS += -L$$PWD/3rdparty/libs/ -lassimp-vc140-mt

DEPENDPATH += $$PWD/3rdparty/libs

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    vertexShader.vs \
    3rdparty/3rdparty.pri \
    meshFragShader.fs \
    pointFragShader.fs


# BOOST lib configuration
DEFINES += ROOM
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
