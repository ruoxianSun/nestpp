
Qt +=opengl

INCLUDEPATH +=\
    $$PWD\
    $$PWD/glm\
    $$PWD/glew/include
LIBS += \
    -lopengl32
SOURCES += \
    $$PWD/camera/CCamera.cpp \
    $$PWD/glew/src/glew.c

HEADERS += \
    $$PWD/camera/CCamera.h
