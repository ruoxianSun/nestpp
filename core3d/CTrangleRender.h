#ifndef CTRANGLERENDER_H
#define CTRANGLERENDER_H

#include "CCore3d.h"
class CTrangleRender:public CRender
{
public:
    CTrangleRender():CRender ()
    {
        _nbo=0;
    }
    void setup(CGeometry*geom=0);
    void render(int tag=0x0004);
    int _nbo;
};

#endif // CTRANGLERENDER_H
