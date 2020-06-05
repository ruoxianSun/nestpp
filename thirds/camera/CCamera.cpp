#include "CCamera.h"
#include <iostream>
#include <functional>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
using namespace std;
#define OUTINFO_2_PARAM(fmt,var1,var2) {CHAR sOut[256];CHAR sfmt[50];sprintf_s(sfmt,"%s%s","INFO--",fmt);sprintf_s(sOut,(sfmt),var1,var2);OutputDebugStringA(sOut);}

namespace sgl
{
    /* 构造函数 */
    CCamera::CCamera()
    {
        _eye = glm::vec3(0, 0, 0);
        _look = glm::vec3(0,0,0);
        _up = glm::vec3();
        _cameraRight = glm::vec3(1.0,0.0,0.0);
        _cameraUp = glm::vec3(0.0, 0.0, 1.0);
        _cameraDirection = glm::vec3(0.0,1.0, 0.0);
        _minLen = 60;
        _maxLen = 800;
    }

    CCamera::~CCamera()
    {
    }

    void CCamera::setLookAt(glm::vec3 eye, glm::vec3 look, glm::vec3 up)
    {
        _eye = glm::vec3(eye);
        _look = glm::vec3(look);
        _up = glm::vec3(up);
        glm::vec3 dirvec = _eye-_look;
        _cameraRight = glm::vec3(glm::cross(_up, dirvec));
        _cameraUp = glm::vec3(glm::cross(dirvec, _cameraRight));
        _cameraDirection=dirvec;
        setViewMatrix();

    }
    float CCamera::getDist()
    {
        return glm::length(_eye);
    }

    void CCamera::setViewMode(float viewAngle, float Near, float Far)
    {
        _viewAngle = viewAngle;
        _nearDist = Near;
        _farDist = Far;
        _project.push_back(glm::perspective(glm::radians(viewAngle), _aspect, Near, Far));
        _type.push_back(shapeType::Perspective);
    }





    glm::vec2 CCamera::getScreenPos(glm::vec3 center, glm::mat4 model, glm::vec4 viewport)
    {
        glm::vec3 pos=glm::project(center,_view*model,_project.back(),viewport);
        return glm::vec2(pos.x,viewport[3]-pos.y);
    }

    glm::vec3 CCamera::getOrthoPos(float sx, float sy)
    {
        auto func =
                [](const glm::vec3 &p, const glm::mat4 &viewMatrix, const glm::mat4&projectMatrix, const glm::vec4&rect)->glm::vec3
        {
            glm::vec4 clip(2 * p.x / rect[2] - 1, 1 - 2 * p.y / rect[3], p.z, 1.0f);
            glm::mat4 inversem = glm::inverse(projectMatrix*viewMatrix);
            glm::vec4 world = inversem*clip;
            return glm::vec3(world /= world.w);
        };
        float x = sx;
        float y = sy;
        glm::mat4 ortho = getOrthoMatrix(viewport());
        glm::vec3 org0 = func(glm::vec3(viewport()[2] / 2.0, viewport()[3] / 2.0, 0.0f),_view,ortho,viewport());
        glm::vec3 targ0 = func(glm::vec3(x, y, 0.0f),_view,ortho,viewport());
        glm::vec3 targ = func(glm::vec3(x, y, 1.0f),_view,ortho,viewport());
        return  (targ0 - org0) + eye();
    }


    glm::vec2 CCamera::getOffsetInCameraClipZero(glm::vec2 s, glm::vec2 e, glm::vec4 viewport)
    {
        glm::vec3 dir = -cameraDirection();
        glm::vec3 org(0.0f);

        Ray3d rs = getCameraRay(s, viewport);
        Ray3d re = getCameraRay(e, viewport);

        glm::vec3 cs = intersectPlane({org, dir},rs);
        glm::vec3 ce = intersectPlane({org, dir},re);

        glm::vec3 vec = ce-cs;

        float ud = glm::dot(vec, glm::normalize(cameraUp()));
        float lr = glm::dot(vec, glm::normalize(cameraRight()));
        return glm::vec2(lr, ud);
    }

