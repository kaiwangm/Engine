#pragma once
#include <Engine/Runtime/GameFramework/Common/UComponent.h>
#include <Engine/Runtime/Renderer/Renderer.h>

#include <Engine/Runtime/GameFramework/StaticMesh/UChunkComponent.h>
#include <glm/gtx/intersect.hpp>

#include <Engine/Runtime/GameFramework/StaticMesh/SimplexNoise/Simplex.h>

#include <vector>

namespace Engine
{
    class UChunkArrayWorldComponent final : public UComponent
    {
    public:
        struct Chunk
        {
            UChunkComponent* component = nullptr;
            glm::vec3        position  = glm::vec3(0.0f);
        };

    private:
        constexpr static uint32_t WorldSizeX  = 8;
        constexpr static uint32_t WorldSizeY  = 12;
        constexpr static uint32_t WorldSizeZ  = 8;
        constexpr static float    WorldBottom = ((float)0 - (float)WorldSizeY / 2) * 32.0f;
        constexpr static float    WorldLeft   = ((float)0 - (float)WorldSizeX / 2) * 32.0f;
        constexpr static float    WorldFront  = ((float)0 - (float)WorldSizeZ / 2) * 32.0f;
        constexpr static float    WorldTop    = ((float)WorldSizeY - (float)WorldSizeY / 2) * 32.0f;
        constexpr static float    WorldRight  = ((float)WorldSizeX - (float)WorldSizeX / 2) * 32.0f;
        constexpr static float    WorldBack   = ((float)WorldSizeZ - (float)WorldSizeZ / 2) * 32.0f;
        constexpr static uint32_t ChunkSize   = UChunkComponent::ChunkSize;

        Chunk m_Chunks[WorldSizeX][WorldSizeY][WorldSizeZ];

    public:
        UChunkArrayWorldComponent()
        {
            for (uint32_t x = 0; x < WorldSizeX; x++)
            {
                for (uint32_t y = 0; y < WorldSizeY; y++)
                {
                    for (uint32_t z = 0; z < WorldSizeZ; z++)
                    {
                        m_Chunks[x][y][z].component = new UChunkComponent();
                        m_Chunks[x][y][z].component->SetOwner(this);
                        m_Chunks[x][y][z].position = glm::vec3(((float)x - (float)WorldSizeX / 2) * 32.0f,
                                                               ((float)y - (float)WorldSizeY / 2) * 32.0f,
                                                               ((float)z - (float)WorldSizeZ / 2) * 32.0f);
                    }
                }
            }

            for (uint32_t x = 0; x < WorldSizeX; x++)
            {
                for (uint32_t y = 0; y < WorldSizeY; y++)
                {
                    for (uint32_t z = 0; z < WorldSizeZ; z++)
                    {
                        if (m_Chunks[x][y][z].component != nullptr)
                        {
                            m_Chunks[x][y][z].component->SetTopChunkPointer(
                                y == WorldSizeY - 1 ? nullptr : m_Chunks[x][y + 1][z].component);
                            m_Chunks[x][y][z].component->SetBottomChunkPointer(
                                y == 0 ? nullptr : m_Chunks[x][y - 1][z].component);
                            m_Chunks[x][y][z].component->SetLeftChunkPointer(x == 0 ? nullptr :
                                                                                      m_Chunks[x - 1][y][z].component);
                            m_Chunks[x][y][z].component->SetRightChunkPointer(
                                x == WorldSizeX - 1 ? nullptr : m_Chunks[x + 1][y][z].component);
                            m_Chunks[x][y][z].component->SetFrontChunkPointer(z == 0 ? nullptr :
                                                                                       m_Chunks[x][y][z - 1].component);
                            m_Chunks[x][y][z].component->SetBackChunkPointer(
                                z == WorldSizeZ - 1 ? nullptr : m_Chunks[x][y][z + 1].component);
                        }
                    }
                }
            }

            GenTerrain();

            for (uint32_t x = 0; x < WorldSizeX; x++)
            {
                for (uint32_t y = 0; y < WorldSizeY; y++)
                {
                    for (uint32_t z = 0; z < WorldSizeZ; z++)
                    {
                        if (m_Chunks[x][y][z].component != nullptr)
                        {
                            m_Chunks[x][y][z].component->BuildVertexData();
                        }
                    }
                }
            }
        }

