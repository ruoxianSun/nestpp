#include <GL/glew.h>
#include "CTrangleRender.h"



void CTrangleRender::setup(CGeometry *geom)
{
    auto calNormal=[](const glm::vec3&a,const glm::vec3&b,const glm::vec3&c){
        return glm::normalize(glm::cross(a-c,a-b));
    };
    if (geom)
        _data = geom;
    if (!_data)return;

    CRender::setup(geom);


    std::vector<glm::vec3> normals;
    normals.resize(geom->indexSize());
    for(int i=0;i<geom->indexSize()/3;i++)
    {
        auto v0=_data->_vertexs[_data->_indexs[i*3+0]];
        auto v1=_data->_vertexs[_data->_indexs[i*3+1]];
        auto v2=_data->_vertexs[_data->_indexs[i*3+2]];
        auto normal=calNormal(v0,v1,v2);
        normals[i*3+0]=normal;
        normals[i*3+1]=normal;
        normals[i*3+2]=normal;
    }

    if(!isValid(_nbo))
        glGenBuffers(1,(GLuint*)&_nbo);
    glBindBuffer(GL_ARRAY_BUFFER,_nbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3f)*normals.size(), &normals[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);


    glBindVertexArray(0);
}

void CTrangleRender::render(int tag)
{
    if(_data==0)return;
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER,_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (point3f), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER,_nbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(point3f), (GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ebo);
    glDrawElements(tag, _data->indexSize(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
