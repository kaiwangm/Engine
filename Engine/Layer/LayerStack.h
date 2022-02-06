# pragma once

#include "Core.h"
#include "Layer.h"

namespace Engine
{
    class LayerStack
    {
        public:
            LayerStack();
            ~LayerStack();

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* layer);
            void PopLayer(Layer* layer);
            void PopOverlay(Layer* layer);

            std::vector<Layer*>::iterator begin(){ return m_LayerVector.begin(); }
            std::vector<Layer*>::iterator end(){ return m_LayerVector.end(); }
        
        private:
            std::vector<Layer*> m_LayerVector;
            std::vector<Layer*>::iterator m_LayerInsert;
    };
}