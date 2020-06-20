#ifndef CRENDERMESH_H
#define CRENDERMESH_H

#include "CCore3d.h"
class CRenderMesh:public CRender
{
public:
    CRenderMesh():CRender ()
    {
        _nbo=0;
    }
    ~CRenderMesh();
    void setup(CGeometry*geom=0);
    void render(int tag=0x0004);
    int _nbo;
};

#endif // CRENDERMESH_H
