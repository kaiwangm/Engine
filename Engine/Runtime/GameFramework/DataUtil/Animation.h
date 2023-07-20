#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cmath>

#include <assimp/Importer.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Renderer/Renderer.h>

#include <Engine/Runtime/GameFramework/DataUtil/Mesh.h>

namespace Engine
{
    struct JointTransform
    {
        glm::vec3 Position;
        glm::quat Rotation;

        JointTransform() = default;
        JointTransform(const glm::vec3& position, const glm::quat& rotation) : Position(position), Rotation(rotation) {}

        glm::mat4 GetLocalTransform() const
        {
            glm::mat4 positionMat = glm::translate(glm::mat4(1.0f), Position);
            glm::mat4 rotationMat = glm::toMat4(Rotation);
            return positionMat * rotationMat;
        }

        static JointTransform
        Interpolate(const JointTransform& frameA, const JointTransform& frameB, const float progression)
        {
            glm::vec3 position = glm::mix(frameA.Position, frameB.Position, progression);
            glm::quat rotation = glm::slerp(frameA.Rotation, frameB.Rotation, progression);
            return JointTransform(position, rotation);
        }
    };

    struct KeyFrame
    {
        float                                           TimeStamp;
        std::unordered_map<std::string, JointTransform> Pose;

        KeyFrame(const float timeStamp, const std::unordered_map<std::string, JointTransform>& pose) :
            TimeStamp(timeStamp), Pose(pose)
        {}

        JointTransform GetJointTransform(const std::string jointName) const
        {
            auto iter = Pose.find(jointName);
            if (iter != Pose.end())
            {
                return iter->second;
            }

            return JointTransform {};
        }
    };

    class Animation
    {
    public:
        Animation(const float timeLength, const std::vector<KeyFrame>& frames) :
            m_TimeLength(timeLength), m_KeyFrames(frames)
        {}

        float                        GetTimeLength() const { return m_TimeLength; }
        const std::vector<KeyFrame>& GetKeyFrames() { return m_KeyFrames; }

    private:
        float                 m_TimeLength;
        std::vector<KeyFrame> m_KeyFrames;
    };

    class Animator
    {
    public:
        Animator() {}

        void DoAnimation(const Ref<Animation> animation)
        {
            m_AnimationTime    = 0;
            m_CurrentAnimation = animation;
        }

        void Update(const float timeStep)
        {
            if (m_CurrentAnimation == nullptr)
            {
                return;
            }

            IncreaseAnimationTime(timeStep);
            m_CurrentPose = CalCulateCurrentAnimationPose();
        }

        std::unordered_map<std::string, JointTransform> GetCurrentPose() { return m_CurrentPose; }

    private:
        Ref<Animation>                                  m_CurrentAnimation;
        float                                           m_AnimationTime = 0;
        std::unordered_map<std::string, JointTransform> m_CurrentPose;

        void IncreaseAnimationTime(const float timeStep)
        {
            m_AnimationTime = m_AnimationTime + timeStep;
            if (m_AnimationTime > m_CurrentAnimation->GetTimeLength())
            {
                m_AnimationTime = std::fmod(m_AnimationTime, m_CurrentAnimation->GetTimeLength());
            }
        }

        std::unordered_map<std::string, JointTransform> CalCulateCurrentAnimationPose()
        {
            auto [previousFrame, nextFrame] = GetPreviousAndNextFrame();
            float progression               = CalCulateProgression(previousFrame, nextFrame);
            return InterpolatePoses(previousFrame, nextFrame, progression);
        }

        float CalCulateProgression(const KeyFrame& previousFrame, const KeyFrame& nextFrame)
        {
            float totalTime   = nextFrame.TimeStamp - previousFrame.TimeStamp;
            float currentTime = m_AnimationTime - previousFrame.TimeStamp;
            return currentTime / totalTime;
        }

        std::unordered_map<std::string, JointTransform>
        InterpolatePoses(const KeyFrame& previousFrame, const KeyFrame& nextFrame, float progression)
        {
            std::unordered_map<std::string, JointTransform> currentPose;
            for (const auto& [jointName, transform] : previousFrame.Pose)
            {
                JointTransform previousTransform = previousFrame.GetJointTransform(jointName);
                JointTransform nextTransform     = nextFrame.GetJointTransform(jointName);

                currentPose[jointName] = JointTransform::Interpolate(previousTransform, nextTransform, progression);
            }
            return currentPose;
        }

