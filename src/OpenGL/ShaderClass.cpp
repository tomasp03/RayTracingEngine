#include "ShaderClass.h"


Shader::Shader(const char* vertSource, const char* fragSource)
{
	std::string vertexCode = Load(vertSource);
	std::string fragmentCode = Load(fragSource);

	const char* vertex = vertexCode.c_str();
	const char* fragment = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex, NULL);
	glCompileShader(vertexShader);
	ErrorCheck(vertexShader, GL_VERTEX_SHADER);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment, NULL);
	glCompileShader(fragmentShader);
	ErrorCheck(vertexShader, GL_FRAGMENT_SHADER);


	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);

	glLinkProgram(m_ID);
	ErrorCheck();

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	Delete();
}

std::string  Shader::Load(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

void Shader::Activate()
{
	glUseProgram(m_ID);
}

void Shader::Deactivate()
{
	glUseProgram(0);
}

void Shader::Delete()
{
	glDeleteProgram(m_ID);
}

void Shader::uni1f(const char* name, float data1)
{
	glUniform1f(glGetUniformLocation(m_ID, name), data1);
}

void Shader::uni2f(const char* name, float data1, float data2)
{
	glUniform2f(glGetUniformLocation(m_ID, name), data1, data2);
}

void Shader::uni3f(const char* name, float data1, float data2, float data3)
{
	glUniform3f(glGetUniformLocation(m_ID, name), data1, data2, data3);
}

void Shader::uni4f(const char* name, float data1, float data2, float data3, float data4)
{
	glUniform4f(glGetUniformLocation(m_ID, name), data1, data2, data3, data4);
}

void Shader::ErrorCheck(GLuint shaderID, GLenum type)
{
	int  success;
	char infoLog[1024];
	std::string shaderType = (type - 0x8B30) ? "VERTEX" : "FRAGMENT";
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::" + shaderType + "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::ErrorCheck()
{
	int  success;
	char infoLog[1024];

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}
