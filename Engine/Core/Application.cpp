#include "Application.h"
#include "Log.h"
#include "ImGuiLayer.h"
#include "Input.h"

namespace Engine
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        Engine::Log::Init();
        ENGINE_CORE_TRACE("Engine Initialization.");

        s_Instance = this;
        m_Window = Window::Create();
        m_Window->SetEventCallback(BIND_EVENT(Application::onEvent));

        m_ImGuiLayer = new ImGuiLayer();
        m_LayerStack.reset(new LayerStack());

        PushOverlay(m_ImGuiLayer);

        // ------------ OpenGL Triangle -------- //

        // Set Vertex Arrays
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        // Set Vertex Buffer
        float vertices[3 * 7] = {
            -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        };
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        
        m_VertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position"},
            { ShaderDataType::Float4, "a_Color"}
        });

        m_VertexBuffer->ApplyLayout();

        // Set Index Buffer
        uint32_t indices[3] = {0, 1, 2};
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        // Set Shader
        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {   
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                //color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
    }

    Application::~Application()
    {   
        ENGINE_CORE_TRACE("Engine Shutdown.");
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack->PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack->PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ClientDraw();

            glBindVertexArray(m_VertexArray);
            m_Shader->Bind();
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount() , GL_UNSIGNED_INT, nullptr);
            m_Shader->UnBind();
            
            //auto[x, y] = Input::GetMousePostion();

            //ENGINE_CORE_INFO("{0}, {1}", x, y);

            for(auto layer : *m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();
            for(auto layer : *m_LayerStack)
            {
                layer->OnImGuiRender();
            }  
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::onWindowClose));

        //ENGINE_CORE_INFO("Application <-- Event::{}", e);

        for(auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
        {
            if (e.m_Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}