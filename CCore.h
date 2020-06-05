#ifndef __CCORE_DATA_H__
#define __CCORE_DATA_H__
#include "glm\glm.hpp"
#include "glm\gtc\constants.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <qvector.h>
#include <vector>
#include <memory>
typedef glm::vec3 point3f;
typedef glm::vec3 point3d;
typedef glm::vec4 point4f;
#include <QDebug>
namespace sgl{
class CCamera;
}
using namespace sgl;
namespace glm
{
	static bool operator <(const point3f& p1, const point3f& p2);
}
QDebug operator <<(QDebug&dbg,point3f p);

class CPlane{
  public:
    CPlane(glm::vec3&org,glm::vec3 &dir)
    {
        _org=org;
        _dir=glm::normalize(dir);
    }
    CPlane(glm::vec3&v0,glm::vec3&v1,glm::vec3&v2)
    {
        _org=v0;
        _dir=glm::normalize(glm::cross(v0-v1,v0-v2));
    }
    bool intersect(CPlane&other)
    {
        float val=glm::dot(other._dir,_dir);
        if(val==1||val==-1)//two planes parallel to each other
        {
            return false;
        }
    }
    glm::vec3 _org;
    glm::vec3 _dir;
};
class CGeom
{
public:
    void clear();
	void translate(glm::mat4& mat);
    int insertVertex(const point3f & v);
	point3f& operator[](const int& n);
	int indexSize() { return _indexs.size(); }
	std::vector<point3f> _vertexs;
	std::vector<int> _indexs;
    std::map<point3f,int> caches;
};
class CRender
{
public:
	CRender() {
		_vao = _vbo =_ebo= 0;
        _data=0;
	}
	~CRender();
    virtual void setup(CGeom *data=0);
	virtual void use();
	virtual void unuse();
    virtual void render(int type = 0x0004);//default GL_TRIANGLES
	CGeom*_data;
    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _ebo;
protected:
    bool isValid(unsigned int v){
        return (v>0);
    }
};

class CLineRender:public CRender
{
public:
	CLineRender() :CRender() {}
	virtual void render(int type = 0x0001); // GL_LINES 
};
class Shader;
class CObj
{
public:
	CObj()
	{
        _data = 0;
		_render = 0;
		_modelMatrix = glm::mat4(1.0f);
        _isSelected=false;
	}
    virtual ~CObj(){}
    virtual void setColor(glm::vec3 color){_color=color;}
    virtual void render(CCamera*camera);
    virtual void pushMatrix(CCamera*camera);
    virtual void popMatrix();
    std::shared_ptr<CGeom> _data;
    std::shared_ptr<CRender> _render;
    std::shared_ptr<Shader>_shader;
	glm::mat4 _modelMatrix;
    bool _isSelected;
    glm::vec3 _color;
};
class CLines3d :public CObj
{
public:
	CLines3d() :CObj() {
        _data = std::make_shared<CGeom>();
        _render = std::make_shared<CLineRender>();
	}
    void render(CCamera*camera) { CObj::render(camera); }
	~CLines3d()
    {

	}
};
class CGrid3d
{
public:
    CGrid3d(float major = 20, float minor = 5);
    void set(float platX, float platY,float platZ,QString device="");
    void set(QString device,QString plat);
    void setShader();
    void setModelMatrix(glm::mat4 mat);
	void setColor(Shader*shader, glm::vec3 color);
    void render(CCamera*camera, glm::mat4 model, glm::vec3 color);
	CLines3d grids, subGrids,other;
    CLines3d optionals;
    CLines3d _plats;
	float _platX, _platY,_platZ;
	int _major, _minor;
    bool _needOptional;
    bool _drawPlat;
};
class CArrows3d:public CObj
{
public:
	CArrows3d(float radius=5.f,float length=10.f) :CObj()
		, _radius(radius), _length(length)
    {
        _data = std::make_shared<CGeom>();
        _render = std::make_shared<CLineRender>();
	}	
    void setColor(glm::vec3 color);
    void setShader();
	void update(float axisLength);
	void translate(glm::mat4&m);
    void translate(const glm::mat4&m);
    void render(CCamera*camera);
	~CArrows3d()
    {
	}
	float _radius;
    float _length;
	CLines3d _line;
};
class CAxis3d
{
public:
    void setShader();
	void setAxisLength(const point3f&org,float length);
    void render(CCamera*camera);
    CArrows3d axis3d[3];
	float _axisLength;
};
#endif
