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
        m_LayerStack = new LayerStack();

        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        glGenBuffers(1, &m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

        float vertices[3 * 3] = {
            -0.5f, -0.5f,  0.0f,
             0.5f, -0.5f,  0.0f,
             0.0f,  0.5f,  0.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

        unsigned int indices[3] = {0, 1, 2};
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            out vec3 v_Position;

            void main()
            {   
                v_Position = a_Position;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
            }
        )";

        m_Shader = new Shader(vertexSrc, fragmentSrc);
    }

    Application::~Application()
    {   
        delete m_Shader;
        delete m_LayerStack;
        delete m_Window;
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
            ClientDraw();
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(m_VertexArray);
            m_Shader->Bind();
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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