    glm::mat4 CCamera::getOrthoMatrix(glm::vec4 viewport)
    {
        float w = viewport[2];
        float h = viewport[3];
        float aspect = w / h;
        float height = h*0.5f;
        float b = -height;
        float t = height;
        float l = b*aspect;
        float r = t*aspect;
        return glm::ortho(l, r, b, t, _nearDist - 500, _farDist);
    }

    void CCamera::pushStat(glm::mat4 project, CCamera::shapeType type){
        _project.push_back(project);
        _type.push_back(type);
    }

    void CCamera::popStat(){
        _project.pop_back();
        _type.pop_back();
    }

    void CCamera::setViewMatrix() /* 计算变换后的视点矩阵*/
    {
        _cameraRight = glm::cross(_cameraUp, _cameraDirection);
        _cameraUp = glm::cross(_cameraDirection, _cameraRight);

        _cameraRight = glm::normalize(_cameraRight);
        _cameraUp = glm::normalize(_cameraUp);
        _cameraDirection = glm::normalize(_cameraDirection);

        float m[16];

        m[0] = _cameraRight.x; m[4] = _cameraRight.y; m[8] = _cameraRight.z; m[12] = -glm::dot(_eye, _cameraRight);
        m[1] = _cameraUp.x; m[5] = _cameraUp.y; m[9] = _cameraUp.z; m[13] = -glm::dot(_eye, _cameraUp);
        m[2] = _cameraDirection.x; m[6] = _cameraDirection.y; m[10] = _cameraDirection.z; m[14] = -glm::dot(_eye, _cameraDirection);
        m[3] = 0;  m[7] = 0;  m[11] = 0;  m[15] = 1.0;

        _view[0][0] = m[0]; _view[1][0] = m[4]; _view[2][0] = m[8]; _view[3][0] = m[12];
        _view[0][1] = m[1]; _view[1][1] = m[5]; _view[2][1] = m[9]; _view[3][1] = m[13];
        _view[0][2] = m[2]; _view[1][2] = m[6]; _view[2][2] = m[10]; _view[3][2] = m[14];
        _view[0][3] = m[3]; _view[1][3] = m[7]; _view[2][3] = m[11]; _view[3][3] = m[15];

    }

    void CCamera::dumpCameraInfo(const QString &name)
    {
        QFile fp(name);
        fp.open(QFile::WriteOnly);
        QByteArray buffer;
        buffer+="view angle:"+QByteArray::number(_viewAngle)+"\n";
        buffer+="aspect:"+QByteArray::number(_aspect)+"\n";
        buffer+="nearDist:"+QByteArray::number(_nearDist)+"\n";
        buffer+="far dist:"+QByteArray::number(_farDist)+"\n";
        buffer+="view height:"+QByteArray::number(_viewHeight)+"\n";
        fp.write(buffer);
        fp.close();
    }

    void CCamera::rotateRollCU(double deg)
    {
        glm::mat4 model(1.f);
        glm::mat4 rotate=glm::rotate(glm::mat4(1.f),(float)glm::radians(deg),cameraUp());
        glm::vec3 org=glm::vec3(model*glm::vec4(glm::vec3(0.f),1.f));

        glm::vec3 eyedir=_eye-org;
        glm::vec3 lookdir=_look-org;
        eyedir=glm::vec3(rotate*glm::vec4(eyedir,0.f));
        lookdir=glm::vec3(rotate*glm::vec4(lookdir,0.f));
        _eye=org+eyedir;
        _look=org+lookdir;
        _cameraRight=glm::vec3(rotate*glm::vec4(cameraRight(),0.f));
        _cameraDirection=glm::vec3(rotate*glm::vec4(cameraDirection(),0.f));
        setViewMatrix();
        return;
        glm::mat4 mat = glm::mat4(1.0);
        deg = glm::radians(deg);
        mat = glm::rotate(mat, (float)deg, glm::vec3(_cameraUp.x, _cameraUp.y, _cameraUp.z));

        glm::vec4 dirvec = glm::vec4(_eye.x - _look.x, _eye.y - _look.y, _eye.z - _look.z, 0);
        glm::vec4 eye = mat*dirvec;

        _eye.x = eye.x + _look.x;
        _eye.y = eye.y + _look.y;
        _eye.z = eye.z + _look.z;

        glm::vec4 cameraDirection = mat*glm::vec4(_cameraDirection.x, _cameraDirection.y,_cameraDirection.z,0);

        glm::vec4 cameraRight = mat*glm::vec4(_cameraRight.x, _cameraRight.y, _cameraRight.z, 0);

        _cameraDirection.x = cameraDirection.x;
        _cameraDirection.y = cameraDirection.y;
        _cameraDirection.z = cameraDirection.z;

        _cameraRight.x = cameraRight.x;
        _cameraRight.y = cameraRight.y;
        _cameraRight.z = cameraRight.z;


        setViewMatrix();
    }

