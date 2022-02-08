# pragma once

#include "Core.h"
#include "Layer.h"

namespace Engine
{
    class LayerStack
    {
        public:
            LayerStack() = default;
            ~LayerStack();

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* layer);
            void PopLayer(Layer* layer);
            void PopOverlay(Layer* layer);

            std::vector<Layer*>::iterator begin() { return m_LayerVector.begin(); }
            std::vector<Layer*>::iterator end() { return m_LayerVector.end(); }
            std::vector<Layer*>::reverse_iterator rbegin() { return m_LayerVector.rbegin(); }
            std::vector<Layer*>::reverse_iterator rend() { return m_LayerVector.rend(); }

            std::vector<Layer*>::const_iterator begin() const { return m_LayerVector.begin(); }
            std::vector<Layer*>::const_iterator end()	const { return m_LayerVector.end(); }
            std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_LayerVector.rbegin(); }
            std::vector<Layer*>::const_reverse_iterator rend() const { return m_LayerVector.rend(); }
        
        private:
            std::vector<Layer*> m_LayerVector;
            unsigned int m_LayerInsertIndex = 0;
    };
}