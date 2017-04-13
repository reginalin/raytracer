QT += core opengl
QT -= gui

CONFIG += c++11

TARGET = raytracer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    camera.cpp \
    components.cpp \
    ppm_example.cpp \
    tiny_obj_loader.cpp \
    ray.cpp \
    sphere.cpp \
    meshgeom.cpp \
    squareplane.cpp \
    triangle.cpp \
    cube.cpp \
    geometry.cpp \
    intersection.cpp \
    scene.cpp


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include/glm

DISTFILES += \
    cube.json \
    all_shapes.json \
    cornell_box.json \
    wahoo.obj \
    raytracer.pro.user \
    wahoo.ppm \
    .gitignore \
    camera_wahoo.txt \
    helplog.txt \
    wahoo.mtl

HEADERS += \
    camera.h \
    components.h \
    ppm_example.h \
    tiny_obj_loader.h \
    ray.h \
    cube.h \
    geometry.h \
    intersection.h \
    meshgeom.h \
    scene.h \
    sphere.h \
    squareplane.h \
    triangle.h \
    la.h
