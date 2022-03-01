#include "OpenGLTexture.h"

#include "Stb/stb_image.h"

namespace Engine {
OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
    int width, height, channels;
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    m_Width = width;
    m_Height = height;
    m_Channels = channels;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB,
                        GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

void OpenGLTexture2D::Bind(const uint32_t& slot) const {
    glBindTextureUnit(slot, m_RendererID);
}

void OpenGLTexture2D::UnBind(const uint32_t& slot) const {
    glBindTextureUnit(slot, 0);
}
}  // namespace Engine