        std::pair<KeyFrame, KeyFrame> GetPreviousAndNextFrame()
        {
            auto     keyFrames     = m_CurrentAnimation->GetKeyFrames();
            KeyFrame previousFrame = keyFrames[0];
            KeyFrame nextFrame     = keyFrames[0];

            for (int i = 1; i < keyFrames.size(); ++i)
            {
                nextFrame = keyFrames[i];
                if (nextFrame.TimeStamp > m_AnimationTime)
                {
                    break;
                }
                previousFrame = nextFrame;
            }

            return {previousFrame, nextFrame};
        }
    };

    struct Joint
    {
        int                     m_Index;
        std::string             m_Name;
        std::vector<Ref<Joint>> m_Children;

        glm::mat4 m_AnimatedTransform;
        glm::mat4 m_LocalBindTransform;
        glm::mat4 m_InverseBindTransform;

        Joint() = default;
        Joint(const int index, const std::string& name, const glm::mat4& transform) :
            m_Index(index), m_Name(name), m_LocalBindTransform(transform)
        {}

        void AddChild(Ref<Joint> child) { m_Children.push_back(child); }

        glm::mat4 GetAnimatedTransform() const { return m_AnimatedTransform; }

        void SetAnimatedTransform(const glm::mat4& transform) { m_AnimatedTransform = transform; }

        glm::mat4 GetInverseBindTransform() const { return m_InverseBindTransform; }

        void CalcInverseBindTransform(const glm::mat4& parentBindTransform)
        {
            glm::mat4 bindTransform = parentBindTransform * m_LocalBindTransform;
            m_InverseBindTransform  = glm::inverse(bindTransform);
            for (auto& child : m_Children)
            {
                child->CalcInverseBindTransform(bindTransform);
            }
        }
    };

    class AnimatedMesh
    {
    public:
        AnimatedMesh() {}
        AnimatedMesh(const std::string& path) : m_Directory(path.substr(0, path.find_last_of('/'))), m_JointCount(0)
        {
            // skin

            Assimp::Importer import;
            const aiScene*   scene =
                import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                Log::Core_Error(fmt::format("ERROR::ASSIMP::{0}", std::string(import.GetErrorString())));
                return;
            }

            processNode(scene->mRootNode, scene);

            for (int i = 0; i < scene->mRootNode->mNumChildren; ++i)
            {
                if (std::string(scene->mRootNode->mChildren[i]->mName.data) == "Armature")
                {
                    aiNode* armatureNode = scene->mRootNode->mChildren[i];
                    m_RootJoint          = std::make_shared<Joint>();
                    processJoint(armatureNode, m_RootJoint);
                    m_RootJoint->m_Children[0]->CalcInverseBindTransform(glm::mat4(1.0f));
                }
            }

            for (int i = 0; i < scene->mNumAnimations; ++i)
            {
                const aiAnimation* animation = scene->mAnimations[i];

                std::unordered_map<std::string, std::vector<std::pair<JointTransform, float>>> poses;

                for (int j = 0; j < animation->mNumChannels; ++j)
                {
                    const aiNodeAnim* nodeAnimation = animation->mChannels[j];

                    std::string jointName(nodeAnimation->mNodeName.data);

                    for (int k = 0; k < nodeAnimation->mNumPositionKeys; ++k)
                    {
                        auto posKey = nodeAnimation->mPositionKeys[k];
                        auto rotKey = nodeAnimation->mRotationKeys[k];

                        // glm::quat w x y z
                        poses[jointName].push_back(
                            {JointTransform(
                                 glm::vec3(posKey.mValue.x, posKey.mValue.y, posKey.mValue.z),
                                 glm::quat(rotKey.mValue.w, rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z)),
                             (float)posKey.mTime});
                    }
                }

                int                   length = poses.begin()->second.size();
                std::vector<KeyFrame> keyFrames;

                for (int i = 0; i < length; ++i)
                {
                    float                                           timeStamp = poses.begin()->second[i].second;
                    std::unordered_map<std::string, JointTransform> pose;

                    for (const auto& [jointName, jointInfo] : poses)
                    {
                        pose[jointName] = jointInfo[i].first;
                    }

                    keyFrames.push_back(KeyFrame(timeStamp, pose));
                }

                DoAnimation(std::make_shared<Animation>(keyFrames.back().TimeStamp, keyFrames));
            }
        }

