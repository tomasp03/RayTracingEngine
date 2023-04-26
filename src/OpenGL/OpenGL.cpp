#include "OpenGL.h"

OpenGL OpenGL::m_instance;

OpenGL& OpenGL::Get()
{
    return m_instance;
}

void OpenGL::UpdateTexture(const Image* image)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->size.x, image->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void OpenGL::UpdateViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}

int OpenGL::CreateTexture()
{
    GLuint screenTex;
    glGenTextures(1, &screenTex);
    glBindTexture(GL_TEXTURE_2D, screenTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return screenTex;

}