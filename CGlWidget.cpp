#include <GL/glew.h>
#include "CGlWidget.h"
#include <QMouseEvent>
#include "mainwindow.h"
#include "CShaderManager.h"
bool CGlWidget::isInited=false;


CGlWidget::CGlWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

void CGlWidget::addModels(std::vector<std::shared_ptr<CGeometry> > &geoms)
{
    makeCurrent();
    models.clear();
    for(auto geom:geoms)
    {
        std::shared_ptr<CModel3d> render=std::make_shared<CModel3d>();
        render->_data=geom;
        render->_shader=CShaderManager::getShader(QStringList()<<":/cube.vertex"<<":/cube.frag");
        render->_render->setup(render->_data.get());
        render->_color=glm::vec3(0,0,1);
        models.push_back(render);
    }
    doneCurrent();
}

CGlWidget::~CGlWidget(){
    cleanup();
}

void CGlWidget::cleanup()
{
    qDebug()<<"clean up";
    makeCurrent();
    grid.reset();
    doneCurrent();
}


void CGlWidget::initializeGL(){
    qDebug()<<"initializeGL";
//    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &CGlWidget::cleanup);
    glewInit();
    camera=std::make_shared<CCamera>();
    camera->setLookAt({0,0,500},{0,0,0},{0,1,0});
    camera->setViewport({0,0,width(),height()});
    camera->setViewMode(height());
    grid=std::make_shared<CGrid3d>();
    grid->set(100,100,100);
}

void CGlWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    camera->setViewport({0,0,w,h});
    camera->setViewMode(h);
    qDebug()<<"context:"<<context()->isValid();
}

void CGlWidget::paintGL()
{
    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
    grid->render(camera.get(),glm::mat4(1.f),glm::vec3(1,0,0));
    for(auto render:models)
    {
        render->render(camera.get());
    }
    pushMatrix();
    drawLines({{0,100,0},{100,0,0}});
    popMatrix();
    doneCurrent();
}

void CGlWidget::pushMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera->matrixProject()));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera->matrixView()));

}

void CGlWidget::popMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void CGlWidget::drawLines(std::vector<glm::vec3> lines)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glColor3fv(glm::value_ptr(glm::vec3(0,1,0)));
    glLineWidth(2.f);
    glBegin(GL_LINES);
    for(auto v:lines)
    {
        glVertex3fv(glm::value_ptr(v));
    }
    glEnd();
    glPopAttrib();
}

void CGlWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton)
    {
        _lastPos=e->pos();
    }
}

void CGlWidget::mouseMoveEvent(QMouseEvent *e)
{

    if(e->buttons()&Qt::RightButton)
    {
        QPoint move=e->pos()-_lastPos;
        camera->rotateRollCU(move.x());
        camera->rotateRollCR(move.y());
        _lastPos=e->pos();
    }
    update();
}

void CGlWidget::wheelEvent(QWheelEvent *e)
{
    camera->moveAlongEyeRay(e->delta(),{e->x(),e->y()});
    update();
}
