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

        float Triangle_vertices[3 * 7] = {
            -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        };
        uint32_t Triangle_indices[3] = {0, 1, 2};

        // Set Vertex Buffer
        std::shared_ptr<VertexBuffer> Triangle_VertexBuffer;
        Triangle_VertexBuffer.reset(VertexBuffer::Create(Triangle_vertices, sizeof(Triangle_vertices)));
        Triangle_VertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position"},
            { ShaderDataType::Float4, "a_Color"}
        });

        // Set Index Buffer
        std::shared_ptr<IndexBuffer> Triangle_IndexBuffer;
        Triangle_IndexBuffer.reset(IndexBuffer::Create(Triangle_indices, sizeof(Triangle_indices) / sizeof(uint32_t)));

        // Set Vertex Arrays
        m_Triangle_VertexArray.reset(VertexArray::Create());
        m_Triangle_VertexArray->AddVertexBuffer(Triangle_VertexBuffer);
        m_Triangle_VertexArray->AddIndexBuffer(Triangle_IndexBuffer);
        

        // ---------------Square--------------- //

        float Square_vertices[4 * 7] = {
            -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        };
        uint32_t Square_indices[6] = {0, 1, 2, 2, 3, 0};

        // Set Vertex Buffer
        std::shared_ptr<VertexBuffer> Square_VertexBuffer;
        Square_VertexBuffer.reset(VertexBuffer::Create(Square_vertices, sizeof(Square_vertices)));
        Square_VertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position"},
            { ShaderDataType::Float4, "a_Color"}
        });

        // Set Index Buffer
        std::shared_ptr<IndexBuffer> Square_IndexBuffer;
        Square_IndexBuffer.reset(IndexBuffer::Create(Square_indices, sizeof(Square_indices) / sizeof(uint32_t)));

        // Set Vertex Arrays
        m_Square_VertexArray.reset(VertexArray::Create());
        m_Square_VertexArray->AddVertexBuffer(Square_VertexBuffer);
        m_Square_VertexArray->AddIndexBuffer(Square_IndexBuffer);

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

            
            m_Shader->Bind();

            m_Square_VertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_Square_VertexArray->GetIndexBuffer()->GetCount() , GL_UNSIGNED_INT, nullptr);
            m_Square_VertexArray->UnBind();

            m_Triangle_VertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_Triangle_VertexArray->GetIndexBuffer()->GetCount() , GL_UNSIGNED_INT, nullptr);
            m_Triangle_VertexArray->UnBind();

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