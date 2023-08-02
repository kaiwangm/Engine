#pragma once
#include <glm/glm.hpp>
#include <Engine/Runtime/GameFramework/Common/UComponent.h>

namespace Engine
{
    struct Plane
    {
        glm::vec3 normal;
        float     distance;

        Plane() : normal(glm::vec3(0.f)), distance(0.f) {}
        Plane(const glm::vec3& position, const glm::vec3& inNormal) : normal(glm::normalize(inNormal))
        {
            distance = glm::dot(position, normal);
        }

        float getSignedDistanceToPlane(const glm::vec3& point) const { return glm::dot(normal, point) - distance; }
    };

    struct Frustum
    {
        Plane topFace;
        Plane bottomFace;

        Plane rightFace;
        Plane leftFace;

        Plane farFace;
        Plane nearFace;
    };

    struct FrustumVolume
    {
        virtual bool isOnFrustum(const Frustum& frustum, const UTransformComponent& transform) const = 0;
    };

    struct FrustumSphere : public FrustumVolume
    {
        FrustumSphere() : center(glm::vec3(0.f)), radius(0.f) {}
        FrustumSphere(const glm::vec3& center, float radius) : center(center), radius(radius) {}

        bool isOnFrustum(const Frustum& frustum, const UTransformComponent& transform) const override
        {
            // Get global scale is computed by doing the magnitude of
            // X, Y and Z model matrix's column.
            const glm::vec3 globalScale = transform.GetScale();

            // Get our global center with process it with the global model matrix of our transform
            const glm::vec3 globalCenter {transform.GetTransform() * glm::vec4(center, 1.f)};

            // To wrap correctly our shape, we need the maximum scale scalar.
            const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

            // Max scale is assuming for the diameter. So, we need the half to apply it to our radius
            FrustumSphere globalSphere(globalCenter, radius * (maxScale));

            // Check Firstly the result that have the most chance
            // to faillure to avoid to call all functions.
            return (globalSphere.isOnOrForwardPlane(frustum.leftFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.rightFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.farFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.nearFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.topFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.bottomFace));
        }

        bool isOnOrForwardPlane(const Plane& plane) const { return plane.getSignedDistanceToPlane(center) > -radius; }

        glm::vec3 center;
        float     radius;
    };

    struct FrustumAABB : public FrustumVolume
    {
        glm::vec3 center;
        glm::vec3 extents;

        FrustumAABB() : center(glm::vec3(0.f)), extents(glm::vec3(0.f)) {}

        FrustumAABB(const glm::vec3& min, const glm::vec3& max) :
            center {(max + min) * 0.5f}, extents {max.x - center.x, max.y - center.y, max.z - center.z}
        {}

        FrustumAABB(const glm::vec3& inCenter, float iI, float iJ, float iK) : center {inCenter}, extents {iI, iJ, iK}
        {}

        bool isOnFrustum(const Frustum& frustum, const UTransformComponent& transform) const override
        {
            // Get global scale thanks to our transform
            const glm::vec3 globalCenter {transform.GetTransform() * glm::vec4(center, 1.f)};

            glm::vec3 position = transform.GetPosition();
            glm::vec3 rotation = transform.GetRotation();

            const glm::vec3 objectFront =
                -glm::normalize(glm::rotate(glm::quat(rotation), glm::vec3(0.0f, 0.0f, 1.0f)));
            const glm::vec3 objectRight = glm::normalize(glm::rotate(glm::quat(rotation), glm::vec3(1.0f, 0.0f, 0.0f)));
            const glm::vec3 objectUp(0.0f, 1.0f, 0.0f);

            // Scaled orientation
            const glm::vec3 scaledExtents = transform.GetScale() * extents;

            const glm::vec3 right   = scaledExtents.x * objectRight;
            const glm::vec3 up      = scaledExtents.y * objectUp;
            const glm::vec3 forward = scaledExtents.z * objectFront;

            const float newIi = std::abs(glm::dot(glm::vec3 {1.f, 0.f, 0.f}, right)) +
                                std::abs(glm::dot(glm::vec3 {1.f, 0.f, 0.f}, up)) +
                                std::abs(glm::dot(glm::vec3 {1.f, 0.f, 0.f}, forward));

            const float newIj = std::abs(glm::dot(glm::vec3 {0.f, 1.f, 0.f}, right)) +
                                std::abs(glm::dot(glm::vec3 {0.f, 1.f, 0.f}, up)) +
                                std::abs(glm::dot(glm::vec3 {0.f, 1.f, 0.f}, forward));

            const float newIk = std::abs(glm::dot(glm::vec3 {0.f, 0.f, 1.f}, right)) +
                                std::abs(glm::dot(glm::vec3 {0.f, 0.f, 1.f}, up)) +
                                std::abs(glm::dot(glm::vec3 {0.f, 0.f, 1.f}, forward));

            // We not need to divise scale because it's based on the half extention of the AABB
            const FrustumAABB globalAABB(globalCenter, newIi, newIj, newIk);

            return (
                globalAABB.isOnOrForwardPlane(frustum.leftFace) && globalAABB.isOnOrForwardPlane(frustum.rightFace) &&
                globalAABB.isOnOrForwardPlane(frustum.topFace) && globalAABB.isOnOrForwardPlane(frustum.bottomFace) &&
                globalAABB.isOnOrForwardPlane(frustum.nearFace) && globalAABB.isOnOrForwardPlane(frustum.farFace));
        }

        bool isOnOrForwardPlane(const Plane& plane) const
        {
            // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
            const float r = extents.x * std::abs(plane.normal.x) + extents.y * std::abs(plane.normal.y) +
                            extents.z * std::abs(plane.normal.z);

            return -r <= plane.getSignedDistanceToPlane(center);
        }
    };
}; // namespace Engine