        void processJoint(aiNode* node, Ref<Joint> praentJoint)
        {
            praentJoint->m_LocalBindTransform = glm::transpose(glm::make_mat4(&node->mTransformation.a1));

            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                aiNode*     child = node->mChildren[i];
                std::string name(child->mName.data);
                auto        iter     = m_JointMapping.find(name);
                int         jointIdx = -1;
                if (iter != m_JointMapping.end())
                {
                    jointIdx = iter->second;
                }
                else
                {
                    continue;
                }

                auto childJoint = m_Joints[jointIdx];
                praentJoint->AddChild(childJoint);

                processJoint(child, childJoint);
            }
        }

        void processNode(aiNode* node, const aiScene* scene)
        {
            // process all the node's meshes (if any)
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                m_Meshes.push_back(processMesh(mesh, scene));
            }
            // then do the same for each of its children
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh* amesh, const aiScene* scene)
        {
            std::vector<std::vector<int>>   jointIDs(amesh->mNumVertices);
            std::vector<std::vector<float>> vertexWeights(amesh->mNumVertices);

            for (int i = 0; i < amesh->mNumBones; ++i)
            {
                int         boneIndex = 0;
                std::string boneName(amesh->mBones[i]->mName.data);

                if (m_JointMapping.find(boneName) == m_JointMapping.end())
                {
                    boneIndex = m_JointCount;
                    m_JointCount++;
                    m_JointMapping[boneName] = boneIndex;

                    // glm::mat4 bindMat = glm::transpose(
                    //     glm::make_mat4(&amesh->mBones[i]->mOffsetMatrix.a1));

                    // Log::Core_Trace("{0}", glm::to_string(bindMat));
                    m_Joints.push_back(std::make_shared<Joint>(boneIndex, boneName, glm::mat4(1.0f)));
                }
                else
                {
                    boneIndex = m_JointMapping[boneName];
                }

                for (int j = 0; j < amesh->mBones[i]->mNumWeights; ++j)
                {
                    int   VertexID = amesh->mBones[i]->mWeights[j].mVertexId;
                    float Weight   = amesh->mBones[i]->mWeights[j].mWeight;
                    jointIDs[VertexID].push_back(boneIndex);
                    vertexWeights[VertexID].push_back(Weight);
                }
            }

            std::vector<float>    positions;
            std::vector<float>    texutreCoords;
            std::vector<float>    normals;
            std::vector<int>      bindJointIDs;
            std::vector<float>    weights;
            std::vector<uint32_t> indices;

            for (unsigned int i = 0; i < amesh->mNumVertices; i++)
            {
                positions.push_back(amesh->mVertices[i].x);
                positions.push_back(amesh->mVertices[i].y);
                positions.push_back(amesh->mVertices[i].z);

                if (amesh->mTextureCoords[0])
                {
                    texutreCoords.push_back(amesh->mTextureCoords[0][i].x);
                    texutreCoords.push_back(amesh->mTextureCoords[0][i].y);
                }
                else
                {
                    texutreCoords.push_back(0.0f);
                    texutreCoords.push_back(0.0f);
                }

                normals.push_back(amesh->mNormals[i].x);
                normals.push_back(amesh->mNormals[i].y);
                normals.push_back(amesh->mNormals[i].z);

                for (int j = 0; j < 3; ++j)
                {
                    if (j < jointIDs[i].size())
                    {
                        bindJointIDs.push_back(jointIDs[i][j]);
                    }
                    else
                    {
                        bindJointIDs.push_back(0);
                    }
                }

                for (int j = 0; j < 3; ++j)
                {
                    if (j < vertexWeights[i].size())
                    {
                        weights.push_back(float(vertexWeights[i][j]));
                    }
                    else
                    {
                        weights.push_back(0.0f);
                    }
                }
            }

            for (unsigned int i = 0; i < amesh->mNumFaces; i++)
            {
                aiFace face = amesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            Mesh mesh;

            mesh.AddVertexBuffer(
                positions.data(), amesh->mNumVertices, 3, sizeof(float), {{0, ShaderDataType::Float3, "in_position"}});
            mesh.AddVertexBuffer(texutreCoords.data(),
                                 amesh->mNumVertices,
                                 2,
                                 sizeof(float),
                                 {{1, ShaderDataType::Float2, "in_textureCoords"}});
            mesh.AddVertexBuffer(
                normals.data(), amesh->mNumVertices, 3, sizeof(float), {{2, ShaderDataType::Float3, "in_normal"}});
            mesh.AddVertexBuffer(bindJointIDs.data(),
                                 amesh->mNumVertices,
                                 3,
                                 sizeof(int),
                                 {{3, ShaderDataType::Int3, "in_jointIndices"}});
            mesh.AddVertexBuffer(weights.data(),
                                 amesh->mNumVertices,
                                 3,
                                 sizeof(float),
                                 {{4, ShaderDataType::Float3, "in_weights", true}});

            mesh.AddIndexBuffer(indices.data(), amesh->mNumFaces * 3);

            if (amesh->mMaterialIndex >= 0)
            {
                aiMaterial* material    = scene->mMaterials[amesh->mMaterialIndex];
                auto        diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                m_Textures.insert(m_Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                if (diffuseMaps.size() != 0)
                {
                    mesh.AddTexture(diffuseMaps[0]);
                }

                auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
                m_Textures.insert(m_Textures.end(), specularMaps.begin(), specularMaps.end());
                if (specularMaps.size() != 0)
                {
                    mesh.AddTexture(specularMaps[0]);
                }
            }

            return mesh;
        }

        std::vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
        {
            std::vector<Ref<Texture2D>> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                std::string path(str.C_Str());
                path                   = m_Directory + "/" + path;
                Ref<Texture2D> texture = Texture2D::Create(path);
                textures.push_back(texture);
            }
            return textures;
        }

        void DoAnimation(const Ref<Animation> animation) { m_Animator.DoAnimation(animation); }

        void Update(const float timeStep) { m_Animator.Update(timeStep); }

        void Draw(Ref<Shader> shader, glm::mat4 vpMat, glm::mat4 transform)
        {
            for (const auto& mesh : m_Meshes)
            {
                auto currentPose = m_Animator.GetCurrentPose();

                ApplyPoseToJoints(currentPose, m_RootJoint->m_Children[0], glm::mat4(1.0f));

                shader->Bind();
                for (auto joint : m_Joints)
                {
                    int         jointIdx  = joint->m_Index;
                    std::string jointName = joint->m_Name;

                    std::string paraName       = "jointTransforms[" + std::to_string(jointIdx) + "]";
                    glm::mat4   jointTransform = joint->GetAnimatedTransform();
                    shader->SetMat4(paraName, jointTransform);
                }
                shader->UnBind();

                auto texture = mesh.m_Textures[0];
                texture->Bind(0);
                Renderer::Submit(mesh.m_VertexArray, shader, vpMat, transform);
                texture->UnBind(0);
            }
        }

        void ApplyPoseToJoints(std::unordered_map<std::string, JointTransform> currentPose,
                               Ref<Joint>                                      joint,
                               glm::mat4                                       parentTransform)
        {
            auto currentLocalTransform = currentPose[joint->m_Name].GetLocalTransform();
            auto currentTransform      = parentTransform * currentLocalTransform;

            for (auto chindJoint : joint->m_Children)
            {
                ApplyPoseToJoints(currentPose, chindJoint, currentTransform);
            }

            currentTransform = currentTransform * joint->GetInverseBindTransform();
            joint->SetAnimatedTransform(currentTransform);
        }

    public:
        std::string m_Directory;

        // skin
        std::vector<Mesh>           m_Meshes;
        std::vector<Ref<Texture2D>> m_Textures;

        // skeleton
        std::unordered_map<std::string, int> m_JointMapping;
        std::vector<Ref<Joint>>              m_Joints;
        Ref<Joint>                           m_RootJoint;
        int                                  m_JointCount;

        Animator m_Animator;

        std::vector<glm::mat4> GetJointTransforms()
        {
            std::vector<glm::mat4> jointTransforms(m_JointCount);
            addJointsToVector(m_RootJoint, jointTransforms);
            return jointTransforms;
        }

        void addJointsToVector(const Ref<Joint>& joint, std::vector<glm::mat4>& jointTransforms)
        {
            jointTransforms[joint->m_Index] = joint->GetAnimatedTransform();
            for (const auto child : joint->m_Children)
            {
                addJointsToVector(child, jointTransforms);
            }
        }
    };
} // namespace Engine