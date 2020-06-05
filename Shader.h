#pragma once
#include <GL/glew.h>  
#include <iterator>     // std::istreambuf_iterator
#include <string> 
#include <cstring> 
#include <vector>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
struct ShaderFile
{
	GLenum shaderType;
	const char* filePath;
	ShaderFile(GLenum type, const char* path)
		:shaderType(type), filePath(path){}
};
class CShaderHelper
{
public:
    CShaderHelper():_programId(-1){}
    CShaderHelper(int id):_programId(id){}
    virtual void setUniform(const std::string&key,float v);
    virtual void setUniform(const std::string &key, int v);
    virtual void setUniform(const std::string &key, const glm::vec3 &v);
    virtual void setUniform(const std::string &key, const glm::vec4 &v);
    virtual void setUniform(const std::string &key, const glm::mat4 &v);
    virtual GLuint getAttrib(const std::string&key);
protected:
    GLuint get(const std::string&key);
public:
    GLuint _programId;
};
class Shader:public CShaderHelper
{
public:
    typedef std::shared_ptr<Shader> Ptr;
    static Ptr New(){return std::make_shared<Shader>();}

	Shader();
	Shader(const Shader &sder);
    virtual Shader& operator =(const Shader& sder);
    virtual void SetShader(const char* vertexPath, const char* fragPath);
    virtual void SetShader(const char* vertexPath, const char* fragPath, const char* geometryPath);
    virtual void use();
    virtual void EndShader();
    virtual ~Shader();
public:
protected:
	/*
	* ���ļ����ض����ƬԪ��ɫ��
	* ���ݲ���Ϊ [(��ɫ���ļ����ͣ���ɫ���ļ�·��)+]
	*/
	void loadFromFile(std::vector<ShaderFile>& shaderFileVec);
	/*
	* ��ȡ��ɫ������Դ��
	*/
	bool loadShaderSource(const char* filePath, std::string& source);
};
