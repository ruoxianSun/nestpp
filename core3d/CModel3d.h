#ifndef CMODEL3D_H
#define CMODEL3D_H

#include "CCore3d.h"
class CModel3d:public CObject3d
{
public:
    CModel3d();
    void setGeom(std::shared_ptr<CGeometry>geom);
    void render(CCamera*camera);
};

#endif // CMODEL3D_H
