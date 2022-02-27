#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "Log.h"

namespace Engine {
Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    ENGINE_CORE_TRACE("Creating Shader.");

    // Read our shaders into the appropriate buffers
    std::string vertexSource = vertexSrc;  // Get source code for vertex shader.
    std::string fragmentSource =
        fragmentSrc;  // Get source code for fragment shader.

    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = (const GLchar*)vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        ENGINE_CORE_ERROR("{0}", infoLog.data());
        ENGINE_CORE_ERROR("Vertex shader compilation failed.");

        // In this simple program, we'll just leave
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar*)fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        ENGINE_CORE_ERROR("{0}", infoLog.data());
        ENGINE_CORE_ERROR("Fragment shader compilation failed.");

        // In this simple program, we'll just leave
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_RendererID = glCreateProgram();
    GLuint program = m_RendererID;

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Use the infoLog as you see fit.
        ENGINE_CORE_ERROR("{0}", infoLog.data());
        ENGINE_CORE_ERROR("Shader Link failed.");

        // In this simple program, we'll just leave
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}
Shader::~Shader() {
    glDeleteProgram(m_RendererID);

    ENGINE_CORE_TRACE("Deleting Shader.");
}

void Shader::Bind() const { glUseProgram(m_RendererID); }

void Shader::UnBind() const { glUseProgram(0); }

void Shader::SetInt(const std::string& name, const int& value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}
void Shader::SetIntArray(const std::string& name, int* values, uint32_t count) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
}

void Shader::SetFloat(const std::string& name, const float& value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void Shader::SetFloat2(const std::string& name, const glm::vec2& vector) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& vector) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& vector) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& matrix) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& matrix) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
}  // namespace Engine