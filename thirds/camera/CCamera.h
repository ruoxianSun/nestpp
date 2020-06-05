#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <QFile>

namespace sgl
{
using Ray3d = typename std::array<glm::vec3,2>;
using Plane3d= Ray3d;
static glm::vec3 intersectPlane(Plane3d plane,Ray3d ray);
class CCamera
{
public:
    enum shapeType { Perspective, Ortho };
    CCamera();
    ~CCamera();
    void setLookAt(glm::vec3 eye, glm::vec3 look, glm::vec3 up);
    float getDist();
    void setViewport(glm::vec4 vp){_viewport=vp;_aspect=vp[2]/vp[3];}
    glm::vec4 viewport(){return _viewport;}
    void setViewMode(float viewAngle, float Near, float Far);
    void setViewMode(float left, float right, float bottom, float top, float near, float far);
    void setProject(float left, float right, float bottom, float top, float near, float far);
    void rotateRollCU(double deg);
    void rotateRollCR(double deg);
    void moveAlongCR(float fUnits);
    void moveAlongCU(float fUnits);
    void moveAlongCD(float fUnits);
    void moveAlongEyeRay(float d,glm::vec2 pos);
    glm::vec3 eye() { return _eye; }
    glm::vec3 look() { return _look; }
    float aspect() { return _aspect; }
    float viewAngle() { return _viewAngle; }
    glm::vec3 cameraRight() { return _cameraRight; }
    glm::vec3 cameraUp() { return _cameraUp; }
    glm::vec3 cameraDirection() { return _cameraDirection; }

    Ray3d getCameraRay(glm::vec2 p, glm::vec4 viewport);
    glm::vec2 getScreenPos(glm::vec3 center,glm::mat4 model,glm::vec4 viewport);
    glm::vec3 getOrthoPos(float sx, float sy);
    glm::vec2 getOffsetInCameraClipZero(glm::vec2 s, glm::vec2 e, glm::vec4 viewport);
    glm::mat4 getOrthoMatrix(glm::vec4 viewport);
    void pushStat(glm::mat4 project,shapeType type);
    void popStat();
private:
    void setViewMatrix();
public:
    void dumpCameraInfo(const QString&name);
    glm::mat4 matrixView(){return _view;}
    glm::mat4 matrixProject(){return _project.back();}
public:
    glm::mat4     _view;
    std::vector<glm::mat4> _project;
    glm::vec3      _eye,_look, _up;
    glm::vec4 _viewport;
    std::vector<shapeType> _type;
    float          _viewAngle, _aspect, _nearDist, _farDist,_viewHeight;
private:
    glm::vec3      _cameraUp,_cameraRight,_cameraDirection;
    float          _minLen, _maxLen;
};
}

