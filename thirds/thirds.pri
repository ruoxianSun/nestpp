
Qt +=opengl

INCLUDEPATH +=\
    $$PWD\
    $$PWD/glm\
    $$PWD/glew/include\
    $$PWD/qtcss/include
LIBS += \
    -lopengl32
CONFIG(debug,debug|release){
LIBS+=\
   -L$$PWD/qtcss/lib/debug -lQtCss
}
CONFIG(release,debug|release){
LIBS+=\
   -L$$PWD/qtcss/lib/release -lQtCss
}
SOURCES += \
    $$PWD/camera/CCamera.cpp \
    $$PWD/glew/src/glew.c

HEADERS += \
    $$PWD/camera/CCamera.h
