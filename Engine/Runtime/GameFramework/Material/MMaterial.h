#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../Common/UObject.h"

namespace Engine
{
    class MMaterial : public UObject
    {
    private:
        std::string m_Name;

    public:
        MMaterial(const std::string& name) : m_Name(name) {}
        MMaterial() {}
        virtual ~MMaterial() {}

        std::string GetName() const { return m_Name; }
        void        SetName(const std::string& name) { m_Name = name; }
    };
}