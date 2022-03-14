#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Engine {
OpenGLTexture2D::OpenGLTexture2D() {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
    int width, height, channels;
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        Log::Core_Error("Could not open texture file \"{0}\".", path);
    }

    m_Width = width;
    m_Height = height;
    m_Channels = channels;

    GLenum internalFormate = 0, dataFormate = 0;
    if (m_Channels == 4) {
        internalFormate = GL_RGBA8;
        dataFormate = GL_RGBA;
    } else if (m_Channels == 3) {
        internalFormate = GL_RGB8;
        dataFormate = GL_RGB;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, internalFormate, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormate,
                        GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

void OpenGLTexture2D::Bind(const uint32_t& slot) const {
    glBindTextureUnit(slot, m_RendererID);
}

void OpenGLTexture2D::UnBind(const uint32_t& slot) const {
    glBindTextureUnit(slot, 0);
}

void OpenGLTexture2D::Load(void* data, uint32_t width, uint32_t height) {
    m_Width = width;
    m_Height = height;
    m_Channels = 3;
    // glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB,
                        GL_UNSIGNED_BYTE, data);
}

void* OpenGLTexture2D::GetTextureID() const {
    return (void*)(uint64_t)m_RendererID;
}
}  // namespace Engine