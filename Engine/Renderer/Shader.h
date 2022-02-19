#pragma once

#include "Core.h"

namespace Engine
{
    class Shader
    {
        public:
            Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
            ~Shader();

            void Bind() const;
            void UnBind() const;

        private:
            GLuint m_RendererID;
    };
}