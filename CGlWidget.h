#ifndef CGLWIDGET_H
#define CGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <camera/CCamera.h>
#include "CModel3d.h"
using namespace sgl;
class CGlWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit CGlWidget(QWidget *parent = nullptr);
    void addModels(std::vector<std::shared_ptr<CGeom>> &geoms);
    ~CGlWidget();
signals:
public slots:
    void cleanup();
protected:
    void initializeGL();
    void resizeGL(int w,int h);
    void paintGL();
    void pushMatrix();
    void popMatrix();
    void drawLines(std::vector<glm::vec3> lines);
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent*e);
    void wheelEvent(QWheelEvent*e);
protected:
    std::shared_ptr<CCamera> camera;
    QPoint _lastPos;
    std::shared_ptr<CGrid3d> grid;
    static bool isInited;
    std::vector<std::shared_ptr<CObj>> models;
};

#endif // CGLWIDGET_H
