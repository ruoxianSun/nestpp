#include "CShaderManager.h"
#include "CShaderManager.h"



CShaderManager* CShaderManager::_instance=0;


CShaderManager *CShaderManager::Instance(){
    if(!_instance)
    {
        _instance=new CShaderManager;
    }
    return _instance;
}

CShaderManager::~CShaderManager(){
    _shaders.clear();
    if(_instance)
    {
        _instance->deleteLater();
        _instance=0;
    }
}

Shader::Ptr CShaderManager::getShader(const QString &name)
{
//    if(_shaders.contains(name))
//    {
//        return _shaders[name];
//    }
    return loadShader(name);
}

Shader::Ptr CShaderManager::shader(const QStringList &files)
{
    QString name=files.join(".");
    Shader::Ptr s=Shader::New();
    if(Instance()->_shaders.contains(name))
    {
        return Instance()->_shaders[name];
    }
    if(files.size()>=2)
    {
        QString vert=files[0];
        QString frag=files[1];
        if(files.size()==3)
        {
            QString geom=files[2];
            Shader*ss=s.get();
            ss->SetShader(vert.toLatin1().data(),frag.toLatin1().data(),geom.toLatin1().data());
        }
        else {
            Shader*ss=s.get();
            ss->SetShader(vert.toLatin1().data(),frag.toLatin1().data());
        }
        Instance()->_shaders[name]=s;
    }
    return s;
}

Shader::Ptr CShaderManager::getShader(const QStringList &files)
{
    QString vertexShaderSource=files[0];
    QString fragShaderSource=files[1];
    Shader::Ptr shader=Shader::New();
    shader->SetShader(vertexShaderSource.toLatin1().data(),fragShaderSource.toLatin1().data());
    return shader;
}


CShaderManager::CShaderManager(QObject *parent) : QObject(parent)
{

}

Shader::Ptr CShaderManager::loadShader(const QString &name)
{
    QString base(":");
    QString vertexShaderSource=base+"/"+name+".vs";
    QString fragShaderSource=base+"/"+name+".fs";
    Shader::Ptr shader=Shader::New();
    shader->SetShader(vertexShaderSource.toLatin1().data(),fragShaderSource.toLatin1().data());
    _shaders[name]=shader;
    return shader;
}