    void CCamera::rotateRollCR(double deg)
    {
        glm::mat4 model(1.f);
        glm::mat4 rotate=glm::rotate(glm::mat4(1.f),(float)glm::radians(deg),cameraRight());
        glm::vec3 org=glm::vec3(model*glm::vec4(glm::vec3(0.f),1.f));

        glm::vec3 eyedir=_eye-org;
        glm::vec3 lookdir=_look-org;
        eyedir=glm::vec3(rotate*glm::vec4(eyedir,0.f));
        lookdir=glm::vec3(rotate*glm::vec4(lookdir,0.f));
        _eye=org+eyedir;
        _look=org+lookdir;
        _cameraUp=glm::vec3(rotate*glm::vec4(_cameraUp,0.f));
        _cameraDirection=glm::vec3(rotate*glm::vec4(_cameraDirection,0.f));
        setViewMatrix();
        return;
        glm::mat4 mat = glm::mat4(1.0);

        deg = glm::radians(deg);
        mat = glm::rotate(mat, (float)deg, glm::vec3(_cameraRight.x, _cameraRight.y, _cameraRight.z));

        glm::vec4 dirvec = glm::vec4(_eye.x - _look.x, _eye.y - _look.y, _eye.z - _look.z, 0);
        glm::vec4 eye = mat*dirvec;
        _eye.x = eye.x + _look.x;
        _eye.y = eye.y + _look.y;
        _eye.z = eye.z + _look.z;

        glm::vec4 cameraDirection = glm::vec4(_cameraDirection.x, _cameraDirection.y, _cameraDirection.z, 0);
        glm::vec4 cameraRight = glm::vec4(_cameraRight.x, _cameraRight.y, _cameraRight.z, 0);
        glm::vec4 cameraUp = glm::vec4(_cameraUp.x, _cameraUp.y, _cameraUp.z, 0);
        cameraDirection = mat*cameraDirection;
        cameraUp = mat*cameraUp;

        _cameraDirection.x = cameraDirection.x;
        _cameraDirection.y = cameraDirection.y;
        _cameraDirection.z = cameraDirection.z;


        _cameraUp.x = cameraUp.x;
        _cameraUp.y = cameraUp.y;
        _cameraUp.z = cameraUp.z;

        setViewMatrix();
    }

    void CCamera::moveAlongCD(float dist)
    {
        if (_type.back() == Perspective)
        {
            glm::vec3 dirvec = glm::vec3(_eye.x - _look.x, _eye.y - _look.y, _eye.z - _look.z);
            float len = glm::length(dirvec) - dist;

            if ((len <= _minLen) || (len >= _maxLen))
                return;
            dirvec = glm::normalize(dirvec);
            dirvec = dirvec * (float)dist;

            _eye = _eye - dirvec;
            setViewMatrix();
        }
        else if (_type.back()==Ortho)
        {
            float maxHeight = tanf(_viewAngle)*_maxLen*0.25;
            float height = (_viewHeight-dist)*0.5f;
            if (height < maxHeight)height = maxHeight;
            std::cout << "view height:" << height * 2 << std::endl;
            float b = -height;
            float t = height;
            float l = b*_aspect;
            float r = t*_aspect;
            setViewMode(l, r, b, t, _nearDist - 500, _farDist);
        }
    }