        void GenTerrain()
        {
            Simplex::seed(777777);

            for (int x = WorldLeft; x < WorldRight; x += 1.0f)
            {

                for (int z = WorldFront; z < WorldBack; z += 1.0f)
                {
                    float height = Simplex::noise(glm::vec2(x, z) / 384.0f) * 30.0f + 15.0f;
                    for (int y = WorldBottom; y < WorldTop; y += 1.0f)
                    {
                        if (y < height)
                        {
                            if (y > height - 1.0f)
                            {
                                if (y > 25.0f)
                                {
                                    SetVoxel(glm::ivec3(x, y, z), 5);
                                }
                                else
                                {
                                    SetVoxel(glm::ivec3(x, y, z), 2);
                                }
                            }
                            else if (y > height - 5.0f)
                            {
                                if (y > 25.0f)
                                {
                                    SetVoxel(glm::ivec3(x, y, z), 4);
                                }
                                else
                                {
                                    SetVoxel(glm::ivec3(x, y, z), 3);
                                }
                            }
                            else
                            {
                                SetVoxel(glm::ivec3(x, y, z), 4);
                            }
                        }
                    }
                }
            }
        }

        ~UChunkArrayWorldComponent()
        {
            for (uint32_t x = 0; x < WorldSizeX; x++)
            {
                for (uint32_t y = 0; y < WorldSizeY; y++)
                {
                    for (uint32_t z = 0; z < WorldSizeZ; z++)
                    {
                        if (m_Chunks[x][y][z].component != nullptr)
                        {
                            delete m_Chunks[x][y][z].component;
                            m_Chunks[x][y][z].component = nullptr;
                        }
                    }
                }
            }
        }

        void TickLogic(float deltaTime)
        {
            for (uint32_t x = 0; x < WorldSizeX; x++)
            {
                for (uint32_t y = 0; y < WorldSizeY; y++)
                {
                    for (uint32_t z = 0; z < WorldSizeZ; z++)
                    {
                        m_Chunks[x][y][z].component->TickLogic(deltaTime);
                    }
                }
            }
        }

        int32_t GetWorldSizeX() const { return WorldSizeX; }
        int32_t GetWorldSizeY() const { return WorldSizeY; }
        int32_t GetWorldSizeZ() const { return WorldSizeZ; }

        Chunk& GetChunkRef(int32_t x, int32_t y, int32_t z) { return m_Chunks[x][y][z]; }

        struct IntersectResult
        {
            bool intersected = false;

            glm::ivec3 voxelIndex;
            glm::vec3  voxelPosition;
            uint32_t   voxelID;
            // Bottom Top Back Right Front Left
            uint32_t   faceIndex;
            glm::ivec3 chunkIndex;
        };

        IntersectResult GetVoxelPtr(float x, float y, float z) const
        {
            for (int32_t i = 0; i < WorldSizeX; i++)
            {
                for (int32_t j = 0; j < WorldSizeY; j++)
                {
                    for (int32_t k = 0; k < WorldSizeZ; k++)
                    {
                        if (m_Chunks[i][j][k].component != nullptr)
                        {
                            glm::vec3 chunkPositionAA = m_Chunks[i][j][k].position;
                            glm::vec3 chunkPositionBB = m_Chunks[i][j][k].position + glm::vec3(ChunkSize);

                            if (x >= chunkPositionAA.x && x < chunkPositionBB.x && y >= chunkPositionAA.y &&
                                y < chunkPositionBB.y && z >= chunkPositionAA.z && z < chunkPositionBB.z)
                            {
                                IntersectResult result;
                                result.intersected   = true;
                                result.voxelIndex    = glm::ivec3(glm::floor(x - chunkPositionAA.x),
                                                               glm::floor(y - chunkPositionAA.y),
                                                               glm::floor(z - chunkPositionAA.z));
                                result.voxelPosition = chunkPositionAA +
                                                       glm::vec3(glm::floor(x - chunkPositionAA.x),
                                                                 glm::floor(y - chunkPositionAA.y),
                                                                 glm::floor(z - chunkPositionAA.z)) +
                                                       glm::vec3(0.5f);
                                result.voxelID = m_Chunks[i][j][k].component->GetVoxel(
                                    result.voxelIndex.x, result.voxelIndex.y, result.voxelIndex.z);
                                result.faceIndex  = -1;
                                result.chunkIndex = glm::ivec3(i, j, k);

                                return result;
                            }
                        }
                    }
                }
            }

            return {false, glm::ivec3(0), glm::vec3(0.0f), 0, 0, glm::ivec3(0)};
        }

