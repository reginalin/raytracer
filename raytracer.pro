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
    scene.cpp \
    intersection.cpp \
    cube.cpp \
    geometry.cpp \
    meshgeom.cpp \
    sphere.cpp \
    squareplane.cpp \
    triangle.cpp \
    material.cpp \
    color.cpp



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
    wahoo.mtl \
    text_nor_maps/154.JPG \
    transparent_containing_objects.json \
    sphere.json \
    mesh.json \
    all_shapes_old.json \
    text_nor_maps/154_norm.JPG \
    text_nor_maps/165.JPG \
    text_nor_maps/165_norm.JPG \
    text_nor_maps/171.JPG \
    text_nor_maps/171_norm.JPG \
    text_nor_maps/197.JPG \
    text_nor_maps/197_norm.JPG \
    tex_nor_maps/154.jpg \
    tex_nor_maps/154_norm.jpg \
    tex_nor_maps/165.jpg \
    tex_nor_maps/165_norm.jpg \
    tex_nor_maps/171.jpg \
    tex_nor_maps/171_norm.jpg \
    tex_nor_maps/197.jpg \
    tex_nor_maps/197_norm.jpg

HEADERS += \
    camera.h \
    components.h \
    ppm_example.h \
    tiny_obj_loader.h \
    ray.h \
    scene.h \
    intersection.h \
    cube.h \
    geometry.h \
    meshgeom.h \
    sphere.h \
    squareplane.h \
    triangle.h \
    la.h \
    material.h \
    color.h
