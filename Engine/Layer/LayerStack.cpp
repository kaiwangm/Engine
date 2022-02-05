#include "LayerStack.h"

namespace Engine
{
    LayerStack::LayerStack()
    {
        m_LayerInsert = m_LayerVector.begin();
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_LayerVector)
            delete layer;
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_LayerInsert = m_LayerVector.emplace(m_LayerInsert, layer);
    }

    void LayerStack::PushOverlay(Layer* layer)
    {
        m_LayerVector.emplace_back(layer);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_LayerVector.begin(), m_LayerVector.end(), layer);

        if (it != m_LayerVector.end())
        {
            m_LayerVector.erase(it);
            m_LayerInsert--;
        }
    }

    void LayerStack::PopOverlay(Layer* layer)
    {
        auto it = std::find(m_LayerVector.begin(), m_LayerVector.end(), layer);
        
        if (it != m_LayerVector.end())
            m_LayerVector.erase(it);
    }
}