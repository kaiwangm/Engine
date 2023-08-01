#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <Engine/Runtime/GameFramework/Common/UObject.h>

namespace Engine
{
    class MMaterial : public UObject
    {
    private:
        std::string m_Name;
        std::string m_MaterialType;

    public:
        MMaterial(const std::string& name, const std::string& materialType) : m_Name(name), m_MaterialType(materialType)
        {}
        MMaterial() {}
        virtual ~MMaterial() {}

        std::string GetName() const { return m_Name; }
        std::string GetMaterialType() const { return m_MaterialType; }
        void        SetName(const std::string& name) { m_Name = name; }
        void        SetMaterialType(const std::string& materialType) { m_MaterialType = materialType; }
    };
} // namespace Engine