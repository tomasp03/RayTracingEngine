#pragma once
#include <iostream>
#include <fstream>
#include <glad/glad.h>

class Shader
{
private:
	unsigned int m_ID;
	void ErrorCheck(GLuint shaderID, GLenum type);
	void ErrorCheck();

public:
	Shader(const char* vertSource, const char* fragSource);
	~Shader();
	std::string Load(const char* filename);
	unsigned int ID() { return m_ID; };
	void Activate();
	void Deactivate();
	void Delete();

	//unifom upload
	void uni1f(const char* name, float data);
	void uni2f(const char* name, float data1, float data2);
	void uni3f(const char* name, float data1, float data2, float data3);
	void uni4f(const char* name, float data1, float data2, float data3, float data4);
};


