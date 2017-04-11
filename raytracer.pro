QT += core
QT -= gui

CONFIG += c++11

TARGET = raytracer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    camera.cpp \
    components.cpp \
    mat4.cpp \
    ppm_example.cpp \
    tiny_obj_loader.cpp \
    vec4.cpp \
    ray.cpp \
<<<<<<< HEAD
    intersection.cpp
=======
    scene.cpp
>>>>>>> 3f354c3a70be2a8447dabbbb5bd72931adfba91d

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DISTFILES += \
    cube.json \
    all_shapes.json \
    cornell_box.json

HEADERS += \
    camera.h \
    components.h \
    mat4.h \
    ppm_example.h \
    tiny_obj_loader.h \
    vec4.h \
    ray.h \
<<<<<<< HEAD
    intersection.h
=======
    scene.h
>>>>>>> 3f354c3a70be2a8447dabbbb5bd72931adfba91d