    void CCamera::moveAlongEyeRay(float d, glm::vec2 pos)
    {
        glm::mat4 model(1.f);
        if (_type.back() == Perspective)
        {
            float t=(d>=0?-1:1);
            Ray3d ray=getCameraRay(pos,viewport());
            glm::vec3 dir=ray[1];
            glm::vec3 look=glm::vec3(model*glm::vec4(glm::vec3(0.f),1.f));
            float nearDistance=glm::dot(dir,look-_eye);
            float distance=nearDistance+fabs(d);
            double angle=0.5f*atan(distance)+0.25f*glm::pi<float>();
            double len=tan(angle)*_nearDist*tan(glm::radians(_viewAngle*0.5f));
            if(t<0&&(glm::dot(dir,look-_eye)-len)<_nearDist)return;
            _eye+=-dir*(float)len*t;

            setViewMatrix();
        }
        else if (_type.back()==Ortho)
        {
            double angle=0.5f*atan(_viewHeight+fabs(d))+0.25f*glm::pi<float>();
            double len=tan(angle)*_nearDist*tan(glm::radians(_viewAngle*0.5f));
            len=len*(d>=0?1:-1);
            if(_viewHeight-len<0)return;
            glm::vec2 center=glm::vec2(viewport()[2]*0.5f,viewport()[3]*0.5f);
            glm::vec2 mov= getOffsetInCameraClipZero(center ,pos,viewport());
            moveAlongCR(mov.x);
            moveAlongCU(mov.y);
            float height = (_viewHeight-len)*0.5f;
            float b = -height;
            float t = height;
            float l = b*_aspect;
            float r = t*_aspect;
            setViewMode(l, r, b, t, _nearDist - 500, _farDist);

            mov= getOffsetInCameraClipZero(center ,pos,viewport());
            moveAlongCR(-mov.x);
            moveAlongCU(-mov.y);
        }
    }



    Ray3d CCamera::getCameraRay(glm::vec2 p, glm::vec4 viewport)
    {
        auto func =
                [](const glm::vec3& p, const glm::mat4& viewMatrix, const glm::mat4& projectMatrix, const glm::vec4& rect)-> glm::vec3
        {
            glm::vec4 clip(2 * p.x / rect[2] - 1, 1 - 2 * p.y / rect[3], p.z, 1.0f);
            glm::mat4 inversem = glm::inverse(projectMatrix * viewMatrix);
            glm::vec4 world = inversem * clip;
            return glm::vec3(world /= world.w);
        };
        glm::vec3 worldPosFar = func(glm::vec3(p, 1.0f), this->_view, this->_project.back(), viewport);
        if (_type.back() == Perspective)
        {
            return {eye(), glm::normalize(worldPosFar-eye())};
        }
        else
        {
            glm::vec3 worldPosCenter = func(glm::vec3(viewport[2] / 2.0, viewport[3] / 2.0, 1.0f), this->_view, this->_project.back(), viewport);
            glm::vec3 worldPosNear = func(glm::vec3(p, -1.0f), this->_view, this->_project.back(), viewport);
            return {(worldPosFar - worldPosCenter)+eye(), glm::normalize(worldPosFar - worldPosNear)};
        }
    }

    void CCamera::moveAlongCR(float fUnits)
    {
        _eye += _cameraRight* fUnits;
        setViewMatrix();
    }

    void CCamera::moveAlongCU(float fUnits)
    {
        _eye += _cameraUp* fUnits;
        setViewMatrix();
    }
    void CCamera::setViewMode(float lt, float rt, float bm, float tp, float nr, float fr)
    {
        _type.push_back(shapeType::Ortho);
        setProject(lt, rt, bm, tp, nr, fr);
    }

    void CCamera::setProject(float lt, float rt, float bm, float tp, float nr, float fr)
    {
        _viewHeight = glm::abs(tp)+glm::abs(bm);
        _project.push_back(glm::ortho(lt, rt, bm, tp, nr, fr));
    }

    glm::vec3 intersectPlane(Plane3d plane, Ray3d ray)
    {
        float distance;
        if (!glm::intersectRayPlane(ray[0], ray[1], plane[0], plane[1], distance))
        {
            plane[1] = -plane[1];
            if (!glm::intersectRayPlane(ray[0], ray[1], plane[0], plane[1], distance))
            {
                return glm::vec3(ray[0].x,ray[0].y,plane[0].z);
            }
        }
        return ray[0] + ray[1] * distance;
    }

}
