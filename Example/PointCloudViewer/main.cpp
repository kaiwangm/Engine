#define ENGINE_USE_ENTRYPOINT
#include <Engine/Runtime/Engine.h>

#include <ImGuizmo.h>
#include <nfd.h>

namespace Engine
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer(bool& app_open_ref) : Layer("EditorLayer"), m_app_open(app_open_ref)
        {
            m_World = std::make_shared<UWorld>();

            auto camera_viewport = std::make_shared<PerspectiveCamera>(60.0f, 1.778f, 0.1f, 800.0f);
            auto camera_s                             = m_World->AddActor<ACamera>("viewport camera", camera_viewport);
            camera_s.GetTransformComponent().SetPosition(glm::vec3 {0.339f, 3.711f, 8.815f});
            camera_s.GetTransformComponent().SetRotation(glm::vec3 {-0.088f, -6.732f, 0.000f});
            camera_s.GetTransformComponent().SetScale(glm::vec3 {1.000f, 1.000f, 1.000f});
            camera_s.SetIsControlled(true);
            camera_s.SetIsViewportCamera(true);

            auto camera_aa                      = std::make_shared<PerspectiveCamera>(60.0f, 1.778f, 0.1f, 800.0f);
            auto camera_a                       = m_World->AddActor<ACamera>("camera_aa", camera_aa);
            camera_a.GetTransformComponent().SetPosition(glm::vec3 {3.570f, 2.371f, 2.175f});
            camera_a.GetTransformComponent().SetRotation(glm::vec3 {-0.115f, 2.404f, 0.000f});
            camera_a.GetTransformComponent().SetScale(glm::vec3 {1.000f, 1.000f, 1.000f});
            camera_a.SetIsControlled(false);
            camera_a.SetIsViewportCamera(false);

            m_World->m_ControlledActor = static_cast<AActor*>(camera_s.GetCameraComponent().GetOwner());
            m_World->m_MainCamera      = static_cast<ACamera*>(camera_s.GetCameraComponent().GetOwner());
            m_Camera                   = camera_viewport;

            auto skybox =
                m_World->AddActor<ASkybox>("skybox", "Assert/Editor/Skybox/TheSkyIsOnFire/the_sky_is_on_fire_8k.hdr");

            m_World->AddActor<AActor>("EmptyActor");
            m_World->AddActor<AActor>("EmptyActor00");
            m_World->AddActor<AActor>("EmptyActor01");

            m_Callbacks["Open"] = std::function<void()>([&]() {
                std::string filepath = OpenFile();
                std::string suffix   = filepath.substr(filepath.find_last_of(".") + 1);

                if (suffix == "ply")
                {
                    auto pointcloud = m_World->AddActor<APointCloud>("pointcloud", filepath);
                    pointcloud.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 0.0f, 0.0f});
                    pointcloud.GetTransformComponent().SetScale(glm::vec3 {0.01f, 0.01f, 0.01f});
                }
            });
        }

        void OnAttach() override {}

        void OnDetach() override {}

        void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

        void TickLogic() override
        {
            m_World->TickLogic(m_LayerUpdateMeta.m_timeStep, m_LayerUpdateMeta.m_nowTime, m_IsWindowFocused);
        }

        void TickRender() override { m_World->TickRender(m_LayerUpdateMeta.m_timeStep); }

        void TickGui() override
        {
            Gui::DockSpace(m_app_open, m_Callbacks);

            m_World->TickGui(m_LayerUpdateMeta.m_timeStep);

            bool is_color_focused = false;
            Gui::ShowViewport(
                "ViewPort :: Color", m_World->m_FrameRenderBuffer, true, is_color_focused, std::function {[&]() {
                    EditorGui();
                }});

            m_IsWindowFocused = is_color_focused;

            ImGui::ShowExampleAppLog(NULL);
        }

        void EditorGui()
        {
            float camDistance = 8.f;

            static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
            static ImGuizmo::MODE      mCurrentGizmoMode(ImGuizmo::LOCAL);
            static bool                useSnap         = false;
            static float               snap[3]         = {1.f, 1.f, 1.f};
            static float               bounds[]        = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};
            static float               boundsSnap[]    = {0.1f, 0.1f, 0.1f};
            static bool                boundSizing     = false;
            static bool                boundSizingSnap = false;

            ImGuiIO& io                  = ImGui::GetIO();
            float    viewManipulateRight = io.DisplaySize.x;
            float    viewManipulateTop   = 0;

            ImGuizmo::SetDrawlist();
            float windowWidth  = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
            viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
            viewManipulateTop   = ImGui::GetWindowPos().y;

            m_World->m_MainCamera->GetCameraComponent().GetCamera().SetViewPort(windowWidth, windowHeight);
            m_World->m_MainCamera->GetCameraComponent().GetCamera().RecalculateProjectionMatrix();

            glm::mat4 cameraView       = glm::inverse(m_World->m_MainCamera->GetTransformComponent().GetTransform());
            glm::mat4 cameraProjection = m_World->m_MainCamera->GetCameraComponent().GetCamera().GetProjectionMatrix();
            glm::mat4 identityMatrix   = glm::mat4(1.0f);

            ImGuizmo::DrawGrid(
                glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(identityMatrix), 8.0f);

            if (m_World->entity_selected != entt::null &&
                m_World->GetRegistry().valid(m_World->entity_selected) == true &&
                m_World->GetRegistry().any_of<UTransformComponent>(m_World->entity_selected) == true)
            {
                auto&     tansformComponent = m_World->GetRegistry().get<UTransformComponent>(m_World->entity_selected);
                glm::mat4 transform         = tansformComponent.GetTransform();

                ImGuizmo::Manipulate(glm::value_ptr(cameraView),
                                     glm::value_ptr(cameraProjection),
                                     mCurrentGizmoOperation,
                                     mCurrentGizmoMode,
                                     glm::value_ptr(transform),
                                     NULL,
                                     useSnap ? &snap[0] : NULL,
                                     boundSizing ? bounds : NULL,
                                     boundSizingSnap ? boundsSnap : NULL);

                if (ImGuizmo::IsUsing())
                {
                    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                    ImGuizmo::DecomposeMatrixToComponents(
                        glm::value_ptr(transform), matrixTranslation, matrixRotation, matrixScale);

                    tansformComponent.SetPosition(
                        glm::vec3 {matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]});

                    ImGuizmo::RecomposeMatrixFromComponents(
                        matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(transform));
                }
            }

            ImGuizmo::ViewManipulate(glm::value_ptr(cameraView),
                                     camDistance,
                                     ImVec2(viewManipulateRight - 128, viewManipulateTop),
                                     ImVec2(128, 128),
                                     0x10101010);
        }

        std::string OpenFile()
        {
            std::string filepath = "";
            nfdchar_t*  outPath  = NULL;
            nfdresult_t result   = NFD_OpenDialog(NULL, NULL, &outPath);
            if (result == NFD_OKAY)
            {
                filepath = outPath;
                Log::Info("Opening file: {}", filepath);
                free(outPath);
            }
            else if (result == NFD_CANCEL)
            {
                Log::Info("User pressed cancel.");
            }
            else
            {
                Log::Error("Error: {}", NFD_GetError());
            }

            return filepath;
        }

    private:
        Ref<Camera> m_Camera;
        Ref<UWorld> m_World;
        bool&       m_app_open;

    private:
        bool m_IsWindowFocused = false;
    };

    class PointCloudViewer : public Application
    {
    public:
        PointCloudViewer(const std::string& basedir, const std::string& filepath) :
            Application("PointCloudViewer", 1700, 1000, basedir)
        {
            Log::Info("PointCloudViewer Initialization.");
            PushLayer(std::make_shared<DockSpaceLayer>(m_Running));
            PushLayer(std::make_shared<EditorLayer>(m_Running));
            Log::Trace("PointCloudViewer Initialization Success.");
        }

        ~PointCloudViewer() { Log::Trace("PointCloudViewer Shutdown."); }
    };

    Scope<Application> CreateApplication(int argc, char** argv)
    {
        std::string basedir = argv[0];
        basedir             = basedir.substr(0, basedir.find_last_of("\\/"));

        std::string filepath;
        if (argc >= 2)
        {
            filepath = argv[1];
        }

        return std::make_unique<PointCloudViewer>(basedir, filepath);
    }
} // namespace Engine
