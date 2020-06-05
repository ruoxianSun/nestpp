#ifndef CMODELRENDER_H
#define CMODELRENDER_H

#include "CCore.h"
class CModel3d:public CObj
{
public:
    CModel3d():CObj ()
    {
        _data=std::make_shared<CGeom>();
        _render=std::make_shared<CRender>();
    }
    void render(CCamera*camera);
};

#endif // CMODELRENDER_H
