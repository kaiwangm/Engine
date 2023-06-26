#include <Engine/Runtime/Engine.h>

namespace Engine
{
    class OctreeObject
    {
    public:
        OctreeObject(const std::string& path, uint32_t maxLevel);
        void RenderOctree(uint32_t level, const Ref<Shader> shader);
        void RenderOctreeGui();

        void CacheGL();

    private:
        unsigned char                            texture[1280][1280][3];
        Ref<HashOctree<std::array<uint32_t, 3>>> m_HashOctree;
        std::vector<Ref<VertexArray>>            m_Octree_VertexArrays;
        Ref<Texture2D>                           m_Octree_Texture;
    };
} // namespace Engine