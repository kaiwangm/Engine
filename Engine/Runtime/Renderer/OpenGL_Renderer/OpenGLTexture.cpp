#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Engine
{
    OpenGLTexture2D::OpenGLTexture2D() { glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID); }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
    {
        int      width, height, channels;
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_default);
        if (data == nullptr)
        {
            Log::Core_Error("Could not open texture file \"{0}\".", path);
        }

        m_Width    = width;
        m_Height   = height;
        m_Channels = channels;

        GLenum internalFormate = 0, dataFormate = 0;
        if (m_Channels == 4)
        {
            internalFormate = GL_RGBA8;
            dataFormate     = GL_RGBA;
        }
        else if (m_Channels == 3)
        {
            internalFormate = GL_RGB8;
            dataFormate     = GL_RGB;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, internalFormate, m_Width, m_Height);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormate, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    void OpenGLTexture2D::Bind(const uint32_t& slot) const { glBindTextureUnit(slot, m_TextureID); }

    void OpenGLTexture2D::UnBind(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    void OpenGLTexture2D::Load(void* data, uint32_t width, uint32_t height)
    {
        m_Width    = width;
        m_Height   = height;
        m_Channels = 3;
        // glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, GL_RGB8, m_Width, m_Height);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    void* OpenGLTexture2D::GetTextureID() const { return (void*)(uint64_t)m_TextureID; }

    OpenGLCubeMap::OpenGLCubeMap(const std::string& path) : m_Path(path)
    {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_TextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

        const std::vector<std::string> textures_faces {
            path + "/px.png", path + "/nx.png", path + "/py.png", path + "/ny.png", path + "/pz.png", path + "/nz.png"};

        for (GLuint i = 0; i < textures_faces.size(); i++)
        {
            int      width, height, channels;
            stbi_uc* image = stbi_load(textures_faces[i].c_str(), &width, &height, &channels, STBI_default);

            if (image == nullptr)
            {
                Log::Core_Error("Could not open texture file \"{0}\".", path);
            }

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

            stbi_image_free(image);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void OpenGLCubeMap::Bind(const uint32_t& slot) const { glBindTextureUnit(slot, m_TextureID); }

    void OpenGLCubeMap::UnBind(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    void* OpenGLCubeMap::GetTextureID() const { return (void*)(uint64_t)m_TextureID; }
} // namespace Engine