        void SetVoxel(glm::ivec3 position, uint32_t voxelID)
        {
            glm::ivec3 chunkIndex = (position - glm::ivec3(WorldLeft, WorldBottom, WorldFront)) / glm::ivec3(ChunkSize);
            glm::ivec3 voxelIndex = (position - glm::ivec3(WorldLeft, WorldBottom, WorldFront)) % glm::ivec3(ChunkSize);

            if (chunkIndex.x < 0 || chunkIndex.x >= WorldSizeX || chunkIndex.y < 0 || chunkIndex.y >= WorldSizeY ||
                chunkIndex.z < 0 || chunkIndex.z >= WorldSizeZ)
            {
                return;
            }

            m_Chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].component->SetVoxel(
                voxelIndex.x, voxelIndex.y, voxelIndex.z, voxelID);
        }

        uint32_t GetVoxel(const IntersectResult& result) const
        {
            if (result.chunkIndex.x >= 0 && result.chunkIndex.x < WorldSizeX && result.chunkIndex.y >= 0 &&
                result.chunkIndex.y < WorldSizeY && result.chunkIndex.z >= 0 && result.chunkIndex.z < WorldSizeZ)
            {
                return m_Chunks[result.chunkIndex.x][result.chunkIndex.y][result.chunkIndex.z].component->GetVoxel(
                    result.voxelIndex.x, result.voxelIndex.y, result.voxelIndex.z);
            }

            return 0;
        }

        void ExcavateVoxel(const IntersectResult& result)
        {
            if (result.chunkIndex.x < 0 || result.chunkIndex.x >= WorldSizeX || result.chunkIndex.y < 0 ||
                result.chunkIndex.y >= WorldSizeY || result.chunkIndex.z < 0 || result.chunkIndex.z >= WorldSizeZ)
            {
                return;
            }

            m_Chunks[result.chunkIndex.x][result.chunkIndex.y][result.chunkIndex.z].component->SetVoxel(
                result.voxelIndex.x, result.voxelIndex.y, result.voxelIndex.z, 0);
        }

        void PlaceVoxel(const IntersectResult& result, uint32_t voxelID)
        {
            const glm::ivec3 voxelBias[6] {
                glm::ivec3(0, -1, 0),
                glm::ivec3(0, 0, 1),
                glm::ivec3(1, 0, 0),
                glm::ivec3(0, 0, -1),
                glm::ivec3(-1, 0, 0),
                glm::ivec3(0, 1, 0),
            };

            uint32_t faceIndex = result.faceIndex;

            glm::ivec3 chunkIndex = result.chunkIndex;
            glm::ivec3 voxelIndex = result.voxelIndex;

            voxelIndex = voxelIndex + voxelBias[faceIndex];
            chunkIndex = chunkIndex + voxelIndex / glm::ivec3(ChunkSize);
            voxelIndex = voxelIndex % glm::ivec3(ChunkSize);

            if (chunkIndex.x < 0 || chunkIndex.x >= WorldSizeX || chunkIndex.y < 0 || chunkIndex.y >= WorldSizeY ||
                chunkIndex.z < 0 || chunkIndex.z >= WorldSizeZ)
            {
                return;
            }

            if (m_Chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].component->GetVoxel(
                    voxelIndex.x, voxelIndex.y, voxelIndex.z) != 0)
            {
                return;
            }

