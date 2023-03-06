#pragma once
#include "../DataUtil/PointCloud.h"
#include "../Common/UComponent.h"

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
    };
}; // namespace Engine