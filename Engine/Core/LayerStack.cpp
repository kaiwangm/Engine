#include "LayerStack.h"

namespace Engine
{
    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_LayerVector)
		{
			layer->OnDetach();
			delete layer;
		}
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_LayerVector.emplace(m_LayerVector.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(Layer* layer)
    {
        m_LayerVector.emplace_back(layer);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_LayerVector.begin(), m_LayerVector.begin() + m_LayerInsertIndex, layer);
		if (it != m_LayerVector.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_LayerVector.erase(it);
			m_LayerInsertIndex--;
		}
    }

    void LayerStack::PopOverlay(Layer* layer)
    {
        auto it = std::find(m_LayerVector.begin() + m_LayerInsertIndex, m_LayerVector.end(), layer);
		if (it != m_LayerVector.end())
		{
			layer->OnDetach();
			m_LayerVector.erase(it);
		}
    }
}