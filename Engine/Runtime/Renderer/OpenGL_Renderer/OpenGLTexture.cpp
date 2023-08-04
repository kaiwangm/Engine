#include <Engine/Runtime/Renderer/OpenGL_Renderer/OpenGLTexture.h>
#include <Engine/Runtime/Renderer/Shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/gtx/compatibility.hpp>

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void         renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        // clang-format off
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        // clang-format on

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void         renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, //
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, //
            1.0f,  1.0f,  0.0f, 1.0f, 1.0f, //
            1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, //
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

namespace Engine
{
    OpenGLTexture2D::OpenGLTexture2D() { glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID); }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    {
        int width, height, channels;
        data = stbi_load(path.c_str(), &width, &height, &channels, STBI_default);
        if (data == nullptr)
        {
            Log::Core_Error(fmt::format("Could not open texture file \"{0}\".", path));
        }

        m_Width    = width;
        m_Height   = height;
        m_Channels = channels;
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
        else if (m_Channels == 2)
        {
            internalFormate = GL_RG8;
            dataFormate     = GL_RG;
        }
        else if (m_Channels == 1)
        {
            internalFormate = GL_R8;
            dataFormate     = GL_RED;
        }
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_TextureID);
        if (data != nullptr)
        {
            stbi_image_free(data);
        }
    }

    void OpenGLTexture2D::Buffer()
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureStorage2D(m_TextureID, std::log2(std::max(m_Width, m_Height)), internalFormate, m_Width, m_Height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormate, GL_UNSIGNED_BYTE, data);

        glGenerateTextureMipmap(m_TextureID);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::Clear()
    {
        if (m_TextureID != 0)
        {
            glDeleteTextures(1, &m_TextureID);
        }
        m_TextureID = 0;
    }

    void OpenGLTexture2D::Bind(const uint32_t& slot) const { glBindTextureUnit(slot, m_TextureID); }

    void OpenGLTexture2D::UnBind(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    void* OpenGLTexture2D::GetTextureID() const { return (void*)(uint64_t)m_TextureID; }

    OpenGLTexture3D::OpenGLTexture3D() { glCreateTextures(GL_TEXTURE_3D, 1, &m_TextureID); }

    OpenGLTexture3D::OpenGLTexture3D(const uint32_t& width, const uint32_t& height, const uint32_t& depth) :
        m_Width(width), m_Height(height), m_Depth(depth)
    {
        GLfloat* data = new GLfloat[width * height * depth * 4];
        // fill data
        for (uint32_t i = 0; i < width * height * depth * 4; i += 4)
        {
            data[i]     = 1.0f;
            data[i + 1] = 1.0f;
            data[i + 2] = 1.0f;
            data[i + 3] = 1.0f;
        }

        glCreateTextures(GL_TEXTURE_3D, 1, &m_TextureID);
        glBindTexture(GL_TEXTURE_3D, m_TextureID);
        // glTextureStorage3D(m_TextureID, 1, GL_RGBA16F, m_Width, m_Height, m_Depth);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexStorage3D(GL_TEXTURE_3D, std::log2(width), GL_RGBA16F, width, height, depth);
        glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, GL_RGBA, GL_FLOAT, data);
        glGenerateMipmap(GL_TEXTURE_3D);

        glBindTexture(GL_TEXTURE_3D, 0);

        delete[] data;

        // glTextureSubImage3D(m_TextureID, 0, 0, 0, 0, m_Width, m_Height, m_Depth, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    void OpenGLTexture3D::Bind(const uint32_t& slot) const { glBindTextureUnit(slot, m_TextureID); }

    void OpenGLTexture3D::UnBind(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    void* OpenGLTexture3D::GetTextureID() const { return (void*)(uint64_t)m_TextureID; }

    OpenGLCubeMap::OpenGLCubeMap(const std::string& path) : m_Path(path)
    {
        if (path == "")
        {
            glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_CubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
            for (unsigned int i = 0; i < 6; ++i)
            {
                // note that we store each face with 16 bit floating point values
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Irradiance Texture
            glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_Irradiance);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_Irradiance);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            return;
        }
        std::string suffix;
        if (path.size() >= 4)
        {
            suffix = path.substr(path.size() - 4, 4);
        }
        if (suffix == ".hdr")
        {
            // Load img
            stbi_set_flip_vertically_on_load(true);
            int    width, height, channels;
            float* data = stbi_loadf(path.c_str(), &width, &height, &channels, STBI_default);
            GLuint hdrTexture;
            if (data)
            {
                glGenTextures(1, &hdrTexture);
                glBindTexture(GL_TEXTURE_2D, hdrTexture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            stbi_set_flip_vertically_on_load(false);

            // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

            GLuint captureFBO, captureRBO;
            glGenFramebuffers(1, &captureFBO);
            glGenRenderbuffers(1, &captureRBO);

            glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
            glm::mat4 captureViews[]    = {
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

            // SkyBox Texture
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 4096, 4096);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

            glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_CubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
            for (unsigned int i = 0; i < 6; ++i)
            {
                // note that we store each face with 16 bit floating point values
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 4096, 4096, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Ref<Shader> hdrtocubemap = Shader::Create("hdrtocubemap",
                                                      "Assets/Editor/Shader/vertex_hdrtocubemap.glsl",
                                                      "Assets/Editor/Shader/fragment_hdrtocubemap.glsl",
                                                      "Path");
            // convert HDR equirectangular environment map to cubemap equivalent
            hdrtocubemap->Bind();
            hdrtocubemap->SetInt("equirectangularMap", 0);
            hdrtocubemap->SetMat4("projection", captureProjection);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, hdrTexture);

            glViewport(0, 0, 4096, 4096); // don't forget to configure the viewport to the capture dimensions.
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            for (unsigned int i = 0; i < 6; ++i)
            {
                hdrtocubemap->SetMat4("view", captureViews[i]);
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_CubeMap, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                renderCube();
            }

            glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            hdrtocubemap->UnBind();

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            ComputeIrradianceTexture();

            ComputeSphereHarmonicsParameters();

            // Prefilter Map

            glGenTextures(1, &m_Prefilter);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_Prefilter);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

            Ref<Shader> prefilter = Shader::Create("prefilter",
                                                   "Assets/Editor/Shader/vertex_hdrtocubemap.glsl",
                                                   "Assets/Editor/Shader/fragment_prefilter.glsl",
                                                   "Path");
            // convert HDR equirectangular environment map to cubemap equivalent
            prefilter->Bind();
            prefilter->SetInt("environmentMap", 0);
            prefilter->SetMat4("projection", captureProjection);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);

            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            unsigned int maxMipLevels = 8;
            for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
            {
                // reisze framebuffer according to mip-level size.
                unsigned int mipWidth  = static_cast<unsigned int>(128 * std::pow(0.5, mip));
                unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
                glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);

                float roughness = (float)mip / (float)(maxMipLevels - 1);
                prefilter->SetFloat("roughness", roughness);
                for (unsigned int i = 0; i < 6; ++i)
                {
                    prefilter->SetMat4("view", captureViews[i]);
                    glFramebufferTexture2D(
                        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_Prefilter, mip);

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    renderCube();
                }
            }

            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            prefilter->UnBind();

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            // pbr: generate a 2D LUT from the BRDF equations used.
            // ----------------------------------------------------

            // load texture data from file
            int      lut_width, lut_height, lut_nrComponents;
            stbi_uc* lut_data = stbi_load(
                "Assets/Editor/Texture/ibl_brdf_lut.png", &lut_width, &lut_height, &lut_nrComponents, STBI_default);

            glGenTextures(1, &m_BrdfLut);
            glBindTexture(GL_TEXTURE_2D, m_BrdfLut);
            glTextureStorage2D(m_BrdfLut, 1, GL_RGB8, lut_width, lut_height);

            glTextureParameteri(m_BrdfLut, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_BrdfLut, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureSubImage2D(m_BrdfLut, 0, 0, 0, lut_width, lut_height, GL_RGB, GL_UNSIGNED_BYTE, lut_data);

            stbi_image_free(lut_data);

            glBindTexture(GL_TEXTURE_2D, 0);

            glDeleteRenderbuffers(1, &captureRBO);
            glDeleteFramebuffers(1, &captureFBO);
        }
        else
        {
            const std::vector<std::string> textures_faces {path + "/px.png",
                                                           path + "/nx.png",
                                                           path + "/py.png",
                                                           path + "/ny.png",
                                                           path + "/pz.png",
                                                           path + "/nz.png"};

            glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_CubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);

            for (GLuint i = 0; i < textures_faces.size(); i++)
            {
                int      width, height, channels;
                stbi_uc* image = stbi_load(textures_faces[i].c_str(), &width, &height, &channels, STBI_default);

                if (image == nullptr)
                {
                    Log::Core_Error(fmt::format("Could not open texture file \"{0}\".", path));
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
    }

    OpenGLCubeMap::~OpenGLCubeMap()
    {
        glDeleteTextures(1, &m_CubeMap);
        glDeleteTextures(1, &m_Irradiance);
        glDeleteTextures(1, &m_Prefilter);
    }

    void OpenGLCubeMap::ComputeIrradianceTexture()
    {
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[]    = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

        GLuint captureFBO, captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        // Irradiance Texture
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_Irradiance);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Irradiance);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        Ref<Shader> irradiance_convolution = Shader::Create("irradiance_convolution",
                                                            "Assets/Editor/Shader/vertex_hdrtocubemap.glsl",
                                                            "Assets/Editor/Shader/fragment_irradiance_convolution.glsl",
                                                            "Path");
        // convert HDR equirectangular environment map to cubemap equivalent
        irradiance_convolution->Bind();
        irradiance_convolution->SetInt("environmentMap", 0);
        irradiance_convolution->SetMat4("projection", captureProjection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);

        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            irradiance_convolution->SetMat4("view", captureViews[i]);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_Irradiance, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderCube();
        }

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        irradiance_convolution->UnBind();

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        glDeleteRenderbuffers(1, &captureRBO);
        glDeleteFramebuffers(1, &captureFBO);
    }

    void OpenGLCubeMap::ComputeSphereHarmonicsParameters()
    {
        // Generate Spherical Harmonics Coefficients from Cubemap
        int             IrradianceMapSize        = 32;
        const glm::vec3 cubemapFaceNormals[6][3] = {
            {{0, 0, -1}, {0, -1, 0}, {1, 0, 0}}, // posx
            {{0, 0, 1}, {0, -1, 0}, {-1, 0, 0}}, // negx

            {{1, 0, 0}, {0, 0, 1}, {0, 1, 0}},   // posy
            {{1, 0, 0}, {0, 0, -1}, {0, -1, 0}}, // negy

            {{1, 0, 0}, {0, -1, 0}, {0, 0, 1}},  // posz
            {{-1, 0, 0}, {0, -1, 0}, {0, 0, -1}} // negz
        };

        typedef std::array<std::array<std::array<unsigned char, 3>, 32>, 32> cube_image;
        std::vector<cube_image>                                              cubeMapData(6);
        sh_data.resize(9);

        for (int i = 0; i < 6; i++)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_Irradiance);
            glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_UNSIGNED_BYTE, cubeMapData[i].data());
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }

        std::vector<std::vector<glm::vec3>> cubeMapVecs;

        for (int i = 0; i < 6; i++)
        {
            std::vector<glm::vec3> faceVecs;
            for (int v = 0; v < IrradianceMapSize; v++)
            {
                for (int u = 0; u < IrradianceMapSize; u++)
                {
                    const float FU = (2.0 * u / (IrradianceMapSize - 1.0)) - 1.0;
                    const float FV = (2.0 * v / (IrradianceMapSize - 1.0)) - 1.0;

                    glm::vec3 vecX = cubemapFaceNormals[i][0] * FU;
                    glm::vec3 vecY = cubemapFaceNormals[i][1] * FV;
                    glm::vec3 vecZ = cubemapFaceNormals[i][2];

                    faceVecs.push_back(glm::normalize(vecX + vecY + vecZ));
                }
            }
            cubeMapVecs.push_back(faceVecs);
        }

        std::function<float(float, float, int, int)> texelSolidAngle = [](float aU, float aV, int width, int height) {
            const float U = (2.0 * (aU + 0.5) / width) - 1.0;
            const float V = (2.0 * (aV + 0.5) / height) - 1.0;

            const float invResolutionW = 1.0 / width;
            const float invResolutionH = 1.0 / height;

            const float x0 = U - invResolutionW;
            const float y0 = V - invResolutionH;
            const float x1 = U + invResolutionW;
            const float y1 = V + invResolutionH;

            std::function<float(float, float)> areaElement = [](float x, float y) {
                return (float)std::atan2(x * y, std::sqrt(x * x + y * y + 1.0));
            };

            const float angle = areaElement(x0, y0) - areaElement(x0, y1) - areaElement(x1, y0) + areaElement(x1, y1);

            return angle;
        };

        float weightAccum = 0.0f;
        for (int i = 0; i < 6; i++)
        {
            const cube_image& pixels = cubeMapData[i];
            for (int y = 0; y < IrradianceMapSize; y++)
            {
                for (int x = 0; x < IrradianceMapSize; x++)
                {
                    const glm::vec3& texelVector = cubeMapVecs[i][y * IrradianceMapSize + x];

                    const float weight = texelSolidAngle(x, y, IrradianceMapSize, IrradianceMapSize);
                    // weights
                    const float weight1 = weight * 4.0f / 17.0f;
                    const float weight2 = weight * 8.0f / 17.0f;
                    const float weight3 = weight * 15.0f / 17.0f;
                    const float weight4 = weight * 5.0f / 68.0f;
                    const float weight5 = weight * 15.0f / 68.0f;

                    const float dx = texelVector.x;
                    const float dy = texelVector.y;
                    const float dz = texelVector.z;

                    for (int c = 0; c < 3; c++)
                    {
                        const float value = pixels[y][x][c] / 255.0f;

                        sh_data[0][c] += value * weight1;
                        sh_data[1][c] += value * weight2 * dx;
                        sh_data[2][c] += value * weight2 * dy;
                        sh_data[3][c] += value * weight2 * dz;

                        sh_data[4][c] += value * weight3 * dx * dz;
                        sh_data[5][c] += value * weight3 * dx * dy;
                        sh_data[6][c] += value * weight3 * dy * dx;

                        sh_data[7][c] += value * weight4 * (3.0f * dz * dz - 1.0f);
                        sh_data[8][c] += value * weight5 * (dx * dx - dy * dy);

                        weightAccum += weight;
                    }
                }
            }
        }

        for (int i = 0; i < 9; i++)
        {
            sh_data[i][0] = sh_data[i][0] * (4.0f * glm::pi<float>()) / weightAccum;
            sh_data[i][1] = sh_data[i][1] * (4.0f * glm::pi<float>()) / weightAccum;
            sh_data[i][2] = sh_data[i][2] * (4.0f * glm::pi<float>()) / weightAccum;
        }
    }

    void OpenGLCubeMap::Bind(const uint32_t& slot) const { glBindTextureUnit(slot, m_CubeMap); }

    void OpenGLCubeMap::UnBind(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    void* OpenGLCubeMap::GetTextureID() const { return (void*)(uint64_t)m_CubeMap; }

    void OpenGLCubeMap::BindIrradianceTexture(const uint32_t& slot) const { glBindTextureUnit(slot, m_Irradiance); }

    void OpenGLCubeMap::BindPrefilterTexture(const uint32_t& slot) const { glBindTextureUnit(slot, m_Prefilter); }

    void OpenGLCubeMap::BindBrdfLutTexture(const uint32_t& slot) const { glBindTextureUnit(slot, m_BrdfLut); }

    void* OpenGLCubeMap::GetIrradianceTextureID() const { return (void*)(uint64_t)m_Irradiance; }

    void* OpenGLCubeMap::GetPrefilterTextureID() const { return (void*)(uint64_t)m_Prefilter; }

    void* OpenGLCubeMap::GetBrdfLutTextureID() const { return (void*)(uint64_t)m_BrdfLut; }

} // namespace Engine