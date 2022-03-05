#pragma once
#include "Core.h"
#include "Texture.h"

namespace Engine {
class OpenGLTexture2D : public Texture2D {
   public:
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; }
    virtual uint32_t GetChannels() const override { return m_Channels; }

    virtual void Bind(const uint32_t& slot) const override;
    virtual void UnBind(const uint32_t& slot) const override;

    std::string GetPath() const { return m_Path; }

   private:
    std::string m_Path;
    uint32_t m_Width, m_Height, m_Channels;
    GLuint m_RendererID;
};
}  // namespace Engine