#include "CModel3d.h"
#include "Shader.h"
#include "camera/CCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "CRenderMesh.h"
#include "CShaderManager.h"
CModel3d::CModel3d():CObject3d ()
{
    _data=std::make_shared<CGeometry>();
#define TEST_TRIANGLE
#ifdef TEST_TRIANGLE
    _render=std::make_shared<CRenderMesh>();
#else
    _render=std::make_shared<CLineRender>();
#endif
}

void CModel3d::setGeom(std::shared_ptr<CGeometry> geom)
{

#ifdef TEST_TRIANGLE
    _shader=CShaderManager::getShader(QStringList()<<":/basic.vs"<<":/basic.fs");
#else
    _shader=CShaderManager::getShader(QStringList()<<":/cube.vs"<<":/cube.fs");
#endif
    _data=geom;
    _render->setup(geom.get());
}



void CModel3d::render(CCamera *camera)
{
    if (!_shader)return;
    if (!_render)return;
    auto mv=camera->matrixView()*_modelMatrix;
    auto mvp=camera->matrixProject()*mv;
    _shader->use();

#ifdef TEST_TRIANGLE
    _shader->setUniform("projection",camera->matrixProject());
    _shader->setUniform("model",_modelMatrix);
    _shader->setUniform("view",camera->matrixView());
    _shader->setUniform("material.ambient",glm::vec3(0.6,0.7,0.8));
    _shader->setUniform("material.diffuse",glm::vec3(0.3));
    _shader->setUniform("material.specular",glm::vec3(0.2));
    _shader->setUniform("material.shininess",50.f);
    _shader->setUniform("light.color",glm::vec3(1.f));
    _shader->setUniform("light.direction",-camera->cameraDirection());
    _shader->setUniform("viewPos",camera->eye());
#else

    _shader->setUniform("specColor",glm::vec3(1,0,0));
    _shader->setUniform("projection",camera->matrixProject());
    _shader->setUniform("view",camera->matrixView());
    _shader->setUniform("model",_modelMatrix);
#endif
    _render->render();
    _shader->EndShader();
}
