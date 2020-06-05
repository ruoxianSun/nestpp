#include "GL\glew.h"
#include "CCore3d.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm\gtc\type_ptr.hpp"
#include "glm/gtx/intersect.hpp"
#include "CShaderManager.h"
#include <camera/CCamera.h>
void CRender::use()
{
}
void CRender::unuse()
{
}

void CRender::setup(CGeometry *data)
{
    if (data)
        _data = data;
    if (!_data)return;

    if (!isValid(_vao))
        glGenVertexArrays(1,(GLuint*)&(_vao));
    glBindVertexArray(_vao);

    if (!isValid(_vbo))
        glGenBuffers(1, (GLuint*)&_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3f)*_data->_vertexs.size(), &_data->_vertexs[0], GL_DYNAMIC_DRAW);

    if (!isValid(_ebo))
        glGenBuffers(1,(GLuint*) &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*_data->_indexs.size(), &_data->_indexs[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}
void CRender::render(int type)
{
    if(_data==0)return;
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER,_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point3f), (GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ebo);
    glDrawElements(type, _data->indexSize(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CLineRender::render(int type)
{
    CRender::render(GL_LINES);
}

bool glm::operator<(const point3f& p1, const point3f& p2)
{
    if (p1.x < p2.x)return true;
    else if (p1.x == p2.x)
        if (p1.y < p2.y)return true;
		else if (p1.y == p2.y)
			if (p1.z < p2.z)return true;
	return false;
}

void CGeometry::clear() {
    _vertexs.clear();
    _indexs.clear();
    caches.clear();
}

void CGeometry::translate(glm::mat4& mat)
{
    for (int i = 0; i < _vertexs.size(); i++)
	{
		glm::vec4 tmp = mat * glm::vec4(_vertexs[i], 1.0f);
		_vertexs[i] = glm::vec3(tmp /= tmp.w);
	}
}

int CGeometry::insertVertex(const point3f &v)
{
    if(caches.find(v)==caches.end())
    {
        caches[v]=_vertexs.size();
        _vertexs.push_back(v);
    }
    int index=caches[v];
    _indexs.push_back(index);
    return index;
}

point3f& CGeometry::operator[](const int& n)
{
	assert(n >= _indexs.size());
	return _vertexs[_indexs[n]];
}

CRender::~CRender()
{
    if(isValid(_vao))
        glDeleteVertexArrays(1, (GLuint*)&_vao);
	_vao = 0;
    if(isValid(_vbo))
        glDeleteBuffers(1, (GLuint*)&_vbo);
	_vbo = 0;
    if(isValid(_ebo))
        glDeleteBuffers(1,(GLuint*)&_ebo);
    _ebo=0;

}




void CObject3d::render(CCamera *camera)
{
    if (!_shader)return;
    if (!_render)return;
    _shader->use();
    _shader->setUniform("specColor",_color);
    _shader->setUniform("projection",camera->matrixProject());
    _shader->setUniform("view",camera->matrixView());
    _shader->setUniform("model",_modelMatrix);
    _render->render();
    _shader->EndShader();
}

void CObject3d::pushMatrix(CCamera *cc){
    CCamera&camera=*cc;
    glm::mat4  project = camera.matrixProject();
    glm::mat4  view = camera.matrixView();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(project));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(view*_modelMatrix));
}



void CObject3d::popMatrix(){
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();}

void CGrid3d::setColor(Shader*shader, glm::vec3 color)
{
	shader->use();
	glUniform3f(glGetUniformLocation(shader->_programId, "specColor"), color.x, color.y, color.z);
	shader->EndShader();
}

void CGrid3d::render(CCamera*camera, glm::mat4 model, glm::vec3 color)
{
    setModelMatrix(model);
    glLineWidth(2.0f);
    grids.setColor(color);
    grids.render(camera);
    glLineWidth(1.0f);
    subGrids.setColor(color);
    subGrids.render(camera);
    glLineWidth(2.0f);
    other.setColor(color);
    other.render(camera);
    if(_needOptional)
    {
        glLineWidth(2.0f);
        optionals.setColor(glm::vec3(220.f, 0.f, 0.f)/255.0f);
        optionals.render(camera);

    }
    if(_drawPlat)
    {
        glLineWidth(2.0f);
        _plats.setColor(glm::vec3(220.f, 0.f, 0.f)/255.0f);
        _plats.render(camera);
    }

}
CGrid3d::CGrid3d(float major, float minor) :_major(major), _minor(minor),_needOptional(false)
{
    _drawPlat=false;
}

void CGrid3d::set(float platX, float platY,float platZ,QString device)
{    
    setShader();
    if(device == "LA01")
        _needOptional = true;
    else
        _needOptional = false;
    _platX = platX;
    _platY = platY;
    _platZ = platZ;
    float sz = _platZ, ez = 0.005;
    grids._data->clear();
    subGrids._data->clear();
    other._data->clear();
    optionals._data->clear();

    int bxStep = _platX*.5f /_major;
    int byStep = _platY*.5f/_major;
    float sy = -_platY*0.5f, ey = _platY*0.5f;
    float sx = -_platX*0.5f, ex = _platX*0.5f;
    for (int i = -bxStep; i <= bxStep;i++)
    {
        grids._data->insertVertex(point3f(i*_major, byStep*_major, ez));
        grids._data->insertVertex(point3f(i*_major, -byStep*_major, ez));
    }
    for (int i = -byStep; i <= byStep; i++)
    {
        grids._data->insertVertex(point3f(bxStep*_major, i*_major, ez));
        grids._data->insertVertex(point3f(-bxStep*_major, i*_major, ez));
    }
    grids._render->setup(grids._data.get());


	other._data->insertVertex(point3f(sx, sy, sz));
	other._data->insertVertex(point3f(ex, sy, sz));

	other._data->insertVertex(point3f(ex, sy, sz));
	other._data->insertVertex(point3f(ex, ey, sz));

	other._data->insertVertex(point3f(ex, ey, sz));
	other._data->insertVertex(point3f(sx, ey, sz));

	other._data->insertVertex(point3f(sx, ey, sz));
	other._data->insertVertex(point3f(sx, sy, sz));

	other._data->insertVertex(point3f(sx, sy, sz));
	other._data->insertVertex(point3f(sx, sy, ez));

	other._data->insertVertex(point3f(ex, sy, sz));
	other._data->insertVertex(point3f(ex, sy, ez));

	other._data->insertVertex(point3f(ex, ey, sz));
	other._data->insertVertex(point3f(ex, ey, ez));

    other._data->insertVertex(point3f(sx, ey, sz));
    other._data->insertVertex(point3f(sx, ey, ez));




    other._data->insertVertex(point3f(sx, sy, 0));
    other._data->insertVertex(point3f(ex, sy, 0));

    other._data->insertVertex(point3f(ex, sy, 0));
    other._data->insertVertex(point3f(ex, ey, 0));

    other._data->insertVertex(point3f(ex, ey, 0));
    other._data->insertVertex(point3f(sx, ey, 0));

    other._data->insertVertex(point3f(sx, ey, 0));
    other._data->insertVertex(point3f(sx, sy, 0));

    other._render->setup(other._data.get());

    int subxstep = _platX*.5f/_minor, subystep = _platY*.5f /_minor;
    for (int i = -subystep; i <= subystep; i++)
    {
        subGrids._data->insertVertex(point3f(subxstep*_minor, i*_minor, ez));
        subGrids._data->insertVertex(point3f(-subxstep*_minor, i*_minor, ez));
    }
    for (int i = -subxstep; i <= subxstep; i++)
    {
        subGrids._data->insertVertex(point3f(i*_minor, subystep*_minor, ez));
        subGrids._data->insertVertex(point3f(i*_minor, -subystep*_minor, ez));
    }
    subGrids._data->insertVertex(point3f(2*_minor, sy, ez));
    subGrids._data->insertVertex(point3f(0, sy -2*_minor, ez));
    subGrids._data->insertVertex(point3f(0, sy -2*_minor, ez));
    subGrids._data->insertVertex(point3f(-2*_minor, sy, ez));
    subGrids._render->setup(subGrids._data.get());

    optionals._data->insertVertex(point3f(-165, -3, ez));
    optionals._data->insertVertex(point3f(165, -3, ez));

    optionals._data->insertVertex(point3f(165, -3, ez));
    optionals._data->insertVertex(point3f(165, 3, ez));

    optionals._data->insertVertex(point3f(165, 3, ez));
    optionals._data->insertVertex(point3f(-165, 3, ez));

    optionals._data->insertVertex(point3f(-165, -3, ez));
    optionals._data->insertVertex(point3f(-165, 3, ez));

    optionals._data->insertVertex(point3f(-165, -129.40, ez));
    optionals._data->insertVertex(point3f(165, -129.40, ez));

    optionals._data->insertVertex(point3f(165, -129.40, ez));
    optionals._data->insertVertex(point3f(165, 129.40, ez));

    optionals._data->insertVertex(point3f(165, 129.40, ez));
    optionals._data->insertVertex(point3f(-165, 129.40, ez));

    optionals._data->insertVertex(point3f(-165, -129.40, ez));
    optionals._data->insertVertex(point3f(-165, 129.40, ez));

    optionals._render->setup(optionals._data.get());
}

void CGrid3d::set(QString device,QString plat)
{
    _drawPlat=false;
    if(device=="TP02" && plat=="PC-S")
    {
        _drawPlat=true;
        setShader();
        _plats._data->clear();
        float ez = 0.005;
        float x=110.0*0.5;
        float y=61.0*0.5;
        _plats._data->insertVertex(point3f(-x, -y, ez));
        _plats._data->insertVertex(point3f(x, -y, ez));

        _plats._data->insertVertex(point3f(x, -y, ez));
        _plats._data->insertVertex(point3f(x, y, ez));

        _plats._data->insertVertex(point3f(x, y, ez));
        _plats._data->insertVertex(point3f(-x, y, ez));

        _plats._data->insertVertex(point3f(-x, -y, ez));
        _plats._data->insertVertex(point3f(-x, y, ez));
        _plats._render->setup(_plats._data.get());
    }
}

void CGrid3d::setShader()
{
    grids._shader=CShaderManager::Instance()->getShader("cube");
    subGrids._shader=CShaderManager::Instance()->getShader("cube");
    other._shader=CShaderManager::Instance()->getShader("cube");
    optionals._shader=CShaderManager::Instance()->getShader("cube");
    _plats._shader=CShaderManager::Instance()->getShader("cube");
}

void CGrid3d::setModelMatrix(glm::mat4 mat)
{
    grids._modelMatrix = mat;
    subGrids._modelMatrix = mat;
    other._modelMatrix = mat;
    optionals._modelMatrix = mat;
    _plats._modelMatrix=mat;
}
QDebug operator <<(QDebug &dbg, point3f p){
    dbg.nospace()<<"<"<<p.x<<","<<p.y<<","<<p.z<<">";
    return dbg;
}



