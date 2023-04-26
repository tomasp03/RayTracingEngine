#pragma once
#include <glad/glad.h>
#include <Image.h>

class OpenGL
{
public:
	OpenGL(const OpenGL&) = delete;
	static OpenGL& Get();

	int CreateTexture();
	void UpdateViewport(int width, int height);
	void UpdateTexture(const Image* image);

private:
	OpenGL() {}
	static OpenGL m_instance;
};

