#include "Shader.h"
#include <QFile>
#include <fstream>  
#include <sstream> 
Shader::Shader()
{
}
Shader::Shader(const Shader &sder)
{
    *this=sder;
}
Shader& Shader::operator =(const Shader& sder)
{
	if (this == &sder)
	{
		return *this;
	}
    _programId = sder._programId;
	return *this;
}
void Shader::SetShader(const char* vertexPath, const char* fragPath)
{
	std::vector<ShaderFile> fileVec;
	fileVec.push_back(ShaderFile(GL_VERTEX_SHADER, vertexPath));
	fileVec.push_back(ShaderFile(GL_FRAGMENT_SHADER, fragPath));
	loadFromFile(fileVec);
}
void Shader::SetShader(const char* vertexPath, const char* fragPath, const char* geometryPath)
{
	std::vector<ShaderFile> fileVec;
	fileVec.push_back(ShaderFile(GL_VERTEX_SHADER, vertexPath));
	fileVec.push_back(ShaderFile(GL_FRAGMENT_SHADER, fragPath));
	fileVec.push_back(ShaderFile(GL_GEOMETRY_SHADER, geometryPath));
	loadFromFile(fileVec);
}
void Shader::use()
{
	glUseProgram(this->_programId);
}
void Shader::EndShader()
{
	glUseProgram(0);
}


Shader::~Shader()
{
	if (this->_programId)
	{
		glDeleteProgram(this->_programId);
    }
}
/*
* 从文件加载顶点和片元着色器
* 传递参数为 [(着色器文件类型，着色器文件路径)+]
*/
void Shader::loadFromFile(std::vector<ShaderFile>& shaderFileVec)
{
	std::vector<GLuint> shaderObjectIdVec;
	std::string vertexSource, fragSource;
	std::vector<std::string> sourceVec;
	size_t shaderCount = shaderFileVec.size();
	// 读取文件源代码
	for (size_t i = 0; i < shaderCount; ++i)
	{
		std::string shaderSource;
		if (!loadShaderSource(shaderFileVec[i].filePath, shaderSource))
		{
			std::cout << "Error::Shader could not load file:" << shaderFileVec[i].filePath << std::endl;
			return;
		}
		sourceVec.push_back(shaderSource);
	}
	bool bSuccess = true;
	// 编译shader object
	for (size_t i = 0; i < shaderCount; ++i)
	{
		GLuint shaderId = glCreateShader(shaderFileVec[i].shaderType);
		const char *c_str = sourceVec[i].c_str();
		glShaderSource(shaderId, 1, &c_str, NULL);
		glCompileShader(shaderId);
		GLint compileStatus = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus); // 检查编译状态
		if (compileStatus == GL_FALSE) // 获取错误报告
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> errLog(maxLength);
			glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errLog[0]);
			std::cout << "Error::Shader file [" << shaderFileVec[i].filePath << " ] compiled failed,"
				<< &errLog[0] << std::endl;
			bSuccess = false;
		}
		shaderObjectIdVec.push_back(shaderId);
	}
	// 链接shader program
	if (bSuccess)
	{
		this->_programId = glCreateProgram();
		if (_programId == 1)
		{
			int jh = 0;
		}
		else if (_programId == 2)
		{
			int jh = 0;
		}
		else if (_programId ==3)
		{
			int jh = 0;
		}
		for (size_t i = 0; i < shaderCount; ++i)
		{
			glAttachShader(this->_programId, shaderObjectIdVec[i]);
		}
		glLinkProgram(this->_programId);
		GLint linkStatus;
		glGetProgramiv(this->_programId, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(this->_programId, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> errLog(maxLength);
			glGetProgramInfoLog(this->_programId, maxLength, &maxLength, &errLog[0]);
			std::cout << "Error::shader link failed," << &errLog[0] << std::endl;
		}
		
	}
	// 链接完成后detach 并释放shader object
	for (size_t i = 0; i < shaderCount; ++i)
	{
		if (this->_programId != 0)
		{
			glDetachShader(this->_programId, shaderObjectIdVec[i]);
		}
		glDeleteShader(shaderObjectIdVec[i]);
	}
}

/*
* 读取着色器程序源码
*/
bool Shader::loadShaderSource(const char* filePath, std::string& source)
{
    QFile fp(filePath);
    fp.open(QFile::ReadOnly);
    source=fp.readAll().toStdString();
    fp.close();
	return true;
}

void CShaderHelper::setUniform(const std::string &key, float v)
{
    glUniform1f(get(key),v);
}

void CShaderHelper::setUniform(const std::string &key, int v)
{
    glUniform1i(get(key),v);
}

void CShaderHelper::setUniform(const std::string &key, const glm::vec3 &v)
{
    glUniform3f(get(key),v.x,v.y,v.z);
}

void CShaderHelper::setUniform(const std::string &key, const glm::vec4 &v)
{
    glUniform4f(get(key),v.x,v.y,v.z,v.w);
}

void CShaderHelper::setUniform(const std::string &key, const glm::mat4 &v)
{
    glUniformMatrix4fv(get(key),1, GL_FALSE,glm::value_ptr(v));
}

GLuint CShaderHelper::getAttrib(const std::string &key)
{
    return glGetAttribLocation(_programId,key.c_str());
}

GLuint CShaderHelper::get(const std::string &key)
{
    return glGetUniformLocation(_programId,key.c_str());
}
