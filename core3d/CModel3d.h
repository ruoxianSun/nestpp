#ifndef CMODEL3D_H
#define CMODEL3D_H

#include "CCore3d.h"
class CModel3d:public CObject3d
{
public:
    CModel3d():CObject3d ()
    {
        _data=std::make_shared<CGeometry>();
        _render=std::make_shared<CRender>();
    }
    void render(CCamera*camera);
};

#endif // CMODEL3D_H
