#include "CModel3d.h"
#include "Shader.h"
#include "camera/CCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "CTrangleRender.h"
#include "CShaderManager.h"
CModel3d::CModel3d():CObject3d ()
{
    _data=std::make_shared<CGeometry>();
#define TEST_TRIANGLE
#ifdef TEST_TRIANGLE
    _render=std::make_shared<CTrangleRender>();
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
    _shader->setUniform("ProjectionMatrix",camera->matrixProject());
    _shader->setUniform("ModelViewMatrix",mv);
    _shader->setUniform("NormalMatrix",glm::mat3(glm::inverseTranspose(mv)));
    _shader->setUniform("MVP",mvp);
    _shader->setUniform("Material.Ka",glm::vec3(0.6,0.7,0.8));
    _shader->setUniform("Material.Kd",glm::vec3(0.29));
    _shader->setUniform("Material.Ks",glm::vec3(0.3));
    _shader->setUniform("Material.Shininess",20.f);
    _shader->setUniform("Light.La",glm::vec3(0.5));
    _shader->setUniform("Light.Ld",glm::vec3(0.5));
    _shader->setUniform("Light.Ls",glm::vec3(0.6));
    _shader->setUniform("Light.Position",glm::vec3(1.f,-5000.f,1.f));
#else

    _shader->setUniform("specColor",glm::vec3(1,0,0));
    _shader->setUniform("projection",camera->matrixProject());
    _shader->setUniform("view",camera->matrixView());
    _shader->setUniform("model",_modelMatrix);
#endif
    _render->render();
    _shader->EndShader();
}
