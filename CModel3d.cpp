#include "CModel3d.h"
#include "Shader.h"
#include "camera/CCamera.h"

void CModel3d::render(CCamera *camera)
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
