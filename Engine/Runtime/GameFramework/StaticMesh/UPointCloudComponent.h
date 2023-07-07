#pragma once
#include <Engine/Runtime/GameFramework/DataUtil/PointCloud.h>
#include <Engine/Runtime/GameFramework/Common/UComponent.h>

namespace Engine
{
    class UPointCloudComponent final : public UComponent
    {
    private:
        PointCloud m_PointCloud;

    public:
        UPointCloudComponent() : m_PointCloud() {}
        UPointCloudComponent(const std::string path) : m_PointCloud(path) {}

        PointCloud& GetPointCloud() { return m_PointCloud; }
        std::string GetFilePath() { return m_PointCloud.m_FilePath; }
        int GetNumPoints() { return m_PointCloud.m_NumPoints; }
    };
}; // namespace Engine