            m_Chunks[chunkIndex.x][chunkIndex.y][chunkIndex.z].component->SetVoxel(
                voxelIndex.x, voxelIndex.y, voxelIndex.z, voxelID);
        }

        std::vector<IntersectResult>
        IntersectRay(const glm::vec3& origin, const glm::vec3& direction, float maxDistance)
        {
            std::vector<IntersectResult> results;
            glm::vec3                    position = origin;
            glm::vec3                    delta    = 0.1f * glm::normalize(direction);

            while (glm::length(delta) < maxDistance)
            {
                glm::vec3 checkPosition = position + delta;

                IntersectResult result = GetVoxelPtr(checkPosition.x, checkPosition.y, checkPosition.z);

                if (result.intersected && result.voxelID != 0)
                {
                    if (results.size() == 0 || results.size() != 0 && results.back().chunkIndex != result.chunkIndex &&
                                                   results.back().voxelIndex != result.voxelIndex)
                    {
                        const glm::vec3 voxelVertexPositions[8] = {
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(1.0f, 0.0f, 0.0f),
                            glm::vec3(1.0f, 0.0f, 1.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f),
                            glm::vec3(1.0f, 1.0f, 0.0f),
                            glm::vec3(1.0f, 1.0f, 1.0f),
                            glm::vec3(0.0f, 1.0f, 1.0f),
                        };

                        const glm::ivec3 voxelIndices[12] = {
                            glm::ivec3(2, 1, 0),
                            glm::ivec3(3, 2, 0),
                            glm::ivec3(3, 7, 6),
                            glm::ivec3(2, 3, 6),
                            glm::ivec3(2, 6, 5),
                            glm::ivec3(1, 2, 5),
                            glm::ivec3(1, 5, 4),
                            glm::ivec3(0, 1, 4),
                            glm::ivec3(0, 4, 7),
                            glm::ivec3(3, 0, 7),
                            glm::ivec3(5, 6, 7),
                            glm::ivec3(4, 5, 7),
                        };

                        float distance = FLT_MAX;
                        for (int32_t i = 0; i < 6; i++)
                        {
                            glm::vec2 instersctionPointA = glm::vec2(0.0f);
                            float     distanceA          = FLT_MAX;
                            bool      isIntersectedA     = glm::intersectRayTriangle(
                                origin,
                                direction,
                                result.voxelPosition + voxelVertexPositions[voxelIndices[i * 2][0]] - glm::vec3(0.5f),
                                result.voxelPosition + voxelVertexPositions[voxelIndices[i * 2][1]] - glm::vec3(0.5f),
                                result.voxelPosition + voxelVertexPositions[voxelIndices[i * 2][2]] - glm::vec3(0.5f),
                                instersctionPointA,
                                distanceA);

                            glm::vec2 instersctionPointB = glm::vec2(0.0f);
                            float     distanceB          = FLT_MAX;
                            bool      isIntersectedB     = glm::intersectRayTriangle(
                                origin,
                                direction,
                                result.voxelPosition + voxelVertexPositions[voxelIndices[i * 2 + 1][0]] -
                                    glm::vec3(0.5f),
                                result.voxelPosition + voxelVertexPositions[voxelIndices[i * 2 + 1][1]] -
                                    glm::vec3(0.5f),
                                result.voxelPosition + voxelVertexPositions[voxelIndices[i * 2 + 1][2]] -
                                    glm::vec3(0.5f),
                                instersctionPointB,
                                distanceB);

                            if (isIntersectedA)
                            {
                                if (distanceA < distance)
                                {
                                    distance         = distanceA;
                                    result.faceIndex = i;
                                }
                            }
                            else if (isIntersectedB)
                            {
                                if (distanceB < distance)
                                {
                                    distance         = distanceB;
                                    result.faceIndex = i;
                                }
                            }
                        }

                        results.push_back(result);
                    }
                }

                delta += 0.1f * glm::normalize(direction);
            }

            return results;
        }
    };
}; // namespace Engine