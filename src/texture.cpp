#include "texture.h"

#include "texturedquadshaderprogram.h"
#include "quad.h"

Texture::Texture()
{
    m_quad = NULL;
    m_program = NULL;
}

Texture::~Texture()
{
    safeDelete(m_quad);
    safeDelete(m_program);
}

void Texture::create(const char *imageFile, GLint magFilter, GLint minFilter)
{
    QImage image(imageFile);
    _create(image.bits(), GL_RGBA, image.width(), image.height(), GL_UNSIGNED_BYTE,
            magFilter, minFilter);
}

void Texture::createColorTexture(int width, int height, GLint magFilter, GLint minFilter)
{
    _create(0, GL_RGBA, width, height, GL_UNSIGNED_BYTE, magFilter, minFilter);
}

void Texture::createDepthTexture(int width, int height)
{
    _create(0, GL_DEPTH_COMPONENT, width, height, GL_FLOAT, GL_NEAREST, GL_NEAREST);
}

void Texture::_create(
        const GLvoid *data, GLint format, int width, int height, GLenum type,
        GLint magFilter, GLint minFilter)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
    glActiveTexture(GL_TEXTURE0);
}

void Texture::unbind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

void Texture::renderFullScreen()
{
    if (!m_quad)
    {
        m_quad = new Quad();
        m_quad->init();
    }
    if (!m_program)
    {
        m_program = new TexturedQuadShaderProgram();
        m_program->create();
    }
    m_program->bind();
    bind(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(m_program->id, "tex"), 0);
    m_quad->draw();
    unbind(GL_TEXTURE0);
    m_program->unbind();
}